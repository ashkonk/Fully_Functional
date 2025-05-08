/*!*****************************************************************************
\par filename: CameraBehavior.h
\par  authors: Ashkon Khalkhali
\par     date: 11/14/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include <string>
#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include <vector>
#include "Component.h"
#include "Scene.h"
#include "Sound.h"
#include "Engine.h"
#include "Entity.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/

namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
  /*!*************************************************************************
    \brief
      This is an example of a component class.
  ***************************************************************************/

  //static void DoorBehaviorCollisionHandler(Entity door, Entity player);
  typedef class Sprite Sprite;

  class CameraBehavior : public Component
  {
  private:
    Engine* engine_{ nullptr };
    //Location (in world space) of next scene entrance
    glm::vec3 playerPos_;
    glm::vec3 initPos_;
    glm::vec4 bounds;
    bool detectiveMode_ = false;
    std::shared_ptr<Sound> backgroundMusic; 
    std::shared_ptr<Sound> trainSounds;

    bool target_ = false;
    glm::vec3 targetPos_;

  public:
    CameraBehavior(Engine* engine, Entity parent) : Component(parent),
      engine_(engine), playerPos_{}, initPos_{} {}
     
    void Init();           //!< Our ExampleComponent Init Function. 

    void Read(const rapidjson::Value& value);

    void Target(glm::vec3 pos);

    void Untarget();

    void Update(float dt); //!< Our ExampleComponent Update Function.

    bool GetMode();

    static std::string_view Name() { return "CameraBehavior"; }

  }; /* ExampleComponent */

}; /* Normalloc */