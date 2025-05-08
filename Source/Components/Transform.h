/*!*****************************************************************************
\par filename: Transform.h
\par  authors: Alijah Rosner
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
*******************************************************************************/
#pragma once
#include <glm/glm.hpp>
#include <rapidjson/document.h>
#include "Component.h"
#include <string_view>

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/

namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{
  /*!***************************************************************************
    \brief
      This is the transform class
  *****************************************************************************/
  class Transform : public Component
  {

  public:

      Transform(Entity parent) : Component(parent){};
      void Read(const rapidjson::Value& value);
      glm::mat4* GetMatrix();
      const glm::vec3* GetTranslation() const; 
      float GetRotation() const; 
      const glm::vec3* GetScale() const; 
      void SetTranslation(const glm::vec3* translation);
      void SetRotation(float rotation);
      void SetScale(const glm::vec3* scale);
      static std::string_view Name() { return "Transform"; }
  private: 

      glm::vec3 translation_ = {};
      glm::vec3 scale_ = {};
      glm::mat4 matrix_ = {};
      glm::vec3 rotationVec = {0, 0, -1};
      float rotation_ = 0.f;
      bool isDirty_ = true; 
  }; 

}; /* FFunctional */