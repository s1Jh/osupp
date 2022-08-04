#include "Log.hpp"

NS_BEGIN

namespace detail
{

SectionEntry::~SectionEntry()
{
    std::scoped_lock<std::recursive_mutex> lock(::PROJECT_NAMESPACE::log::logMutex);

    ::PROJECT_NAMESPACE::log::sections.erase(
        ::PROJECT_NAMESPACE::log::sections.end() - 1);
}

SectionEntry::SectionEntry(const std::string &section)
{
    std::scoped_lock<std::recursive_mutex> lock(::PROJECT_NAMESPACE::log::logMutex);

    ::PROJECT_NAMESPACE::log::sections.push_back(section);
}

} // namespace detail

std::recursive_mutex log::logMutex;

#ifdef RELEASE
bool log::enableDebug = false;
#else

bool log::enableDebug = true;

#endif

bool log::enabled = true;

std::vector<std::string> log::sections;

[[maybe_unused]] void log::setDebug(bool ns)
{
    std::scoped_lock<std::recursive_mutex> lock(::PROJECT_NAMESPACE::log::logMutex);
    enableDebug = ns;
}

void log::enable()
{
    std::scoped_lock<std::recursive_mutex> lock(::PROJECT_NAMESPACE::log::logMutex);
    enabled = true;
}

void log::disable()
{
    std::scoped_lock<std::recursive_mutex> lock(::PROJECT_NAMESPACE::log::logMutex);
    enabled = false;
}

NS_END