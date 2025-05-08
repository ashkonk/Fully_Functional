/*!*****************************************************************************
\par filename: Volume.h
\par  authors: Jonathan Meyer, Ashkon Khalkhali, Alijah Rosner, Mason Dulay
\par     date: 09/09/2022

\brief
  This is the Volume file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "Behavior.h"
#include "Audio.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    /*!*************************************************************************
      \brief
        This is an example of a component class.
    ***************************************************************************/

    class VolumeBehavior : public Behavior<VolumeBehavior> /* Example Component  * * * * */
    {
    private:
        static float volume;   //!< this is an example of a member value.
        Audio* audio_;
        float xBase_;
        static float xOffset_;
        glm::vec3 scaleInit;
        glm::vec3 translationInit;

    public:

        VolumeBehavior(Engine* engine, Entity parent) : Behavior(parent), audio_(engine->GetSystem<Audio>()), xBase_(0.0f), scaleInit(0), translationInit(0) {};

        void Update(float dt);

        void IncrementVolume();

        void DecrementVolume();

        static float GetVolume();

        static void SetVolume(float newVolume);

        static std::string_view Name() { return "VolumeBehavior"; }

        void Read(const rapidjson::Value& value);

    }; /* ExampleComponent */

}; /* Normalloc */