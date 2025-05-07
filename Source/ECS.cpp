/*!*****************************************************************************
\par filename: ECS.cpp
\par  authors: Ashkon Khalkhali
\par     date: 09/09/2022

\brief
  This file contains the implimentation of our Entity Component System.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "ECS.h"

namespace FFunctional
{
  template<typename ComponentType>
  inline bool ECS::Has(EntityID id)
  {
    ComponentManagerPtr<ComponentType> typeManager = GetManager<ComponentType>();
    return (typeManager && typeManager->Has(id));
  }

  template<typename ComponentType>
  inline bool ECS::HasManager() const
  {
    auto typeManager = GetManager<ComponentType>();
    return (typeManager && typeManager->GetSize());
  }

  template<typename ComponentType>
  inline ComponentManagerPtr<ComponentType> ECS::GetManager() const
  {
    for (IComponentManagerPtr componentManager : ComponentManagerList_)
    {
      if (ComponentType::Name() == componentManager->GetManagedType())
      {
        auto ManagerType = std::dynamic_pointer_cast<ComponentManager<ComponentType>>(componentManager);
        return ManagerType;
      }
    }
    return nullptr;
  }

  template<typename ComponentType, typename ...Args>
  inline ComponentType& ECS::ComponentAdd(EntityID entity, Args... args)
  {
    ComponentManagerPtr<ComponentType> managerType = GetManager<ComponentType>();
    if (!managerType)
    {
      managerType = std::make_shared<ComponentManager<ComponentType>>();
      ComponentManagerList_.push_back(managerType);
    }
    return managerType->Create(entity, std::forward<Args>(args)...);;
  }

  template<typename ComponentType>
  inline ComponentType& ECS::ComponentGetFirst()
  {
    auto compManager = GetManager<ComponentType>();
    auto& comp = compManager->GetFirstComponent();
    return comp;
  }

  template<typename ComponentType>
  inline ComponentType& ECS::ComponentGet(EntityID entity, size_t index) {
    auto componentManager = GetManager<ComponentType>();
    auto& component = componentManager->GetComponent(entity, index);
    return component;
  }

  template<typename T>
  inline void ECS::Update(float dt)
  {
    if (Active_)
    {
      for (IComponentManagerPtr manager : ComponentManagerList_)
      {
        if (T::Name() == manager->GetManagedType())
        {
          manager->Update(dt);
        }
      }
    }
  }
  inline void ECS::Add(IComponentManagerPtr newManager)
  {
    ComponentManagerList_.push_back(newManager);
  }

  inline void ECS::SetActive(bool flag)
  {
    Active_ = flag;
  }

  inline bool ECS::GetActive()
  {
    return Active_;
  }

  inline EntityID ECS::EntityCreate()
  {
    return ++EntityCounter_;
  }

  inline void ECS::AddToKillList(EntityID entity)
  {
    for (IComponentManagerPtr managers : ComponentManagerList_)
    {
      managers->Kill(entity);
    }
  }

  inline EntityID ECS::EntityCopy(ECS* oldEcs, EntityID entity)
  {
    EntityID newEntity = EntityCreate();
    for (IComponentManagerPtr OldComponentManager : oldEcs->ComponentManagerList_)
    {
      if (OldComponentManager->Has(entity))
      {
        IComponentManagerPtr newManager;
        for (IComponentManagerPtr componentManager : ComponentManagerList_)
        {
          if (OldComponentManager->GetManagedType() == componentManager->GetManagedType())
          {
            newManager = componentManager;
          }
        }
        if (!newManager)
        {
          newManager = std::shared_ptr<IComponentManager>(OldComponentManager->CloneType());
          Add(newManager);
        }
        newManager->Copy(OldComponentManager.get(), entity, newEntity, this);
      }
    }
    return newEntity;
  }

  inline void* ECS::ComponenetGetByName(EntityID entity, std::string_view name)
  {
    for (IComponentManagerPtr managers : ComponentManagerList_)
    {
      if (managers->GetManagedType() == name)
      {
        return managers->GetBehavior(entity);
      }
    }
    return nullptr;
  }

  inline void ECS::Init(EntityID id)
  {
    for (IComponentManagerPtr managers : ComponentManagerList_)
    {
      if (managers->Has(id))
      {
        managers->Init(id);
      }
    }
  }

  inline void ECS::Draw() const
  {
    for (IComponentManagerPtr ComponentManager : ComponentManagerList_)
    {
      ComponentManager->Render();
    }
  }

  inline void ECS::Clear()
  {

    clear_ = true;
  }

  inline void ECS::DestroyQueuedEntities()
  {
    for (IComponentManagerPtr manager : ComponentManagerList_)
    {
      manager->ExecuteKillList();
    }
    if (clear_)
    {
      for (IComponentManagerPtr componentManager : ComponentManagerList_)
      {
        componentManager->Clear();
      }
    }
  }

  inline void ECS::ForceClear()
  {

    for (IComponentManagerPtr componentManager : ComponentManagerList_)
    {
      componentManager->Clear();
    }
  }

  inline void ECS::Exit()
  {

  }

}