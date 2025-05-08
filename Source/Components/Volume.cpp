/*!*****************************************************************************
\par filename: Volume.cpp
\par  authors: Jonathan Meyer, Ashkon Khalkhali, Alijah Rosner, Mason Dulay
\par     date: 09/09/2022

\brief
  This is the Volume file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "Volume.h"
#include "Sprite.h"

#define WAIT_TIME 0.5f

namespace FFunctional
{
	float VolumeBehavior::xOffset_ = 0.0f;
	float VolumeBehavior::volume = 0.5f;

	void VolumeBehavior::Update(float)
	{
		audio_->SetMasterVolume(volume);

		GetParent().ComponentGet<Sprite>().SetUVs(glm::vec4(0.0f, 0.0f, volume, 1.0f));
		glm::vec3 vec = glm::vec3(volume * scaleInit.x, scaleInit.y, scaleInit.z);
		glm::vec3 pos = glm::vec3((volume - 1) * scaleInit.x * 0.5f, 0, 0);
		pos += translationInit;

		GetParent().ComponentGet<Transform>().SetScale(&vec);
		GetParent().ComponentGet<Transform>().SetTranslation(&pos);
	}

	void VolumeBehavior::IncrementVolume()
	{
		if (volume < 1.0f)
		{
			// set volume higher
			volume += 0.1f;
			// set slider offset
			xOffset_ += 0.0085f;
		}
	}

	void VolumeBehavior::DecrementVolume()
	{
		if (volume > 0.0f)
		{
			// set volume lower
			volume -= 0.1f;

			if (volume < 0.0f)
			{
				volume = 0.0f;
			}

			// set slider offset
			xOffset_ -= 0.0085f;
		}
	}

	float VolumeBehavior::GetVolume()
	{
		return volume;
	}

	void VolumeBehavior::SetVolume(float newVolume)
	{
		volume = newVolume;
	}

	void VolumeBehavior::Read(const rapidjson::Value& value)
	{
		xBase_ = value["xBase"].GetFloat();

		if (value.HasMember("InitialScale"))
		{
			JsonConvert(value["InitialScale"], scaleInit);
		}

		if (value.HasMember("InitialTranslation"))
		{
			JsonConvert(value["InitialTranslation"], translationInit);
		}
	}
}