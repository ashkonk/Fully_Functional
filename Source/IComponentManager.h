/*!*****************************************************************************
\par filename: IComponentManager.h
\par  authors: Ashkon Khalkhali
\par     date: 09/21/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include <string_view>
#include "EntityID.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{
  class ECS;

  //Interfaces let you hold a bunch of objects that have similar behaviors
  //Since our Component Manager is templated, we use an interface to hold all of them
  //together. Brothers in Arms

  /*!***************************************************************************
    \brief
      This is an example of a component class.
  *****************************************************************************/
  struct IComponentManager //: public Component /* Example Component  * * * * * * */
  {
      virtual void Init() = 0;     

      virtual void Init(EntityID id) = 0;

      virtual void Update(float dt) = 0; //!< Our ExampleComponent Update Function.

      virtual void Render() const = 0;

      virtual void Kill(EntityID entity) = 0;

      virtual void ExecuteKillList() = 0;

      virtual ~IComponentManager() { };

      virtual std::string_view GetManagedType() const = 0;

      virtual bool Has(EntityID id) const = 0;

      virtual void Clear() = 0; 

      virtual void Copy(const IComponentManager* compManager, EntityID entity, EntityID newEntity, ECS* ecs) = 0;

      virtual void* GetBehavior(EntityID entity) = 0;

      virtual IComponentManager* CloneType() const = 0;

  }; /* ExampleComponent */

}; /* Normalloc */