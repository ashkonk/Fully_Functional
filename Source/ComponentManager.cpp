/*!*****************************************************************************
\par filename: ComponentManager.h
\par  authors: Ashkon Khalkhali
\par     date: 09/21/2022

\brief
  Manager for components
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#pragma once

#include "ComponentManager.h"


namespace FFunctional
{

  static void UpdateMovedIndexes(EntityID movedEntity, std::unordered_map<EntityID, std::vector<size_t>>& idMap,
    size_t oldIndex, size_t newIndex);

  template<typename T>
  using ComponentManagerPtr = std::shared_ptr<ComponentManager<T>>;

  template<typename C>
  inline ComponentManager<C>::ComponentManager()
    : ComponentList_(), KillList_(),
    IDtoIndex_(), IndextoID_()
  {
  }

  //return the first valid component in list
  template<typename C>
  inline C& ComponentManager<C>::GetFirstComponent()
  {
    return ComponentList_.front();
  }

  template<typename C>
  const std::vector<C>& ComponentManager<C>::GetComponentList()
  {
    return ComponentList_;
  }

  //returns the first component of type requested
  template<typename C>
  inline C& ComponentManager<C>::GetComponent(EntityID entity)
  {
    auto iterator = IDtoIndex_.find(entity);
    size_t index = iterator->second.at(0);
    //  if (index != ComponentList_.size())
      //{
    return ComponentList_[index];
    //    }
  }

  template<typename C>
  std::vector<C*> ComponentManager<C>::GetAllComponent(EntityID entity)
  {
    std::vector<C*> components;
    auto iterator = IDtoIndex_.find(entity);
    for (size_t index : iterator->second)
    {
      components.push_back(&ComponentList_[index]);
    }
    return components;
  }

  template<typename C>
  std::vector<const C*> ComponentManager<C>::GetAllComponent(EntityID entity) const
  {
    std::vector<const C*> components;
    auto iterator = IDtoIndex_.find(entity);
    for (size_t index : iterator->second)
    {
      components.push_back(&ComponentList_[index]);
    }
    return components;
  }

  template<typename C>
  inline size_t ComponentManager<C>::GetComponentIndex(EntityID entity) const
  {
    auto iterator = IDtoIndex_.find(entity);
    size_t index = iterator->second.at(0);
    return index;
  }

  template<typename C>
  inline const C& ComponentManager<C>::GetComponent(EntityID entity, size_t index) const
  {
    auto iterator = index + IDtoIndex_.find(entity);

    size_t value = iterator->second.at(0);
    return ComponentList_[value];
  }

  template<typename C>
  inline C& ComponentManager<C>::GetComponent(EntityID entity, size_t index)
  {
      auto iterator = IDtoIndex_.find(entity);

      for (int i = 0; i < index; ++i, ++iterator);
      // iterator += index;

      size_t value = iterator->second.at(0);
      return ComponentList_[value];
  }

  template<typename C>
  C& ComponentManager<C>::operator[](unsigned index)
  {
    return ComponentList_[index];
  }

  template<typename C>
  inline unsigned ComponentManager<C>::GetSize() const
  {
    {
      return (unsigned)ComponentList_.size();
    }
  }

  template<typename C>
  inline void ComponentManager<C>::Init()
  {
    {
      for (C& components : ComponentList_)
      {
        components.Init();
      }
    }
  }

  template<typename C>
  inline void ComponentManager<C>::Init(EntityID id)
  {
    std::vector<C*> component = GetAllComponent(id);
    for (C* components : component)
    {
      components->Init();
    }
  }

  template<typename C>
  inline void ComponentManager<C>::Copy(const IComponentManager* compManager, EntityID oldEntity, EntityID newEntity, ECS* ecs)
  {
    const ComponentManager<C>* manager = dynamic_cast<const ComponentManager<C>*>(compManager);
    std::vector<const C*> compsToAdd = manager->GetAllComponent(oldEntity);
    for (const C* components : compsToAdd)
    {
      C& newComp = Create(newEntity, *components);
      newComp.SetParent(ecs, newEntity);
    }
  }

  template<typename C>
  inline IComponentManager* ComponentManager<C>::CloneType() const
  {
    return new(ComponentManager);
  }

  template<typename C>
  inline void ComponentManager<C>::Update(float dt)
  {
    for (C& components : ComponentList_)
    {
      components.Update(dt*components.GetDilation());
    }
    // ExecuteKillList();
  }

  template<typename C>
  inline std::string_view ComponentManager<C>::GetManagedType() const
  {
    return C::Name();
  }

  template<typename C>
  inline void ComponentManager<C>::Kill(EntityID entity)
  {
    {
      KillList_.push_back(entity);
    }
  }

  template<typename C>
  inline bool ComponentManager<C>::Has(EntityID id) const
  {
    if (IDtoIndex_.find(id) != IDtoIndex_.end())
    {
      return true;
    }
    return false;
  }

  template<typename C>
  void ComponentManager<C>::Clear()
  {
    ComponentList_.clear();
    KillList_.clear();

    IDtoIndex_.clear();
    IndextoID_.clear();
  }

  //get the vector of indexes attached to entity
  //push back that vector
  template<typename C>
  template<typename ...Args>
  inline C& ComponentManager<C>::Create(EntityID entity,
    Args&&... args)
  {
    ComponentList_.emplace_back( std::forward<Args>(args)... );
    IDtoIndex_[entity].push_back(ComponentList_.size() - 1);
    IndextoID_.insert({ (ComponentList_.size() - 1), entity });
    return ComponentList_.back();
  }

  template<typename C>
  inline void* ComponentManager<C>::GetBehavior(EntityID entity)
  {
    auto iterator = IDtoIndex_.find(entity);
    size_t index = iterator->second.at(0);
    return &ComponentList_[index];
  }

  template<typename C>
  inline void ComponentManager<C>::ExecuteKillList()
  {
    for (EntityID entities : KillList_)
    {
      auto iteratorRemove = IDtoIndex_.find(entities);

      if (iteratorRemove == IDtoIndex_.end())
      {
        continue;
      }

      auto indexesToRemove = iteratorRemove->second;
      for (size_t index : indexesToRemove)
      {
        auto indexReplace = ComponentList_.size() - 1;
        //id of components you're swapping with index
        auto idReplace = IndextoID_[indexReplace];
        std::swap(ComponentList_[index], ComponentList_.back());
        ComponentList_.pop_back();

        UpdateMovedIndexes(idReplace, IDtoIndex_, indexReplace, index);

        IndextoID_.erase(indexReplace);
        IndextoID_[index] = idReplace;
      }
      IDtoIndex_.erase(entities);
    }
    KillList_.clear();
  }

  // Take a reference for map

  inline static void UpdateMovedIndexes(EntityID movedEntity, std::unordered_map<EntityID, std::vector<size_t>>& idMap,
    size_t oldIndex, size_t newIndex)
  {
    // all indexes of moved entity
    std::vector<size_t>& indexList = idMap.find(movedEntity)->second;
    // iterator to spot of old index
    std::vector<size_t>::iterator indexIterator = std::find(indexList.begin(), indexList.end(), oldIndex);
    // moving where it thinks it exists
    if (indexIterator != indexList.end())
    {
      *indexIterator = newIndex;
    }
  }
}
