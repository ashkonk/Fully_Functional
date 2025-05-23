/*!*****************************************************************************
\par filename: Sound.h
\par  authors: Alijah Rosner
\par     date: 09/27/2022

\brief
  This is the sound file for code implimentation.
  Copyright � 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include "Stream.h"
#include "Component.h"
#include "fmod.h"
#pragma warning(push, 0)
#include "fmod_errors.h"
#pragma warning(pop)
#include <unordered_map>
/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    class Audio;

    /*!*************************************************************************
    \brief
      Sound class
  ***************************************************************************/
    class Sound : public Component
    {

    public:

        Sound(Engine* engine, Entity parent) : Component(parent), audioSystem_(engine->GetSystem<Audio>()) {}

        Sound(Sound&& sound);

        Sound& operator=(Sound&& sound);

        void Read(const rapidjson::Value& value);

        void Update(float dt);

        bool ValidityCheck() const;

        ~Sound();

        void Play();

        void SetIsPlaying(bool value);

        void Stop();

        void Pause();

        bool GetIsPlaying();

        bool GetIsLooping();

        void SetIsLooping(bool isLooping);

        float GetVolume();

        void SetVolume(float volumeMultiplier);

        float GetPitch();

        void SetPitch(float pitch);

        float GetSpeed();

        FMOD_SOUND* GetSoundFile() const;

        std::string GetName() const; 

        void SetSpeed(float speed);

        static std::string_view Name() { return "Sound"; }

    private:
        FMOD_SOUND* soundFile_ = 0;
        std::string name_; 
        Audio* audioSystem_;
        std::string filepath_;
        FMOD_CHANNELGROUP* groupChannel_ = 0;
        FMOD_CHANNEL* soundChannel_ = 0; 
        FMOD_SYSTEM* system_ = 0; 
        bool isPlaying_ = false;
        bool isLooping_ = false; 
        float volume_ = 0.0f; 
        float pitch_ = 0.0f;
        float speed_ = 0.0f;
        float maxVolume_ = 0.0f;
        float maxDistance_ = 0.0f; 
        std::string type_; 
        bool inRange = false; 

    };


}; 