/*!*****************************************************************************
\par filename: Detection.cpp
\par  authors: Mason Dulay
\par     date: 11/30/2022

\brief
  This is the Detection file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "Detection.h"
#include "CCTV.h"
#include "AttendantA.h"
#include "VisionCone.h"
#include "GuardBehavior.h"
#include "TrashFire.h"
#include "Sprite.h"

namespace FFunctional
{
	std::shared_ptr<Image> Detection::GetImage() const
	{
		if (mode_ == 0)
		{
			return source_;
		}
		else
		{
			return source2_;
		}
	}

	void Detection::Read(const rapidjson::Value& value)
	{
		if (value.HasMember("tex_path"))
		{
			JsonConvertString(value["tex_path"], texture_);

			source_ = (Graphics::LoadImage(texture_.data()));
		}
		
		if (value.HasMember("tex_path2"))
		{
			JsonConvertString(value["tex_path2"], texture2_);

			source2_ = (Graphics::LoadImage(texture2_.data()));
		}
	}

	void Detection::Update(float dt)
	{
		dt;
		// attendant detection
		if (parent_.GetParent()->Has<AttendantBehavior>(parent_.GetEntity()))
		{
			// get ecs
			ECS* ecs = GetParent().GetParent(); 
			// get behavior
			AttendantBehavior& aBehavior = ecs->ComponentGetFirst<AttendantBehavior>();
			// get transform->translation
			Transform aTransform = aBehavior.GetParent().ComponentGet<Transform>();
			translation_ = *aTransform.GetTranslation();
			translation_.y += 0.2f;

			// get vCone
			VisionConeBehavior& vBehavior = ecs->ComponentGetFirst<VisionConeBehavior>();

			if (vBehavior.GetColliding() && aBehavior.GetDisabled() == false)
			{
				mode_ = 1;
			}
			else
			{
				mode_ = 0;
			}

			if (aBehavior.GetDisabled() == true)
			{
				opacity_ = 0.0f;
			}
		}
		// gaurd detection
		else if (parent_.GetParent()->Has<GuardBehavior>(parent_.GetEntity()))
		{
			// get ecs
			ECS* ecs = GetParent().GetParent();
			// get behavior
			GuardBehavior& gBehavior = ecs->ComponentGetFirst<GuardBehavior>();
			// get transform->translation
			Transform& gTransform = gBehavior.GetParent().ComponentGet<Transform>();
			translation_ = *gTransform.GetTranslation();
			translation_.y += 0.2f;


			if (gBehavior.GetColliding())
			{
				mode_ = 1;
			}
			else
			{
				mode_ = 0;
			}


		}
		// CCTV detection
		else if (parent_.GetParent()->Has<CCTVBehavior>(parent_.GetEntity()))
		{
			// get ecs
			ECS* ecs = GetParent().GetParent();
			// get behavior
			CCTVBehavior& cBehavior = ecs->ComponentGet<CCTVBehavior>(GetParent().GetEntity());
			// get transform->translation
			Transform& cTransform = cBehavior.GetParent().ComponentGet<Transform>();
			translation_ = *cTransform.GetTranslation();
			translation_.y += 0.2f;

			//FuseBoxBehavior& fBoxBehavior = ecs->ComponentGetFirst<FuseBoxBehavior>();

			if (cBehavior.GetColliding())
			{
				mode_ = 1;
			}
			else
			{
				mode_ = 0;
			}

		}
	}

	std::string Detection::GetTexture()
	{
		if (mode_ == 0)
		{
			return texture_;
		}
		else
		{
			return texture2_;
		}
	}

	void Detection::SetTexture(std::string NewTexture)
	{
		texture_ = NewTexture;
		source_ = (Graphics::LoadImage(texture_.data()));
	}

	void Detection::SetTexture2(std::string NewTexture)
	{
		texture2_ = NewTexture;
		source2_ = (Graphics::LoadImage(texture2_.data()));
	}

	glm::vec3 Detection::GetTranslation()
	{
		return translation_;
	}

	glm::vec3 Detection::GetScale()
	{
		return scale_;
	}

	glm::vec3 Detection::GetRotationVec()
	{
		return rotationVec_;
	}

	float Detection::getRotation()
	{
		return rotation_;
	}

	float Detection::getOpacity()
	{
		return opacity_;
	}
}