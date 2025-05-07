/*!*****************************************************************************
\par filename: TeleportBehavior.h
\par  authors: Ashkon Khalkhali
\par     date: 11/23/2022

\brief
    This file contains the fuse box key behavior. 
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include "Stream.h"
#include <glm/glm.hpp>
#include <vector>
#include "Engine.h"
#include "StateMachine.h"
#include "Collider.h"
#include "Behavior.h"


/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
  /*  class TeleportCooldown : public Behavior<TeleportCooldown>
    {
    private: 
        Engine* engine_{ nullptr };

    public: 
        TeleportCooldown(Engine* engine, Entity parent) : engine_(engine), Behavior(parent) {}
        void Read(const rapidjson::Value&);
        void Update(float dt);

        static std::string_view Name() { return "TelportCooldown"; }

    };*/
    /*!*************************************************************************
      \brief
        FuseboxKey Behavior class
    ***************************************************************************/
    class TeleportBehavior : public Behavior<TeleportBehavior>
    {
    private:
        Engine* engine_{ nullptr };

    public:
        TeleportBehavior(Engine* engine, Entity parent) : engine_(engine), Behavior(parent) {}
        void Read(const rapidjson::Value&);
        void Update(float dt);

        static std::string_view Name() { return "TeleportBehavior"; }


    };

};