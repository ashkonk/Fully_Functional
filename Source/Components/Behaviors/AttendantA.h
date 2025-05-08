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

enum class BehaviorState
{
    Passive,
    Alerted
};

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    /*!*************************************************************************
      \brief
        Attendant movement state machine class.
    ***************************************************************************/
    class AttendantMovement : public StateMachine<BehaviorState> //: public Component /* Example Component  * * * * */
    {
    private:
        Entity Parent_;
        std::vector<glm::vec3> Positions_; 
        float Speed_;
        int Route_;

        //helper functions
        void UpdatePassivePosition(float dt);
        void UpdateAlertedPosition(float dt);
    public:

        int GetRoute();
        void Read(const rapidjson::Value&);
        AttendantMovement(Entity parent) : Parent_(parent), Route_(0), Speed_(0.0f) {}
        virtual void OnInit() override;
        virtual void OnUpdate(float dt) override;
        virtual void OnExit() override;

    };

  /*!*************************************************************************
    \brief
      Attendant Behavior class.
  ***************************************************************************/
    class AttendantBehavior : public Behavior<AttendantBehavior>
    {
    private:
        std::shared_ptr<AttendantMovement> Movement_;
        Engine* engine_;
        bool isDisabled_;
        Entity particles_; 
        bool particlesCreated_; 
        bool particlesDestroyed_; 

    public:
        AttendantBehavior(Engine* engine, Entity parent) :
            Behavior(parent),
            engine_(engine),
            isDisabled_(false),
            particlesCreated_(false),
            particlesDestroyed_(false),
            particles_(GetParent().GetParent(), GetParent().GetParent()->EntityCreate()),
            Movement_(new AttendantMovement(parent)) {}
        int GetRouteFromBehavior();
        void Read(const rapidjson::Value&);
        bool GetDisabled() const; 
        void SetDisabled(bool truth);
        void Update(float dt);

        void EnterCollision2D(const Collider& self, const Collider& other);

        static std::string_view Name() { return "AttendantBehavior"; }
    };
};