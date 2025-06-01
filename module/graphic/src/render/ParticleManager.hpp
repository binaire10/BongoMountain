#ifndef GAMEENGINEBONGO_PARTICLEMANAGER_HPP
#define GAMEENGINEBONGO_PARTICLEMANAGER_HPP

#include <graphic/config.hpp>
#include <render/VertexLayout.hpp>
#include <render/VertexBufferObject.hpp>
#include <render/Texture.hpp>
#include <render/Shader.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace render
{
    class BM_GRAPHIC_DCL ParticleManager
    {
    public:
        struct Particle
        {
            glm::vec3 position;
            glm::vec2 size;
            glm::quat rotation;
            glm::quat rotationSpeed;
            glm::vec3 speed;
            glm::vec4 color;
            int       texIndex;
            float     startTime;
            float     finishTime;
        };
        void initialize(unsigned particleMax, RenderDevice *device_);

        void configure(glm::mat4 projection, float currentTime);

        void addParticle(glm::vec3 position,
                         glm::vec2 size,
                         glm::vec3 speed,
                         glm::quat rotation,
                         glm::quat rotationSpeed,
                         float     ttl,
                         glm::vec4 color,
                         Texture  *texture = nullptr);


        void draw();

        void destroy();

    private:
        bool                   m_isUpdate;
        float                  m_currentTime;
        Texture                m_white;
        VertexBufferObject     m_buffers;
        VertexLayout           m_vao;
        Shader                 m_shader;
        std::vector<Texture *> m_textures;
        std::vector<Particle>  m_particles;
        RenderDevice          *device;
    };
}// namespace render

#endif// GAMEENGINEBONGO_PARTICLEMANAGER_HPP
