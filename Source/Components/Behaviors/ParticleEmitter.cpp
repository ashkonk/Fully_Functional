/*!*****************************************************************************
\par filename: ParticleEmitter.cpp
\par  authors: Mason Dulay
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "ParticleEmitter.h"
#include "Transform.h"
#include "PlayerController.h"
#include "CCTV.h"

#define START 1
#define STOP 0

namespace FFunctional
{

	void ParticleEmitter::Read(const rapidjson::Value& value)
	{
		if (value.HasMember("Timer"))
		{
			timer_ = value["Timer"].GetFloat();
		}

		if (value.HasMember("SpawnRate"))
		{
			spawnrate_ = value["SpawnRate"].GetFloat();
		}

		if (value.HasMember("LimitX") && value.HasMember("LimitY"))
		{
			limitX = value["LimitX"].GetFloat();
			limitY = value["LimitY"].GetFloat();
		}

		JsonConvertString(value["tex_path"], texture_);

		if (value.HasMember("MaxDuration"))
		{
			maxDuration_ = value["MaxDuration"].GetFloat();
		}

		if (value.HasMember("Emit"))
		{
			emit = value["Emit"].GetBool();
		}

		if (value.HasMember("scale"))
		{
			JsonConvert(value["scale"], scale);
		}

		source_ = (Graphics::LoadImage(texture_.data()));
	}

	void ParticleEmitter::Update(float dt)
	{
		if (maxDuration_ <= 0)
		{
			// do nothing
		}
		else if (emit == STOP)
		{
			Pvec_.clear();
			return;
		}
		else
		{
			emitterTimer_ += dt;
			if (emitterTimer_ > maxDuration_)
			{
				emit = STOP;
			}
		}

		// check all of the sparks
		for (int i = 0; i < Pvec_.size(); ++i)
		{
			// take dt away from lifetime of current particle
			Pvec_[i].lifetime -= dt;

			Pvec_[i].translation_.x += Pvec_[i].speed_.x;
			Pvec_[i].translation_.y += Pvec_[i].speed_.y;

			// if the lifetime is over swap and pop back
			if (Pvec_[i].lifetime <= 0)
			{
				std::swap(Pvec_[i], Pvec_.back());
				Pvec_.pop_back();
			}
		}

		// add to spawn variable
		spawn_ += dt;
		// if it is time to spawn another particle
		if (spawn_ >= spawnrate_)
		{
			// get the transform
			Transform transformPos = GetParent().ComponentGet<Transform>();

			// create new particle
			pEmit temp = PCreate(timer_);
			// get translation of fusebox
			temp.translation_ = *transformPos.GetTranslation();

			temp.rotationVec = { 0.0f , 0.0f, 1.0f };
			// push_back the particle to vector
			Pvec_.push_back(temp);
			
			// reset spawn variable
			spawn_ = 0;
		}
	}

	// Create a Particle and return it
	pEmit ParticleEmitter::PCreate(float lifetime)
	{
		pEmit temp;
		temp.lifetime = lifetime;
		temp.scale_ = scale;
		temp.opacity_ = 1.0f;
		temp.speed_ = RandomPoint();
		return temp;
	}

	// generates a point for the particle to travel to between directional limit positive and negative
	// ex: limitX = 0.5
	// the point generated would then be between -0.5 & 0.5
	// particle positions get incremented by this so realistically 0.5 is too big
	glm::vec3 ParticleEmitter::RandomPoint()
	{
		glm::vec3 point;
		point.x = ((float)rand() / RAND_MAX) * (limitX - (limitX * -1.0f)) - limitX;
		point.y = ((float)rand() / RAND_MAX) * (limitY - (limitY * -1.0f)) - limitY;
		return point;
	}

	void ParticleEmitter::SetTexture(std::string NewTexture)
	{
		texture_ = NewTexture;
	}

	std::shared_ptr<Image> ParticleEmitter::GetImage() const
	{
		return source_;
	}

	std::vector<pEmit>& ParticleEmitter::GetPVec()
	{
		return Pvec_;
	}

	void ParticleEmitter::SetEmit(int state)
	{
		if (state)
		{
			emit = START;
		}
		else
		{
			emit = STOP;
		}
	}

	std::string ParticleEmitter::GetTexture()
	{
		return texture_;
	}
}