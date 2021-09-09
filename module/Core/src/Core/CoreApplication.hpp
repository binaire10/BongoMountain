//
// Created by binaire on 20/06/2021.
//

#ifndef GAMEENGINEBONGO_COREAPPLICATION_HPP
#define GAMEENGINEBONGO_COREAPPLICATION_HPP

#include "config.hpp"

#include "functional.hpp"
#include <Event/Event.h>

#include <memory>
#include <list>
#include <functional>
#include <atomic>

class BM_CORE_DCL CoreApplication
{
public:
    using EventHandle = Core::function_handle<void(Event &)>;

    CoreApplication(const CoreApplication &) = delete;
    CoreApplication &operator=(const CoreApplication &) = delete;

    CoreApplication() noexcept;
    virtual ~CoreApplication() = default;
    static void        dispatchEvent(Event &event) noexcept;
    static inline void dispatchEvent(Event &&event) noexcept { dispatchEvent(event); }
    void               dispatch(Event &event) noexcept;

    int exec();

    [[nodiscard]] inline const std::list<EventHandle> &getListeners() const noexcept { return listeners; }

    [[nodiscard]] inline std::list<EventHandle> &getListeners() noexcept { return listeners; }

    static CoreApplication *getApp() { return app; }

    inline void addListener(EventHandle &&listener) { listeners.push_back(std::move(listener)); }
    inline void addListener(const EventHandle &listener) { listeners.push_back(listener); }

    inline void removeListener(const EventHandle &listener)
    {
        listeners.remove(listener);
    }

    [[nodiscard]] inline bool isRunning() const noexcept { return running; }

    inline void exit(int exitCode) noexcept
    {
        CoreApplication::exitCode = exitCode;
        running                   = false;
    }

    inline Core::function_handle<void()> swapMainLoop(Core::function_handle<void()> loop)
    {
        std::swap(loop, mainLoop);
        return std::move(loop);
    }

    inline void setMainLoop(Core::function_handle<void()> mainLoop) { CoreApplication::mainLoop = std::move(mainLoop); }

private:
    static CoreApplication *app;

    std::list<EventHandle>        listeners;
    std::atomic<bool>             running = false;
    int                           exitCode;
    Core::function_handle<void()> mainLoop;
};


#endif// GAMEENGINEBONGO_COREAPPLICATION_HPP
