/*!*****************************************************************************
\par filename: CCTV.cpp
\par  authors: Alijah Rosner
\par     date: 10/16/2022

\brief
    This file contains the implementation for both the Fusebox behavior, and 
    the CCTV behavior.
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

//#include <glm/gtx/transform.hpp>
#include "SoundBehavior.h"
#include "Audio.h"
#include <string>

namespace FFunctional
{
    void SoundBehavior::Read(const rapidjson::Value& value)
    {
        timeToWait_ = value["WaitTime"].GetFloat();
        filepath_ = value["FilePath"].GetString();
        inRange_ = value["InRange"].GetBool();

        play_ = engine_->GetSystem<Audio>()->GetSound(filepath_);

        /*rapidjson::Document docu = OpenJson(filepath_);
        play_.Load(docu, engine_->GetSystem<Audio>());
        play_.SetInRange(inRange_);*/
    }

    void SoundBehavior::Update(float dt)
    {
        if (play_)
        {
            timer_ += dt; 
            if (timer_ > timeToWait_)
            {
                timer_ = 0.0f; 

                if (!play_->GetIsPlaying())
                {
                    play_->Play();
                }
            }

        }
    }
}