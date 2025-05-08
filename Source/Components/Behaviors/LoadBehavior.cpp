/*!*****************************************************************************
\par filename: stub.cpp
\par  authors: Ashkon Khalkhali
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "LoadBehavior.h"
#include "Sprite.h"
#include "Graphics.h"
#include "Manager.h"
#include "Scene.h"

namespace FFunctional
{

  void LoadBehavior::Init()
  {

  }

  void LoadBehavior::Read(const rapidjson::Value& value)
  {
    if (value.HasMember("Ready"))
    {
      IsReady = value.GetBool();
    }
    value;
  }

  void LoadBehavior::Update(float dt)
  {
    if (CheckReady())
    {
      parent_.ComponentGet<Sprite>().SetOpacity(0.0f);
    }
    else
    {
      Manager<Scene>* sceneManager = engine_->GetSystem<Manager<Scene>>();
      for (unsigned int i = 0; i < sceneManager->Size(); ++i)
      {
        sceneManager->At(i)->SetDrawing(false);
        sceneManager->At(i)->SetRunning(false);

      }
      parent_.ComponentGet<Sprite>().SetOpacity(100.0f);

    }
    dt;
  }

  bool LoadBehavior::CheckReady()
  {
    return Graphics::AsyncImageReady();
  }

}