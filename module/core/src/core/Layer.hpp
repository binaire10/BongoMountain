#ifndef BONGOMOUNTAIN_LAYER_HPP
#define BONGOMOUNTAIN_LAYER_HPP

class Event;

namespace core
{
    class Layer
    {
    public:
        constexpr explicit Layer(int priority = 0)
            : priority{ priority }
        {}

        virtual ~Layer() = default;

        virtual void onAttach()            = 0;
        virtual void onDetach()            = 0;
        virtual void onBegin()             = 0;
        virtual void onEnd()               = 0;
        virtual void onUpdate()            = 0;
        virtual void onEvent(Event &event) = 0;

        [[nodiscard]] constexpr int getPriority() const { return priority; }

    protected:
        int priority;
    };
}// namespace core

#endif// BONGOMOUNTAIN_LAYER_HPP
