/*!*****************************************************************************
\par filename: Audio.cpp
\par  authors: Alijah Rosner
\par     date: 09/27/2022

\brief
  This is the audio file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Audio.h"
#include <iostream>

namespace FFunctional
{

    // Initialize the ...
    void Audio::ERRCHECK(FMOD_RESULT checkResult)
    {
        if (checkResult != FMOD_OK)
        {
            FMOD_ErrorString(result_);
        }
    }

    Audio::Audio()
    {
        result_ = FMOD_System_Create(&soundSystem_, FMOD_VERSION);
        ERRCHECK(result_);

        result_ = FMOD_System_Init(soundSystem_, 32, FMOD_INIT_NORMAL, 0);
        ERRCHECK(result_);

        FMOD_System_CreateChannelGroup(soundSystem_, "Master", &masterVolume_);
        FMOD_System_CreateChannelGroup(soundSystem_, "SFX", &sfxChannel_);
        FMOD_System_CreateChannelGroup(soundSystem_, "Music", &musicChannel_);

        FMOD_ChannelGroup_AddGroup(masterVolume_, sfxChannel_, false, &dspConnect_);
        FMOD_ChannelGroup_AddGroup(masterVolume_, musicChannel_, false, &dspConnect_);

        rapidjson::Document docu = OpenJson("./Data/Audio/SFX/AllAudio.json");
        CreateAllSounds(docu);
    }

    // Create and Initialize the FMOD System
    void Audio::Init()
    {
        

    }

    // Update the Audio System
    void Audio::Update(float dt)
    {
        dt;
        result_ = FMOD_System_Update(soundSystem_);
        ERRCHECK(result_);
    }

    void Audio::Draw() const
    {
    }

    // Cleanup the Audio System
    Audio::~Audio()
    {
        // Close and Release the FMOD system
        library_.clear();

        FMOD_ChannelGroup_Release(sfxChannel_);
        FMOD_ChannelGroup_Release(musicChannel_);
        FMOD_ChannelGroup_Release(masterVolume_);

        result_ = FMOD_System_Close(soundSystem_);
        ERRCHECK(result_);
        result_ = FMOD_System_Release(soundSystem_);
        ERRCHECK(result_);
    }

    void Audio::SetParent(Engine* engine)
    {
        mainEngine_ = engine;
    }

    void Audio::SetMasterVolume(float volume)
    {
        if (volume < 0)
        {
            volume = 0;
        }
        FMOD_ChannelGroup_SetVolume(masterVolume_, volume);
    }

    std::shared_ptr<Sound> Audio::GetSound(std::string string) const
    {
        if (!library_.contains(string))
        {
            return NULL;
        }
        return library_.at(string);
    }

    std::unordered_map<std::string, std::shared_ptr<Sound>>* Audio::GetLibrary()
    {
        return &library_;
    }



    FMOD_SYSTEM* Audio::GetSystem()
    {
        return soundSystem_;
    }

    Engine* Audio::GetParent() const
    {
        return mainEngine_;
    }

    std::string_view Audio::GetName() const
    {
        return Name();
    }

    FMOD_CHANNELGROUP* Audio::GetSFXChannel() const
    {
        return sfxChannel_;
    }

    FMOD_CHANNELGROUP* Audio::GetMusicChannel() const
    {
        return musicChannel_;
    }

    void Audio::CreateAllSounds(rapidjson::Value& value)
    {
        auto soundList = value["Sounds"].GetArray();

        for (rapidjson::SizeType i = 0; i < soundList.Size(); i++)
        {
            auto sound = soundList[i].GetObj();
            std::string tempName = sound["Name"].GetString();

            std::string fileName = sound["Filepath"].GetString();

            rapidjson::Document docu = OpenJson(fileName);

            std::unique_ptr uniqueSound = std::make_unique<Sound>(docu, this);

            library_.insert({ tempName, std::move(uniqueSound) });
        }
    }
}