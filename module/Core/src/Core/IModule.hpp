//
// Created by binaire on 15/06/2021.
//

#ifndef GAMEENGINEBONGO_IMODULE_HPP
#define GAMEENGINEBONGO_IMODULE_HPP

class IModule
{
public:
    virtual ~IModule()         = default;
    virtual void load()        = 0;
    virtual void unload()      = 0;
    virtual void beforeFrame() = 0;
    virtual void afterFrame()  = 0;
};

#endif// GAMEENGINEBONGO_IMODULE_HPP
