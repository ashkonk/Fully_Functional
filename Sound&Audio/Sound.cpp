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
#include "PlayerController.h"
#include <glm/vec3.hpp>


namespace FFunctional
{
    Sound::Sound(Sound&& sound) : Component(sound.parent_), soundFile_(sound.soundFile_), name_(sound.name_),
        audioSystem_(sound.audioSystem_), filepath_(sound.filepath_),
        groupChannel_(sound.groupChannel_), soundChannel_(sound.soundChannel_),
        system_(sound.system_), volume_(sound.volume_), maxVolume_(sound.maxVolume_),
        maxDistance_(sound.maxDistance_), type_(sound.type_) 
    { 
        sound.soundFile_ = nullptr;
    }

    Sound& Sound::operator=(Sound&& sound)
    {
        soundFile_ = sound.soundFile_;
        name_ = (sound.name_);
        audioSystem_ = (sound.audioSystem_), filepath_ = (sound.filepath_),
            groupChannel_ = (sound.groupChannel_), soundChannel_ = (sound.soundChannel_),
            system_ = (sound.system_), volume_ = (sound.volume_), maxVolume_ = (sound.maxVolume_),
            maxDistance_ = (sound.maxDistance_), type_ = (sound.type_);

        sound.soundFile_ = nullptr; 
        return *this; 
    }

    void Sound::Read(const rapidjson::Value& value)
    {
        system_ = audioSystem_->GetSystem();

        name_ = value["Name"].GetString();
        filepath_ = value["FilePath"].GetString();
        volume_ = value["Volume"].GetFloat();
        maxVolume_ = volume_;
        isLooping_ = value["Looping"].GetBool();
        type_ = value["Type"].GetString();
        maxDistance_ = value["MaxDistance"].GetFloat();

        if (type_ == "SFX")
        {
            groupChannel_ = audioSystem_->GetSFXChannel();
        }
        else if (type_ == "Music")
        {
            groupChannel_ = audioSystem_->GetMusicChannel();
        }

        FMOD_RESULT result = FMOD_System_CreateSound(system_, filepath_.c_str(), FMOD_LOOP_NORMAL | FMOD_2D, 0, &soundFile_);

        FMOD_ErrorString(result);
    }

    void Sound::Update(float dt)
    {
        dt;
        if (!GetParent().Has<PlayerController>() || type_ != "Music")
        {
            if (GetParent().GetParent()->HasManager<PlayerController>())
            {
                PlayerController& player = GetParent().GetParent()->ComponentGetFirst<PlayerController>();
                Transform& playerTransform = player.GetParent().ComponentGet<Transform>();

                Transform& soundTransform = GetParent().ComponentGet<Transform>();

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

        }
        else
        {
            inRange = true;
        }
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

    void Sound::Play()
    {
        //SetVolume(volume_);

        if (inRange)
        {
            FMOD_System_PlaySound(system_, soundFile_, groupChannel_, false, &soundChannel_);
            FMOD_Channel_SetVolume(soundChannel_, volume_);
            isPlaying_ = true;
        }
        else
        {
            Stop();
        }
    }

    void Sound::SetIsPlaying(bool value)
    {
        isPlaying_ = value;
    }

    void Sound::Stop()
    {
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
        return isLooping_;
    }

    void Sound::SetIsLooping(bool isLooping)
    {
        int soundCheck = 0;
        if (isLooping == true)
        {
            soundCheck = -1;
        }
        FMOD_Sound_SetLoopCount(soundFile_, soundCheck);
        isLooping_ = isLooping;
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

    std::string Sound::GetName() const
    {
        return name_;
    }

    void Sound::SetSpeed(float speed)
    {
        speed_ = speed;
    }



}