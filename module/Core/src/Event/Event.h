#ifndef EVENT_H
#define EVENT_H

#include <string_view>
#include <Core/config.hpp>

#define BIT(X) (1u << (X))

enum class EventType {
    WindowResize,
    WindowClose,
    KeyPressed,
    KeyReleased,
    KeyTyped,
    MouseMoved,
    MouseScrolled,
    MouseButtonPressed,
    MouseButtonReleased
};

enum class EventCategory {
    None        = 0,
    Application = BIT(0u),
    Input       = BIT(1u),
    Keyboard    = BIT(2u),
    Mouse       = BIT(3u),
    MouseButton = BIT(4u),
};
#undef BIT

constexpr EventCategory operator|(EventCategory e1, EventCategory e2)
{
    using Number = std::underlying_type_t<EventCategory>;
    return static_cast<EventCategory>(static_cast<Number>(e1) | static_cast<Number>(e2));
}

class Event
{
public:
    [[nodiscard]] virtual EventCategory    getCategory() const noexcept = 0;
    [[nodiscard]] virtual EventType        getType() const noexcept     = 0;
    [[nodiscard]] virtual std::string_view getName() const noexcept     = 0;

    [[nodiscard]] constexpr bool isHandle() const noexcept { return m_handle; }

    inline void setHandle(bool handle) { m_handle = handle; }

    inline bool IsInCategory(EventCategory category)
    {
        using Number = std::underlying_type_t<EventCategory>;
        return static_cast<Number>(getCategory()) & static_cast<Number>(category);
    }

private:
    bool m_handle = false;
};

#define EVENT_CLASS_TYPE(X)                                                                       \
                                                                                                  \
    static constexpr const std::string_view eventName = #X;                                       \
    static constexpr EventType              getStaticType() noexcept { return EventType::X; }     \
    static constexpr const char *           getStaticName() noexcept { return eventName.data(); } \
    EventType                               getType() const noexcept override;                    \
    std::string_view                        getName() const noexcept override;

#define EVENT_CLASS_CATEGORY(X)                                               \
    static constexpr EventCategory getStaticCategory() noexcept { return X; } \
    EventCategory                  getCategory() const noexcept override;

#define EVENT_CLASS_TYPE_IMPL(C)                                             \
    EventType        C::getType() const noexcept { return getStaticType(); } \
    std::string_view C::getName() const noexcept { return getStaticName(); }

#define EVENT_CLASS_CATEGORY_IMPL(C) \
    EventCategory C::getCategory() const noexcept { return getStaticCategory(); }


#endif// EVENT_H
