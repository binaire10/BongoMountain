#include "Extern.hpp"
#include "benchmark/benchmark.h"

using fun_sym = void(Event&);

namespace
{
    auto fun = [](Event &h) { benchmark::DoNotOptimize(h); };
    fun_sym *ptr = fun;
}

event_handler_function *getFunPtr() { return ptr; }

EventHandler getHandlerF() { return EventHandler{ ptr }; }

EventHandler getHandlerC() { return EventHandler{ fun }; }

std::function<event_handler_function> getStdFun() { return fun; }

std::function<event_handler_function> getStdFunPtr() { return ptr; }
