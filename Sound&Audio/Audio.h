/*!*****************************************************************************
\par filename: Audio.h
\par  authors: Alijah Rosner
\par     date: 09/27/2022

\brief
  This is the audio file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include "Sound.h"
#include <unordered_map>
#include <memory>
#include "fmod.h"
#include "Stream.h"
#include "ISystem.h"
#pragma warning(push, 0)
#include "fmod_errors.h"
#pragma warning(pop)

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    /*!*************************************************************************
    \brief
      Audio class. 
  ***************************************************************************/
    class Audio : public ISystem
    {

    public:

        void Init() override;

        void ERRCHECK(FMOD_RESULT checkResult);
  
        void Update(float dt) override;

        void Draw() const override;

        void SetParent(Engine* engine) override;

        void SetMasterVolume(float volume);

        static std::string_view Name() { return "Audio"; }

        FMOD_SYSTEM* GetSystem();

        Engine* GetParent() const override;

        virtual std::string_view GetName() const override;

        FMOD_CHANNELGROUP* GetSFXChannel() const;

        FMOD_CHANNELGROUP* GetMusicChannel() const;

        ~Audio();


    private:
        Engine* mainEngine_;
        FMOD_SYSTEM* soundSystem_;
        FMOD_CHANNELGROUP* masterVolume_ = 0; 
        FMOD_SOUND* sound_;
        FMOD_CHANNELGROUP* sfxChannel_ = 0;
        FMOD_CHANNELGROUP* musicChannel_ = 0; 
        FMOD_RESULT result_;
        FMOD_DSPCONNECTION* dspConnect_; 
    };


}; /* Normalloc */