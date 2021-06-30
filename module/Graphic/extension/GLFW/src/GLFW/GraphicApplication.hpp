//
// Created by binaire on 28/06/2021.
//

#ifndef GAMEENGINEBONGO_GLFW_GRAPHICAPPLICATION_HPP
#define GAMEENGINEBONGO_GLFW_GRAPHICAPPLICATION_HPP

#include <Graphic/config.hpp>
#include <Core/CoreApplication.hpp>

class BM_GRAPHIC_DCL GraphicApplication : public CoreApplication
{
public:
    void processEvent() noexcept;
};


#endif// GAMEENGINEBONGO_GLFW_GRAPHICAPPLICATION_HPP
