/*!*****************************************************************************
\par filename: PlayerController.cpp
\par  authors: Mason Dulay, Ashkon Khalkhali, Alijah Rosner
\par     date: 10/07/2022

\brief
    This is the PlayerController file for code implimentation.
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Sprite.h"
#include "PlayerController.h"
#include "Transform.h"
#include "PoisonVial.h"
#include "Entity.h"
#include "Animation.h"
#include "CameraBehavior.h"
#include "VisionCone.h"
#include "Physics.h"
#include <glm/gtx/vector_angle.hpp>

#define UNREFERENCED_PARAMETER(P) P // used for onUpdate

namespace FFunctional
{

    void PlayerController::Read(const rapidjson::Value& value)
    {
        movement_->Read(value["PlayerMovement"]);
        vision_->Read(value["PlayerVision"]);
    }

    void PlayerController::Update(float dt)
    {
        GetParent();
        movement_->Update(dt);
        vision_->Update(dt);
        TogglePauseMenu();
    }

    void PlayerController::SetStatus(bool truth)
    {
        alive_ = truth;
    }

    bool PlayerController::GetStatus()
    {
        return alive_;
    }

    /* Start of Alijah's code */
    void PlayerController::AddToInventory(std::string name, Entity& entity)
    {
        inventory_.insert({ name, entity });
    }

    std::unordered_map<std::string, Entity&>* PlayerController::GetInventory()
    {
        return &inventory_;
    }

    void PlayerController::EnterCollision2D(const Collider& self,
        const Collider& other)
    {
        self;
        ECS* ecs = parent_.GetParent();
        if (ecs->GetActive() == true)
        {
            if (other.GetParent().Has<AttendantBehavior>())
            {
                colliding_ = true;
            }
        }
    }

    void PlayerController::ExitCollision2D(const Collider& self,
        const Collider& other)
    {
        self;
        ECS* ecs = parent_.GetParent();
        if (ecs->GetActive() == true)
        {
            if (other.GetParent().Has<AttendantBehavior>())
            {
                colliding_ = false;
            }
        }
    }

    void PlayerController::PoisonAttendant()
    {
        if (inventory_.contains("PoisonVial"))
        {
            AttendantBehavior* attendant = &GetParent().GetParent()->ComponentGetFirst<AttendantBehavior>();
            if (!attendant->GetDisabled() && input_->GetKeyState(GLFW_KEY_G) == Keystate::Down)
            {
                Entity vfx(GetParent().GetParent(), GetParent().GetParent()->EntityCreate());
                Entity throwVfx(GetParent().GetParent(), GetParent().GetParent()->EntityCreate());
                rapidjson::Document docu = OpenJson("./Data/PoisonVFX.json");
                vfx.Read(docu, engine_);
                Transform* vfxTrans = &vfx.ComponentGet<Transform>();
                Animation* vfxAnim = &vfx.ComponentGet<Animation>();
                vfxAnim->SetIsLooping(true);
                vfxAnim->TryPlay("Active");
                glm::vec3 offset = *attendant->GetParent().ComponentGet<Transform>().GetTranslation();
                offset.y += 0.25f;
                vfxTrans->SetTranslation(&offset);
                attendant->SetDisabled(true);
            }
        }
    }
    /* End of Alijah's code */

    Engine* PlayerController::GetEngine() const
    {
        return engine_;
    }

    void PlayerController::SetParent(ECS* ecs, EntityID id)
    {
        movement_->SetParent(Entity(ecs, id));
        Component::SetParent(ecs, id);
    }

    void PlayerMovement::Read(const rapidjson::Value& value)
    {
        SetNextState((MoveState)value["MoveState"].GetInt());
        standSpeed_ = value["MaxSpeed"].GetFloat();
        crouchSpeed_ = float(value["CrouchSpeed"].GetFloat() / 1.25);
        crouchAccel_ = float(value["Acceleration"].GetFloat() / 1.25);
        standAccel_ = value["Acceleration"].GetFloat();
        walkingSound_ = audio_->GetSound("AmbroseWalk");
        
        if (value.HasMember("Teleport"))
            hasTeleport = value["Teleport"].GetBool();
        /*rapidjson::Document docu = OpenJson("./Data/Audio/SFX/AmbroseWalk.json");
        walkingSound_.Load(docu, audio_);*/


        teleportSound_ = audio_->GetSound("TeleportSound");
       /* rapidjson::Document docuTwo = OpenJson("./Data/Audio/SFX/TeleportSound.json");
        teleportSound_.Load(docuTwo, audio_);*/
        teleportSound_->SetInRange(true);
        teleportSound_->SetIsLooping(false);
        rapidjson::Document docuThree = OpenJson("./Data/HexFlash.json");
        hexFlash.Read(docuThree, input_->GetParent());
        hexFlash.ComponentGet<Collider>().SetEnabled(false);

    }

    void PlayerMovement::OnInit()
    {

        switch (GetCurrState())
        {
        case MoveState::Idle:
            walkingSound_->Stop();
            //teleportSound_.Stop();
            break;
        case MoveState::Standing:
            //walkingSound_.UpdateCheck(parent_, parent_);
            walkingSound_->SetInRange(true);
            walkingSound_->Play();
            break;
        case MoveState::Crouching:
            walkingSound_->Stop();
            break;
        case MoveState::Teleporting:
            hexFlash.ComponentGet<Collider>().SetEnabled(true);
            walkingSound_->Stop();
            break;
        }
    }

    void PlayerMovement::FindDoor()
    {
        if (parent_.GetParent()->HasManager<DoorBehavior>())
        {
            DoorBehavior& nextLevel = parent_.GetParent()->ComponentGetFirst<DoorBehavior>();

            Transform& doorTransform = nextLevel.GetParent().ComponentGet<Transform>();
            Transform& playerTrans = parent_.ComponentGet<Transform>();

            playerTrans.SetTranslation(doorTransform.GetTranslation());
            return;
        }
    }

    void PlayerMovement::PrintData()
    {
        glm::vec3 currPos = *parent_.ComponentGet<Transform>().GetTranslation();
        std::cout << "Position: " << currPos.x << " , " << currPos.y << std::endl;
        return;
    }

    static bool godMode = false;
    static bool inputTrigger = false;
    static bool pickupTrigger = false;
    static bool cheating = false;
    static bool debugging = false;

    void PlayerMovement::OnUpdate(float dt)
    {
        glm::vec3 offset = { 0, -0.15, 0 };

        if (parent_.GetParent()->GetActive())
        {
            if (Component::GetGlobalTimeDialation() < 0.9f) //slowdown only happens when you lose.
            {
                SetNextState(MoveState::Losing);
            }
            if (input_->GetKeyState(GLFW_KEY_EQUAL) == Keystate::Released && !cheating)
            {
                FindDoor();
                cheating = true;
                return;
            }
            if (input_->GetKeyState(GLFW_KEY_P) == Keystate::Down && !debugging)
            {
                PrintData();
                debugging = true;
                return;
            }

            if (!flashing_)
            {
                glm::vec3 playerTrans = *parent_.ComponentGet<Transform>().GetTranslation();
                playerTrans += offset;
                hexFlash.ComponentGet<Transform>().
                    SetTranslation(&playerTrans);
                hexFlash.ComponentGet<Sprite>().SetOpacity(0.0f);
            }

            if (flashed_)
            {
                timer += dt;
                if (timer > 1.0f)
                {
                    onCooldown_ += 1;
                    timer = 0.0f;
                }
            }

            if (onCooldown_ >= 5)
            {
                flashed_ = false;
                onCooldown_ = 0;
            }

            if (input_->GetKeyState(GLFW_KEY_G) == Keystate::Triggered)
            {
                pickup_ = true;
                //parent_.ComponentGet<PlayerController>().SetHeight(0.0f);
            }

            //pickupTrigger = input_->GetKeyState(GLFW_KEY_G) == Keystate::Up;

            Animation& animation = parent_.ComponentGet<Animation>();
            // if velocity < maxSpeed_; 
            //velocity == prev.velocity + acceleration * dt; 
            //                 5                0.12;

            switch (GetCurrState())
            {
            case MoveState::Idle:
            {
                UpdateIdle(animation, dt);
                break;
            }
            case MoveState::Standing:
            {
                UpdateStanding(animation, dt);
                break;
            }
            case MoveState::Crouching:
            {
                UpdateCrouch(animation, dt);
                break;
            }
            case MoveState::Teleporting:
            {
                UpdateTeleporting(animation, dt);
                break;
            }
            case MoveState::Losing:
            {
                UpdateLosing(animation, dt);
                break;
            }
            }

            // if velocity < maxSpeed
            //velocity = accel + maxSpeed * dt; 
            if (input_->GetKeyState(GLFW_KEY_EQUAL) == Keystate::Up)
                cheating = false;
            if (input_->GetKeyState(GLFW_KEY_P) == Keystate::Up)
                debugging = false;
        }
    }

    std::string_view PlayerController::Name()
    {
        return "PlayerController";
    }

    int PlayerController::GetVisionState()
    {
        if (vision_->GetCurrState() == VisionState::Detective)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    bool PlayerController::GetCrouch()
    {
        if (movement_->GetCurrState() == MoveState::Crouching)
        {
            return true;
        }
        return false;
    }

    void PlayerMovement::OnExit()
    {
        switch (GetCurrState())
        {
        case MoveState::Idle:
            break;
        case MoveState::Standing:
            break;
        case MoveState::Crouching:
            break;
        case MoveState::Teleporting:
            hexFlash.ComponentGet<Collider>().SetEnabled(false);
            break;
        case MoveState::Losing:
            break;
        }
    }

    void PlayerMovement::UpdateIdle(Animation& animation, float dt)
    {
        dt;
        Physics& physics = parent_.ComponentGet<Physics>();
        glm::vec3 velocity = *physics.GetVelocity();

        if (input_->GetKeyState(GLFW_KEY_LEFT_CONTROL) == Keystate::Down)
        {
            initialCrouch_ = true;
            SetNextState(MoveState::Crouching);
        }
        if (input_->GetKeyState(GLFW_KEY_SPACE) == Keystate::Down && !flashed_)
        {
            if (hasTeleport)
            {

                SetNextState(MoveState::Teleporting);
            }
        }

        if (!flashing_ && input_->GetKeyState(GLFW_KEY_W) == Keystate::Down || input_->GetKeyState(GLFW_KEY_S) == Keystate::Down
            || input_->GetKeyState(GLFW_KEY_D) == Keystate::Down || input_->GetKeyState(GLFW_KEY_A) == Keystate::Down)
        {
            SetNextState(MoveState::Standing);
        }
        else
        {
            if (animationFlag_ == 1)
            {
                if (pickup_)
                {
                    animation.SetIsLooping(false);
                    animation.TryPlay("BackInteract");
                }
                else
                {
                    animation.SetIsLooping(true);

                    animation.TryPlay("BackIdle");

                }
            }
            else if (animationFlag_ == 2)
            {
                if (pickup_)
                {
                    animation.SetIsLooping(false);

                    animation.TryPlay("FrontInteract");
                }
                else
                {
                    animation.SetIsLooping(true);

                    animation.TryPlay("FrontIdle");
                }
            }
            else
            {
                if (pickup_)
                {
                    animation.SetIsLooping(false);

                    animation.TryPlay("SideInteract");
                }
                else
                {

                    animation.SetIsLooping(true);

                    animation.TryPlay("SideIdle");
                }
            }
        }
    }

    void PlayerMovement::UpdateLosing(Animation& animation, float dt)
    {
        animation;
        dt;
    }

    void PlayerMovement::UpdateStanding(Animation& animation, float dt)
    {
        dt;
        Physics& physics = parent_.ComponentGet<Physics>();
        glm::vec3 velocity = *physics.GetVelocity();

        if (input_->GetKeyState(GLFW_KEY_LEFT_CONTROL) == Keystate::Down)
        {
            initialCrouch_ = true;
            SetNextState(MoveState::Crouching);
        }
        if (input_->GetKeyState(GLFW_KEY_SPACE) == Keystate::Down && !flashed_)
        {
            velocity.y = 0;
            velocity.x = 0;
            physics.SetAcceleration(&velocity);
            physics.SetVelocity(&velocity);
            if (hasTeleport)
            {
                SetNextState(MoveState::Teleporting);
            }
        }
        bool moving = false;

        //if player wants to go up
        if (input_->GetKeyState(GLFW_KEY_W) == Keystate::Down)
        {
            moving = true;
            pickup_ = false;
            velocity.y += standAccel_ * dt;
            // set animation and sprite upward
        }
        if (input_->GetKeyState(GLFW_KEY_S) == Keystate::Down)
        {
            moving = true;
            pickup_ = false;

            velocity.y -= standAccel_ * dt;

            // set animation and sprite downward
        }

        if (!moving)
        {
            velocity.y *= standAccel_ * dt * 0.5f;
        }

        moving = false;

        if (input_->GetKeyState(GLFW_KEY_D) == Keystate::Down)
        {
            pickup_ = false;
            moving = true;
            velocity.x += standAccel_ * dt;

        }
        if (input_->GetKeyState(GLFW_KEY_A) == Keystate::Down)
        {
            moving = true;
            pickup_ = false;

            velocity.x -= standAccel_ * dt;

            // set animation and sprite left
        }


        float angle = glm::degrees(atan2(velocity.y, velocity.x));

        if (angle < 0)
        {
            angle += 360;
        }

        if (angle >= 135 && angle <= 225)
        {
            //left
            glm::vec3 scale = { -0.4, 0.4, 0 };
            animation.SetIsLooping(true);

            animation.TryPlay("SideWalk");
            parent_.ComponentGet<Transform>().SetScale(&scale);
            animationFlag_ = 3;
        }
        else if (angle >= (360 - 45) || angle <= 45)
        {
            //right
            animation.SetIsLooping(true);
            glm::vec3 scale = { 0.4, 0.4, 0 };
            animation.TryPlay("SideWalk");
            parent_.ComponentGet<Transform>().SetScale(&scale);
            animationFlag_ = 4;
        }
        else if (angle >= 45 && angle <= (180 - 45))
        {
            //up
            animation.SetIsLooping(true);
            animation.TryPlay("BackWalk");
            animationFlag_ = 1;
        }
        else if (angle >= (180 + 45) && angle <= (360 - 45))
        {
            //down
            animation.SetIsLooping(true);
            animation.TryPlay("FrontWalk");
            animationFlag_ = 2;
        }

        if (!moving)
        {
            velocity.x *= standAccel_ * dt * 0.5f;
        }

        velocity = glm::clamp(velocity, glm::vec3(-standSpeed_), glm::vec3(standSpeed_));
        float magnitude = glm::length(velocity);

        if (magnitude <= 0.0125f)
        {
            velocity.x = 0;
            velocity.y = 0;
            SetNextState(MoveState::Idle);
        }

        physics.SetVelocity(&velocity);

    }

    float PlayerController::GetHeight() const
    {
        return height_;
    }

    void PlayerController::SetHeight(float height)
    {
        height_ = height;
    }

    int PlayerController::GetMoveState()
    {
        if (movement_->GetCurrState() == MoveState::Idle)
        {
            return 0;
        }
        else if (movement_->GetCurrState() == MoveState::Standing)
        {
            return 1;
        }
        else if (movement_->GetCurrState() == MoveState::Crouching)
        {
            return 2;
        }
        else if (movement_->GetCurrState() == MoveState::Teleporting)
        {
            return 3;
        }
        return -1;
    }

    void PlayerController::TogglePauseMenu()
    {
        if (input_->GetKeyState(GLFW_KEY_ESCAPE) == Keystate::Triggered)
        {
            Manager<Scene>* sceneManager = (engine_->GetSystem<Manager<Scene>>());
            std::shared_ptr<Scene> newScene = LoadScene(*sceneManager, "./Data/PauseMenuMain.json");
            for (unsigned i = 0; i < sceneManager->Size(); ++i)
            {
                std::shared_ptr<Scene> sceneBuffer = sceneManager->At(i);
                //if scene already exists, return it
                if (sceneBuffer->getECS().HasManager<PlayerController>())
                {
                    //set sprites to active
                    SetECSState(sceneBuffer->getECS(), false);
                }
            }
        }
    }

    int PlayerMovement::GetTeleportCD()
    {
        return onCooldown_;
    }

    int PlayerController::GetonCooldown()
    {
        return movement_->GetTeleportCD();
    }

    void PlayerMovement::UpdateCrouch(Animation& animation, float dt)
    {
        PlayerController& player = parent_.ComponentGet<PlayerController>();
        if (input_->GetKeyState(GLFW_KEY_SPACE) == Keystate::Down && !flashed_)
        {
            if (hasTeleport)
            {
                SetNextState(MoveState::Teleporting);

            }
            player.SetHeight(0.5f);
        }
        bool moving = false;
        Physics& physics = parent_.ComponentGet<Physics>();
        Transform& transform = parent_.ComponentGet<Transform>();
        glm::vec3 trans = *transform.GetTranslation();

        glm::vec3 offset = { 0, 0.04, 0 };

        player.SetHeight(0.5f);


        // glm::vec3 scaleOhYeah = { -0.32, 0.32, 0 };
         //parent_.ComponentGet<Transform>().SetScale(&scaleOhYeah);
        animation.SetIsLooping(false);

        // animation.TryPlay("CrouchIdle");


        glm::vec3 velocity = *physics.GetVelocity();

        if (input_->GetKeyState(GLFW_KEY_LEFT_CONTROL) == Keystate::Up && !initialCrouch_)
        {
            glm::vec3 scale;
            if (animationFlag_ == 3)
            {
                scale = { -0.4, 0.4, 0 };
            }
            else
            {
                scale = { 0.4, 0.4, 0 };
            }
            transform.SetScale(&scale);
            velocity.x = 0;
            velocity.y = 0;
            //trans += offset;
            transform.SetTranslation(&trans);
            initialCrouch_ = true;
            player.SetHeight(1.0f);
            SetNextState(MoveState::Idle);
        }

        //if player wants to go up
        if (input_->GetKeyState(GLFW_KEY_W) == Keystate::Down)
        {
            moving = true;
            pickup_ = false;
            velocity.y += crouchAccel_ * dt;
            // set animation and sprite upward
        }
        if (input_->GetKeyState(GLFW_KEY_S) == Keystate::Down)
        {
            moving = true;
            pickup_ = false;

            velocity.y -= crouchAccel_ * dt;

            // set animation and sprite downward
        }

        if (!moving)
        {
            velocity.y *= crouchAccel_ * dt * 0.5f;
        }

        moving = false;

        if (input_->GetKeyState(GLFW_KEY_D) == Keystate::Down)
        {
            pickup_ = false;
            moving = true;
            velocity.x += crouchAccel_ * dt;

        }
        if (input_->GetKeyState(GLFW_KEY_A) == Keystate::Down)
        {
            moving = true;
            pickup_ = false;

            velocity.x -= crouchAccel_ * dt;

            // set animation and sprite left
        }


        if (velocity.x < 0)
        {
            glm::vec3 scale = { -0.4, 0.4, 0 };
            animation.SetIsLooping(true);

            animation.TryPlay("Crouch");
            parent_.ComponentGet<Transform>().SetScale(&scale);
            animationFlag_ = 3;
            if (initialCrouch_)
            {
                //trans -= offset;
                transform.SetTranslation(&trans);
                initialCrouch_ = false;
            }
        }
        else if (velocity.x > 0)
        {
            animation.SetIsLooping(true);
            glm::vec3 scale = { 0.4, 0.4, 0 };
            animation.TryPlay("Crouch");
            parent_.ComponentGet<Transform>().SetScale(&scale);
            animationFlag_ = 4;
            if (initialCrouch_)
            {
                //trans -= offset;
                transform.SetTranslation(&trans);
                initialCrouch_ = false;
            }
        }
        else if (velocity.y > 0)
        {
            animation.SetIsLooping(true);
            glm::vec3 scale = { 0.4, 0.4, 0 };
            animation.TryPlay("Crouch");
            parent_.ComponentGet<Transform>().SetScale(&scale);
            animationFlag_ = 4;
            if (initialCrouch_)
            {
                //trans -= offset;
                transform.SetTranslation(&trans);
                initialCrouch_ = false;
            }
        }
        else if (velocity.y < 0)
        {
            glm::vec3 scale = { -0.4, 0.4, 0 };
            animation.SetIsLooping(true);

            animation.TryPlay("Crouch");
            parent_.ComponentGet<Transform>().SetScale(&scale);
            animationFlag_ = 3;
            if (initialCrouch_)
            {
                //trans -= offset;
                transform.SetTranslation(&trans);
                initialCrouch_ = false;
            }
        }
        else
        {
            glm::vec3 scale = { 0.4, 0.4, 0 };
            parent_.ComponentGet<Transform>().SetScale(&scale);

            animation.TryPlay("CrouchIdle");
        }

        if (!moving)
        {
            velocity.x *= crouchAccel_ * dt * 0.5f;
        }

        velocity = glm::clamp(velocity, glm::vec3(-crouchSpeed_), glm::vec3(crouchSpeed_));

        physics.SetVelocity(&velocity);
    }

    void PlayerMovement::UpdateTeleporting(Animation& animation, float dt)
    {
        if (input_->GetKeyState(GLFW_KEY_LEFT_CONTROL) == Keystate::Up)
        {
            
            PlayerController& player = parent_.ComponentGet<PlayerController>();
            player.SetHeight(1.0f);
        }

        if (animationFlag_ == 1)
        {
            if (pickup_)
            {
                animation.SetIsLooping(false);
                animation.TryPlay("BackInteract");
            }
            else
            {
                animation.SetIsLooping(true);

                animation.TryPlay("BackIdle");

            }
        }
        else if (animationFlag_ == 2)
        {
            if (pickup_)
            {
                animation.SetIsLooping(false);

                animation.TryPlay("FrontInteract");
            }
            else
            {
                animation.SetIsLooping(true);

                animation.TryPlay("FrontIdle");
            }
        }
        else
        {
            if (pickup_)
            {
                animation.SetIsLooping(false);

                animation.TryPlay("SideInteract");
            }
            else
            {

                animation.SetIsLooping(true);

                animation.TryPlay("SideIdle");
            }
        }
        Physics& physics = parent_.ComponentGet<Physics>();
        glm::vec3 velocity = *physics.GetVelocity();
        velocity.y = 0;
        velocity.x = 0;
        physics.SetAcceleration(&velocity);
        physics.SetVelocity(&velocity);
        
        if (input_->GetKeyState(GLFW_KEY_SPACE) == Keystate::Down)
        {
            flashing_ = true;
            if (!flashed_)
            {
                bool moving = false; 
                hexFlash.ComponentGet<Animation>().TryPlay("Active");
                hexFlash.ComponentGet<Sprite>().SetOpacity(1.0f);
                float hexAccel = 8.0f;
                float hexSpeed = 0.75f;
                Physics& hexPhysics = hexFlash.ComponentGet<Physics>();
                glm::vec3 flashVelocity = *hexPhysics.GetVelocity();
                glm::vec3 playerVec = { 0, 0, 0 };
                Physics& playerPhys = parent_.ComponentGet<Physics>();
                playerPhys.SetVelocity(&playerVec);

                Transform& hexTrans = hexFlash.ComponentGet<Transform>();
                glm::vec3 hexPos = *hexTrans.GetTranslation();

                Transform& playerTransform = parent_.ComponentGet<Transform>();
                glm::vec3 playerPos = *playerTransform.GetTranslation();

                float distance = glm::distance(hexPos, playerPos);

                if (distance <= 0.75f)
                {
                    if (input_->GetKeyState(GLFW_KEY_W) == Keystate::Down)
                    {
                        moving = true;
                        flashVelocity.y += hexAccel * dt;
                        // set animation and sprite upward
                    }
                    if (input_->GetKeyState(GLFW_KEY_S) == Keystate::Down)
                    {
                        moving = true;
                        flashVelocity.y -= hexAccel * dt;

                        // set animation and sprite downward
                    }
                    if (input_->GetKeyState(GLFW_KEY_D) == Keystate::Down)
                    {
                        moving = true;
                        flashVelocity.x += hexAccel * dt;

                    }
                    if (input_->GetKeyState(GLFW_KEY_A) == Keystate::Down)
                    {
                        moving = true;
                        flashVelocity.x -= hexAccel * dt;

                        // set animation and sprite left
                    }

                    if (!moving)
                    {
                        flashVelocity.x *= crouchAccel_ * dt * 0.5f;
                        flashVelocity.y *= crouchAccel_ * dt * 0.5f;
                    }

                    flashVelocity = glm::clamp(flashVelocity, glm::vec3(-hexSpeed), glm::vec3(hexSpeed));
                    hexPhysics.SetVelocity(&flashVelocity);
                }
                else
                {
                    hexTrans.SetTranslation(hexPhysics.GetOldTranslation());
                    hexPhysics.SetAcceleration(&playerVec);
                    //velocity = glm::clamp(velocity, glm::vec3(-hexSpeed), glm::vec3(hexSpeed));
                    moving = false;
                    hexPhysics.SetVelocity(&playerVec);
                }
                //flashed_ = true; 
            }
        }
        else if (input_->GetKeyState(GLFW_KEY_SPACE) == Keystate::Up && hexFlash.ComponentGet<Sprite>().GetOpacity() != 0.0f)
        {
                glm::vec3 vec = { 0, 0, 0 };
                glm::vec3 offset = { 0, 0.15, 0 };
                Physics& hexPhys = hexFlash.ComponentGet<Physics>();
                hexPhys.SetVelocity(&vec);
                hexPhys.SetAcceleration(&vec);
                Transform& parentTrans = parent_.ComponentGet<Transform>();
                offset += *hexFlash.ComponentGet<Transform>().GetTranslation();
                parentTrans.SetTranslation(&offset);
                flashed_ = true;
                flashing_ = false;
                teleportSound_->Play();
                //hexFlash.ComponentGet<Sprite>().SetOpacity(0.0f);
                SetNextState(MoveState::Idle);

        }
    }


    void PlayerMovement::SetParent(Entity parent)
    {
        parent_ = parent;
    }

    void PlayerVision::Read(const rapidjson::Value& value)
    {
        SetNextState((VisionState)value["VisionState"].GetInt());
    }

    void PlayerVision::OnInit()
    {
        state_ = false;
        switch (GetCurrState())
        {
        case VisionState::Normal:
            break;

        case VisionState::Detective:
            break;
        }
    }

    void PlayerVision::OnUpdate(float dt)
    {
        timer_ += dt;
        timer2_ += dt;
        // in normal going to detective
        if (input_->GetKeyState(GLFW_KEY_V) == Keystate::Down && state_ == 0 && timer2_ >= 2.0f)
        {
            state_ = 1;
            timer_ = 0.0f;
            SetNextState(VisionState::Detective);
        }
        // in detective going to normal
        if (input_->GetKeyState(GLFW_KEY_V) == Keystate::Down && state_ == 1 && timer_ >= 2.0f)
        {
            state_ = 0;
            timer2_ = 0.0f;
            SetNextState(VisionState::Normal);
        }
    }

    void PlayerVision::OnExit()
    {
    }

}