/*!*****************************************************************************
\par filename: GuardBehavior.h
\par  authors: Alijah Rosner
\par     date: 11/16/2022

\brief
    This file contains the behavior component and the state machine belonging to
    Guard Drone.
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include <vector>
#include "StateMachine.h"
#include "Component.h"
#include "Behavior.h"
#include "Audio.h"
#include "Sound.h"

enum class GuardState
{
  Passive,
  Investigating,
  Chase,
  Puzzle
};

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
  /*!*************************************************************************
    \brief
      Attendant movement state machine class.
  ***************************************************************************/
  class GuardMovement : public StateMachine<GuardState> //: public Component /* Example Component  * * * * */
  {
  private:
    Entity parent_;
    std::vector<glm::vec3> positions_;
    std::vector<glm::vec3> puzzlePositions_;
    int puzzleRoute_;
    float speed_;
    bool caught_; 
    int route_;
    float alertCounter_; 
    glm::vec3 investigatePosition_;
    Entity investigateEntity_;
    float detectTime_;

    //helper functions
    void UpdatePassivePosition(float dt);
    void UpdateAlertedPosition(float dt);
    void UpdateInvestigating(float dt);
    void UpdateChase(float dt);
  public:
    bool GetCaught() const;
    void Read(const rapidjson::Value&);
    GuardMovement(Entity parent) : parent_(parent), route_(0), speed_(0.0f),
    alertCounter_(0), investigatePosition_(0), investigateEntity_(nullptr, 0) {}
    virtual void OnInit() override;
    virtual void OnUpdate(float dt) override;
    void UpdatePuzzlePosition(float dt);
    virtual void OnExit() override;
    float GetAlertCounter() const;
    void SetInvestigatePosition(const glm::vec3 vector);
    float GetDetectTime() const;

    void SetInvestigateEntity(const Entity& entity);

  };

  /*!*************************************************************************
    \brief
      Attendant Behavior class.
  ***************************************************************************/
  class GuardBehavior : public Behavior<GuardBehavior>
  {
  private:
    std::shared_ptr<GuardMovement> movement_;
    bool colliding_;
    bool distracted_;
    float timer_ = 0.0f; 
    std::shared_ptr<Sound> walk_{ nullptr };
    //Entity visionCone_; 
   // Entity visionCone_;
    Engine* engine_;
    bool colliding = false;


  public:
      GuardBehavior(Engine* engine, Entity parent) :
          Behavior(parent),
          engine_(engine),
          distracted_(false),
        colliding_(false),
      movement_(new GuardMovement(parent)) /* visionCone_(parent_.GetParent(), parent_.GetParent()->EntityCreate()) */{}
    bool GetColliding() const;
    bool GetDistracted() const;
    void Read(const rapidjson::Value&);
    void Update(float dt);

    void EntityFound(const Vision& self, const Entity& other);
    void EntityLost(const Vision& self, const Entity& other);


    static std::string_view Name() { return "GuardBehavior"; }

  };

};