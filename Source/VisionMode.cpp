/*!*****************************************************************************
\par filename: VisionMode.cpp
\par  authors: Mason Dulay
\par     date: 11/30/2022

\brief
  This is the VisionMode file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "VisionMode.h"
#include "Sprite.h"
#include "PlayerController.h"

namespace FFunctional
{
	void VisionMode::Update(float dt)
	{
		dt;

		ECS* ecs = GetParent().GetParent();
		// get vision mode
		PlayerController pController = ecs->ComponentGetFirst<PlayerController>();
		mode_ = pController.GetVisionState();

		switch (mode_)
		{
			case 0:
			{
				glm::vec4 uvs = { 0.0f, 0.0f, 0.5f, 1.0f };
				Entity parent = GetParent();
				Sprite* counter = &parent.ComponentGet<Sprite>();
				counter->SetUVs(uvs);
				break;
			}
			case 1:
			{
				glm::vec4 uvs = { 0.5f, 0.0f, 0.5f, 1.0f };
				Entity parent = GetParent();
				Sprite* counter = &parent.ComponentGet<Sprite>();
				counter->SetUVs(uvs);
				break;
			}
		}
	}

	void VisionMode::Read(const rapidjson::Value&)
	{

	}
}