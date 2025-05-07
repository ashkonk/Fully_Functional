/*!*****************************************************************************
\par filename: stub.h
\par  authors: Jonathan Meyer, Ashkon Khalkhali, Alijah Rosner, Mason Dulay
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include <vector>
#include "StateMachine.h"
#include "Component.h"
#include "Scene.h"
#include "ECS.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
enum class DoorState
{
  Idle,
  Triggered
};

namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
  /*!*************************************************************************
    \brief
      This is an example of a component class.
  ***************************************************************************/
  class ColliderState : public StateMachine<DoorState>
  {
  private:
    const char* SceneDir_;
    Engine* Engine_;
    void sceneInit(Manager<Scene>& sceneManager);
  public:


    ColliderState(Engine* engine) : Engine_(engine), SceneDir_(nullptr) {}
    // Inherited via StateMachine
    virtual void OnInit() override;

    virtual void OnUpdate(float dt) override;

    virtual void OnExit() override;

    virtual void Read(const rapidjson::Value& value) override;

  };

  class DoorBehavior : public Component
  {
  private:
    int someExample = 0;   //!< this is an example of a member value.
    Engine* engine_{ nullptr };
    std::shared_ptr<StateMachine<DoorState>> collider_;

  public:
    DoorBehavior(Engine* engine, Entity parent) : Component(parent), engine_(engine), collider_(new ColliderState(engine)) {}
     
    void Init();           //!< Our ExampleComponent Init Function. 

    void Update(float dt); //!< Our ExampleComponent Update Function.

    static std::string_view Name() { return "DoorBehavior"; }

  }; /* ExampleComponent */

}; /* Normalloc */