/*!*****************************************************************************
\par filename: stub.h
\par  authors: Jonathan Meyer, Ashkon Khalkhali, Alijah Rosner, Mason Dulay
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include <utility>

#include "Input.h"
#include "Window.h"
#include "StateMachine.h"
#include "Entity.h"
#pragma once


/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
  // B Bucket type beat
  // 
  //using IStateMachinePtr = IStateMachine*;
  //class IStateMachine;

  //enum class ItemType
  //{
  //  Invalid,
  //  Drag,
  //  Click
  //};

  enum class ClickState
  {
    Ready,
    Active,
    Sending,
    Retrieving
  };


  class InteractableState : public StateMachine<ClickState>
  {
  public:
    InteractableState(Entity parent, Input* input, GLFWwindow* window) : parent_(parent), 
      input_(input), window_(window), selected_(false), destination_(), offset_() {}
    virtual void OnInit() override;
    virtual void OnUpdate(float dt) override;
    virtual void OnExit() override;
    virtual void Read(const rapidjson::Value& value) override;

  private:

    Entity parent_;
    Input* input_;
    GLFWwindow* window_;

    //we will use this to determine which interactable
    bool selected_;

    glm::vec3 destination_;
    glm::vec3 offset_;

    //Cursor Info (Click & Pos)
    std::pair<bool, glm::vec3> ClickData();

    void ClickUpdateReady();
    void ClickUpdateActive();
    void ClickUpdateSending();
    void ClickUpdateRetrieving();

    void SetDestination(const glm::vec3 pos);
    const glm::vec3 GetDestination();



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

  };

  //B Bucket stuff
  //enum class DragState
  //{
  //  Idle,
  //  Selected,
  //  Active
  //};

  //class DragItem : public StateMachine<DragState>
  //{
  //public:
  //  DragItem(Entity parent) : parent_(parent) {}

  //  // Inherited via StateMachine
  //  virtual void OnInit() override;
  //  virtual void OnUpdate(float dt) override;
  //  virtual void OnExit() override;
  //  virtual void Read(const rapidjson::Value& value) override;

  //  std::pair<bool, glm::vec3> DragData();
  //  void DragUpdateIdle();
  //  void DragUpdateSelected();
  //  void DragUpdateReady();


  //private:
  //  Entity parent_;
  //};

}; /* Normalloc */