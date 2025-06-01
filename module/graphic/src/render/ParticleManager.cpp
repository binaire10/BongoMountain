#include "ParticleManager.hpp"
#include <algorithm>
#include <graphic/Image.hpp>
#include "RenderDevice.hpp"
#include "VertexLayoutDescriptor.h"

namespace
{
    static constexpr int u_ViewProjection = 1;
    static constexpr int u_Timestamp      = 0;

    static constexpr std::string_view shader_source = R"(
// Basic Texture Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_Size;
layout(location = 2) in vec4 a_Rotation;
layout(location = 3) in vec4 a_RotationSpeed;
layout(location = 4) in vec3 a_Speed;
layout(location = 5) in vec4 a_Color;
layout(location = 6) in int a_TexIndex;
layout(location = 7) in int a_StartTime;

layout(location = 0) out VertexShaderOutput {
    vec3 Position;
    vec2 Size;
    vec4 Rotation;
	vec4 Color;
    flat int TexIndex;
} Output;

layout(location = 0) uniform float u_Timestamp;

float modulus(float a, float b) {
    return a - (b * floor(a/b));
}

vec4 quad_mult(vec4 p, vec4 q) {
    return vec4(
        p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y,
        p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z,
        p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x,
        p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z
    );
}

vec4 deltaQuat(vec4 q_angular, float deltaTime) {
        float angle = 2.0 * acos(q_angular.w);
        vec3 axis = normalize(q_angular.xyz);

        float deltaAngle = angle * deltaTime;
        float halfAngle = deltaAngle / 2.f;
        float s = sin(halfAngle);
        return normalize(vec4(axis * s, cos(halfAngle)));
}


void main()
{
    float timeStep = u_Timestamp - a_StartTime;
	Output.Position = a_Position + a_Speed * timeStep;
    Output.Size = a_Size;


    vec4 q = deltaQuat(a_RotationSpeed, timeStep);

    Output.Rotation = quad_mult(q, a_Rotation);
	Output.TexIndex = a_TexIndex;
	Output.Color = a_Color;
}

#type geometry
#version 450 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout(location = 0) in VertexShaderOutput {
    vec3 Position;
    vec2 Size;
    vec4 Rotation;
    vec4 Color;
    flat int TexIndex;
} Input[];


layout(location = 0) out Vertex {
    vec4 Color;
    vec2 TexCoord;
    flat int TexIndex;
} Output;

layout(location = 1) uniform mat4 u_ViewProjection;

vec3 rotate_vertex_position(vec3 position, vec4 q)
{
  vec3 v = position.xyz;
  return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
}

void main()
{
    vec2 size = Input[0].Size / 2.0f;
    Output.TexIndex = Input[0].TexIndex;
    Output.Color = Input[0].Color;

    vec4 rotate = Input[0].Rotation;
    vec3 position = Input[0].Position;

    mat4 transform = u_ViewProjection * mat4(
        vec4(1, 0, 0, 0),
        vec4(0, 1, 0, 0),
        vec4(0, 0, 1, 0),
        vec4(position, 1)
    );

    gl_Position = transform * vec4(rotate_vertex_position(vec3(-size/2, 0), rotate), 1);
    Output.TexCoord = vec2(0, 0);
    EmitVertex();

    gl_Position = transform * vec4(rotate_vertex_position(vec3(-size.x/2,  size.y/2, 0), rotate), 1);
    Output.TexCoord = vec2(0, 1);
    EmitVertex();

    gl_Position = transform * vec4(rotate_vertex_position(vec3( size.x/2, -size.y/2, 0), rotate), 1);
    Output.TexCoord = vec2(1, 0);
    EmitVertex();

    gl_Position = transform * vec4(rotate_vertex_position(vec3(size/2, 0), rotate), 1);
    Output.TexCoord = vec2(1, 1);
    EmitVertex();
    EndPrimitive();
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;


layout(location = 0) in Vertex {
    vec4 Color;
    vec2 TexCoord;
    flat int TexIndex;
} Input;

layout (binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	color = Input.Color * texture(u_Textures[Input.TexIndex], Input.TexCoord);
}
)";

    constexpr auto layout_buffer =
            render::make_layout(render::AttributeLayout{ "a_Position", render::ShaderDataType::Float3 },
                                render::AttributeLayout{ "a_Size", render::ShaderDataType::Float2 },
                                render::AttributeLayout{ "a_Rotation", render::ShaderDataType::Float4 },
                                render::AttributeLayout{ "a_RotationSpeed", render::ShaderDataType::Float4 },
                                render::AttributeLayout{ "a_Speed", render::ShaderDataType::Float3 },
                                render::AttributeLayout{ "a_Color", render::ShaderDataType::Float4 },
                                render::AttributeLayout{ "a_TexIndex", render::ShaderDataType::Int },
                                render::AttributeLayout{ "a_StartTime", render::ShaderDataType::Float },
                                render::AttributeLayout{ "a_FinishTime", render::ShaderDataType::Float }/*,
                                render::AttributeLayout{ "a_ParticleId", render::ShaderDataType::Int }*/);

    static constexpr auto compare_particle = [](const render::ParticleManager::Particle &a,
                                                const render::ParticleManager::Particle &b) {
        return a.finishTime > b.finishTime;
    };

    static_assert(sizeof(render::ParticleManager::Particle) == layout_buffer.stride);
}// namespace

void render::ParticleManager::initialize(unsigned int particleMax, RenderDevice *device_)
{
    static constexpr uint32_t white = 0xFF'FF'FF'FFu;

    this->device = device_;

    m_white = device->createTexture(
        graphic::ImageView(1, 1, graphic::TextureFormat::RGBA_8bit, reinterpret_cast<const std::byte *>(&white)));
    m_buffers = device->createVertexBuffer(particleMax * sizeof(Particle));
    m_vao     = device->createVertexLayout(layout_buffer);
    //    m_vao     = BasicVertexArray::create(layout_buffer, m_buffers, 0);

    m_shader = device->createShaderFromCode(shader_source);
    m_textures.reserve(32);

    m_particles.reserve(particleMax);

    BM_CORE_INFO("Create Particle manager with {} max particle handle of {} octets", particleMax, sizeof(Particle));
    BM_CORE_INFO("Memory use {} octets", m_particles.capacity() * sizeof(Particle));
}

void render::ParticleManager::configure(glm::mat4 projection, float currentTime)
{
    m_currentTime = currentTime;
    m_shader.bind();
    m_shader.setUniformValue(u_Timestamp, currentTime);
    m_shader.setUniformValue(u_ViewProjection, projection);
    while(m_particles.size() > 0 && m_particles.front().finishTime < m_currentTime)
    {
        std::pop_heap(m_particles.begin(), m_particles.end(), compare_particle);
        m_particles.pop_back();
        m_isUpdate = true;
    }
}

void render::ParticleManager::draw()
{
    if(m_isUpdate)
    {
        m_buffers.overwrite(0, m_particles.data(), m_particles.size());
        m_isUpdate = false;
    }
    for(unsigned i{}; i < m_textures.size(); ++i) m_textures[i]->bind(i);
    device->drawPoints(m_buffers, m_vao, m_particles.size());
}
void render::ParticleManager::addParticle(glm::vec3        position,
                                          glm::vec2        size,
                                          glm::vec3        speed,
                                          glm::quat        rotation,
                                          glm::quat        rotationSpeed,
                                          float            ttl,
                                          glm::vec4        color,
                                          render::Texture *texture)
{
    m_isUpdate       = true;
    float startTime  = m_currentTime;
    float finishTime = startTime + ttl;
    texture          = texture ? texture : &m_white;
    auto pos         = m_textures.end();

    if((pos = std::find(m_textures.begin(), m_textures.end(), texture)) == m_textures.end())
    {
        if(m_textures.size() != m_textures.capacity())
            pos = m_textures.insert(m_textures.end(), texture);
        else
            throw std::overflow_error{ "too many texture use" };
    }

    Particle newParticle{
        position,  size,       rotation, rotationSpeed, speed, color, (int)std::distance(m_textures.begin(), pos),
        startTime, finishTime, /*
                    LightUid::generate()*/
    };
    if(m_particles.size() == m_particles.capacity())
    {
        std::pop_heap(m_particles.begin(), m_particles.end(), compare_particle);
        m_particles.pop_back();
    }

    m_particles.emplace_back(newParticle);
    std::push_heap(m_particles.begin(), m_particles.end(), compare_particle);
    //    return newParticle.particleId;
}

void render::ParticleManager::destroy()
{
    m_white.destroy();
    m_buffers.destroy();
    m_vao.destroy();
    m_shader.destroy();
    m_textures.clear();
    m_particles.clear();
}
