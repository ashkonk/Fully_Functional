/*!*****************************************************************************
\par filename: AttendantA.h
\par  authors: Ashkon Khalkhali
\par     date: 10/16/2022

\brief
    This file contains the behavior component and the state machine belonging to
    Attendant A.
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include <vector>
#include "StateMachine.h"
#include "Component.h"
#include "Behavior.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    

  /*!*************************************************************************
    \brief
      Attendant Behavior class.
  ***************************************************************************/
    class MovingObject : public Behavior<MovingObject>
    {
    private:
        Engine* engine_;
        int route_;
        bool dissolve_;
        float speed_;
        std::vector<glm::vec3> positions_;


    public:
        MovingObject(Engine* engine, Entity parent) :
            engine_(engine), route_(0), dissolve_(true),
            Behavior(parent)
            {}
        void Read(const rapidjson::Value&);
        void Update(float dt);

        void EnterCollision2D(const Collider& self, const Collider& other);
        void ExitCollision2D(const Collider& self, const Collider& other);

        static std::string_view Name() { return "MovingObject"; }
        void UpdatePosition(float dt);
        void UpdateDissolve();
        //void UpdateDissolve(float dt);
    };
};