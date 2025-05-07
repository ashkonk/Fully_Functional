/*!*****************************************************************************
\par filename: TeleportUI.h
\par  authors: Mason Dulay
\par     date: 09/09/2022

\brief
  This is the TeleportUI file for code implimentation.
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

    class TeleportUI : public Behavior<TeleportUI>
    {
    private:

    public:

        TeleportUI(Entity parent) : Behavior(parent) {};

        void Update(float dt); //!< Our ExampleComponent Update Function.

        static std::string_view Name() { return "TeleportUI"; }

        void Read(const rapidjson::Value& value);

    }; /* ExampleComponent */

}; /* Normalloc */