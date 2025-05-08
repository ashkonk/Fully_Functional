/*!*****************************************************************************
\par filename: TutorialText.cpp
\par  authors: Mason Dulay
\par     date: 09/09/2022

\brief
  This is the TutorialText file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "TutorialText.h"
#include "Sprite.h"
#include "PlayerController.h"
#include "CameraBehavior.h"
#include "Camera.h"
#include "Transform.h"
#include <glm/gtx/vector_angle.hpp>


namespace FFunctional
{
    void TutorialText::Update(float dt)
    {
        Transform& transform = GetParent().ComponentGet<Transform>();
        glm::vec3 pos = *transform.GetTranslation();


        if (loadingIn_ == false && dissolving_ == false)
        {
            parent_.ComponentGet<Sprite>().SetOpacity(0.0f);

        }
        if (loadingIn_ == true)
        {

            parent_.ComponentGet<Sprite>().SetOpacity(parent_.ComponentGet<Sprite>().GetOpacity() + dt * 2);
        }
        if (dissolving_ == true)
        {
            parent_.ComponentGet<Sprite>().SetOpacity(parent_.ComponentGet<Sprite>().GetOpacity() - dt * 4);
            canResolve_ = false;
        }

        if (parent_.ComponentGet<Sprite>().GetOpacity() == 0 && !canResolve_)
        {
          parent_.Destroy();
        }
    }

    void TutorialText::EnterCollision2D(const Collider& self, const Collider& other)
    {

        if (canResolve_)
            if (other.GetParent().GetEntity() == parent_.GetParent()->ComponentGetFirst<PlayerController>().GetParent().GetEntity())
            {

                loadingIn_ = true;
                dissolving_ = false;
                sound_->Play();
            }
        self;
        other;
    }

    void TutorialText::ExitCollision2D(const Collider& self, const Collider& other)
    {
        self;
        other;
        if (canResolve_)
            if (other.GetParent().GetEntity() == parent_.GetParent()->ComponentGetFirst<PlayerController>().GetParent().GetEntity())
                dissolving_ = true;
                loadingIn_ = false;
    }

    void TutorialText::Read(const rapidjson::Value& value)
    {
        eventBool = false;
        if (value.HasMember("DistThresh"))
            distThresh = value["DistThresh"].GetFloat();

        if (value.HasMember("CameraTime"))
            durationCap_ = value["CameraTime"].GetFloat();

        sound_ = engine_->GetSystem<Audio>()->GetSound("TextSound");
        /*rapidjson::Document docu = OpenJson("./Data/Audio/SFX/TextSound.json");
        sound_.Load(docu, engine_->GetSystem<Audio>());*/
        sound_->SetInRange(true);
        sound_->SetIsLooping(false);
    }
}