/*!*****************************************************************************
\par filename: Sound.cpp
\par  authors: Alijah Rosner
\par     date: 09/27/2022

\brief
  This is the sound file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Sound.h"
#include "Audio.h"

namespace FFunctional
{

    //------------------------------------------------------------------------------
    // Public Variables:
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    // Private Variables:
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    // Private Function Declarations:
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    // Public Functions:
    //------------------------------------------------------------------------------

    Sound::Sound(const rapidjson::Value& value, Audio* system)
    {
        system_ = system->GetSystem();

        name_ = value["Name"].GetString();
        filepath_ = value["FilePath"].GetString();
        volume_ = value["Volume"].GetFloat();
        maxVolume_ = volume_;
        isLooping_ = value["Looping"].GetBool();
        type_ = value["Type"].GetString();
        maxDistance_ = value["MaxDistance"].GetFloat();

        if (type_ == "SFX")
        {
            groupChannel_ = system->GetSFXChannel();
        }
        else if (type_ == "Music")
        {
            groupChannel_ = system->GetMusicChannel();
        }

        FMOD_RESULT result = FMOD_System_CreateSound(system_, filepath_.c_str(), FMOD_LOOP_NORMAL | FMOD_2D, 0, &soundFile_);
        FMOD_ErrorString(result);
    }

    void Sound::UpdateCheck(Entity& player, Entity& other)
    {
        if (player.GetEntity() == other.GetEntity())
        {
            inRange = true;
            FMOD_Channel_SetVolume(soundChannel_, volume_);

        }
        else if (type_ != "Music")
        {
            Transform& playerTransform = player.ComponentGet<Transform>();

            Transform& soundTransform = other.ComponentGet<Transform>();

            float distance = glm::distance(*playerTransform.GetTranslation(), *soundTransform.GetTranslation());

            if (distance > maxDistance_)
            {
                volume_ = 0.0f;
                inRange = false;
                isPlaying_ = false;
            }
            else
            {
                if (volume_ <= maxVolume_)
                {
                    volume_ = (maxDistance_ - distance) / maxDistance_;
                    FMOD_Channel_SetVolume(soundChannel_, volume_);
                }
                inRange = true;
            }

        }
        else
        {
            inRange = true;
        }
    }

    void Sound::SetInRange(bool truth)
    {
        inRange = truth; 
    }

    bool Sound::ValidityCheck() const
    {
        if (soundFile_)
        {
            return true;
        }
        return false;
    }

    Sound::~Sound()
    {
        FMOD_Sound_Release(soundFile_);
    }

    std::string Sound::GetName()
    {
        return name_; 
    }

    void Sound::Play()
    {
        if (inRange)
        {
            FMOD_System_PlaySound(system_, soundFile_, groupChannel_, false, &soundChannel_);
           // FMOD_Channel_SetChannelGroup(soundChannel_, groupChannel_);
            FMOD_Channel_SetVolume(soundChannel_, volume_);
            isPlaying_ = true;
        }
    }

    void Sound::SetIsPlaying(bool value)
    {
        isPlaying_ = value;
    }

    void Sound::Stop()
    {
       // FMOD_System_PlaySound(system_, soundFile_, groupChannel_, true, &soundChannel_);
        FMOD_Channel_Stop(soundChannel_);
        isPlaying_ = false;
    }

    void Sound::Pause()
    {
        FMOD_RESULT result = FMOD_System_PlaySound(system_, soundFile_, groupChannel_, true, &soundChannel_);
        isPlaying_ = false;
        FMOD_ErrorString(result);

    }

    bool Sound::GetIsPlaying()
    {

        return isPlaying_;

    }

    bool Sound::GetIsLooping()
    {

        return isPlaying_;

    }

    void Sound::SetIsLooping(bool isLooping)
    {
        int soundCheck = 0;
        if (isLooping == true)
        {
            soundCheck = -1;
        }
        FMOD_Sound_SetLoopCount(soundFile_, soundCheck);
        isPlaying_ = isLooping;
    }

    float Sound::GetVolume()
    {
        return volume_;

    }

    void Sound::SetVolume(float volumeMultiplier)
    {
        if (volumeMultiplier == 0.0f)
        {
            isPlaying_ = false;
        }
        FMOD_Channel_SetVolume(soundChannel_, volumeMultiplier);
        volume_ = volumeMultiplier;

    }

    float Sound::GetPitch()
    {

        return pitch_;

    }

    void Sound::SetPitch(float pitch)
    {

        FMOD_Channel_SetPitch(soundChannel_, pitch);
        pitch_ = pitch;

    }

    float Sound::GetSpeed()
    {
        return speed_;
    }

    FMOD_SOUND* Sound::GetSoundFile() const
    {
        return soundFile_;
    }

    void Sound::SetSpeed(float speed)
    {

        speed_ = speed;
    }



}
