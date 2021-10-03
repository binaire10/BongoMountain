#ifndef GAMEENGINEBONGO_EXTERN_HPP
#define GAMEENGINEBONGO_EXTERN_HPP

#include <functional>
#include <Event/Event.h>
#include <benchmark/benchmark.h>
#include <Core/functional.hpp>

using event_handler_function = void(Event &);

using EventHandler = Core::function_handle<event_handler_function>;


event_handler_function *              getFunPtr();
std::function<event_handler_function> getStdFun();
std::function<event_handler_function> getStdFunPtr();
EventHandler       getHandlerF();
EventHandler       getHandlerC();


#endif// GAMEENGINEBONGO_EXTERN_HPP
