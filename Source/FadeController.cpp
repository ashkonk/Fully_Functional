/*!*****************************************************************************
\par filename: FadeController.cpp
\par  authors: Jonathan Meyer
\par     date: 04/14/2023

\brief
  This is the implementation for our scene transition/fade controller.
  Copyright � 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "FadeController.h"
#include "Sprite.h"
#include "SceneLoader.h"
#include "Input.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/

namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
  void FadeControllerBehavior::Read(const rapidjson::Value& value)
  {
    if (value.HasMember("duration"))
      duration_ = value["duration"].GetFloat();
    
    if (value.HasMember("scenePath"))
      nextScene_ = value["scenePath"].GetString();

    if (value.HasMember("Intro"))
        intro_ = value["Intro"].GetBool();


    wait_ = 0.0f;
    if (value.HasMember("wait"))
      wait_ = value["wait"].GetFloat();
  }

  void FadeControllerBehavior::FadeOut()
  {
    fadeout_ = true;
    timer_ = 0.0f;
  }

  void FadeControllerBehavior::Update(float dt)
  {
    timer_ += dt;

    if (wait_ && timer_ > wait_)
    {
      FadeOut();
      timer_ += dt;
    }

    if (intro_)
    {
        Input& input = *engine_->GetSystem<Input>();

        if (input.GetKeyState(GLFW_KEY_SPACE) == Keystate::Triggered || input.GetButtonState(GLFW_MOUSE_BUTTON_LEFT) == Keystate::Triggered
            || input.GetButtonState(GLFW_MOUSE_BUTTON_RIGHT) == Keystate::Triggered || input.GetKeyState(GLFW_KEY_ENTER) == Keystate::Triggered
            || input.GetKeyState(GLFW_KEY_ESCAPE) == Keystate::Triggered)
        {
            fadeout_ = true;
            timer_ = 5.0f; 
        }

    }

    /* fade out */
    if (fadeout_)
    {
      Sprite& sprite = parent_.ComponentGet<Sprite>();
      sprite.SetOpacity(std::min(std::max((timer_) / duration_, 0.0f), 1.0f));

      if (timer_ / duration_ > 1.1f && nextScene_.size())
      {
        //change scene
        Manager<Scene>* sceneManager = (engine_->GetSystem<Manager<Scene>>());
        std::shared_ptr<Scene> newScene = LoadScene(*sceneManager,
          nextScene_.c_str());
        
        if (scene_)
        {
            engine_->SetLastLevel(engine_->GetLastLevel() + 1);
            scene_->Destroy();
        }
      }

      return;
    }

    /* fade in */
    Sprite& sprite = parent_.ComponentGet<Sprite>();
    sprite.SetOpacity(std::max((duration_ - timer_)/duration_, 0.0f));
  }
}; /* Normalloc */