#include "CoreApplication.hpp"
#include <Core/Assert.hpp>

CoreApplication *CoreApplication::app = nullptr;

CoreApplication::CoreApplication() noexcept
{
    BM_ASSERT(!app, "application already create");
    app = this;
}

void CoreApplication::dispatchEvent(Event &event) noexcept
{
    BM_ASSERT(app, "application not found");
    app->dispatch(event);
}
void CoreApplication::dispatch(Event &event) noexcept
{
    for(auto &l : listeners) l->handle(event);
}

CoreApplication *CoreApplication::getApp() { return app; }

int CoreApplication::exec()
{
    BM_ASSERT(mainLoop, "undefined process");
    running = true;

    while(running) mainLoop();
    return 0;
}
