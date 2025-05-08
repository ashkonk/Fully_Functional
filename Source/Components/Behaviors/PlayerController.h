/*!*****************************************************************************
\par filename: PlayerController.h
\par  authors: Mason Dulay, Ashkon Khalkhali
\par     date: 10/07/2022

\brief
  This is the PlayerController header file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include <rapidjson/document.h>
#include <unordered_map>
#include "Behavior.h"
#include "StateMachine.h"
#include "Component.h"
#include "Engine.h"
#include "Input.h"
//#include "Sound.h"
#include "Audio.h"
#include "Animation.h"
#include "AttendantA.h"
#include "Scene.h"
#include "DoorBehavior.h"

enum class MoveState
{
    Idle,
    Standing,
    Crouching,
    Teleporting,
    Losing
};

enum class VisionState
{
    Normal,
    Detective
};

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{

    class PlayerMovement : public StateMachine<MoveState>
    {
    private:
        Entity parent_;
        Entity hexFlash;
        Input* input_;
        Audio* audio_;
        bool pickup_;
        int animationFlag_;
        float standSpeed_;
        float crouchSpeed_;
        float standAccel_; 
        float crouchAccel_;
        bool flashing_;
        int onCooldown_; 
        float timer;
        bool flashed_;
        bool initialCrouch_ = true;
        void FindDoor();
        std::shared_ptr<Sound> walkingSound_;
        std::shared_ptr<Sound> teleportSound_;
        bool hasTeleport = false; 
        bool isPaused_;

        void PrintData();

    public:
        void Read(const rapidjson::Value&);
      void SetParent(Entity parent);
        PlayerMovement(Entity parent, Input* input, Audio* audio) : parent_(parent), 
             input_(input), audio_(audio), onCooldown_(false), standSpeed_(0.1f), flashing_(false), flashed_(false), pickup_(false), animationFlag_(0), standAccel_(0),
            hexFlash(parent_.GetParent(), parent_.GetParent()->EntityCreate()) {}
        // Inherited via StateMachine
        virtual void OnInit() override;
        virtual void OnUpdate(float dt) override;
        virtual void OnExit() override;

        void UpdateIdle(Animation& animation, float dt);
        void UpdateStanding(Animation& animation, float dt);
        void UpdateLosing(Animation& animation, float dt);
        void UpdateCrouch(Animation& animation, float dt);
        void UpdateTeleporting(Animation& animation, float dt);

        int GetTeleportCD();

    };

    class PlayerVision : public StateMachine<VisionState>
    {
    private:
        Input* input_;
        Entity parent_; 
        float timer_;
        float timer2_;
        int state_;

    public:
        void Read(const rapidjson::Value&);
        PlayerVision(Entity parent, Input* input) : input_(input), parent_(parent), timer_(0.0f), timer2_(0.0f), state_(0) {}
        // Inherited via StateMachine
        virtual void OnInit() override;
        virtual void OnUpdate(float dt) override;
        virtual void OnExit() override;
    };

    /*!*************************************************************************
      \brief
        Player Controller class that defines how the player moves
    ***************************************************************************/
    class PlayerController : public Behavior<PlayerController> /* Example Component  * * * * */
    {
    private:
        Input* input_ = nullptr;
        Engine* engine_{ nullptr };
        bool alive_; 
        bool colliding_;
        float height_;
        //std::vector<Entity> inventory_;
        std::unordered_map<std::string, Entity&> inventory_;
        std::shared_ptr<PlayerMovement> movement_;
        std::shared_ptr<StateMachine<VisionState>> vision_;

    public:
        PlayerController(Engine* engine, Entity parent) : 
            Behavior(parent),
            engine_(engine),
            alive_(true),
            colliding_(false),
            height_(1.0f),
            movement_(new PlayerMovement(parent, engine->GetSystem<Input>(), engine->GetSystem<Audio>())),
            vision_(new PlayerVision(parent, engine->GetSystem<Input>()))
        {
            input_ = engine->GetSystem<Input>();
        }

        Engine* GetEngine() const;
        void Read(const rapidjson::Value&);
        void AddToInventory(std::string name, Entity& entity);
        std::unordered_map<std::string, Entity&>* GetInventory();
        ////where the state machine would go.
        void Update(float dt);
        void SetStatus(bool truth);
        bool GetStatus();
        void SetParent(ECS* ecs, EntityID id) override;
        void EnterCollision2D(const Collider& self, const Collider& other);
        void ExitCollision2D(const Collider& self, const Collider& other);
        void PoisonAttendant();
        int GetVisionState();

        float GetHeight() const;

        void SetHeight(float height);

        int GetMoveState();

        bool GetCrouch();

        void TogglePauseMenu();

        static std::string_view Name();

        int GetonCooldown();

    }; /* ExampleComponent */

    //Alijah's code for poison collision w/ attendant
    //static void PlayerCollisionHandler(Entity owner, Entity enemy);

}; /* Normalloc */