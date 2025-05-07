/*!*****************************************************************************
\par filename: Disruptions.h
\par  authors: Mason Dulay
\par     date: 11/30/2022

\brief
  This is the Disruptions file for code implimentation.
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
    class Disruptions : public Component /* Example Component  * * * * */
    {
    private:
        int caused_;
        int tasksDone = 0;

    public:
        Disruptions(Entity parent) : Component(parent), caused_(0)
        {}

        void Init() {};     

        void Update(float dt); 

        void Read(const rapidjson::Value& value);

        static std::string_view Name() { return "Disruptions"; }

        int GetTasksDone() const;

    }; /* ExampleComponent */

}; /* Normalloc */