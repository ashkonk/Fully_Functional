/*!*****************************************************************************
\par filename: VisionMode.h
\par  authors: Mason Dulay
\par     date: 11/30/2022

\brief
  This is the VisionMode file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "Component.h"

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
    class VisionMode : public Component /* Example Component  * * * * */
    {
    private:
        int mode_;

    public:
        VisionMode(Entity parent) : Component(parent), mode_(0) {}

        void Init() {}

        void Update(float dt); 

        void Read(const rapidjson::Value& value);

        static std::string_view Name() { return "VisionMode"; }

    }; /* ExampleComponent */

}; /* Normalloc */