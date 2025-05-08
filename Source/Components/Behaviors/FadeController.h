/*!*****************************************************************************
\par filename: FadeController.h
\par  authors: Jonathan Meyer
\par     date: 04/14/2023

\brief
  This is the header file for our scene transition/fade controller.
  Copyright � 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "StateMachine.h"
#include "Behavior.h"
#include "Scene.h"
#include <string>

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/

namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
  /*!*************************************************************************
    \brief
      This is the fade controller class.
  ***************************************************************************/
  class FadeControllerBehavior : public Behavior<FadeControllerBehavior>
  {
  private:
    Engine* engine_;
    Scene* scene_;
    bool skip = false; 
    bool intro_ = false; 

    float duration_ = 1.0f, timer_ = 0.0f;
    float wait_ = 0.0f;
    bool fadeout_ = false;
    std::string nextScene_ = "";

  public:
    FadeControllerBehavior(Engine* engine, Entity parent, Scene* scene) : Behavior(parent), engine_(engine), scene_(scene) {}

    void Read(const rapidjson::Value& value);

    void Update(float dt); //!< Our ExampleComponent Update Function.

    void FadeOut();

    static std::string_view Name() { return "FadeController"; }

  }; /* ExampleComponent */

}; /* Normalloc */