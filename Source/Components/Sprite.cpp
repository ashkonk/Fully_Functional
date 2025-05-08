/*!*****************************************************************************
\par filename: Sprite.cpp
\par  authors: Jonathan Meyer
\par     date: 09/30/2022

\brief
  This is the implementation of our engines' sprite component.

  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Sprite.h"
#include "Transform.h"
#include "Entity.h"
#include "Stream.h"
#include "Graphics.h"

namespace FFunctional
{

  void Sprite::Read(const rapidjson::Value& value)
  {
    JsonConvertString(value["tex_path"], tex_path_);
    zPriority = 0;
    SetOpacity(1.0f);

    if (value.HasMember("z_priority"))
        zPriority = -value["z_priority"].GetFloat();
    
    isStatic = false;
    if (value.HasMember("static"))
        isStatic = value["static"].GetBool();

    if (value.HasMember("opacity"))
        SetOpacity(value["opacity"].GetFloat());


    if (value.HasMember("overlay"))
      SetOverlay(value["overlay"].GetBool());

    //source_ = Graphics::LoadImage(tex_path_.data());
    source_ = (Graphics::LoadImage(tex_path_.data()));
    uvs_ = glm::vec4{0, 0, 1, 1}; // default

    if (value.HasMember("UVs"))
    {
        auto uvs = value["UVs"].GetArray();

        glm::vec4 myUVs = glm::vec4(uvs[0].GetFloat(), uvs[1].GetFloat(), uvs[2].GetFloat(), uvs[3].GetFloat());
        SetUVs(myUVs);
    }

    //buffer = nullptr;
    if (value.HasMember("Text"))
    {
        //sprintf_s(buffer, value["Text"].GetString());
        /*std::string string = value["Text"].GetString();
        buffer = string.c_str();*/
        buffer = value["Text"].GetString();
    }

    if (value.HasMember("Rows") && value.HasMember("Cols"))
    {
        rows_ = value["Rows"].GetInt();
        cols_ = value["Cols"].GetInt();
    }

    // make sure that when loading an image, the renderer/graphics system
    // checks to make sure that none of the loaded sprite components has
    // the same path name.

    // if they find a match, just copy over the shared pointer.
  }

  bool Sprite::GetOverlay() const
  {
    return overlay_;
  }

  void Sprite::SetOverlay(bool o)
  {
    overlay_ = o;
  }

  float Sprite::GetBottomAnchor() const
  {
      // calculate where on the screen the bottom the sprite is.
      // this is used for z-ordering
      float anchorPoint = 0;
      Transform& transform = GetParent().ComponentGet<Transform>();

      if (&transform)
      {
          anchorPoint += transform.GetTranslation()->y;
          anchorPoint -= fabsf(transform.GetScale()->y) / 2;
      }

      return anchorPoint + zPriority;
  }

  void Sprite::SetStatic(bool static_)
  {
    isStatic = static_;
  }

  bool Sprite::GetStatic() const
  {
      return isStatic;
  }

  void Sprite::SetPriority(float z)
  {
      zPriority = z; 
  }

  float Sprite::GetPriority() const
  {
      return zPriority;
  }

  std::string Sprite::GetText() const
  {
      return buffer.c_str();
  }

  glm::vec4 Sprite::GetTextUVs(unsigned int frameIndex) const
  {
      float uSize = 1.0f / cols_;
      float vSize = 1.0f / rows_;
      float u = uSize * int(frameIndex % cols_);
      float v = vSize * int(frameIndex / cols_);
      glm::vec4 uv = { u, v, uSize, vSize };
      return uv;
  }


  void Sprite::SetActive(bool flag)
  {
    active_ = flag;
  }

  void Sprite::SetOpacity(float opacity)
  {
      opacity_ = opacity;
  }

  float Sprite::GetOpacity() const
  {
      return opacity_;
  }

  void Sprite::Dissolve()
  {
    opacity_ -= 0.01f;
  }

  bool Sprite::GetActive() const
  {
    return active_;
  }

  std::shared_ptr<Image> Sprite::GetImage() const
  {
    return source_;
  }

  glm::vec4 Sprite::GetUVs() const
  {
    return uvs_;
  }

  void Sprite::SetUVs(glm::vec4 uvs)
  {
    uvs_ = uvs;
  }

  bool Drawable::operator<(const Drawable& rhs) const
  {
      return floorPosY < rhs.floorPosY;
  }

  unsigned Drawable::GetIndex() const
  {
      return index;
  }


}
