/*!*****************************************************************************
\par filename: Entity.h
\par  authors: Alijah Rosner
\par     date: 09/09/2022

\brief
  Implementation for our Entity class.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "ECS.h"
#include "Stream.h"
#include "Engine.h"
#include "Trace.h"

#include <string>
#include <map>
#include <memory>

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/

namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{
  /*!***************************************************************************
    \brief
      This is the entity class and it's function
  *****************************************************************************/

  struct Signal
  {
    template<typename component>
    Signal(const component& seed) : parent(seed.GetParent().GetEntity()),
      parentEcs(seed.GetParent().GetParent()), componentName(std::string(component::Name()))
    {};

    EntityID parent;
    ECS* parentEcs;
    std::string componentName;

  };

  class Entity //: public Component /* Example Component  * * * * * * */
  {

  public:
    /* Default constructor */
    Entity(ECS* Ecs_, EntityID id);

    /* Default destructor  */
    ~Entity();

    /*void Read(const rapidjson::Value &value);*/

    void Destroy();

    bool IsDestroyed();

    void Init();

    void Read(const rapidjson::Value& value, Engine* engine);

    template<typename T>
    bool Has() const
    {
      if (parent_->Has<T>(entityID_))
      {
        return true;
      }
      return false;
    }

    //void* are problematic, maybe strings?

    void AddSignal(std::string_view name, Signal obj)
    {
      (*signalMap_)[std::string(name)].push_back(obj);
    }

    template<typename ...Args>
    std::vector<Signal> GetSignals(std::string_view name)
    {
      return (*signalMap_)[std::string(name)];
    }

    void SetParent(ECS* parent)
    {
      parent_ = parent;
    }

    ECS* GetParent()
    {
      return parent_;
    }
    EntityID GetEntity();

    template<typename T>
    T& ComponentGet(size_t index = 0) const
    {
      return parent_->ComponentGet<T>(entityID_, index);
    }

    template<typename T, typename ...Args>
    T& ComponentAdd(Args... args)
    {
      return parent_->ComponentAdd<T>(entityID_, std::forward<Args>(args)...);
    }

    //DANGEROUS.
    void* ComponentGetByName(std::string_view typeName);


    //Get and set components through the ECS
    //Set ECS through constructor


  private:
    EntityID entityID_;
    bool isDestroyed_;
    ECS* parent_;

    using signalMap_t = std::map<std::string, std::vector<Signal>>;

    //can't pass info using stringview
    std::shared_ptr<signalMap_t> signalMap_;
  };

}; /* FFunctional */