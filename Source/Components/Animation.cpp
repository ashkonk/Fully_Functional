/*!*****************************************************************************
\par filename: Animation.cpp
\par  authors: Alijah Rosner
\par     date: 09/09/2022

\brief
  Implements the animation component.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "Animation.h"
#include "Sprite.h"
#include "ECS.h"
#include "Stream.h"
#include "Engine.h"
namespace FFunctional
{


    void Animation::Read(const rapidjson::Value& value)
    {


        std::string tempName;
        std::vector<glm::vec4>* vect = new std::vector<glm::vec4>();
        glm::vec4 vec;


        bool playOnInit_ = true;
        if (value.HasMember("PlayOnInit"))
            playOnInit_ = value["PlayOnInit"].GetBool();

        auto sequences = value["Sequences"].GetArray();

        for (rapidjson::SizeType k = 0; k < sequences.Size(); k++)
        {
            auto sequence = sequences[k].GetObj();
            tempName = sequence["Name"].GetString();
            frameDuration_ = sequence["FrameDuration"].GetFloat();

            if (sequence.HasMember("Looping"))
            {
                isLooping_ = sequence["Looping"].GetBool();
            }


            if (sequence.HasMember("Rows") && sequence.HasMember("Columns"))
            {
                vec = { 0, 0, 1, 1 };
                if (sequence.HasMember("SequenceUVs"))
                {
                    auto preConvert = sequence["SequenceUVs"].GetArray();

                    for (rapidjson::SizeType j = 0; j < 4; j++)
                    {
                        vec[j] = (preConvert[j].GetFloat());
                    }
                }
                MakeUVs(sequence["Rows"].GetFloat(), sequence["Columns"].GetFloat(), vec, tempName);
            }
            else
            {
                auto preConvert = sequence["Frames"].GetArray();
                for (rapidjson::SizeType i = 0; i < preConvert.Size(); i++)
                {
                    const rapidjson::Value& data_vec = preConvert[i].GetArray();
                    for (rapidjson::SizeType j = 0; j < data_vec.Size(); j++)
                    {
                        vec[j] = (data_vec[j].GetFloat());
                    }
                    vect->push_back(vec);
                }

                sequenceList_.insert({ tempName, vect });

            }

        }



        if (playOnInit_)
        {
          auto it = sequenceList_.begin();
          Play(it->first);
        }
        else if (sequenceList_.size())
        {
          Sprite& sprite = GetParent().ComponentGet<Sprite>();
          sprite.SetUVs(sequenceList_.begin()->second->at(0));
        }
    }

    void Animation::Play(std::string name)
    {
        isDone_ = false;
        frameTimeLeft_ = 0.0f;
        currentSequence = (sequenceList_[name]);
        currentFrame_ = 0;

        Sprite& sprite = GetParent().ComponentGet<Sprite>();
        sprite.SetUVs(currentSequence->at(currentFrame_));
    }

    void Animation::Update(float dt)
    {
      if (!currentSequence)
        return;

        if (currentSequence->size() != 0)
        {
            frameTimeLeft_ -= dt;
            while (frameTimeLeft_ < 0.0f)
            {
                frameTimeLeft_ += frameDuration_;
                currentFrame_++;
                if (currentFrame_ >= currentSequence->size())
                  isDone_ = true;

                if (isLooping_)
                  currentFrame_ %= currentSequence->size();
                //currentFrame_ %= isLooping_? currentSequence->size():INT32_MAX;
                
                if (currentFrame_ < currentSequence->size())
                {
                    Sprite& sprite = GetParent().ComponentGet<Sprite>();
                    sprite.SetUVs(currentSequence->at(currentFrame_));
                }
            }
        }
    }

    void Animation::TryPlay(std::string name)
    {
        if (sequenceList_[name] != currentSequence)
        {
            Play(name);
        }
    }

    void Animation::SetIsLooping(bool truth)
    {
        isLooping_ = truth;
    }

    void Animation::MakeUVs(float row, float column, glm::vec4 vec, std::string name)
    {
        float width = 1.0f / column;
        float height = 1.0f / row;
        std::vector<glm::vec4>* vect = new std::vector<glm::vec4>();

        for (int i = 0; i < row * column; i++)
        {
            float yOffset = float((int)i / (int)column) / row;
            float xOffset = float((int)i % (int)column) / column;
            vect->push_back(glm::vec4(xOffset * vec.z + vec.x, yOffset * vec.w + vec.y, width * vec.z, height * vec.w));

        }
        sequenceList_.insert({ name, vect });
    }

    bool Animation::GetIsDone() const
    {
        return isDone_;
    }
    float Animation::GetPriority() const
    {
      return parent_.ComponentGet<Sprite>().GetPriority();
    }
}