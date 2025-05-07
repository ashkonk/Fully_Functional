/*!*****************************************************************************
\par filename: Camera.cpp
\par  authors: Jonathan Meyer
\par     date: 11/4/2022

\brief
  This is the implementation of our engines' camera component.

  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Camera.h"
#include "Entity.h"

namespace FFunctional
{
    void Camera::Read(const rapidjson::Value& value)
    {
        if (value.HasMember("priority"))
            priority_ = value["priority"].GetInt();
    }

    void Camera::SetPriority(int priority)
    {
        priority_ = priority;
    }
    
    int Camera::GetPriority()
    {
        return priority_;
    }

    bool Camera::operator<(const Camera& rhs) const
    {
        return (priority_ < rhs.priority_);
    }
}
