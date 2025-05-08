/*!*****************************************************************************
\par filename: CCTV.h
\par  authors: Alijah Rosner
\par     date: 10/16/2022

\brief
    This file contains the classes for both the Fusebox behavior, and
    the CCTV behavior.
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include <vector>
#include <string_view>
#include "Engine.h"
#include "StateMachine.h"
#include "Component.h"
#include "PlayerController.h"
#include "Collider.h"
#include "Behavior.h"
#include "Vision.h"


/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    /*!*************************************************************************
      \brief
        Fusebox behavior class
    ***************************************************************************/
    //class FuseBoxBehavior : public Behavior<FuseBoxBehavior> //: public Component /* Example Component  * * * * */
    //{
    //private:
    //    Engine* engine_{ nullptr };
    //    bool active_;
    //    bool colliding_; 

    //public:
    //    void Read(const rapidjson::Value&);
    //    FuseBoxBehavior(Engine* engine, Entity parent) : engine_(engine), Behavior(parent), active_(true), colliding_(false) {}
    //    void Update(float dt);
    //    bool GetActive() const; 
    //    void TurnOff();
    //    

    //    static std::string_view Name() { return "FuseBoxBehavior"; }
    //    void EnterCollision2D(const Collider& self, const Collider& other);
    //    void ExitCollision2D(const Collider& self, const Collider& other);




    //};


    /*!*************************************************************************
      \brief
        CCTV Behavior class.
    ***************************************************************************/
    class CCTVBehavior : public Behavior<CCTVBehavior>
    {
    private:
        Engine* engine_;
        bool colliding_;
        float maxRotation_; 
        float rotationSum_;
        float originalRotation_; 
        float rotateDirection_;
        std::vector<glm::vec3> points_;
        float timer_; 
        bool active_;
        float rotateSpeed_;
        int count_;
        float detectTime_;



    public:
        CCTVBehavior(Engine* engine, Entity parent) : engine_(engine), Behavior(parent), count_(0), maxRotation_(90), 
            active_(true), rotateSpeed_(20.0), rotationSum_(0), colliding_(false), rotateDirection_(1)
            
                {}
        void Read(const rapidjson::Value&);
        void Init();
        void Update(float dt);
        void SetActive(bool active);
        bool GetActive() const;
        bool GetColliding();



        static std::string_view Name() { return "CCTVBehavior"; }
        void EntityFound(const Vision& self, const Entity& other);

        void EntityLost(const Vision& self, const Entity& other);

    };
    static void CCTVBehaviorCollisionHandler(Entity owner, Entity player);


};