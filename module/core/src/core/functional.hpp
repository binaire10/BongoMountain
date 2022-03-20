#ifndef BONGOMOUNTAIN_FUNCTIONAL_HPP
#define BONGOMOUNTAIN_FUNCTIONAL_HPP

#include <utility>
#include <atomic>

namespace core
{
    namespace detail
    {
        template<typename RetT, typename... ArgT>
        class function_abstract
        {
        public:
            virtual ~function_abstract() = default;
            virtual RetT call(ArgT...)   = 0;

            void acquireHandle() { ++counter; }
            bool releaseHandle() { return --counter == 0; }

        private:
            std::atomic<unsigned> counter = 0;
        };

        template<typename K, typename RetT, typename... ArgT>
        class function_instance
            : K
            , public function_abstract<RetT, ArgT...>
        {
        public:
            explicit function_instance(K &&obj) noexcept
                : K{ std::move(obj) }
            {}

            explicit function_instance(const K &obj)
                : K{ obj }
            {}

            RetT call(ArgT... arg) final { return K::operator()(std::forward<ArgT>(arg)...); }
        };

        template<typename RetT, typename... ArgT>
        class function_handler
        {
            using base_type = function_abstract<RetT, ArgT...>;
            template<typename K>
            using instance_type = function_instance<K, RetT, ArgT...>;

        public:
            function_handler() noexcept = default;

            inline function_handler(function_handler &&move) noexcept
                : handler{ move.handler }
            {
                move.handler = nullptr;
            }

            inline function_handler(const function_handler &cpy)
                : handler{ cpy.handler }
            {
                handler->acquireHandle();
            }

            template<typename K, typename = std::enable_if_t<!std::is_base_of_v<function_handler, std::decay_t<K>>>>
            function_handler(K &&fonctor) noexcept
                : handler{ new instance_type<std::decay_t<K>>{ fonctor } }
            {
                handler->acquireHandle();
            }

            template<typename K>
            function_handler(const K &fonctor)
                : handler{ new instance_type<std::decay_t<K>>{ fonctor } }
            {
                handler->acquireHandle();
            }

            inline function_handler(RetT (*ptr)(ArgT...))
                : function_handler([ptr](ArgT... arg) { return ptr(std::forward<ArgT>(arg)...); })
            {}

            inline ~function_handler()
            {
                if(handler && handler->releaseHandle())
                    delete handler;
            }

            inline function_handler &operator=(function_handler &&move) noexcept
            {
                if(handler && handler->releaseHandle())
                    delete handler;
                handler      = move.handler;
                move.handler = nullptr;
                return *this;
            }

            inline function_handler &operator=(const function_handler &cpy)
            {
                if(handler && handler->releaseHandle())
                    delete handler;
                handler = cpy.handler;
                handler->acquireHandle();
                return *this;
            }

            RetT operator()(ArgT... arg) const { return handler->call(std::forward<ArgT>(arg)...); }

            explicit operator bool() { return handler; }

            bool operator!() { return !handler; }

            bool operator==(const function_handler &hdl) const noexcept {
                return hdl.handler == handler;
            }

            bool operator!=(const function_handler &hdl) const noexcept {
                return hdl.handler != handler;
            }

        private:
            base_type *handler = nullptr;
        };


        template<typename ProtoT>
        struct get_function_impl
        {};

        template<typename RetT, typename... ArgT>
        struct get_function_impl<RetT(ArgT...)>
        {
            using type = function_handler<RetT, ArgT...>;
        };

        template<typename RetT, typename... ArgT>
        struct get_function_impl<RetT(ArgT...) noexcept>
        {
            using type = function_handler<RetT, ArgT...>;
        };
    }// namespace detail

    template<typename ProtoT>
    using function_handle = typename detail::get_function_impl<ProtoT>::type;
}// namespace core

#endif// BONGOMOUNTAIN_FUNCTIONAL_HPP
