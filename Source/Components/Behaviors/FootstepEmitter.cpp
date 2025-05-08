/*!*****************************************************************************
\par filename: FootstepEmitter.cpp
\par  authors: Mason Dulay
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "FootstepEmitter.h"
#include "Transform.h"
#include "PlayerController.h"
#include "Physics.h"

namespace FFunctional
{

	void FootstepEmitter::Read(const rapidjson::Value& value)
	{
		if (value.HasMember("Timer"))
		{
			timer_ = value["Timer"].GetFloat();
		}

		if (value.HasMember("SpawnRate"))
		{
			spawnrate_ = value["SpawnRate"].GetFloat();
		}

		if (value.HasMember("tex_path"))
		{
			JsonConvertString(value["tex_path"], texture_);

			source_ = (Graphics::LoadImage(texture_.data()));
		}	
	}

	void FootstepEmitter::Update(float dt)
	{
		// check all of the footsteps
		for (int i = 0; i < Pvec_.size(); ++i)
		{
			// take dt away from lifetime of current particle
			Pvec_[i].lifetime -= dt;

			// if the lifetime is over swap and pop back
			if (Pvec_[i].lifetime <= 0)
			{
				std::swap(Pvec_[i], Pvec_.back());
				Pvec_.pop_back();
			}
		}

		// add to spawn variable
		spawn_ += dt;
		// if it is time to spawn another footstep
		if (spawn_ >= spawnrate_)
		{
			// get the enemy position
			// get the transform of attendant
			Transform compTransform = GetParent().ComponentGet<Transform>();
			//const glm::vec3 *oldPhysicsTranslation = GetParent().ComponentGet<Physics>().GetOldTranslation();
			// left foot
			if (footCurr_ == false)
			{
				// create new particle
				Particle temp = PCreate(timer_, left);
				// get translation of component
				temp.translation_ = *compTransform.GetTranslation();
				temp.rotation_ = compTransform.GetRotation();
				// set rotation based on route
				// left foot going left (x current < x old)
				//if(temp.translation_.x < oldPhysicsTranslation->x)
				//{
				//	// rotation
				//	temp.rotation_ = 190.0f;
				//		
				//	temp.translation_.x -= 0.05f;
				//	temp.translation_.y -= 0.185f;
				//}
				//// left foot going down (y current < y old)
				//else if(temp.translation_.y < oldPhysicsTranslation->y)
				//{
				//	// rotation
				//	temp.rotation_ = glm::radians<float>(180);

				//	// left foot placement
				//	temp.translation_.x += 0.03f;
				//	temp.translation_.y -= 0.185f;
				//}
				//// left foot going right (x current > x old)
				//else if(temp.translation_.x > oldPhysicsTranslation->x)
				//{
				//	// rotation
				//	temp.rotation_ = -190.0f;

				//	temp.translation_.y -= 0.135f;
				//}
				//// left foot going up (y current > y old)
				//else if(temp.translation_.y > oldPhysicsTranslation->y)
				//{
				//	// rotation
				//	temp.rotation_ = 0.0f;

				//	// left foot placement
				//	temp.translation_.x -= 0.03f;
				//	temp.translation_.y -= 0.185f;
				//}
				temp.rotationVec = { 0.0f , 0.0f, 1.0f };
				// push_back the particle to vector
				Pvec_.push_back(temp);
			}

			// right foot
			else
			{
				// create new particle
				Particle temp = PCreate(timer_, right);
				// get translation of component
				temp.translation_ = *compTransform.GetTranslation();
				temp.rotation_ = compTransform.GetRotation();
				// set rotation based on route
				// right foot going left (x current < x old)
				//if(temp.translation_.x < oldPhysicsTranslation->x)
				//{
				//	// rotation
				//	temp.rotation_ = 190.0f;

				//	temp.translation_.x -= 0.05f;
				//	temp.translation_.y -= 0.135f;
				//}
				//// right foot going down (y current < y old)
				//else if(temp.translation_.y < oldPhysicsTranslation->y)
				//{
				//	// rotation
				//	temp.rotation_ = glm::radians<float>(180);

				//	// right foot placement
				//	temp.translation_.x -= 0.03f;
				//	temp.translation_.y -= 0.185f;
				//}
				//// right foot going right (x current > x old)
				//else if(temp.translation_.x > oldPhysicsTranslation->x)
				//{
				//	// rotation
				//	temp.rotation_ = -190.0f;

				//	temp.translation_.y -= 0.185f;
				//}
				//// right foot going up (y current > y old)
				//else if(temp.translation_.y > oldPhysicsTranslation->y)
				//{
				//	// rotation
				//	temp.rotation_ = 0.0f;

				//	// right foot placement
				//	temp.translation_.x += 0.03f;
				//	temp.translation_.y -= 0.185f;
				//}
				temp.rotationVec = { 0.0f , 0.0f, 1.0f };
				// push_back the particle to vector
				Pvec_.push_back(temp);
			}

			// change the next foot
			footCurr_ = !footCurr_;
			
			// reset spawn variable
			spawn_ = 0;
		}

		// get the vision state from the player
		int CurrState = GetParent().GetParent()->ComponentGetFirst<PlayerController>().GetVisionState();

		// if in DETECTIVE mode show footsteps
		if (CurrState == 1 )
		{
			// fade in 0 ----> 1
			for (int i = 0; i < Pvec_.size(); ++i)
			{
				Pvec_[i].opacity_ = 1.0f;
			}
		}
		// if in NORMAL mode hide footsteps
		else
		{
			for (int i = 0; i < Pvec_.size(); ++i)
			{
				Pvec_[i].opacity_ = 0.0f;
			}
		}
	}

	// Create a Particle and return it
	Particle FootstepEmitter::PCreate(float lifetime, int foot)
	{
		Particle temp;
		temp.lifetime = lifetime;
		temp.foot = foot; 
		temp.scale_ = { 0.15f, 0.15f, 0.0f };
		temp.opacity_ = 0.0f;
		return temp;
	}

	void FootstepEmitter::SetTexture(std::string NewTexture)
	{
		texture_ = NewTexture;
		source_ = (Graphics::LoadImage(texture_.data()));
	}

	std::shared_ptr<Image> FootstepEmitter::GetImage() const
	{
		return source_;
	}

	std::vector<Particle>& FootstepEmitter::GetPVec()
	{
		return Pvec_;
	}

	std::string FootstepEmitter::GetTexture()
	{
		return texture_;
	}
}