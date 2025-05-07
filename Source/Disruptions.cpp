/*!*****************************************************************************
\par filename: Disruptions.cpp
\par  authors: Mason Dulay
\par     date: 11/30/2022

\brief
  This is the Disruptions file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "Disruptions.h"
#include "Sprite.h"
#include "TrashFire.h"
#include "CCTV.h"
#include "AttendantA.h"

namespace FFunctional
{
	void Disruptions::Update(float dt)
	{
		dt;

		caused_ = 0;

		ECS* ecs = GetParent().GetParent();

		// check the status of fusebox

		// check the status of trashfire
		TrashFireBehavior tFireBehavior = ecs->ComponentGetFirst<TrashFireBehavior>();
		if (tFireBehavior.GetTrashFireUsed() == true)
		{
			++caused_;
		}

		// check the status of attendant
		AttendantBehavior attendantBehavior = ecs->ComponentGetFirst<AttendantBehavior>();
		if (attendantBehavior.GetDisabled() == true)
		{
			++caused_;
		}

		if (GetParent().GetParent()->HasManager<DoorBehavior>())
		{
			DoorBehavior& door = GetParent().GetParent()->ComponentGetFirst<DoorBehavior>();
			caused_ += door.GetDebugTasks();
		}

		caused_ = (caused_ > 3) ? 3 : caused_;
		switch (caused_)
		{
			case 1:
			{
				glm::vec4 uvs = { 0.25f, 0.0f, 0.25f, 1.0f };
				Entity parent = GetParent();
				Sprite* counter = &parent.ComponentGet<Sprite>();
				counter->SetUVs(uvs);
				break;
			}
			case 2:
			{
				glm::vec4 uvs = { 0.5f, 0.0f, 0.25f, 1.0f };
				Entity parent = GetParent();
				Sprite* counter = &parent.ComponentGet<Sprite>();
				counter->SetUVs(uvs);
				break;
			}
			case 3:
			{
				glm::vec4 uvs = { 0.75f, 0.0f, 0.25f, 1.0f };
				Entity parent = GetParent();
				Sprite* counter = &parent.ComponentGet<Sprite>();
				counter->SetUVs(uvs);
				break;
			}
			default:
			{
				glm::vec4 uvs = { 0.0f, 0.0f, 0.25f, 1.0f };
				Entity parent = GetParent();
				Sprite* counter = &parent.ComponentGet<Sprite>();
				counter->SetUVs(uvs);
				break;
			}
		}
		tasksDone = caused_;
	}

	void Disruptions::Read(const rapidjson::Value&)
	{

	}

	int Disruptions::GetTasksDone() const
	{
		return tasksDone;
	}
}