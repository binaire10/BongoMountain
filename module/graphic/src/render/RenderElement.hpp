#ifndef BONGOMOUNTAIN_RENDERELEMENT_HPP
#define BONGOMOUNTAIN_RENDERELEMENT_HPP

namespace render
{
    class RenderDevice;
    class RenderElement
    {
    public:
        constexpr explicit RenderElement(RenderDevice *owner)
            : owner(owner)
        {}
        virtual ~RenderElement() = default;

        [[nodiscard]] constexpr RenderDevice *getOwner() const { return owner; }

    private:
        RenderDevice *owner;
    };
}// namespace render

#endif// BONGOMOUNTAIN_RENDERELEMENT_HPP
