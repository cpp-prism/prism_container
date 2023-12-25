#include "include/prism/container.hpp"
namespace prism {
Container::Container()
{
}

Container *  Container::get()
{
    static Container instance;
    return &instance;
}


}// namespace  prism
