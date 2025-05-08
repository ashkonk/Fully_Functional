/*!*****************************************************************************
\par filename: TeleportUI.cpp
\par  authors: Jonathan Meyer, Ashkon Khalkhali, Alijah Rosner, Mason Dulay
\par     date: 09/09/2022

\brief
  This is the TeleportUI file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "TeleportUI.h"
#include "Sprite.h"
#include "PlayerController.h"

namespace FFunctional
{
	void TeleportUI::Update(float)
	{
		int cooldown = GetParent().GetParent()->ComponentGetFirst<PlayerController>().GetonCooldown();
		if (cooldown == 0)
		{
			GetParent().ComponentGet<Sprite>().SetOpacity(1.0f);
		}
		else
		{
			float newOpacity = float(cooldown) / 5;
			GetParent().ComponentGet<Sprite>().SetOpacity(newOpacity);
		}
	}

	void TeleportUI::Read(const rapidjson::Value&)
	{
	}
}