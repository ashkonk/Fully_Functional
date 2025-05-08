/*!*****************************************************************************
\par filename: Vision.h
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
      This is the Occluder class
  *****************************************************************************/
  class Vision : public Component
  {

  public:

    Vision(Entity parent) : Component(parent), offset_() { };

    ~Vision();

    void Read(const rapidjson::Value& value);
    void Update(float dt);
    void Init();

    static std::string_view Name() { return "Vision"; }

    void SetAngle(float theta);

    float GetAngle() const;

    void SetFOV(float fov);

    float GetFOV() const;

    void SetViewDist(float dist);

    float GetViewDist() const;

    glm::vec2 GetPos() const;

    glm::vec3 GetFrustrum() const;

    static float GetMinDistance();
    
    static void SetMinDistance(float newDist);

    static void WipeMinDistance();

  private:
    glm::vec3 offset_;
    std::vector<glm::vec3> mesh_;
    float angle_ = 0.0f, fov_ = 90.0f, distance_ = 1.0f;
    bool lastVisible_ = false;

    static float minDistance_;
  };

}; /* FFunctional */