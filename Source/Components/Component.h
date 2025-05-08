/*!*****************************************************************************
\par filename: Component.h
\par  authors: Alijah Rosner
\par     date: 09/11/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#ifndef COMPONENT_HEADER
#define COMPONENT_HEADER
#include "Entity.h"
#include "glm/vec3.hpp"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/


namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{

  /*!***************************************************************************
    \brief
      This is a public struct for storing time dilation nodes that affect
      the dt of our components.
  *****************************************************************************/
  struct TimeNode
  {
    glm::vec3 pos;             /*!< The position of our time dilation node.   */
    float radius, rate, curve; /*!< The properties of our time dilation node. */
  };



  /*!***************************************************************************
    \brief
      This is an example of a component class.
  *****************************************************************************/
  class Component //: public Component /* Example Component  * * * * * * */
  {
    //inheirited classes can see it
  protected:
      Entity parent_;
      static float globalTime_;

  public:
      Component(Entity parent);
      virtual void Init();
      virtual void Update(float dt);
      virtual void Draw() const; 
      Entity GetParent() const; 
      virtual ~Component();
      virtual void SetParent(Entity parent);
      virtual void SetParent(ECS* ecs, EntityID id);

      float GetDilation(bool relativity = false) const;

      static void SetGlobalTimeDialation(float f);

      static float GetGlobalTimeDialation();

      static void SetTimeNode(Component* cid, glm::vec3 pos, float rate,
      float radius, float curve);

      static void UnsetTimeNode(Component* cid);

      static std::map<Component*, TimeNode>& GetNodes();

  }; /* ExampleComponent */

}; /* Normalloc */
#endif