/*!*****************************************************************************
\par filename: Camera.h
\par  authors: Jonathan Meyer
\par     date: 11/4/2022

\brief
  This file contains the public declarations for the camera component.

  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include <rapidjson/document.h>
#include "Component.h"
#include <string_view>
#include <string>
#include "Stream.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/

namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{
    /*!***************************************************************************
      \brief
        This is the camera class
    *****************************************************************************/
    class Camera : public Component
    {
    private:
        int priority_;
        
    public:
        Camera(Entity parent) : Component(parent), priority_(0) {};

        void Read(const rapidjson::Value& value);
        static std::string_view Name() { return "Camera"; };
        void SetPriority(int priority);
        int GetPriority();

        bool operator<(const Camera& rhs) const;
    };

}; /* FFunctional */