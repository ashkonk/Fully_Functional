/*!*****************************************************************************
\par filename: ECS.h
\par  authors: Ashkon Khalkhali
\par     date: 09/25/2022

\brief
  This file contains the class and public declarations for 
  our Entity Component System.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include <vector>
#include <memory>
#include "EntityID.h"
#include "ComponentManager.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{
  /*!***************************************************************************
    \brief
      This is an example of a component class.
  *****************************************************************************/
  using IComponentManagerPtr = std::shared_ptr<IComponentManager>;

  class ECS //: public Component /* Example Component  * * * * * * */
  {
  private:
    std::vector<IComponentManagerPtr> ComponentManagerList_;

    EntityID EntityCounter_ = 0;

    bool Active_ = false;

    bool clear_ = false;

    void Add(IComponentManagerPtr newManager);

  public:

    void SetActive(bool flag);

    bool GetActive();

    EntityID EntityCreate();

    void AddToKillList(EntityID entity);

    template<typename ComponentType>
    bool Has(EntityID id);


    template<typename ComponentType>
    bool HasManager() const;


    template<typename ComponentType, typename ...Args>
    ComponentType& ComponentAdd(EntityID entity, Args... args);

    EntityID EntityCopy(ECS* oldEcs, EntityID entity);

    template<typename ComponentType>
    ComponentType& ComponentGetFirst();

    template<typename ComponentType>
    ComponentType& ComponentGet(EntityID entity, size_t index = 0);

    void* ComponenetGetByName(EntityID entity, std::string_view name);

    template<typename ComponentType>
    ComponentManagerPtr<ComponentType> GetManager() const;


    void ForceClear();

    void Init(EntityID id);        //!< Our ExampleComponent Initialization Function. 

    void Draw() const;

    void Clear();

    template<typename T>
    void Update(float dt);

    void DestroyQueuedEntities();

    void Exit();           //!< Our ExampleComponent Shutdown Function.

  }; /* ExampleComponent */

}; /* Normalloc */

#include "ECS.cpp"