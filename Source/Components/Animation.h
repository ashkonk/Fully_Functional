/*!*****************************************************************************
\par filename: Animation.h
\par  authors: Alijah Rosner
\par     date: 09/09/2022

\brief
  Header file for the animation component. 
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include "Component.h"
#include "Stream.h"
#include "rapidjson/document.h"
#include <unordered_map>
#include "glm/vec4.hpp"
#include <vector>
/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    /*!*************************************************************************
      \brief
        This is the animation component class.
    ***************************************************************************/
    class Animation : public Component
    {
    public:
        Animation(Entity parent) : Component(parent) {};

        void Read(const rapidjson::Value& value);

        void Update(float dt);

        void TryPlay(std::string name);

        void SetIsLooping(bool truth);

        void Play(std::string name);

        void MakeUVs(float row, float column, glm::vec4 vec, std::string name);

        bool GetIsDone() const;

        float GetPriority() const;

        static std::string_view Name() { return "Animation"; }

    private:
        std::unordered_map<std::string, std::vector<glm::vec4>*> sequenceList_ = std::unordered_map<std::string, std::vector<glm::vec4>*>();
        
        std::vector<glm::vec4>* currentSequence = NULL; 
        
        size_t currentFrame_ = 0;

        float rowWidth_ = 0; 

        // The time remaining for the current frame.
        float frameTimeLeft_ = 0;

        float frameDuration_ = 0.0625f;
        
        bool isLooping_;


        bool isDone_ = false; 

    };

};