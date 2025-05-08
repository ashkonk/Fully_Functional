/*!*****************************************************************************
\par filename: Detected.h
\par  authors: Mason Dulay
\par     date: 09/09/2022

\brief
  This is the Detected file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "Behavior.h"
#include "Sound.h"
#include "Engine.h"

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

    class Detected : public Behavior<Detected> /* Example Component  * * * * */
    {
    private:
        bool declining;
        float beforeFill;
        std::shared_ptr<Sound> initialCaught_;
        Engine* engine_{ nullptr };

    public:

        Detected(Engine* engine, Entity parent) : engine_(engine), Behavior(parent), declining(true), beforeFill(0.0f) {}

        void Update(float dt); //!< Our ExampleComponent Update Function.

        void Read(const rapidjson::Value& value);

        static std::string_view Name() { return "Detected"; }

    }; /* ExampleComponent */

}; /* Normalloc */