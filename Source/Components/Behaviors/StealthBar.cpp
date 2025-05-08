/*!*****************************************************************************
\par filename: StealthBar.cpp
\par  authors: Mason Dulay
\par     date: 03/03/2023

\brief
  This is the StealthBar file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "StealthBar.h"
#include "StealthBarBG.h"
#include "Scene.h"
#include "SceneLoader.h"
#include "PlayerController.h"
#include <iostream>

namespace FFunctional
{
	float StealthBar::fill_ = 0.0f;
	float StealthBar::anim_ = 0.0f;


	void StealthBar::Read(const rapidjson::Value& value)
	{
		if (value.HasMember("InitialScale"))
		{
			JsonConvert(value["InitialScale"], scaleInit);
		}

		if (value.HasMember("InitialTranslation"))
		{
			JsonConvert(value["InitialTranslation"], translationInit);
		}

		lose_ = engine_->GetSystem<Audio>()->GetSound("Lose");
		/*rapidjson::Document docu = OpenJson("./Data/Audio/SFX/Lose.json");
		lose_.Load(docu, engine_->GetSystem<Audio>());*/
		lose_->SetInRange(true);
		lose_->SetIsLooping(false);
	}

	void StealthBar::SetFill(float fill)
	{
		fill_ = std::max(fill_, fill);
		fill_ = std::clamp(fill_, 0.0f, 1.0f);
	}

	void StealthBar::ClearFill()
	{
		fill_ /= 2.0f;
	}

	float StealthBar::GetFill()
	{
		if (fill_ > 0.0f)
		{
			engine_->GetTrace().Message("Player detection is at: ", fill_);
		}
		return fill_;
	}

	void StealthBar::Update(float dt)
	{
		timer_ += dt;
		if (input_->GetKeyState(GLFW_KEY_J) == Keystate::Triggered && timer_ > 1.0f)
		{
			godmode_ = !godmode_;
			timer_ = 0.0f;
		}

		if (godmode_ == true)
		{
			fill_ = 0;
		}

		static const float durr = 1.0f;
		if (fill_ >= 1.0f)
		{
				if (!lose_->GetIsPlaying())
				{

				lose_->Play();
				}
			anim_ += 1.0f/60.0f;
			anim_ = std::clamp(anim_, 0.0f, durr);

			Component::SetGlobalTimeDialation(1.0f - anim_/durr);

			if (anim_ >= durr)
			{
				anim_ = 0.0f;
				fill_ = 0.0f;
				Component::SetGlobalTimeDialation(1.0f);
				std::string fileName = "./Data/LoseScreenMain.json";
				Manager<Scene>* sceneManager = engine_->GetSystem<Manager<Scene>>();
				std::shared_ptr<Scene> newScene = LoadScene(*sceneManager, fileName.c_str());
				GetParent().GetParent()->SetActive(false);
				GetParent().GetParent()->Clear();
			}
			return;
		}

		GetParent().ComponentGet<Sprite>().SetUVs(glm::vec4(0.0f, 0.0f, fill_, 1.0f));
		glm::vec3 vec = glm::vec3(fill_*scaleInit.x, scaleInit.y, scaleInit.z);
		glm::vec3 pos = glm::vec3((fill_ - 1)*scaleInit.x*0.5f, 0, 0);
		pos += translationInit;

		GetParent().ComponentGet<Transform>().SetScale(&vec);
		GetParent().ComponentGet<Transform>().SetTranslation(&pos);
		
		if (fill_ > 0.0f)
		{
			GetParent().GetParent()->ComponentGetFirst<StealthBarBG>().SetEyeClosed(false);
			fill_ = std::clamp(fill_ - (dt*dt)*(4.0f/fill_), 0.0f, 1.0f); //4.0f is hardcoded
			
			//ClearFill();
		}
		else
		{
			GetParent().GetParent()->ComponentGetFirst<StealthBarBG>().SetEyeClosed(true);
			
		}
	}
}