#include "ParticleManager.hpp"
#include <algorithm>
#include <Graphic/RenderCommand.hpp>

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

layout(location = 0) out VertexOutput {
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

void main()
{
    float timeStep = u_Timestamp - a_StartTime;
	Output.Position = a_Position + a_Speed * timeStep;
    Output.Size = a_Size;


    vec4 p = vec4(a_Rotation.yzw, a_Rotation.x);
    vec4 q = vec4(sin(asin(a_RotationSpeed.yzw) * timeStep), cos(acos(a_RotationSpeed.x) * timeStep));

    Output.Rotation = quad_mult(p, q);
	Output.TexIndex = a_TexIndex;
	Output.Color = a_Color;
}

#type geometry
#version 450 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout(location = 0) in VertexInput {
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
    ) * mat4(
        1 - 2 * rotate.y*rotate.y - 2 * rotate.z*rotate.z,
        2 * rotate.x*rotate.y + 2 * rotate.z*rotate.w,
        2 * rotate.x*rotate.z - 2 * rotate.y*rotate.w,
        0,

        2 * rotate.x*rotate.y - 2 * rotate.z*rotate.w,
        1 - 2 * rotate.x*rotate.x - 2 * rotate.z*rotate.z,
        2 * rotate.y*rotate.z + 2 * rotate.x*rotate.w,
        0,

        2 * rotate.y*rotate.z + 2 * rotate.y*rotate.w,
        2 * rotate.y*rotate.z - 2 * rotate.x*rotate.w,
        2 * rotate.x*rotate.x - 2 * rotate.y*rotate.y,
        0,

        0, 0, 0, 1
    );

    gl_Position = transform * vec4(vec3(-size, 0), 1);
    Output.TexCoord = vec2(0, 0);
    EmitVertex();

    gl_Position = transform * vec4(vec3(-size.x,  size.y, 0), 1);
    Output.TexCoord = vec2(0, 1);
    EmitVertex();

    gl_Position = transform * vec4(vec3( size.x, -size.y, 0), 1);
    Output.TexCoord = vec2(1, 0);
    EmitVertex();

    gl_Position = transform * vec4(vec3(size, 0), 1);
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

    static constexpr BufferLayout layout_buffer =
        make_layout(BufferElement{ ShaderDataType::Float3, "a_Position" },
                    BufferElement{ ShaderDataType::Float2, "a_Size" },
                    BufferElement{ ShaderDataType::Float4, "a_Rotation" },
                    BufferElement{ ShaderDataType::Float4, "a_RotationSpeed" },
                    BufferElement{ ShaderDataType::Float3, "a_Speed" },
                    BufferElement{ ShaderDataType::Float4, "a_Color" },
                    BufferElement{ ShaderDataType::Int, "a_TexIndex" },
                    BufferElement{ ShaderDataType::Float, "a_StartTime" },
                    BufferElement{ ShaderDataType::Float, "a_FinishTime" },
                    BufferElement{ ShaderDataType::Int, "a_ParticleId" });

    static_assert(sizeof(Render::ParticleManager::Particle) == layout_buffer.stride);
}// namespace

void Render::ParticleManager::initialize(unsigned int particleMax)
{
    static constexpr uint32_t white = static_cast<uint32_t>(0xFF'FF'FF'FF);
    m_white                         = Graphic::Texture::load_texture(&white, 1, 1, Graphic::TextureFormat::RGBA_8bit);

    m_buffers = Graphic::SharedVertexBufferArray<Particle>::create(particleMax);
    m_vao     = Graphic::BasicVertexArray::create(layout_buffer, m_buffers, 0);

    m_shader = Graphic::Shader::createFromCode(shader_source);
    m_textures.reserve(32);

    m_particles.reserve(particleMax);

    BM_CORE_INFO("Create Particle manager with {} max particle handle of {} octets", particleMax, sizeof(Particle));
    BM_CORE_INFO("Memory use {} octets", m_particles.capacity() * sizeof(Particle));
}

void Render::ParticleManager::configure(glm::mat4 projection, float currentTime)
{
    m_currentTime = currentTime;
    m_shader.bind();
    m_shader.setUniformValue(u_Timestamp, currentTime);
    m_shader.setUniformValue(u_ViewProjection, projection);
    while(m_particles.size() > 0 && m_particles.back().finishTime < m_currentTime)
    {
        m_particles.pop_back();
        m_isUpdate = true;
    }
}

void Render::ParticleManager::draw()
{
    if(m_isUpdate)
    {
        m_buffers.set(0, m_particles.data(), m_particles.size());
        m_isUpdate = false;
    }
    for(unsigned i{}; i < m_textures.size(); ++i) m_textures[i]->bind(i);
    Graphic::RenderCommand::drawPoints(m_shader, m_vao, m_particles.size());
}
LightUid Render::ParticleManager::addParticle(glm::vec3         position,
                                      glm::vec2         size,
                                      glm::vec3         speed,
                                      glm::quat         rotation,
                                      glm::quat         rotationSpeed,
                                      float             ttl,
                                      glm::vec4         color,
                                      Graphic::Texture *texture)
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

    Particle newParticle{ position,
                          size,
                          rotation,
                          rotationSpeed,
                          speed,
                          color,
                          (int)std::distance(m_textures.begin(), pos),
                          startTime,
                          finishTime,
                          LightUid::generate() };
    if(m_particles.size() == m_particles.capacity())
        m_particles.pop_back();

    m_particles.emplace(
        std::lower_bound(m_particles.begin(),
                         m_particles.end(),
                         newParticle,
                         [](const Particle &a, const Particle &b) { return a.finishTime > b.finishTime; }),
        newParticle);
    return newParticle.particleId;
}
