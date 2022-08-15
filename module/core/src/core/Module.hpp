#ifndef BONGOMOUNTAIN_MODULE_HPP
#define BONGOMOUNTAIN_MODULE_HPP

#include <filesystem>
#include <string_view>
#include <typeindex>
#include <memory>
#include <any>
#include <list>
#include "Library.hpp"

namespace core
{
    inline namespace v_1_0
    {
        class ServiceDescription
        {
        public:
            [[nodiscard]] const std::type_index &getType() const { return type; }
            template<typename T>
            [[nodiscard]] T *create() const noexcept
            {
                return reinterpret_cast<T *>(create());
            }

        protected:
            [[nodiscard]] virtual void *create() const noexcept = 0;
            ServiceDescription(const std::type_index &type)
                : type(type)
            {}

        private:
            std::type_index type;
        };

        template<typename Base, typename X>
        struct BindInterface
        {
            using base_type = Base;
            using type      = X;
        };

        template<typename Binding>
        class ServiceDescriptionInstance : public ServiceDescription
        {
        public:
            ServiceDescriptionInstance()
                : ServiceDescription{ typeid(typename Binding::base_type) }
            {}

        protected:
            [[nodiscard]] void *create() const noexcept final { return new typename Binding::type; }

            static_assert(std::is_base_of_v<typename Binding::base_type, typename Binding::type>,
                          "type must be a primitive");
        };

        class ServiceLoader
        {
        public:
            virtual ~ServiceLoader()                                                        = default;
            [[nodiscard]] virtual std::vector<ServiceDescription *> getServiceDescription() = 0;
            template<typename T>
            [[nodiscard]] std::unique_ptr<T> create()
            {
                return reinterpret_cast<T *>(create(typeid(T)));
            }

        protected:
            virtual void *create(const std::type_index &type) = 0;
        };

        template<typename... T>
        class ServiceLoaderInstance
            : public ServiceLoader
            , private ServiceDescriptionInstance<T>...
        {
        public:
            std::vector<ServiceDescription *> getServiceDescription() final
            {
                return std::vector<ServiceDescription *>{ static_cast<ServiceDescriptionInstance<T> *>(this)... };
            }

        protected:
            void *create(const std::type_index &type) final
            {
                std::initializer_list<ServiceDescription *> servicesLoader{
                    static_cast<ServiceDescriptionInstance<T> *>(this)...
                };
                for(auto desc : servicesLoader)
                    if(desc->getType() == type)
                        return desc->create<void *>();
                return nullptr;
            }
        };

        class Module
        {
        public:
            inline explicit Module(std::filesystem::path            mfile,
                                   Library                        &&library,
                                   std::unique_ptr<ServiceLoader> &&loader)
                : m_name{ mfile.filename().string() }
                , m_path{ std::move(mfile) }
                , m_library{ std::move(library) }
                , m_serviceLoader{ std::move(loader) }
            {}

            inline explicit Module(std::string                      name,
                                   std::filesystem::path            mfile,
                                   Library                        &&library,
                                   std::unique_ptr<ServiceLoader> &&loader)
                : m_name{ std::move(name) }
                , m_path{ std::move(mfile) }
                , m_library{ std::move(library) }
                , m_serviceLoader{ std::move(loader) }
            {}

            [[nodiscard]] inline const std::unique_ptr<ServiceLoader> &getServiceLoader() { return m_serviceLoader; }

            [[nodiscard]] inline Library &getEntry() { return m_library; }

            [[nodiscard]] inline const std::string &getName() const { return m_name; }

        private:
            std::string                    m_name;
            std::filesystem::path          m_path;
            Library                        m_library;
            std::unique_ptr<ServiceLoader> m_serviceLoader;
        };
    }// namespace v_1_0
}// namespace core

#endif// BONGOMOUNTAIN_MODULE_HPP
