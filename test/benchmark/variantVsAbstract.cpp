#include <variant>
#include <memory>
#include <benchmark/benchmark.h>

namespace SC {
    struct A {
        void do_somethig() {
            ++i;
            benchmark::DoNotOptimize(i);
        }
        unsigned i;
    };

    struct B {
        void do_somethig() {
            --i;
            benchmark::DoNotOptimize(i);
        }
        unsigned i;
    };

    struct C {
        void do_somethig() {
            auto k = i;
            i = k*k;
            benchmark::DoNotOptimize(i);
        }
        unsigned i;
    };

    std::variant<A, B, C> randomValue() {
        unsigned index = unsigned(rand()) % 3;
        switch(index) {
        case 0:
            return A{};
        case 1:
            return B{};
        }
        return C{};
    }
}
namespace AB {
    struct Base {
        virtual void do_somethig() = 0;
    };

    struct A : Base {
        void do_somethig() final {
            --i;
            benchmark::DoNotOptimize(i);
        }
        unsigned i;
    };

    struct B : Base {
        void do_somethig() final {
            auto k = i;
            i = k*k;
            benchmark::DoNotOptimize(i);
        }
        unsigned i;
    };

    struct C : Base {
        void do_somethig() final {
            ++i;
            benchmark::DoNotOptimize(i);
        }
        unsigned i;
    };

    std::unique_ptr<Base> randomValue() {
        unsigned index = unsigned(rand()) % 3;
        switch(index) {
        case 0:
            return std::make_unique<A>();
        case 1:
            return std::make_unique<B>();
        }
        return std::make_unique<C>();
    }
}

static void variantCall(benchmark::State& state) {
    // Code inside this loop is measured repeatedly
    std::variant<SC::A, SC::B, SC::C> variant = SC::randomValue();
    for (auto _ : state) {
        std::visit([](auto &a){
            a.do_somethig();
        }, variant);
        benchmark::DoNotOptimize(variant);
    }
}
// Register the function as a benchmark
BENCHMARK(variantCall);

static void abstractCall(benchmark::State& state) {
    // Code inside this loop is measured repeatedly
    std::unique_ptr<AB::Base> variant = AB::randomValue();
    for (auto _ : state) {
        variant->do_somethig();
        benchmark::DoNotOptimize(variant);
    }
}
// Register the function as a benchmark
BENCHMARK(abstractCall);

BENCHMARK_MAIN();
