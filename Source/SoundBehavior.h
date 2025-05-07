/*!*****************************************************************************
\par filename: CCTV.h
\par  authors: Alijah Rosner
\par     date: 10/16/2022

\brief
    This file contains the classes for both the Fusebox behavior, and
    the CCTV behavior.
    Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include <rapidjson/document.h>

#include <string_view>
#include "Engine.h"
#include "Sound.h"



#include "Behavior.h"



/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{

    /*!*************************************************************************
      \brief
        CCTV Behavior class.
    ***************************************************************************/
    class SoundBehavior : public Behavior<SoundBehavior>
    {
    private:
        Engine* engine_;
        float timer_;
        float timeToWait_;
        std::string filepath_;
        bool inRange_ = true; 
        std::shared_ptr<Sound> play_; 



    public:
        SoundBehavior(Engine* engine, Entity parent) : engine_(engine), Behavior(parent), timer_(0.0f), timeToWait_(0.0f) {}
        void Read(const rapidjson::Value& value);
        void Update(float dt);




        static std::string_view Name() { return "SoundBehavior"; }


    };



};