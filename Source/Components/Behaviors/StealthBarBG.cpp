/*!*****************************************************************************
\par filename: stub.cpp
\par  authors: Jonathan Meyer, Ashkon Khalkhali, Alijah Rosner, Mason Dulay
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "StealthBarBG.h"
#include "Sprite.h"
#include "Transform.h"

namespace FFunctional
{
	void StealthBarBG::SetEyeClosed(bool closed)
	{
		closed_ = closed;
	}

	void StealthBarBG::Update(float)
	{
		glm::vec4 UVs(0);
		if (closed_ == true)
		{
			UVs = { 0, 0, 1.0f, 0.33f };
		}
		else
		{
			UVs = { 0.0f, 0.67f, 1.0f, 0.33f };
		}
		GetParent().ComponentGet<Sprite>().SetUVs(UVs);
	}

	void StealthBarBG::Read(const rapidjson::Value&)
	{
	}
}