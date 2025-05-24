#ifndef BONGOMOUNTAIN_DEPENDENCYMANAGER_HPP
#define BONGOMOUNTAIN_DEPENDENCYMANAGER_HPP

#include <any>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>
#include <list>

#include "type_storage.hpp"
#include "type_traits.hpp"

namespace core
{
    struct ObjectStorage
    {
        inline void clear() {
            objectInstance.clear();
        }

        template<typename T>
        void store(std::shared_ptr<T> &&instance)
        {
            objectInstance.emplace(typeid(T), std::move(instance));
        }

        template<typename T>
        void store(const std::shared_ptr<T> &instance)
        {
            objectInstance.emplace(typeid(T), instance);
        }

        template<typename T>
        std::optional<std::shared_ptr<T>> get()
        {
            if(auto value = objectInstance.find(typeid(T)); value != objectInstance.end())
                return std::any_cast<std::shared_ptr<T>>(value->second);
            return std::nullopt;
        }

        std::unordered_map<std::type_index, std::any> objectInstance;
    };

    template<typename T, typename RepositoryFactory>
    T make_value(RepositoryFactory &factory, ObjectStorage &storage)
    {
        if constexpr(core::is_shared_ptr_v<T>)
        {
            if constexpr(RepositoryFactory::template has<typename T::element_type>())
            {
                return factory.template createStaticShared<typename T::element_type>(storage);
            }
            else if(factory.parents())
            {
                return factory.parents()->template createSharedInstance<typename T::element_type>(storage);
            }
        }
        if constexpr(core::is_unique_ptr_v<T>)
        {
            if constexpr(RepositoryFactory::template has<typename T::element_type>())
            {
                return factory.template createStatic<typename T::element_type>(storage);
            }
            else if(factory.parents())
            {
                return factory.parents()->template createInstance<typename T::element_type>(storage);
            }
        }
        if constexpr(std::is_pointer_v<T>)
        {
            using target_type = std::remove_pointer_t<T>;
            if constexpr(RepositoryFactory::template has<target_type>())
            {
                return factory.template createStatic<target_type>(storage).release();
            }
            else if(factory.parents())
            {
                return factory.parents()->template createInstance<target_type>(storage).release();
            }
        }

        if constexpr(core::is_template_kind_v<std::vector, T>)
        {
            using value_type = typename T::value_type;
            if constexpr(core::is_shared_ptr_v<value_type>)
            {
                return factory.parents()->template createAllSharedInstance<typename T::value_type::element_type>(
                    storage);
            }
            if constexpr(core::is_unique_ptr_v<value_type>)
            {
                return factory.parents()->template createAllInstance<typename T::value_type::element_type>(storage);
            }
        }

        return T{ nullptr };
    }
    struct RepositoryBindings;

    struct OpaqueRepositoryBindings
    {
        OpaqueRepositoryBindings(OpaqueRepositoryBindings *parent = nullptr)
            : parent(parent == nullptr ? this : parent)
        {}
        OpaqueRepositoryBindings(const OpaqueRepositoryBindings &)                = delete;
        OpaqueRepositoryBindings(OpaqueRepositoryBindings &&) noexcept            = delete;
        OpaqueRepositoryBindings &operator=(const OpaqueRepositoryBindings &)     = delete;
        OpaqueRepositoryBindings &operator=(OpaqueRepositoryBindings &&) noexcept = delete;
        virtual ~OpaqueRepositoryBindings()                                       = default;
        template<typename T>
        std::unique_ptr<T> createInstance(ObjectStorage &storage)
        {
            return std::unique_ptr<T>(reinterpret_cast<T *>(
                create(storage, typeid(T))));// NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
        }

        template<typename T>
        std::shared_ptr<T> createSharedInstance(ObjectStorage &storage)
        {
            if(auto value = createShared(storage, typeid(T)); value.has_value())
                return std::any_cast<std::shared_ptr<T>>(value);
            return nullptr;
        }

        template<typename T>
        std::vector<std::unique_ptr<T>> createAllInstance(ObjectStorage &storage)
        {
            auto                            instance = createAll(storage, typeid(T));
            std::vector<std::unique_ptr<T>> data;
            data.reserve(instance.size());
            std::transform(instance.begin(), instance.end(), std::back_inserter(data), [](void *ptr) {
                return std::unique_ptr<T>{ reinterpret_cast<T *>(
                    ptr) };// NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
            });
            return data;
        }

        template<typename T>
        std::vector<std::shared_ptr<T>> createAllSharedInstance(ObjectStorage &storage)
        {
            auto                            instance = createAllShared(storage, typeid(T));
            std::vector<std::shared_ptr<T>> data;
            data.reserve(instance.size());
            std::transform(instance.begin(), instance.end(), std::back_inserter(data), [](auto &ptr) {
                return std::any_cast<std::shared_ptr<T>>(ptr);// NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
            });
            return data;
        }
        OpaqueRepositoryBindings *parents() { return parent; }
        friend struct RepositoryBindings;

    protected:
        virtual void                 *create(ObjectStorage &storage, const std::type_index &type)          = 0;
        virtual std::any              createShared(ObjectStorage &storage, const std::type_index &type)    = 0;
        virtual std::vector<void *>   createAll(ObjectStorage &storage, const std::type_index &type)       = 0;
        virtual std::vector<std::any> createAllShared(ObjectStorage &storage, const std::type_index &type) = 0;

    private:
        OpaqueRepositoryBindings *parent = nullptr;
    };

    template<typename T, typename FactoryRepository, typename ObjectStorageT, std::size_t... idx>
    std::shared_ptr<typename core::FunctionDecompose<T>::result_type>
        make_shared(FactoryRepository &factory, ObjectStorageT &obj, std::index_sequence<idx...>)
    {
        using argument_type = typename core::FunctionDecompose<T>::argument_type;
        using result_type   = typename core::FunctionDecompose<T>::result_type;
        if(auto data = obj.template get<result_type>())
        {
            return *data;
        }
        auto value = std::make_shared<result_type>(
            make_value<std::decay_t<std::tuple_element_t<idx, argument_type>>>(factory, obj)...);
        obj.store(value);
        return value;
    }

    template<typename T, typename FactoryRepository, typename ObjectStorageT>
    std::shared_ptr<underlying_ptr_t<typename core::FunctionDecompose<T>::result_type>>
        make_shared(FactoryRepository &factory, ObjectStorageT &obj)
    {
        return make_shared<T>(
            factory, obj,
            std::make_index_sequence<std::tuple_size_v<typename core::FunctionDecompose<T>::argument_type>>{});
    }

    template<typename T, typename FactoryRepository, typename ObjectStorageT, std::size_t... idx>
    std::unique_ptr<typename core::FunctionDecompose<T>::result_type>
        make_unique(FactoryRepository &factory, ObjectStorageT &obj, std::index_sequence<idx...>)
    {
        using argument_type = typename core::FunctionDecompose<T>::argument_type;
        using result_type   = typename core::FunctionDecompose<T>::result_type;

        return std::make_unique<result_type>(
            make_value<std::decay_t<std::tuple_element_t<idx, argument_type>>>(factory, obj)...);
    }

    template<typename T, typename FactoryRepository, typename ObjectStorageT>
    std::unique_ptr<underlying_ptr_t<typename core::FunctionDecompose<T>::result_type>>
        make_unique(FactoryRepository &factory, ObjectStorageT &obj)
    {
        return make_unique<T>(
            factory, obj,
            std::make_index_sequence<std::tuple_size_v<typename core::FunctionDecompose<T>::argument_type>>{});
    }

    struct RepositoryBindings : OpaqueRepositoryBindings
    {   RepositoryBindings() noexcept = default;
        RepositoryBindings(RepositoryBindings &&mmove) noexcept : factories(std::move(mmove.factories))
        {}

        RepositoryBindings &operator=(RepositoryBindings &&mmove) noexcept
        {
            factories = std::move(mmove.factories);
            return *this;
        }

        template<typename T>
        std::enable_if_t<std::is_base_of_v<OpaqueRepositoryBindings, T>> register_factory()
        {
            factories.emplace_back(std::make_unique<T>(this));
        }

        inline void clear() {
            factories.clear();
        }

    protected:
        void *create(ObjectStorage &storage, const std::type_index &type) final
        {
            for(auto &factory : factories)
            {
                auto pointer = factory->create(storage, type);
                if(pointer != nullptr)
                {
                    return pointer;
                }
            }
            return nullptr;
        }

        std::vector<void *> createAll(ObjectStorage &storage, const std::type_index &type) final
        {
            std::vector<void *> data;
            for(auto &factory : factories)
            {
                auto array = factory->createAll(storage, type);
                if(data.empty())
                {
                    data = std::move(array);
                }
                else
                {
                    data.insert(data.end(), array.begin(), array.end());
                }
            }
            return data;
        }

        std::any createShared(ObjectStorage &storage, const std::type_index &type) final
        {
            for(auto &factory : factories)
            {
                auto pointer = factory->createShared(storage, type);
                if(pointer.has_value())
                {
                    return pointer;
                }
            }
            return std::any{};
        }

        std::vector<std::any> createAllShared(ObjectStorage &storage, const std::type_index &type) final
        {
            std::vector<std::any> data;
            for(auto &factory : factories)
            {
                auto array = factory->createAllShared(storage, type);
                if(data.empty())
                {
                    data = std::move(array);
                }
                else
                {
                    data.insert(data.end(), array.begin(), array.end());
                }
            }
            return data;
        }

    private:
        std::vector<std::unique_ptr<OpaqueRepositoryBindings>> factories;
    };

    template<typename... BindsT>
    struct StaticRepositoryBindings : public OpaqueRepositoryBindings
    {
        //  using interface_types =
        //      SetKind<>::template add<typename BindsT::interface_type...>;
        //  using implementation_types =
        //      SetKind<>::template add<typename BindsT::implementation_type...>;

        template<typename... Arg>
        StaticRepositoryBindings(Arg... arg)
            : OpaqueRepositoryBindings(std::forward<Arg>(arg)...)
        {}

        // using type_table = decltype(SetKind<>::merge(interface_t<BindsT>...{}));
        template<typename T>
        std::unique_ptr<T> createStatic(ObjectStorage &storage)
        {
            return std::tuple_element_t<find_index<T, interface_t<BindsT>...>(), std::tuple<BindsT...>>::createUnique(
                *this, storage);
        }

        template<typename T>
        std::shared_ptr<T> createStaticShared(ObjectStorage &storage)
        {
            return std::tuple_element_t<find_index<T, interface_t<BindsT>...>(), std::tuple<BindsT...>>::createShared(
                *this, storage);
        }

        template<typename T>
        std::array<std::unique_ptr<T>, count_index<std::is_same, T, interface_t<BindsT>...>()>
            createAllStaticInstance(ObjectStorage &storage)
        {
            return createAllStaticInstance<T>(storage, std::index_sequence_for<BindsT...>{});
        }

        template<typename T>
        std::array<std::shared_ptr<T>, count_index<std::is_same, T, interface_t<BindsT>...>()>
            createAllSharedStaticInstance(ObjectStorage &storage)
        {
            return createAllSharedStaticInstance<T>(storage, std::index_sequence_for<BindsT...>{});
        }

        template<typename T>
        static constexpr bool has()
        {
            return contains_type<T, interface_t<BindsT>...>();
        }

    protected:
        template<typename T, std::size_t... idx>
        std::array<std::unique_ptr<T>, count_index<std::is_same, T, interface_t<BindsT>...>()>
            createAllStaticInstance(ObjectStorage &storage, std::index_sequence<idx...>)
        {
            std::array<std::unique_ptr<T>, count_index<std::is_same, T, interface_t<BindsT>...>()> result;
            auto                                                                                  *out = result.begin();

            auto indexTable = detect_index<std::is_same, T, interface_t<BindsT>...>();
            ((indexTable[idx]
                  ? void(*(out++) = std::tuple_element_t<find_index<T, interface_t<BindsT>...>(),
                                                         std::tuple<BindsT...>>::createUnique(*this, storage))
                  : void(nullptr)),
             ...);

            return result;
        }

        template<typename T, std::size_t... idx>
        std::array<std::shared_ptr<T>, count_index<std::is_same, T, interface_t<BindsT>...>()>
            createAllSharedStaticInstance(ObjectStorage &storage, std::index_sequence<idx...>)
        {
            std::array<std::shared_ptr<T>, count_index<std::is_same, T, interface_t<BindsT>...>()> result;
            auto                                                                                  *out = result.begin();

            auto indexTable = detect_index<std::is_same, T, interface_t<BindsT>...>();
            ((indexTable[idx]
                  ? void(*(out++) = std::tuple_element_t<find_index<T, interface_t<BindsT>...>(),
                                                         std::tuple<BindsT...>>::createShared(*this, storage))
                  : void(nullptr)),
             ...);

            return result;
        }

        void *create(ObjectStorage &storage, const std::type_index &type) final
        {
            static constexpr std::array<std::pair<std::reference_wrapper<const std::type_info>,
                                                  void *(*)(StaticRepositoryBindings<BindsT...> &, ObjectStorage &)>,
                                        sizeof...(BindsT)>
                       types{ std::pair<std::reference_wrapper<const std::type_info>,
                                        void *(*)(StaticRepositoryBindings<BindsT...> &, ObjectStorage &)>{
                    std::cref(BindsT::getInterfaceType()),
                    BindsT::template create<StaticRepositoryBindings<BindsT...>, ObjectStorage> }... };
            const auto pos = std::find_if(types.begin(), types.end(), [type](const auto &info) {
                return type == static_cast<const std::type_info &>(info.first);
            });
            if(pos != types.end())
            {
                return pos->second(*this, storage);
            }
            return nullptr;
        }

        std::vector<void *> createAll(ObjectStorage &storage, const std::type_index &type) final
        {
            std::vector<void *> data;
            static_cast<void>((... || createAllInstanceOf<interface_t<BindsT>>(storage, type, data)));
            return data;
        }

        std::any createShared(ObjectStorage &storage, const std::type_index &type) final
        {
            static constexpr std::array<std::pair<std::reference_wrapper<const std::type_info>,
                                                  std::any (*)(StaticRepositoryBindings<BindsT...> &, ObjectStorage &)>,
                                        sizeof...(BindsT)>
                       types{ std::pair<std::reference_wrapper<const std::type_info>,
                                        std::any (*)(StaticRepositoryBindings<BindsT...> &, ObjectStorage &)>{
                    std::cref(BindsT::getInterfaceType()),
                    BindsT::template createAny<StaticRepositoryBindings<BindsT...>, ObjectStorage> }... };
            const auto pos = std::find_if(types.begin(), types.end(), [type](const auto &info) {
                return type == static_cast<const std::type_info &>(info.first);
            });
            if(pos != types.end())
            {
                return pos->second(*this, storage);
            }
            return std::any{};
        }

        std::vector<std::any> createAllShared(ObjectStorage &storage, const std::type_index &type) final
        {
            std::vector<std::any> data;
            static_cast<void>((... || createAllInstanceOf<interface_t<BindsT>>(storage, type, data)));
            return data;
        }

        template<typename T>
        bool createAllInstanceOf(ObjectStorage &storage, const std::type_index &type, std::vector<void *> &data)
        {
            if(type == typeid(T))
            {
                auto sdata = createAllStaticInstance<T>(storage);
                data.resize(sdata.size());
                std::transform(sdata.begin(), sdata.end(), data.begin(),
                               [](auto &ptr) -> void * { return ptr.release(); });
                return true;
            }

            return false;
        }

        template<typename T>
        bool createAllInstanceOf(ObjectStorage &storage, const std::type_index &type, std::vector<std::any> &data)
        {
            if(type == typeid(T))
            {
                auto sdata = createAllSharedStaticInstance<T>(storage);
                data.reserve(data.size() + sdata.size());
                for(auto &ptr : sdata)
                {
                    if(ptr)
                        data.emplace_back(std::move(ptr));
                }
                return true;
            }

            return false;
        }
    };

    template<auto functor>
    struct FunctionMaker
    {
        template<typename... ArgT>
        struct Invoker
        {
            using result_type = typename FunctionDecompose<decltype(functor)>::result_type;
            using object_type = typename FunctionDecompose<decltype(functor)>::object_type;

            using result_function =
                std::conditional_t<std::is_void_v<result_type>, void, std::optional<std::decay_t<result_type>>>;

            template<typename T>
            static bool test(const T &obj)
            {
                return obj;
            }

            template<typename T>
            static bool test(const std::vector<T> &obj)
            {
                return !obj.empty();
            }

            template<typename RepositoryFactory>
            static inline result_function
                invoke(object_type &obj, RepositoryFactory &repository, ObjectStorage &storage)
            {
                auto tuple = std::tuple<object_type &, std::decay_t<ArgT>...>(
                    obj, make_value<std::decay_t<ArgT>>(repository, storage)...);
                if(std::apply([](auto &obj, auto &...value) { return (test(value) && ...); }, tuple))
                    return std::apply(Invoker<ArgT...>{}, std::move(tuple));
                if constexpr(std::is_void_v<result_type>)
                    return;
                else
                    return std::nullopt;
            }

            result_type operator()(object_type &obj, std::decay_t<ArgT> &&...argument) const
            {
                return (obj.*functor)(std::forward<ArgT>(argument)...);
            }
        };
        using callback = FunctionDecompose<decltype(functor)>::template make_argument<Invoker>;
    };

    template<auto... functor>
    using Setters = SetKind<FunctionMaker<functor>...>;


    template<typename ImplementT, typename Init, typename InterfacesT>
    struct FactoryMaker
    {
        using implementation_type = std::decay_t<typename core::FunctionDecompose<ImplementT>::result_type>;
        using argument_type       = typename core::FunctionDecompose<ImplementT>::argument_type;
        using result_type         = typename core::FunctionDecompose<ImplementT>::result_type;

        template<typename FactoryRepository, typename ObjectStorageT>
        static std::shared_ptr<implementation_type> createShared(FactoryRepository &factory, ObjectStorageT &obj)
        {
            return createShared(factory, obj, std::make_index_sequence<std::tuple_size_v<argument_type>>{});
        }

        template<typename FactoryRepository, typename ObjectStorageT, std::size_t... idx>
        static std::shared_ptr<implementation_type>
            createShared(FactoryRepository &factory, ObjectStorageT &obj, std::index_sequence<idx...>)
        {
            auto shared = std::make_shared<result_type>(
                make_value<std::decay_t<std::tuple_element_t<idx, argument_type>>>(factory, obj)...);
            store(shared, factory, obj, InterfacesT{});
            initialize(*shared, factory, obj, Init{});
            return shared;
        }

        template<typename FactoryRepository, typename ObjectStorageT>
        static std::unique_ptr<implementation_type> createUnique(FactoryRepository &factory, ObjectStorageT &obj)
        {
            return createUnique(factory, obj, std::make_index_sequence<std::tuple_size_v<argument_type>>{});
        }

        template<typename FactoryRepository, typename ObjectStorageT, std::size_t... idx>
        static std::unique_ptr<implementation_type>
            createUnique(FactoryRepository &factory, ObjectStorageT &obj, std::index_sequence<idx...>)
        {
            auto shared = std::make_unique<result_type>(
                make_value<std::decay_t<std::tuple_element_t<idx, argument_type>>>(factory, obj)...);
            initialize(*shared, factory, obj, Init{});
            return shared;
        }

        template<typename FactoryRepository, typename ObjectStorageT, typename... InitT>
        static void initialize(typename core::FunctionDecompose<ImplementT>::result_type &impl,
                               FactoryRepository                                         &factory,
                               ObjectStorageT                                            &objects,
                               SetKind<InitT...>)
        {
            (InitT::callback::invoke(impl, factory, objects), ...);
        }
        template<typename FactoryRepository, typename ObjectStorageT, typename... InterfaceT>
        static void store(std::shared_ptr<typename core::FunctionDecompose<ImplementT>::result_type> &impl,
                          FactoryRepository                                                          &factory,
                          ObjectStorageT                                                             &objects,
                          SetKind<InterfaceT...>)
        {
            static_assert((std::is_base_of_v<InterfaceT, implementation_type> && ...),
                          "class instance should implement the defined interface");
            (objects.template store<InterfaceT>(impl), ...);
        }
    };


    template<typename InterfaceT, typename ImplementT, typename Init = SetKind<>, typename InterfacesT = SetKind<>>
    struct Binding
    {
        using factory_type        = FactoryMaker<ImplementT, Init, InterfacesT>;
        using interface_type      = InterfaceT;
        using implementation_type = typename factory_type::implementation_type;
        static_assert(std::is_base_of_v<interface_type, implementation_type>);

        template<typename FactoryRepository, typename ObjectStorageT>
        static std::unique_ptr<implementation_type> createUnique(FactoryRepository &factory, ObjectStorageT &obj)
        {
            return factory_type::createUnique(factory, obj);
        }

        template<typename FactoryRepository, typename ObjectStorageT>
        static std::shared_ptr<implementation_type> createShared(FactoryRepository &factory, ObjectStorageT &obj)
        {
            auto ptr = obj.template get<implementation_type>();
            if(ptr)
            {
                return *ptr;
            }
            return factory_type::createShared(factory, obj);
        }

        template<typename FactoryRepository, typename ObjectStorageT>
        static void *create(FactoryRepository &factory, ObjectStorageT &obj)
        {
            return createUnique(factory, obj).release();
        }

        template<typename FactoryRepository, typename ObjectStorageT>
        static std::any createAny(FactoryRepository &factory, ObjectStorageT &obj)
        {
            return createShared(factory, obj);
        }

        static constexpr const std::type_info &getInstanceType() { return typeid(implementation_type); }

        static constexpr const std::type_info &getInterfaceType() { return typeid(interface_type); }
    };

    template<typename InterfaceT, typename ImplementT, typename Init>
    struct BindingProvider
    {
        using interface_type      = InterfaceT;
        using implementation_type = ImplementT;

        template<typename FactoryRepository, typename ObjectStorageT>
        static std::shared_ptr<interface_type> createShared(FactoryRepository &factory, ObjectStorageT &obj)
        {
            auto parent = make_value<std::shared_ptr<implementation_type>>(factory, obj);
            if(parent)
            {
                auto value = Init::callback::invoke(*parent, factory, obj);
                if(value)
                {
                    return std::move(*value);
                }
            }
            return nullptr;
        }

        template<typename FactoryRepository, typename ObjectStorageT>
        static std::unique_ptr<interface_type> createUnique(FactoryRepository &factory, ObjectStorageT &obj)
        {
            return nullptr;
        }

        template<typename FactoryRepository, typename ObjectStorageT>
        static interface_type *createStatic(FactoryRepository &factory, ObjectStorageT &obj)
        {
            return nullptr;
        }

        template<typename FactoryRepository, typename ObjectStorageT>
        static void *create(FactoryRepository &factory, ObjectStorageT &obj)
        {
            return nullptr;
        }

        template<typename FactoryRepository, typename ObjectStorageT>
        static std::any createAny(FactoryRepository &factory, ObjectStorageT &obj)
        {
            auto parent = make_value<std::shared_ptr<implementation_type>>(factory, obj);
            if(parent)
            {
                auto value = Init::callback::invoke(*parent, factory, obj);
                if(value)
                {
                    return std::make_any<std::shared_ptr<InterfaceT>>(std::move(*value));
                }
            }
            return std::any{};
        }

        static constexpr const std::type_info &getInstanceType() { return typeid(implementation_type); }

        static constexpr const std::type_info &getInterfaceType() { return typeid(interface_type); }
    };

    namespace factory
    {

        template<typename T, typename Interfaces = SetKind<>, typename Setters = SetKind<>>
        struct define_type
        {
            template<typename... Interface>
            using bind = define_type<T, typename Interfaces::template add<Interface...>, Setters>;
            template<typename... Setter>
            using setters = define_type<T, Interfaces, typename Setters::template merge<Setter...>>;

            template<typename>
            struct generate_interface;
            template<typename... I>
            struct generate_interface<SetKind<I...>>
            {
                using type = SetKind<Binding<I, T, Setters, Interfaces>...>;
            };
            using make = typename generate_interface<Interfaces>::type;
        };

    }// namespace factory

    template<typename... BindingsT>
    struct BindingFactory
    {
        template<typename InterfaceT, typename BindT, typename Setter>
        using define = BindingFactory<Binding<InterfaceT, BindT, Setter, SetKind<InterfaceT>>, BindingsT...>;

        template<typename InterfaceT, typename BindT, auto... member>
        using provide = BindingFactory<BindingProvider<InterfaceT, BindT, FunctionMaker<member>>..., BindingsT...>;

        template<typename BindT, typename... T>
        using bind = BindingFactory<Binding<T, BindT, SetKind<>, SetKind<T...>>..., BindingsT...>;

        template<typename InterfaceT, typename... T>
        using bound = BindingFactory<Binding<InterfaceT, T, SetKind<>, SetKind<InterfaceT>>..., BindingsT...>;

        template<typename>
        struct merge_definition;
        template<typename... I>
        struct merge_definition<SetKind<I...>>
        {
            using type = BindingFactory<BindingsT..., I...>;
        };

        template<typename... T>
        using add = typename merge_definition<typename SetKind<>::template merge<T...>>::type;

        using build = StaticRepositoryBindings<BindingsT...>;
    };

    using make_factory = BindingFactory<>;

    template<typename ExportedInterface = SetKind<>, typename ExportedClass = SetKind<>>
    struct DependencyFactory
    {
        template<typename>
        struct remove_void;
        template<typename... I>
        struct remove_void<SetKind<void, I...>>
        {
            using type = SetKind<I...>;
        };

        template<typename, typename>
        struct generate_binding;
        template<typename T, typename... I>
        struct generate_binding<T, SetKind<I...>>
        {
            using type = SetKind<Binding<T, I>...>;
        };

        template<typename>
        struct generate_interface;
        template<typename... I>
        struct generate_interface<SetKind<I...>>
        {
            using type = typename SetKind<>::merge<
                typename generate_binding<I, typename ExportedClass::template instance_of<I>>::type...>::type::
                template make<StaticRepositoryBindings>;
        };

        template<typename I>
        using export_interface = DependencyFactory<typename ExportedInterface::template add_type<I>, ExportedClass>;

        template<typename T>
        using export_class = DependencyFactory<ExportedInterface, typename ExportedClass::template add_type<T>>;

        using build = typename generate_interface<ExportedInterface>::type;
    };

    using bind = DependencyFactory<>;
}// namespace core

#endif
