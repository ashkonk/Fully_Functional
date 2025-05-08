/*!*****************************************************************************
\par filename: SetupFactory.cpp
\par  authors: Jonathan Meyer, Ashkon Khalkhali, Alijah Rosner, Mason Dulay
\par     date: 09/09/2022

\brief
  This is the Setup Factory templatized factory.
  Copyright � 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "SetupFactory.h"
#include "Physics.h"
#include "Transform.h"
#include "Sprite.h"
#include "Input.h"
#include "Engine.h"
#include "PlayerController.h"
#include "AttendantA.h"
#include "Animation.h"
#include "Collider.h"
#include "Button.h"
#include "Camera.h"
#include "FootstepEmitter.h"
#include "ParticleEmitter.h"
#include "Inventory.h"
#include "Disruptions.h"
#include "VisionMode.h"
#include "Detection.h"
#include "TendrilBehavior.h"
#include "Vision.h"
#include "Occluder.h"
#include "StealthBarBG.h"
#include "Shadow.h"
#include "TeleportUI.h"
#include "DestroyTimer.h"
#include "FadeController.h"

namespace FFunctional
{
  template<typename T>
  static void AddToFactory(ComponentFactory& factory)
  {
    factory.Add(T::Name(),
      [](Engine*, Entity& entity, const rapidjson::Value& value)
      {
        T& t = entity.ComponentAdd<T>(entity);
        t.Read(value);
      });
  }

  template<typename T>
  static void AddToFactoryWithScene(ComponentFactory& factory)
  {
    factory.Add(T::Name(),
      [](Engine* engine, Entity& entity, const rapidjson::Value& value)
      {
        auto sceneManager = engine->GetSystem<Manager<Scene>>();
        T& t = entity.ComponentAdd<T>(engine, entity, sceneManager->At(sceneManager->Size() - 1).get());
        t.Read(value);
      });
  }


  void SetupComponentFactory(ComponentFactory& factory)
  {
    AddToFactory<Transform>(factory);
    AddToFactory<Physics>(factory);
    AddToFactory<Sprite>(factory);
    AddToFactory<Animation>(factory);
    AddToFactory<Camera>(factory);
    AddToFactory<FootstepEmitter>(factory);
    AddToFactory<ParticleEmitter>(factory);
    AddToFactory<Disruptions>(factory);
    AddToFactory<VisionMode>(factory);
    AddToFactory<Vision>(factory);
    AddToFactory<Occluder>(factory);
    AddToFactory<Detection>(factory);
    AddToFactory<StealthBarBG>(factory);
    AddToFactory<Shadow>(factory);
    AddToFactory<TeleportUI>(factory);

    AddToFactoryWithScene<Button>(factory);
    AddToFactoryWithScene<DestroyTimerBehavior>(factory);
    AddToFactoryWithScene<FadeControllerBehavior>(factory);
    SetupComponentFactoryEngine(factory);
  }
}