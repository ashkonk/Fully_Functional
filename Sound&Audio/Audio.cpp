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

    // Create and Initialize the FMOD System
    void Audio::Init()
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
        FMOD_ChannelGroup_SetVolume(masterVolume_, volume);
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
}