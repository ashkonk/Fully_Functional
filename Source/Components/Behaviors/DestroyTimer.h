 
/*!*****************************************************************************
\par filename: DestroyTimer.h
\par  authors: Jonathan Meyer
\par     date: 04/14/2023

\brief
  This is the header file for our self destruction timer (garbage collection)
  Copyright � 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "StateMachine.h"
#include "Behavior.h"
#include "Scene.h"
#include "Sound.h"
#include "Audio.h"
/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/

namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
  /*!*************************************************************************
    \brief
      This is the self destruct timer behavior class.
  ***************************************************************************/
  class DestroyTimerBehavior : public Behavior<DestroyTimerBehavior>
  {
  private:
    Engine* engine_;
		Scene* scene_;
    std::shared_ptr<Sound> sound_{ nullptr };
    bool skip = false; 

		float duration_ = 1.0f, sceneDelay_ = 0.0f, start_ = 0.0f;
		float timer_ = 0.0f;

  public:
    DestroyTimerBehavior(Engine* engine, Entity parent, Scene* scene) : Behavior(parent), engine_(engine), scene_(scene) {}

    void Read(const rapidjson::Value& value);

    void Update(float dt); //!< Our ExampleComponent Update Function.

    static std::string_view Name() { return "DestroyTimer"; }

  }; /* ExampleComponent */

}; /* Normalloc */