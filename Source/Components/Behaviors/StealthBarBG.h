/*!*****************************************************************************
\par filename: StealthBarBG.h
\par  authors: Mason Dulay
\par     date: 09/09/2022

\brief
  This is the StealthBarBG file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "Behavior.h"

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

    class StealthBarBG : public Behavior<StealthBarBG> /* Example Component  * * * * */
    {
    private:
        bool closed_;

    public:

        StealthBarBG(Entity parent) : Behavior(parent), closed_(true) {}

        void SetEyeClosed(bool closed);

        void Update(float dt);

        void Read(const rapidjson::Value&);

        static std::string_view Name() { return "StealthBarBG"; }
    }; /* ExampleComponent */

}; /* Normalloc */