/*!*****************************************************************************
\par filename: Occluder.h
\par  authors: Alijah Rosner
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
    Copyright © 2022 DigiPen (USA) Corporation.

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
      This is the Occluder class
  *****************************************************************************/
  class Occluder : public Component
  {

  public:

      Occluder(Entity parent) : Component(parent) {};
      ~Occluder();
      void Read(const rapidjson::Value& value);
      void Update(float dt);

      void Init();

      static std::string_view Name() { return "Occluder"; }

      float SignedDistance(glm::vec3 coords) const;

      glm::vec3 GetOffset() const;

      float GetWidth() const;

      float GetHeight() const;

      float GetDepth() const;

      void SetOffset(glm::vec3 offset);

      void SetWidth(float width);
      
      void SetHeight(float height);

      void SetDepth(float depth);

      glm::vec4 GetDimensions() const;

  private: 
      glm::vec4 points_;
      float width_, height_, depth_;
      glm::vec3 offset_;
  }; 

}; /* FFunctional */