/*!*****************************************************************************
\par filename: CameraBehavior.cpp
\par  authors: Ashkon Khalkhali
\par     date: 11/14/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Sprite.h"
#include "PlayerController.h"
#include "CameraBehavior.h"
#include "Transform.h"
#include "Camera.h"
#include "Physics.h"
#include "TendrilBehavior.h"


namespace FFunctional
{

 static float dt_acum = 0.0f;

    void CameraBehavior::Init()
    {
        trainSounds->Play();
    }

    void CameraBehavior::Target(glm::vec3 pos)
    {
      targetPos_ = pos;
      target_ = true;
    }

    void CameraBehavior::Untarget()
    {
      target_ = false;
    }

    void CameraBehavior::Read(const rapidjson::Value& value)
    {
        if (value.HasMember("InitPosition"))
        {
            JsonConvert(value["InitPosition"], initPos_);
        }

        backgroundMusic = engine_->GetSystem<Audio>()->GetSound("GameMusic");
        /*rapidjson::Document docu = OpenJson("./Data/Audio/Music/Music.json");
        backgroundMusic.Load(docu, engine_->GetSystem<Audio>()); */
        backgroundMusic->SetInRange(true);


        trainSounds = engine_->GetSystem<Audio>()->GetSound("TrainSounds");
       /* rapidjson::Document docuTwo = OpenJson("./Data/Audio/SFX/TrainSounds.json");
        trainSounds.Load(docuTwo, engine_->GetSystem<Audio>());*/
        trainSounds->SetInRange(true);
        trainSounds->SetIsLooping(false);

        bounds = glm::vec4(-1e6, -1e6, 1e6, 1e6);

        if (value.HasMember("Bounds"))
          JsonConvert(value["Bounds"], bounds);
    }

    void CameraBehavior::Update(float dt)
    {
        dt_acum += dt;
        Manager<Scene>* sceneManager = engine_->GetSystem<Manager<Scene>>();

        if (!backgroundMusic->GetIsPlaying())
        {
            backgroundMusic->Play();
        }

        for (unsigned i = 0; i < sceneManager->Size(); ++i)
        {
            std::shared_ptr<Scene> scene = sceneManager->At(i);
            ECS sceneEcs_ = scene->getECS();
            if (sceneEcs_.HasManager<PlayerController>() && sceneEcs_.GetActive())
            {
                Camera& camera = sceneEcs_.ComponentGetFirst<Camera>();
                Entity camParent = camera.GetParent();
                PlayerController& player = sceneEcs_.ComponentGetFirst<PlayerController>();
                Transform& playerTrans = player.GetParent().ComponentGet<Transform>();
                Transform& camTransform = GetParent().ComponentGet<Transform>();

                if (!camParent.IsDestroyed() && sceneEcs_.GetActive())
                {
                  if (camParent.Has<Sprite>())
                  {
                    Sprite& camSprite = camParent.ComponentGet<Sprite>();
                    if (player.GetVisionState() == 1)
                    {
                      detectiveMode_ = true;
                      camSprite.SetOpacity(camSprite.GetOpacity() + (dt * 2.0f));
                      if (camSprite.GetOpacity() > 0.5f)
                      {
                        float opacityWave = (-sinf(dt_acum * 2.0f) + 1) / 4 + 0.3f;
                        camSprite.SetOpacity(opacityWave);

                        if (sceneEcs_.HasManager<TendrilBehavior>())
                        {
                            TendrilBehavior& visionTendril = sceneEcs_.ComponentGetFirst<TendrilBehavior>();
                            visionTendril.SetActive((sinf(dt_acum * 10.0f) + 1) / 4 + 0.5f);
                        }
                      }
                    }
                    else
                    {
                      detectiveMode_ = false;

                      if (camSprite.GetOpacity() > 0.0f)
                      {
                        float opacityReduc = camSprite.GetOpacity() - (dt * 1.5f);
                        camSprite.SetOpacity(opacityReduc);

                        if (sceneEcs_.HasManager<TendrilBehavior>())
                        {
                            TendrilBehavior& visionTendril = sceneEcs_.ComponentGetFirst<TendrilBehavior>();
                            visionTendril.Dissipate(opacityReduc);
                        }
                      }
                    }
                    playerPos_ = *playerTrans.GetTranslation();
                  }
                  else
                  {
                      GetParent().Destroy();
                  }
                }

                const float duration = 0.25f;

                glm::vec3 offset = { 0, 0.2, 0 };

                glm::vec3 newPos = (*camTransform.GetTranslation()) * (1 - dt / duration);

                glm::vec3 move = (target_)? targetPos_ : (*playerTrans.GetTranslation());

                newPos += (move + offset) * (dt / duration);

                newPos.x = std::clamp(newPos.x, bounds.x, bounds.z);
                newPos.y = std::clamp(newPos.y, bounds.y, bounds.w);

                camTransform.SetTranslation(&newPos);


            }
        }
    }
    bool CameraBehavior::GetMode()
    {
      return detectiveMode_;
    }
}