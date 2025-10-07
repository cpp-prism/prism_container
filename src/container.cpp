#ifdef USING_COCOAPODS
#include "prism/container.hpp"
#else
#include "include/prism/container.hpp"
#endif

namespace prism
{
Container::Container()
{
}

Container *Container::get()
{
    static Container instance;
    return &instance;
}

} // namespace  prism
