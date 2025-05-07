/*!*****************************************************************************
\par filename: Button.cpp
\par  authors: Mason Dulay
\par     date: 09/09/2022

\brief
  This is the Button file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "Button.h"
#include "Scene.h"
#include "Camera.h"
#include "Graphics.h"
#include "Manager.h"
#include "PlayerController.h"
#include "Armament.h"
#include "Audio.h"
#include "Volume.h"

namespace FFunctional
{
	bool Button::CursorInside()
	{
		// save the cursor position along with scale and translation
		glm::vec2 cursorPos = input_->GetCursorPos();
		Entity parent = this->GetParent();
		glm::vec3 Scale = *parent.ComponentGet<Transform>().GetScale();
		glm::vec3 Translation = *parent.ComponentGet<Transform>().GetTranslation();

		// fetch the window width and height then scale
		int WindowWidth;
		int WindowHeight;
		glfwGetWindowSize(window_, &WindowWidth, &WindowHeight);

		// get the uvs of sprite
		glm::vec4 uvs_ = this->GetParent().ComponentGet<Sprite>().GetUVs();

		// get ecs
		ECS* ecs = GetParent().GetParent();

		// get transform of camera
		Transform cTransform = ecs->ComponentGetFirst<Camera>().GetParent().ComponentGet<Transform>();

		// world space -> camera space
		Scale *= *cTransform.GetScale();

		// world space -> camera space
		Translation *= *cTransform.GetScale(); 

		// normalize
		float cursorPosX = cursorPos.x / WindowWidth;

		// normalize
		float cursorPosY = 1 - (cursorPos.y / WindowHeight);

		float leftSide = Translation.x + 0.5f - Scale.x / 2.0f;
		float rightSide = Translation.x + Scale.x / 2.0f + 0.5f;

		float botSide = Translation.y + 0.5f - Scale.x / 2.0f;
		float topSide = Translation.y + Scale.y / 2.0f + 0.5f;

		// if the cursor is within the button return true
		if ((cursorPosX > leftSide && cursorPosX < rightSide) && (cursorPosY > botSide && cursorPosY < topSide))
		{
			if (offset_ == 0)
			{
				uvs_ = baseUV_ + vecOffset_;
			}
			else
			{
				uvs_.y = baseUV_.y + offset_;
			}
			this->GetParent().ComponentGet<Sprite>().SetUVs(uvs_);
			return true;
		}
		this->GetParent().ComponentGet<Sprite>().SetUVs(baseUV_);
		// otherwise return false
		return false;
	}

	void Button::ChangeScene()
	{
		Manager<Scene>* sceneManager = (engine_->GetSystem<Manager<Scene>>());
		std::shared_ptr<Scene> newScene = LoadScene(*sceneManager,
			sceneHolder_.c_str());
		buttonScene->Destroy();
	}

	void Button::Update(float)
	{
		glm::vec4 uvs_ = this->GetParent().ComponentGet<Sprite>().GetUVs();
		if (CursorInside() && input_->GetButtonState(GLFW_MOUSE_BUTTON_LEFT) == Keystate::Triggered)
		{
				sound_->Play();
		}
		if (CursorInside() && input_->GetButtonState(GLFW_MOUSE_BUTTON_LEFT) == Keystate::Released)
		{
			if (offset_ == 0)
			{
				uvs_ = baseUV_ + vecOffset_ * 2.0f;
			}
			else
			{
				uvs_.y = baseUV_.y + offset_ * 2;
			}
			this->GetParent().ComponentGet<Sprite>().SetUVs(uvs_);
			switch (function_)
			{
			case Quit:
			{
				//parent_.ComponentGet<Sound>().Play();
				Manager<Scene>* sceneManager = (engine_->GetSystem<Manager<Scene>>());
				std::shared_ptr<Scene> newScene = LoadScene(*sceneManager, "./Data/YesOrNoMain.json");
				break;
			}
			case Pause:
			{
				//parent_.ComponentGet<Sound>().Play();
				sceneHolder_ = "./Data/PauseMenuMain.json";
				ChangeScene();
				break;
			}
			case Play:
			{
				//parent_.ComponentGet<Sound>().Play();
				sceneHolder_ = "./Data/IntroSceneMain.json";
				//sceneHolder_ = "./Data/LevelTutorial0/TutorialRoom0Primary.json";
				ChangeScene();
				break;
			}
			case Retry:
			{
				switch (engine_->GetLastLevel())
				{
					case Level1:
					{
						sceneHolder_ = "./Data/LevelTutorial0/TutorialRoom0Primary.json";
						ChangeScene();
						break;
					}
					case Level2:
					{
						sceneHolder_ = "./Data/LevelTutorial1/TutorialRoom1Primary.json";
						ChangeScene();
						break;
					}
					case Level3:
					{
						sceneHolder_ = "./Data/LevelTutorial2/TutorialRoom2Primary.json";
						ChangeScene();
						break;
					}
					case Level4:
					{
						sceneHolder_ = "./Data/LevelPassenger1/PassengerRoom1Primary.json";
						ChangeScene();
						break;
					}
					case Level5:
					{
						sceneHolder_ = "./Data/LevelPassenger2/Passenger2Primary.json";
						ChangeScene();
						break;
					}
					case Level6:
					{
						sceneHolder_ = "./Data/LevelKitchen/LevelKitchenPrimary.json";
						ChangeScene();
						break;
					}
				}
				break;
			}
			case Resume:
			{
				buttonScene->Destroy();
				Manager<Scene>* sceneManager = engine_->GetSystem<Manager<Scene>>();
				for (unsigned i = 0; i < sceneManager->Size(); ++i)
				{
					std::shared_ptr<Scene> sceneBuffer = sceneManager->At(i);
					//if scene already exists, return it
					if (sceneBuffer->getECS().HasManager<PlayerController>())
					{
						//set sprites to active
						SetECSState(sceneBuffer->getECS(), true);
					}
					if (sceneManager->At(i)->GetName() == "./Data/VolumeMain.json")
					{
						sceneManager->At(i)->Destroy();
					}
				}
				break;
			}
			case Delete:
			{
				GetParent().Destroy();
				break;
			}
			case RealQuit:
			{
				engine_->Stop();
				break;
			}
			case CancelQuit:
			{
				buttonScene->Destroy();
				break;
			}
			case VolumeUp:
			{
				GetParent().GetParent()->ComponentGetFirst<VolumeBehavior>().IncrementVolume();
				break;
			}
			case VolumeDown:
			{
				GetParent().GetParent()->ComponentGetFirst<VolumeBehavior>().DecrementVolume();
				break;
			}
			case Credits:
			{
				Manager<Scene>* sceneManager = (engine_->GetSystem<Manager<Scene>>());
				LoadScene(*sceneManager, "./Data/CreditsMain.json");
				break;
			}
			case Volume:
			{
				if (VolumeOpen_ == false)
				{
					// open menu
					Manager<Scene>* sceneManager = (engine_->GetSystem<Manager<Scene>>());
					LoadScene(*sceneManager, "./Data/VolumeMain.json");
					VolumeOpen_ = true;
				}
				else
				{
					// close menu
					Manager<Scene>* sceneManager = (engine_->GetSystem<Manager<Scene>>());
					for (unsigned int i = 0; i < sceneManager->Size(); ++i)
					{
						if (sceneManager->At(i)->GetName() == "./Data/VolumeMain.json")
						{
							sceneManager->At(i)->Destroy();
							break;
						}
					}
					VolumeOpen_ = false;
				}
				break; 
			}
			case Controls:
			{
				Manager<Scene>* sceneManager = (engine_->GetSystem<Manager<Scene>>());
				LoadScene(*sceneManager, "./Data/ControlsMain.json");
				break;
			}
			case Windowed:
			{
				if (glfwGetWindowMonitor(window_))
				{
					int windowWidth = 1920 * 3 / 4;
					int windowHeight = 1080 * 3 / 4;
					const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
					glfwSetWindowMonitor(window_, NULL, mode->width / 6, mode->height / 6, windowWidth, windowHeight, 0);
				}
				break;
			}
			case Fullscreen:
			{
				if (glfwGetWindowMonitor(window_) == NULL)
				{
					GLFWmonitor* monitor = glfwGetPrimaryMonitor();
					const GLFWvidmode* mode = glfwGetVideoMode(monitor);
					glfwSetWindowMonitor(window_, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
				}
				break;
			}
			case Mute:
			{
				if (VolumeBehavior::GetVolume() > 0.0f)
				{
					VolumeBehavior::SetVolume(0.0f);
				}
				else
				{
					VolumeBehavior::SetVolume(0.5f);
				}
			}
			case Replay:
			{
				sceneHolder_ = "./Data/LevelTutorial0/TutorialRoom0Primary.json";
				ChangeScene();
				engine_->SetLastLevel(Level1);
				break;
			}
			case NoFunction:
			{
				break;
			}
			default:
				break;
			}
		}
	}
		
	void Button::Read(const rapidjson::Value& value)
	{
		if (value.HasMember("Filepath"))
		{
			sceneDir_ = value["Filepath"].GetString();
		}

		if (value.HasMember("Function"))
		{
			function_ = value["Function"].GetInt();
		}
		else
		{
			function_ = NoFunction;
		}

		if (value.HasMember("BaseUV"))
		{
			auto uvs = value["BaseUV"].GetArray();
			glm::vec4 myUVs = glm::vec4(uvs[0].GetFloat(), uvs[1].GetFloat(), uvs[2].GetFloat(), uvs[3].GetFloat());
			baseUV_ = myUVs;
		}

		if (value.HasMember("Offset"))
		{
			offset_ = value["Offset"].GetFloat();
		}

		if (value.HasMember("VectorOffset"))
		{
			auto uvs = value["VectorOffset"].GetArray();
			glm::vec4 myUVs = glm::vec4(uvs[0].GetFloat(), uvs[1].GetFloat(), uvs[2].GetFloat(), uvs[3].GetFloat());
			vecOffset_ = myUVs;
		}
		sound_ = engine_->GetSystem<Audio>()->GetSound("ButtonSound");
		menuMusic_ = engine_->GetSystem<Audio>()->GetSound("ButtonSound");

		sound_->SetInRange(true);
		sound_->SetIsLooping(false);
	}

	std::string_view Button::Name()
	{
		return "Button";
	}

	void Button::SetSceneDir(std::string Dir)
	{
		sceneDir_ = Dir;
	}
}