#pragma once

#include "define.hpp"

#include "Util.hpp"

#include <any>
#include <cstring>
#include <functional>
#include <memory>
#include <type_traits>

NS_BEGIN

namespace detail
{

// From this legend: http://www.cplusplus.com/forum/general/223816/#msg1025150
template<typename T>
struct deduce_type;

template<typename RETURN_TYPE, typename CLASS_TYPE, typename... ARGS>
struct deduce_type<RETURN_TYPE (CLASS_TYPE::*)(ARGS...) const>
{
    using type = std::function<RETURN_TYPE(ARGS...)>;
};

} // detail

template<typename CLOSURE>
auto wrap(const CLOSURE &fn)
{
    return typename detail::deduce_type<decltype(&CLOSURE::operator())>::type(fn);
}

enum class CallbackReturn
{
    Ok, Unsubscribe
};

template<class CBGroup, CBGroup CBType, typename... Parameters> requires std::is_enum_v<CBGroup>
struct IsCallbackHandle
{
    static const bool value = false;
    static const std::integral_constant<CBGroup, CBType> type;
    static const std::function<CallbackReturn(Parameters...)> function;
};

#define MAKE_CALLBACK(group, cbtype, ...)                                      \
  template <> struct IsCallbackHandle<group, cbtype, __VA_ARGS__> {            \
    static const bool value = true;                                            \
  };

namespace detail
{

template<class CBGroup>
struct VirtualCallbackHandle
{
    CBGroup type;
    size_t size = 0;
    bool markedForDeletion = false;
    bool isMember = false;
};

template<class CBGroup, typename... Parameters>
struct CallbackHandle: public VirtualCallbackHandle<CBGroup>
{
    std::function<CallbackReturn(Parameters...)> function;
};

template<class CBGroup, class ThisT, typename... Parameters>
struct MemberCallbackHandle: public VirtualCallbackHandle<CBGroup>
{
    std::function<CallbackReturn(ThisT *, Parameters...)> function;
    ThisT *this_pointer;
};

typedef int ExternalHandle;

template<class CBGroup>
class Callbacks
{
public:
    Callbacks(Callbacks<CBGroup> &other)
        : registeredHandles(other.registeredHandles.size())
    {
        std::scoped_lock lock{mutex, other.mutex};

        auto this_it = registeredHandles.begin();
        auto that_it = other.registeredHandles.cbegin();

        for (; that_it != other.registeredHandles.cend(); ++this_it, ++that_it) {
            VirtualCallbackHandle<CBGroup> *ptr = that_it->get();

            auto *new_handle =
                (VirtualCallbackHandle<CBGroup> *) std::malloc(ptr->size);
            if (!new_handle) {
                log::error("Failed to allocate space for a new handle");
                continue;
            }

            std::memcpy(new_handle, ptr, ptr->size);
            this_it->reset(new_handle);
        }
    }

    Callbacks(Callbacks<CBGroup> &&other) noexcept
        : registeredHandles(std::move(other.registeredHandles))
    {
        std::scoped_lock lock{mutex, other.mutex};

        auto this_it = registeredHandles.begin();
        auto that_it = other.registeredHandles.cbegin();

        for (; that_it != other.registeredHandles.cend(); ++this_it, ++that_it) {
            VirtualCallbackHandle<CBGroup> *ptr = that_it->get();

            auto *new_handle =
                (VirtualCallbackHandle<CBGroup> *) std::malloc(ptr->size);
            if (!new_handle) {
                log::error("Failed to allocate space for a new handle");
                continue;
            }

            std::memcpy(new_handle, ptr, ptr->size);
            this_it->reset(new_handle);
        }
    }

    using VirtualHandle = VirtualCallbackHandle<CBGroup>;

    template<CBGroup Type, typename... Params>
    ExternalHandle
    subscribeCallback(std::function<CallbackReturn(Params...)> func)
    {
        static_assert(IsCallbackHandle<CBGroup, Type, Params...>::value,
                      "Function does not have a valid signature for this callback");

        std::scoped_lock lock{mutex};

        using HandleType = CallbackHandle<CBGroup, Params...>;
        std::unique_ptr<VirtualHandle> handle(
            (VirtualHandle * )(new HandleType{{Type}, func}));
        handle->size = sizeof(HandleType);

        registeredHandles.push_back(std::move(handle));

        return registeredHandles.size() - 1;
    }

    template<CBGroup Type, typename ThisT, typename... Params>
    ExternalHandle
    subscribeCallback(std::function<CallbackReturn(ThisT *, Params...)> func,
                      ThisT *this_ptr)
    {
        static_assert(IsCallbackHandle<CBGroup, Type, Params...>::value,
                      "Function does not have a valid signature for this callback");

        std::scoped_lock lock{mutex};

        using HandleType = MemberCallbackHandle<CBGroup, ThisT, Params...>;

        std::unique_ptr<VirtualHandle> handle(
            (VirtualHandle * )(new HandleType{{Type}, func, this_ptr}));
        handle->isMember = true;
        handle->size = sizeof(HandleType);
        registeredHandles.push_back(std::move(handle));

        return registeredHandles.size() - 1;
    }

    template<
        typename std::enable_if<std::is_enum<CBGroup>::value, bool>::type = true>
    Callbacks()
    {}

    template<CBGroup Type, typename... Params>
    void invokeCallback(Params... params)
    {
        std::scoped_lock lock{mutex};

        for (auto &&handle: registeredHandles) {
            if (handle->type == Type && !handle->markedForDeletion) {
                CallbackReturn ret;

                if (handle->isMember) {
                    using HandleType = MemberCallbackHandle<CBGroup, void, Params...>;

                    auto *real_handle = ((HandleType *) handle.get());
                    auto bound = std::bind(real_handle->function,
                                           real_handle->this_pointer, params...);
                    ret = bound();
                }
                else {
                    using HandleType = CallbackHandle<CBGroup, Params...>;

                    auto *real_handle = ((HandleType *) handle.get());
                    auto bound = std::bind(real_handle->function, params...);
                    ret = bound();
                }

                if (ret == CallbackReturn::Unsubscribe)
                    handle->markedForDeletion = true;
            }
        }

        // delete any handles that were marked for deletion
        registeredHandles.erase(
            std::remove_if(registeredHandles.begin(), registeredHandles.end(),
                           [](auto &&handle)
                           { return handle->markedForDeletion; }),
            registeredHandles.end());
    }

private:
    std::mutex mutex;
    std::vector<std::unique_ptr<VirtualHandle>> registeredHandles;
};

template<class CBGroup>
class StaticCallbacks
{
public:
    using VirtualHandle = VirtualCallbackHandle<CBGroup>;

    template<CBGroup Type, typename... Params>
    static ExternalHandle
    SubscribeCallback(std::function<CallbackReturn(Params...)> func)
    {
        static_assert(IsCallbackHandle<CBGroup, Type, Params...>::value,
                      "Function does not have a valid signature for this callback");

        using HandleType = CallbackHandle<CBGroup, Params...>;

        std::unique_ptr<VirtualHandle> handle(
            (VirtualHandle * )(new HandleType{{Type}, func}));

        RegisteredHandles.push_back(std::move(handle));

        return RegisteredHandles.size() - 1;
    }

    template<CBGroup Type, typename ThisT, typename... Params>
    ExternalHandle
    SubscribeCallback(std::function<CallbackReturn(ThisT *, Params...)> func,
                      ThisT *this_ptr)
    {
        static_assert(IsCallbackHandle<CBGroup, Type, Params...>::value,
                      "Function does not have a valid signature for this callback");

        using HandleType = MemberCallbackHandle<CBGroup, ThisT, Params...>;

        std::unique_ptr<VirtualHandle> handle(
            (VirtualHandle * )(new HandleType{{Type}, func, this_ptr}));

        handle->isMember = true;

        RegisteredHandles.push_back(std::move(handle));

        return RegisteredHandles.size() - 1;
    }

protected:
    template<
        typename std::enable_if<std::is_enum<CBGroup>::value, bool>::type = true>
    StaticCallbacks()
    {}

    template<CBGroup Type, typename... Params>
    static void InvokeCallback(Params... params)
    {
        for (auto &&handle: RegisteredHandles) {
            if (handle->type == Type && !handle->markedForDeletion) {
                CallbackReturn ret;

                if (handle->isMember) {
                    using HandleType = MemberCallbackHandle<CBGroup, void, Params...>;

                    auto *real_handle = ((HandleType *) handle.get());

                    auto bound = std::bind(real_handle->function,
                                           real_handle->this_pointer, params...);
                    ret = bound();
                }
                else {
                    using HandleType = CallbackHandle<CBGroup, Params...>;
                    ret = ((HandleType *) handle.get())->function(params...);
                }

                if (ret == CallbackReturn::Unsubscribe)
                    handle->markedForDeletion = true;
            }
        }

        // delete any handles that were marked for deletion
        RegisteredHandles.erase(
            std::remove_if(RegisteredHandles.begin(), RegisteredHandles.end(),
                           [](auto &&handle)
                           { return handle->markedForDeletion; }),
            RegisteredHandles.end());
    }

private:
    static std::vector<std::unique_ptr<VirtualHandle>> RegisteredHandles;
};

template<class CBGroup>
std::vector<std::unique_ptr<typename StaticCallbacks<CBGroup>::VirtualHandle>>
    StaticCallbacks<CBGroup>::RegisteredHandles;

} // detail

NS_END
