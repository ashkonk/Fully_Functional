/*!*****************************************************************************
\par filename: Interactable.h
\par  authors: Mason Dulay
\par     date: 09/09/2022

\brief
  This is the Armament class declaration for items in Ambrose's Toolkit.
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
#include "InteractableState.h"
#include "Trace.h"


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
  //
  //
  //


  class Armament : public Behavior<Armament> /* Example Component  * * * * */
  {
  private:
    Input* input_;
    Engine* engine_{ nullptr };
    GLFWwindow* window_;
    //std::shared_ptr<StateMachine<ClickState>> itemState_;
    bool ready_, active_, sending_, retrieving_;

    //int type_;
    //InteractableState<ItemType> state_;

    glm::vec3 offset_;
    glm::vec3 inventoryPos_;
    glm::vec3 destination_;
    glm::vec3 playerPos_;

    //float radius_ = 0.5f;
    //float timeRate_ = 0.5f;
    //float curve_ = 3.0f;

    std::vector<float> radius_wheel;
    std::vector<float> curve_wheel;
    std::vector<float> time_wheel;

    unsigned radiusIndex = 0;
    unsigned curveIndex = 0;
    unsigned timeIndex = 0;



    Keystate lastKeyState_ = Keystate::Up;
    Keystate currentKeyState_ = Keystate::Up;

    Keystate lastRightState_ = Keystate::Up;
    Keystate currRightState_ = Keystate::Up;

    Keystate lastCycleState_ = Keystate::Up;
    Keystate currCycleState_ = Keystate::Up;

    /* keeping track of order */
    static int armamentCount_;
    int armamentId = -1;




    void UpdateIdle(const glm::vec3 cameraPos);

    void Fire(const glm::vec3 cursorPos, const glm::vec3 playerPos);

    void Recall(const glm::vec3 playerPos);

    //return cursor click and where in game space
    std::pair<bool, glm::vec3> CursorClick();


    //itemState_(new InteractableState(parent, engine->GetSystem<Input>(), engine->GetSystem<Window>())?
  public:
    Armament(Engine* engine, Entity parent) : Behavior(parent), engine_(engine),
      ready_(false), active_(false), sending_(false), retrieving_(false),
      playerPos_(), offset_(), inventoryPos_(), 
      destination_()//,
      //itemState_(new InteractableState(parent, engine->GetSystem<Input>(), engine->GetSystem<Window>()->GetWindow()))
    {
      Component::UnsetTimeNode(this);
      input_ = engine->GetSystem<Input>();
      window_ = engine->GetSystem<Window>()->GetWindow();
    }

    void SetDestination(glm::vec3 destPos);

    const glm::vec3 GetDestination();

    void SetPlayerPos(glm::vec3 playerPos);

    const glm::vec3 GetPlayerPos();

    bool GetState();

    bool GetActive();
    
    void SetDown();

    //void Init();

    void Update(float dt);

    void Read(const rapidjson::Value& value);

    void Exit();

    static std::string_view Name();

    static void ResetArmament();

  }; /* ExampleComponent */

}; /* Normalloc */