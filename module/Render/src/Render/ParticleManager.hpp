#ifndef GAMEENGINEBONGO_PARTICLEMANAGER_HPP
#define GAMEENGINEBONGO_PARTICLEMANAGER_HPP

#include "config.hpp"
#include <Core/LightUid.hpp>
#include <Graphic/VertexArray.hpp>
#include <Graphic/BufferArray.hpp>
#include <Graphic/Texture.hpp>
#include <Graphic/Shader.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

namespace Render
{
    class BM_RENDER_DCL ParticleManager
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
            LightUid  particleId;
        };
        void initialize(unsigned particleMax);

        void configure(glm::mat4 projection, float currentTime);

        LightUid addParticle(glm::vec3         position,
                             glm::vec2         size,
                             glm::vec3         speed,
                             glm::quat         rotation,
                             glm::quat         rotationSpeed,
                             float             ttl,
                             glm::vec4         color,
                             Graphic::Texture *texture = nullptr);

        void remove(LightUid uid);

        void draw();

    private:
        bool                                       m_isUpdate;
        float                                      m_currentTime;
        Graphic::Texture                           m_white;
        Graphic::SharedVertexBufferArray<Particle> m_buffers;
        Graphic::BasicVertexArray                  m_vao;
        Graphic::Shader                            m_shader;
        std::vector<Graphic::Texture *>            m_textures;
        std::vector<Particle>                      m_particles;
    };
}// namespace Render

#endif// GAMEENGINEBONGO_PARTICLEMANAGER_HPP
