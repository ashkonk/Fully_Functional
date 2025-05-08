/*!*****************************************************************************
\par filename: Occluder.cpp
\par  authors: Alijah Rosner
\par     date: 09/11/2022

\brief
  This is the implementation of occluder
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp> 
#include "Stream.h"
#include "Occluder.h"
#include "Transform.h"
#include <algorithm>
#include "Graphics.h"
#include "PlayerController.h"

namespace FFunctional
{

  void Occluder::Read(const rapidjson::Value& value)
  {
    SetWidth(1.0f);
    SetDepth(1.0f);
    SetHeight(1.0f);

    offset_ = glm::vec3(0.0f);
    if (value.HasMember("Offset"))
      JsonConvert(value["Offset"], offset_);
    if (value.HasMember("Width"))
      SetWidth(value["Width"].GetFloat());
    if (value.HasMember("Height"))
      SetHeight(value["Height"].GetFloat());
    if (value.HasMember("Depth"))
      SetDepth(value["Depth"].GetFloat());
  }

  static float RectSDF(const glm::vec3& coords, float width, float height)
  {
    glm::vec2 edge = glm::vec2(abs(coords.x) - width / 2, abs(coords.y) - height / 2);
    edge.x = std::max(edge.x, 0.0f), edge.y = std::max(edge.y, 0.0f);

    float outside = glm::length(edge);
    float inside = std::min(std::max(edge.x, edge.y), 0.0f);

    return outside + inside;
  }

  float Occluder::SignedDistance(glm::vec3 coords) const
  {
    glm::vec3 newCoords = coords - offset_;
    newCoords -= *GetParent().ComponentGet<Transform>().GetTranslation();
    glm::vec3 newSize = glm::vec3(width_, height_, 0.0f);
    newSize *= *GetParent().ComponentGet<Transform>().GetScale();

    return RectSDF(newCoords, newSize.x, newSize.y);
  }

  float Occluder::GetDepth() const
  {
    return depth_;
  }

  void Occluder::SetDepth(float depth)
  {
    depth_ = depth;
  }

  glm::vec3 Occluder::GetOffset() const
  {
    return offset_;
  }

  float Occluder::GetWidth() const
  {
    return width_;
  }

  float Occluder::GetHeight() const
  {
    return height_;
  }

  void Occluder::SetOffset(glm::vec3 offset)
  {
    offset_ = offset;
  }

  void Occluder::SetWidth(float width)
  {
    width_ = width;
  }

  void Occluder::SetHeight(float height)
  {
    height_ = height;
  }

  std::map<Component*, Occluder&> occluders;

  void Occluder::Init()
  {

  }

  Occluder::~Occluder()
  {
    Graphics::UnsetOccluder(int(distance(occluders.begin(), occluders.find(this))));

    if (occluders.find(this) != occluders.end())
      occluders.erase(occluders.find(this));
    //Graphics::SetOcclusions(occluders);
  }

  void Occluder::Update(float dt)
  {
    occluders.emplace(this, *this);
    Graphics::SetOcclusions(occluders);

    /*if (GetParent().GetParent()->ComponentGetFirst<PlayerController>().GetCrouch())
    {
        SetHeight(height_);
    }
    else
    {
        SetHeight(height_ / 2);
    }*/

    dt;
    // gotta make occluders be able to move 
  }


  glm::vec4 Occluder::GetDimensions() const
  {
    if (!GetParent().GetParent() || !GetParent().GetParent()->HasManager<Transform>())
      return glm::vec4(0, 0, 1, 1);

    if (GetParent().Has<Transform>())
    {
      Transform& transform = GetParent().ComponentGet<Transform>();
      glm::vec4 vec = glm::vec4(*transform.GetTranslation(), 0);

      glm::vec3 scale = *transform.GetScale();
      vec.x += offset_.x, vec.y += offset_.y;
      vec.z = GetWidth() * scale.x, vec.w = GetHeight() * scale.y;
      return vec;
    }
    return glm::vec4(0, 0, 1, 1);
  }

}
