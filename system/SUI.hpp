#pragma once

#include "define.hpp"

#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "CallbackManager.hpp"
#include "EnumOperators.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Rect.hpp"
#include "Renderer.hpp"
#include "df2.hpp"

NS_BEGIN

enum class SUIFlags: uint8_t
{
    Visible = 1 << 0,
};

ENABLE_BITMASK_OPERATORS(SUIFlags)

constexpr SUIFlags SUI_DEFAULT_FLAGS = SUIFlags::Visible;

class SUI
{
public:
    /*static SUI FromDefinitionFile(
            const df2 &definition
    );*/
    class BaseElement
    {
        friend class SUI;

    public:
        virtual ~BaseElement() = default;

        [[nodiscard]] drect getBounds() const;

        [[nodiscard]] std::string getName() const;

        [[nodiscard]] bool isVisible() const;

        void hide();

        void show();

    protected:
        void setBounds(drect n);

        BaseElement(std::string name, drect bounds);

        virtual void update(float delta);

        virtual void draw(Renderer &, const drect &bounds);

    private:
        drect m_Bounds;
        std::string m_sName;
        SUIFlags m_eFlags;
    };

    template<class CallbackGroup>
    class Element: public BaseElement, public detail::Callbacks<CallbackGroup>
    {
        friend class SUI;

    protected:
        Element(const std::string &name, drect bounds);
    };

    template<class UIElement, class... Args>
    UIElement &add(Args &&...args)
    {
        static_assert(
            std::is_base_of_v<BaseElement, UIElement>,
            "Added object must be derived from the SUI::BaseElement class.");
        m_Elements.push_back(std::make_shared<UIElement>(args...));
        return *std::reinterpret_pointer_cast<UIElement>(m_Elements.back());
    }

    void update(float delta);

    void draw(Renderer &renderer);

    template<class T>
    T &get(const std::string &name)
    {
        auto it =
            std::find_if(m_Elements.begin(), m_Elements.end(),
                         [&](const std::shared_ptr<BaseElement> &child) -> bool
                         {
                             return child->getName() == name;
                         });

        if (it != m_Elements.end())
            return *std::reinterpret_pointer_cast<T>(*it);
        else
            return *std::dynamic_pointer_cast<T>(NullElement);
    }

    template<class T>
    const T &get(const std::string &name) const
    {
        auto it =
            std::find_if(m_Elements.begin(), m_Elements.end(),
                         [&](const std::shared_ptr<BaseElement> &child) -> bool
                         {
                             return child->getName() == name;
                         });

        if (it != m_Elements.end())
            return *std::reinterpret_pointer_cast<T>(*it);
        else
            return *std::dynamic_pointer_cast<T>(NullElement);
    }

    [[nodiscard]] bool find(const std::string &name) const
    {
        auto it =
            std::find_if(m_Elements.begin(), m_Elements.end(),
                         [&](const std::shared_ptr<BaseElement> &child) -> bool
                         {
                             return child->getName() == name;
                         });

        return it != m_Elements.end();
    }

private:
    static std::shared_ptr<BaseElement> NullElement;
    std::vector<std::shared_ptr<BaseElement>> m_Elements;
};

template<class CallbackGroup>
SUI::Element<CallbackGroup>::Element(const std::string &name, drect bounds)
    : BaseElement(name, bounds)
{}

NS_END