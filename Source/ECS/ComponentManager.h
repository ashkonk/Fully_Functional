/*!*****************************************************************************
\par filename: ComponentManager.h
\par  authors: Ashkon Khalkhali
\par     date: 09/21/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#pragma once

#include <vector>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "IComponentManager.h"

namespace FFunctional
{

  // keycheck for switching state machines in here,
  // cpp state machines handle all internal code.

  template<typename C>
  class ComponentManager : public IComponentManager
  {
  private:

    std::vector<C> ComponentList_;
    std::vector<EntityID> KillList_;
    std::unordered_map<EntityID, std::vector<size_t>> IDtoIndex_;
    std::unordered_map<size_t, EntityID> IndextoID_;

  public:
    ComponentManager();

    //return the first valid component in list
    C& GetFirstComponent();

    const std::vector<C>& GetComponentList();

    C& GetComponent(EntityID entity);

    std::vector<C*> GetAllComponent(EntityID entity);

    std::vector<const C*> GetAllComponent(EntityID entity) const;

    size_t GetComponentIndex(EntityID entity) const;

    const C& GetComponent(EntityID entity, size_t index = 0) const;
    C& GetComponent(EntityID entity, size_t index = 0);
    
    C& operator[](unsigned index);

    unsigned GetSize() const;

    // Inherited via IComponentManager
    virtual void Init() override;

    // Init on Run time
    virtual void Init(EntityID id) override;


    template<typename ...Args>
    C& Create(EntityID entity, Args&&... args);

    void Copy(const IComponentManager* compManager, EntityID oldEntity, EntityID newEntity, ECS* ecs) override;

    virtual void* GetBehavior(EntityID entity) override;

    IComponentManager* CloneType() const override;


    virtual void Update(float dt) override;

    //Kenny's Favorite
    void ExecuteKillList() override;


    virtual void Render() const override
    {
      //for (const C& components : ComponentList_)
      //{
        //components.Draw();
      //}
    }

    std::string_view GetManagedType() const override;

    // Inherited via IComponentManager
    virtual void Kill(EntityID entity) override;

    bool Has(EntityID id) const override;


    void Clear() override;

  };

  //add to cpp for cool code count :D

  template<typename T>
  using ComponentManagerPtr = std::shared_ptr<ComponentManager<T>>;

};

#include "ComponentManager.cpp"