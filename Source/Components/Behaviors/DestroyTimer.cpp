

/*!*****************************************************************************
\par filename: DestroyTimer.cpp
\par  authors: Jonathan Meyer
\par     date: 04/14/2023

\brief
  This is the implementation of our self destruction timer (garbage collection)
  Copyright � 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "DestroyTimer.h"
#include "Animation.h"
#include "SceneLoader.h"
#include "Sprite.h"
#include "Input.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/

namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    void DestroyTimerBehavior::Read(const rapidjson::Value& value)
    {
        if (value.HasMember("spawnDelay"))
            start_ = value["spawnDelay"].GetFloat();

        if (value.HasMember("destroyDelay"))
            duration_ = value["destroyDelay"].GetFloat();

        if (value.HasMember("sceneDelay"))
            sceneDelay_ = value["sceneDelay"].GetFloat();

        sound_ = engine_->GetSystem<Audio>()->GetSound("AmbientTrains");
        sound_->SetInRange(true);
        sound_->Play();
    }

    void DestroyTimerBehavior::Update(float dt)
    {
        if (skip)
        {
            return; 
        }

        timer_ += dt;

        Animation& rhs = parent_.ComponentGet<Animation>();
        Sprite& sprite = parent_.ComponentGet<Sprite>();

        if (timer_ >= start_)
        {
            rhs.TryPlay("FrameStep");
        }
        Input& input = *engine_->GetSystem<Input>();

        if (input.GetKeyState(GLFW_KEY_SPACE) == Keystate::Triggered || input.GetButtonState(GLFW_MOUSE_BUTTON_LEFT) == Keystate::Triggered
            || input.GetButtonState(GLFW_MOUSE_BUTTON_RIGHT) == Keystate::Triggered || input.GetKeyState(GLFW_KEY_ENTER) == Keystate::Triggered
            || input.GetKeyState(GLFW_KEY_ESCAPE) == Keystate::Triggered)
        {
            skip = true;
        }

        if (skip && sceneDelay_ != 0.0f)
        {
            Manager<Scene>* sceneManager = (engine_->GetSystem<Manager<Scene>>());
            std::shared_ptr<Scene> newScene = LoadScene(*sceneManager,
                "./Data/LevelTutorial0/TutorialRoom0Primary.json");
            skip = false;
            //if (scene_)
            scene_->Destroy();
            return; 
        }

        if (rhs.GetIsDone())
        {

            //hiding instead.

            ////destroy urself >:)
            //parent_.Destroy();


            sprite.SetActive(false);
            sprite.SetOpacity(0.0f);
            if (sceneDelay_ && timer_ >= sceneDelay_ || skip == true)
            {
                Manager<Scene>* sceneManager = (engine_->GetSystem<Manager<Scene>>());
                std::shared_ptr<Scene> newScene = LoadScene(*sceneManager,
                    "./Data/LevelTutorial0/TutorialRoom0Primary.json");
                skip = false;
                if (scene_)
                    scene_->Destroy();
            }
        }
    }
}; /* Normalloc */