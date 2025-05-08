/*!*****************************************************************************
\par filename: Entity.cpp
\par  authors: Alijah Rosner, Ashkon Khalkhali
\par     date: 09/11/2022

\brief
  Implementation of our Entity class
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Entity.h"
#include "Physics.h"
#include "Transform.h"
#include "Stream.h"
#include "Scene.h"
#include "ECS.h"
#include "SetupFactory.h"

namespace FFunctional
{
  //Ashies special constructor
    Entity::Entity(ECS* Ecs_, EntityID id) : parent_(Ecs_), entityID_(id),
    signalMap_(std::make_shared<signalMap_t>())
    {
        isDestroyed_ = false;
    }

    Entity::~Entity()
    {

    }

    void Entity::Destroy()
    {
      parent_->AddToKillList(entityID_);
      isDestroyed_ = true;
    }

    bool Entity::IsDestroyed()
    {
        return isDestroyed_;
    }

    EntityID Entity::GetEntity()
    {
      return entityID_;
    }

    void Entity::Init()
    {
      parent_->Init(entityID_);
    }

    void Entity::Read(const rapidjson::Value& value, Engine* engine)
    {
        const auto& readEntity = value.GetObj();
        auto factory = engine->GetSystem<ComponentFactory>();

        //EntityID id = parent_->EntityCreate();
        Entity entity(parent_, entityID_);

        for (const auto& member : readEntity)
        {
            factory->Create(member.name.GetString(), engine, entity, member.value);
        }
        entity.Init();
    }

    void* Entity::ComponentGetByName(std::string_view typeName)
    {
      return parent_->ComponenetGetByName(entityID_, typeName);
    }

}

