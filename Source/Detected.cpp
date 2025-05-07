/*!*****************************************************************************
\par filename: Detected.cpp
\par  authors: Mason Dulay
\par     date: 09/09/2022

\brief
  This is the Detected file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "Detected.h"
#include "StealthBar.h"
#include "Audio.h"

namespace FFunctional
{
	void Detected::Update(float)
	{
		float fill = GetParent().GetParent()->ComponentGetFirst<StealthBar>().GetFill();

		if (beforeFill >= fill)
		{
			declining = true;
		}
		else
		{
			declining = false;
		}

		if (!declining)
		{
			if (!initialCaught_->GetIsPlaying())
			{
					//initialCaught_.UpdateCheck(parent_, parent_);
					initialCaught_->Play();
			}
		}
		else
		{
			beforeFill = fill;
			initialCaught_->Stop();
		}
		beforeFill = fill;

		GetParent().ComponentGet<Sprite>().SetOpacity(fill);
	}

	void Detected::Read(const rapidjson::Value&)
	{
			initialCaught_ = engine_->GetSystem<Audio>()->GetSound("InitialCaught");
			/*rapidjson::Document docu = OpenJson("./Data/Audio/SFX/InitialCaught.json");*/
			/*initialCaught_->Load(docu, engine_->GetSystem<Audio>());*/
			initialCaught_->SetInRange(true);
	}
}