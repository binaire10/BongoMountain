//
// Created by binaire on 20/06/2021.
//

#ifndef GAMEENGINEBONGO_COREAPPLICATION_HPP
#define GAMEENGINEBONGO_COREAPPLICATION_HPP

#include "config.hpp"
#include <Event/Event.h>

#include <memory>
#include <list>
#include <functional>
#include <atomic>
#include "EventListener.hpp"

class BM_CORE_DCL CoreApplication
{
public:
    CoreApplication(const CoreApplication &) = delete;
    CoreApplication &operator=(const CoreApplication &) = delete;

    CoreApplication() noexcept;
    virtual ~CoreApplication() = default;
    static void dispatchEvent(Event &event) noexcept;
    void        dispatch(Event &event) noexcept;

    int exec();

    [[nodiscard]] inline const std::list<std::unique_ptr<EventListener>> &getListeners() const noexcept
    {
        return listeners;
    }

    [[nodiscard]] inline std::list<std::unique_ptr<EventListener>> &getListeners() noexcept { return listeners; }
    static CoreApplication *                                        getApp();

    inline void addListener(std::unique_ptr<EventListener> listener) { listeners.push_back(std::move(listener)); }


    [[nodiscard]] inline bool isRunning() const noexcept { return running; }

    inline void exit(int exitCode) noexcept
    {
        CoreApplication::exitCode = exitCode;
        running                   = false;
    }

    inline std::function<void()> swapMainLoop(std::function<void()> loop)
    {
        std::swap(loop, mainLoop);
        return std::move(loop);
    }

    inline void setMainLoop(std::function<void()> mainLoop) { CoreApplication::mainLoop = std::move(mainLoop); }

private:
    static CoreApplication *app;

    std::list<std::unique_ptr<EventListener>> listeners;
    std::atomic<bool>                         running = false;
    int                                       exitCode;
    std::function<void()>                     mainLoop;
};


#endif// GAMEENGINEBONGO_COREAPPLICATION_HPP
