/*!*****************************************************************************
\par filename: Interactable.h
\par  authors: Mason Dulay
\par     date: 09/09/2022

\brief
  This is the Interactable file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include <utility>

#include "Sprite.h"
#include "Input.h"
#include "Window.h"
#include "Behavior.h"
#include "Transform.h"
#include "SceneLoader.h"
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
      Button class that has access to input, window, and engine as a behavior
  ***************************************************************************/
  class Interactable : public Behavior<Interactable> /* Example Component  * * * * */
  {
  private:
    Input* input_;
    Engine* engine_{ nullptr };
    Audio* audio_;
    GLFWwindow* window_;
     glm::vec3 offset_;
    bool pickup_, static_, active_;

    const Transform* nearest = nullptr;

    std::pair<bool, glm::vec3> CursorInside();

  public:
    Interactable(Engine* engine, Entity parent) : Behavior(parent), engine_(engine),
      audio_(engine->GetSystem<Audio>()), offset_(),
      static_(false), pickup_(false), active_(false)

    {
      input_ = engine->GetSystem<Input>();
      window_ = engine->GetSystem<Window>()->GetWindow();
    }

    bool GetPickup();

    bool GetActive();
    
    void SetDown();

    void Update(float dt);

    void Read(const rapidjson::Value& value);

    static std::string_view Name();

  }; /* ExampleComponent */

}; /* Normalloc */