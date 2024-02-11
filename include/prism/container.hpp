#ifndef PRISM_CONTAINER_GLOBAL_H
#define PRISM_CONTAINER_GLOBAL_H


#ifdef _WIN32
#  define DECL_EXPORT __declspec(dllexport)
#  define DECL_IMPORT __declspec(dllimport)
#else
#  define DECL_EXPORT  __attribute__((visibility("default")))
//#  define DECL_IMPORT  __attribute__((visibility("hidden")))
#  define DECL_IMPORT  __attribute__((visibility("default")))
#endif

#if defined(PRISM_CONTIANER_LIBRARY)
#define PRISM_CONTAINER_EXPORT DECL_EXPORT
#else
#define PRISM_CONTAINER_EXPORT DECL_IMPORT
#endif

#endif // PRISM_CONTAINER_GLOBAL_H




#ifndef PRISM_CONTAINER_H
#define PRISM_CONTAINER_H

#include <functional>
#include <map>
#include <memory>
#include <unordered_map>

#ifdef _MSC_VER
#pragma warning(disable: 4251)
#endif

namespace prism {
class PRISM_CONTAINER_EXPORT Container
{

  private:
    Container();

  public:
    // int s_nextTypeId = 1000;
    static Container*  get();

    Container(Container&) = delete;
    Container(Container&&) = delete;
    Container& operator=(Container&) = delete;
    Container& operator=(Container&&) = delete;
    // one typeid per type
    template <typename T>
    static size_t GetTypeID()
    {
        static long long typeId = typeid(T).hash_code();
        return typeId;
    }
    class FactoryRoot
    {
      public:
        virtual ~FactoryRoot()
        {
        }
    };

    // todo: consider sorted vector
    std::unordered_map<size_t, std::shared_ptr<FactoryRoot>> m_factories;

    template <typename T>
    class CFactory : public FactoryRoot
    {
        std::function<std::shared_ptr<T>()> m_functor;

      public:
        ~CFactory()
        {
        }

        CFactory(std::function<std::shared_ptr<T>()> functor)
            : m_functor(functor)
        {
        }

        std::shared_ptr<T> resolve_object()
        {
            return m_functor();
        }
    };

    template <typename T>
    std::shared_ptr<T> resolve_object()
    {
        auto typeId = GetTypeID<T>();
        auto factoryBase = m_factories[typeId];
        auto factory = std::static_pointer_cast<CFactory<T>>(factoryBase);
        if (!factory)
            return nullptr;
        return factory->resolve_object();
    }
    template <typename T>
    std::shared_ptr<T> printFactorBaseAddres()
    {
        auto typeId = GetTypeID<T>();
        auto factoryBase = m_factories[typeId];
        auto factory = std::static_pointer_cast<CFactory<T>>(factoryBase);
        //std::cout << "=======================================" << std::endl;
        //std::cout << "typeid of:" << typeid(T).hash_code() << std::endl;
        //std::cout << "typeid metheod address:" << (qlonglong)&GetTypeID<T> << std::endl;
        //std::cout << "container address:" << this << std::endl;
        //std::cout << "typeid : " << typeId << std::endl;
        //std::cout << "factory map address:" << &m_factories << std::endl;
        //std::cout << "factorybase address:" << &m_factories[typeId] << std::endl;
        //std::cout << "=======================================" << std::endl;
        return factory->resolve_object();
    }
    template <typename TInterface, typename T2, typename... T2ps>
    void register_functor()
    {
        m_factories[GetTypeID<TInterface>()] =
            std::make_shared<CFactory<T2>>([=] { return std::make_shared<T2>(resolve_object<T2ps>()...); });
    }
    // Most basic implementation - register a functor
    template <typename TInterface, typename... TS>
    void register_functor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS>... ts)> functor)
    {
        m_factories[GetTypeID<TInterface>()] =
            std::make_shared<CFactory<TInterface>>([=] { return functor(resolve_object<TS>()...); });
    }

    // Register one instance of an object
    template <typename TInterface>
    void register_instance(std::shared_ptr<TInterface> t)
    {
        m_factories[GetTypeID<TInterface>()] =
            std::make_shared<CFactory<TInterface>>([=] { return t; });
    }

    // Supply a function pointer
    template <typename TInterface, typename... TS>
    void register_functor(std::shared_ptr<TInterface> (*functor)(std::shared_ptr<TS>... ts))
    {
        register_functor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS>... ts)>(functor));
    }

    // A factory that will call the constructor, per instance required
    template <typename TInterface, typename TConcrete, typename... TArguments>
    void register_factory()
    {
        register_functor(
            std::function<std::shared_ptr<TInterface>(std::shared_ptr<TArguments>... ts)>(
                [](std::shared_ptr<TArguments>... arguments) -> std::shared_ptr<TInterface> {
                    return std::make_shared<TConcrete>(std::forward<std::shared_ptr<TArguments>>(arguments)...);
                }));
    }

    // A factory that will return one instance for every request
    template <typename TInterface, typename TConcrete, typename... TArguments>
    void register_instance()
    {
        register_instance<TInterface>(std::make_shared<TConcrete>(resolve_object<TArguments>()...));
    }
};

}// namespace prism
#endif // PRISM_CONTAINER_H
