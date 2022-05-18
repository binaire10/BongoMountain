#include <graphic/Graphic.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unistd.h>
#include <render/VertexLayoutDescriptor.h>
#include <fstream>
#include <graphic/Image.hpp>
#include <core/Log.hpp>

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

uniform int width;
uniform int height;
uniform float time;

in vec2 pos;
out vec4 FragColor;

float rand(vec2 co) {
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float dotGridGradient(vec2 p0, vec2 p1) {
    return dot(vec2(rand(p0), rand(p0 / 5)), p1 - p0);
}

void main()
{
    vec2 posScreen = vec2(pos.x * width, pos.y * height);
    vec2 p = pos * 10;
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

    color = vec3(sin(value * p.x *1.1 + time*0.8), sin(value * 2 * (p.y + 1) + time), sin(value * (p.x + p.y) + time));

    FragColor = vec4(color, 1.0f);
})";

int main(int argc, const char **argv)
{
    static constexpr render::RenderDeviceInfo deviceInfo{
        1024, 720, render::opengl_t{}, render::window_t{ .name = "BongoMountain", .isVisible = true }
    };
    static constexpr glm::vec2 array_data[]{ glm::vec2{ 0, 0 }, glm::vec2{ 2, 0 }, glm::vec2{ 0, 2 } };
    static constexpr auto      layout =
        render::make_layout(render::AttributeLayout{ "aPos", render::ShaderDataType::Float2 });
    core::Log      log;
    core::Platform platform{ argc, argv };
    platform.load(platform.getApplicationPath().parent_path().append("extension"));
    platform.attach();
    {
        auto device = graphic::createRenderDevice(platform, deviceInfo);
        if(!device)
            return 0;
        device->make_current();
        device->clear();

        auto vbo    = device->createVertexBuffer(array_data);
        auto vao    = device->createVertexLayout(layout);
        auto shader = device->createShaderFromCode(shaderSource);

        shader.bind();
        auto projectionLocation = shader.getUniformLocation("projection");
        auto timeLocation       = shader.getUniformLocation("time");
        auto widthLocation      = shader.getUniformLocation("width");
        auto heightLocation     = shader.getUniformLocation("height");

        shader.setUniformValue(widthLocation, deviceInfo.width);
        shader.setUniformValue(heightLocation, deviceInfo.height);
        shader.setUniformValue(projectionLocation, glm::ortho(0, 1, 0, 1));
        shader.setUniformValue(timeLocation, 0.f);

        device->drawTriangles(vbo, vao, std::size(array_data));
        device->flush();
        sleep(5);
    }
}