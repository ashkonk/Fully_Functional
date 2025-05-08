/*!*****************************************************************************
\par filename: Sprite.h
\par  authors: Jonathan Meyer
\par     date: 09/30/2022

\brief
  This file contains the public declarations for the sprite component.

  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include <rapidjson/document.h>
#include "Component.h"
#include "Graphics.h"
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
      This is the sprite class
  *****************************************************************************/
  class Sprite : public Component
  {

  public:
    Sprite(Entity parent) : Component(parent), uvs_(glm::vec4(0,0,1,1)),
    zPriority(0), opacity_(1.0f), buffer(""), overlay_(false) {};
    
    void Read(const rapidjson::Value& value);
    static std::string_view Name() { return "Sprite"; }

    void SetActive(bool flag);
    bool GetActive() const;


    std::shared_ptr<Image> GetImage() const;
    float GetBottomAnchor() const;
    
    glm::vec4 GetUVs() const;
    void SetUVs(glm::vec4 uvs);

    void SetOpacity(float opacity);
    float GetOpacity() const;

    void Dissolve();

    void SetStatic(bool static_);
    bool GetStatic() const;

    void SetPriority(float z);
    float GetPriority() const;

    bool GetOverlay() const;
    void SetOverlay(bool o);


    //text implementation
    std::string GetText() const;
    glm::vec4 GetTextUVs(unsigned int frameIndex) const;
    

  private:
    bool active_ = true;
    std::string   tex_path_;
    std::shared_ptr<Image> source_;
    glm::vec4 uvs_;
    float zPriority;
    float opacity_;
    float isStatic;
    bool overlay_;

    // text implementation
    //char buffer[75];
    std::string buffer;
    int rows_;
    int cols_;
  };

  class Drawable
  {
  private:
      unsigned index;  /* index of sprite in the manager */
      float floorPosY; /* the bottom anchor of the sprite on screen */
  
  public:
      Drawable(unsigned i, float posY) : index(i), floorPosY(posY) {};
      bool operator<(const Drawable& rhs) const;

      unsigned GetIndex() const;
  };

}; /* FFunctional */