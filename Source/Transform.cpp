/*!*****************************************************************************
\par filename: Transform.cpp
\par  authors: Alijah Rosner
\par     date: 09/11/2022

\brief
  This is the implementation of transform
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include <glm/gtc/matrix_transform.hpp>
#include "QuickMaths.h"
#include <glm/matrix.hpp> 
#include "Transform.h"
#include "Stream.h"


namespace FFunctional
{

    glm::mat4* Transform::GetMatrix()
    {
      if (isDirty_ == true)
      {
        glm::mat4 scale = glm::scale(glm::mat4(1), scale_);
        glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(rotation_), rotationVec);
        glm::mat4 translate = glm::translate(glm::mat4(1), translation_);
        matrix_ = translate * rotation * scale;
        isDirty_ = false;
        return &matrix_;
      }
      return &matrix_;
    }

    const glm::vec3* Transform::GetTranslation() const
    {
        return &translation_;
    }

    void Transform::Read(const rapidjson::Value& value)
    {

        JsonConvert(value["translation"], translation_);
        JsonConvert(value["scale"], scale_);
        rotation_ = value["rotation"].GetFloat(); 

    }


    float Transform::GetRotation() const
    {
        return rotation_;
    }


    const glm::vec3* FFunctional::Transform::GetScale() const
    {
        return &scale_;
    }


    void FFunctional::Transform::SetTranslation(const glm::vec3* translation)
    {
        VectorSet(&translation_, translation->x, translation->y);
        isDirty_ = true;

    }

    void FFunctional::Transform::SetRotation(float rotation)
    {
        rotation_ = rotation;
        isDirty_ = true;
    }



    void FFunctional::Transform::SetScale(const glm::vec3* scale)
    {
        scale_ = *scale;
        isDirty_ = true;
    }

}
