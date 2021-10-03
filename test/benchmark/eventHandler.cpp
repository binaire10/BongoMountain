#include <benchmark/benchmark.h>
#include <functional>
#include <Event/Application/WindowCloseEvent.h>
#include "Extern.hpp"

void rawFunctionCall(benchmark::State &state) noexcept
{
    WindowCloseEvent             wc{ nullptr };
    auto e = getFunPtr();
    for(auto _ : state)
    {
        benchmark::DoNotOptimize(e);
        for(int i = 0; i < 100; ++i) e(wc);
    }
}

void stdFunctionCall(benchmark::State &state) noexcept
{
    WindowCloseEvent             wc{ nullptr };
    std::function<void(Event &)> e{ getStdFun() };
    for(auto _ : state)
    {
        benchmark::DoNotOptimize(e);
        for(int i = 0; i < 100; ++i) e(wc);
    }
}

void stdFunctionPtrCall(benchmark::State &state) noexcept
{
    WindowCloseEvent             wc{ nullptr };
    std::function<void(Event &)> e{ getStdFunPtr() };
    for(auto _ : state)
    {
        benchmark::DoNotOptimize(e);
        for(int i = 0; i < 100; ++i) e(wc);
    }
}

void abstractClass(benchmark::State &state) noexcept
{
    WindowCloseEvent wc{ nullptr };
    EventHandler     e{ getHandlerC() };
    for(auto _ : state)
    {
        benchmark::DoNotOptimize(e);
        for(int i = 0; i < 100; ++i) e(wc);
    }
}

void abstractFunction(benchmark::State &state) noexcept
{
    WindowCloseEvent wc{ nullptr };
    EventHandler     e{ getHandlerF() };
    for(auto _ : state)
    {
        benchmark::DoNotOptimize(e);
        for(int i = 0; i < 100; ++i) e(wc);
    }
}

BENCHMARK(abstractClass);
BENCHMARK(abstractFunction);
BENCHMARK(stdFunctionCall);
BENCHMARK(stdFunctionPtrCall);
BENCHMARK(rawFunctionCall);

BENCHMARK_MAIN();