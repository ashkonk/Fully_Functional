/*!*****************************************************************************
\par filename: StealthBar.h
\par  authors: Mason Dulay
\par     date: 03/03/2023

\brief
  This is the StealthBar file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "Component.h"
#include "Sprite.h"
#include "Sound.h"
#include "Input.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    /*!*************************************************************************
      \brief
        StealthBar component class, this class handles the UI fill
    ***************************************************************************/

    class StealthBar : public Component /* Example Component  * * * * */
    {
    private:
        static float fill_, anim_;
        glm::vec3 scaleInit;
        glm::vec3 translationInit;
        Engine* engine_; 
        std::shared_ptr<Sound> lose_; 
        bool godmode_;
        Input* input_;
        float timer_;

    public:

        float GetFill();

        static void SetFill(float fill);

        static void ClearFill();

        StealthBar(Engine* engine, Entity parent) : engine_(engine), Component(parent), scaleInit(0), translationInit(0), godmode_(false), timer_(1.0f)
        {
            input_ = engine->GetSystem<Input>();
        }

        void Update(float dt); //!< Our ExampleComponent Update Function.

        void Read(const rapidjson::Value& value);

        static std::string_view Name() { return "StealthBar"; }

    }; /* ExampleComponent */

}; /* Normalloc */