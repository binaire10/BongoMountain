#include <glm/gtc/matrix_transform.hpp>
#include <render/VertexLayoutDescriptor.h>
#include <fstream>
#include <core/Platform.hpp>
#include <core/Log.hpp>
#include <event/Event.hpp>
#include <event/EventDispatcher.hpp>
#include "graphic/ImageLoader.hpp"
#include "graphic/Graphic.hpp"
#include "render/RenderDevice.hpp"
#include "event/application/WindowResizeEvent.h"
#include "event/application/WindowCloseEvent.h"

constexpr std::string_view shaderSource = R"(#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;

out vec2 pos;
uniform mat4 projection;

void main(){
    pos = aPos.xy;
    gl_Position = projection * vec4(aPos, 1);
}
#type fragment
#version 330 core

uniform float time;
uniform float width;
uniform float height;

in vec2 pos;
out vec4 FragColor;

float rand(vec2 co) {
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float dotGridGradient(vec2 p0, vec2 p1) {
    return dot(vec2(rand(p0), rand(p0 / 5)), p1 - p0);
}

vec3 computeColor(vec2 p) {
    vec2 p0 = floor(p);
    vec2 p1 = p0 + vec2(1,1);

    vec3 color =  vec3(0,0,0);

    float n0, n1, ix0, ix1, value;
    n0 = dotGridGradient(p0, p);
    n1 = dotGridGradient(vec2(p0.x, p0.y), p);
    ix0 = mix(n0, n1, p.x - p0.x);
    n0 = dotGridGradient(vec2(p0.x, p1.y), p);
    n1 = dotGridGradient(p1, p);
    ix1 = mix(n0, n1, p.x - p0.x);
    value = mix(ix0, ix1, p.y - p0.y);
    return color = vec3(sin(value * p.x *1.1 + time*0.8), sin(value * 2 * (p.y + 1) + time), sin(value * (p.x + p.y) + time));
}

void main()
{
    vec2 posScreen = vec2(pos.x * width, pos.y * height);
    vec3 color = vec3(0,0,0);

    for(int x = 0; x < 13 ; ++x) {
        for(int y = 0; y < 13 ; ++y) {
            color += computeColor(posScreen + vec2(1.0/(width*width) * (float(x-6) / 6), 1.0/(height*height) * (float(y-6) / 6)));
        }
    }

    FragColor = vec4(color / (13*13), 1.0f);
})";

class CustomLayer final : public core::Layer
{
public:
    static constexpr render::RenderDeviceInfo deviceInfo{
        1600, 1200, render::opengl_t{}, render::window_t{ .name = "BongoMountain", .isVisible = true }
    };
    static constexpr glm::vec2 array_data[]{ glm::vec2{ 0, 0 }, glm::vec2{ 2, 0 }, glm::vec2{ 0, 2 } };
    static constexpr auto      layout =
        render::make_layout(render::AttributeLayout{ "aPos", render::ShaderDataType::Float2 });
    void onAttach() override
    {
        device = graphic::createRenderDevice(core::Platform::getInstance(), deviceInfo);
        if(!device)
            return;

        vbo    = device->createVertexBuffer(array_data);
        vao    = device->createVertexLayout(layout);
        shader = device->createShaderFromCode(shaderSource);
        start  = std::chrono::high_resolution_clock::now();

        shader.bind();
        projectionLocation = shader.getUniformLocation("projection");
        timeLocation       = shader.getUniformLocation("time");
        widthLocation      = shader.getUniformLocation("width");
        heightLocation     = shader.getUniformLocation("height");
    }

    void onDetach() override
    {
        vao.destroy();
        vbo.destroy();
        shader.destroy();
        device = nullptr;
    }

    void onBegin() override
    {
        device->make_current();
        device->clear();
    }
    void onEnd() override {}

    void onUpdate() override
    {
        shader.bind();
        shader.setUniformValue(widthLocation, float(deviceInfo.width / 80));
        shader.setUniformValue(heightLocation, float(deviceInfo.height / 80));
        shader.setUniformValue(projectionLocation, glm::ortho(0, 1, 0, 1));
        shader.setUniformValue(timeLocation, std::chrono::duration_cast<std::chrono::duration<float>>(
                                                 std::chrono::high_resolution_clock::now() - start)
                                                 .count());
        device->drawTriangles(vbo, vao, std::size(array_data));
        device->flush();
    }

    void onEvent(Event &event) override
    {
        EventDispatcher dispatcher{ event };
        dispatcher.dispatch<WindowResizeEvent>([this](const WindowResizeEvent &event) {
            shader.setUniformValue(widthLocation, float(event.getWidth() / 80));
            shader.setUniformValue(heightLocation, float(event.getHeight() / 80));
            device->viewport(glm::vec2(event.getWidth(), event.getHeight()));
            onBegin();
            onUpdate();
            onEnd();
        });
        dispatcher.dispatch<WindowCloseEvent>([](auto &e) { core::Platform::getInstance().exit(); });
    }

private:
    std::unique_ptr<render::RenderDevice>          device;
    render::VertexBufferObject                     vbo;
    render::VertexLayout                           vao;
    render::Shader                                 shader;
    std::chrono::high_resolution_clock::time_point start;

    int timeLocation       = -1;
    int projectionLocation = -1;
    int widthLocation      = -1;
    int heightLocation     = -1;
};

int main(int argc, const char **argv)
{
    core::Log      log;
    core::Platform platform{ argc, argv };
    platform.load(platform.getApplicationPath().parent_path().append("extension"));
    platform.attach();

    graphic::ImageLoader imageLoader;

    platform.addLayer(std::make_shared<CustomLayer>());

    platform.exec();
}