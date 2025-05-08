#include "SetupFactory.h"

#include "Interactable.h"
#include "InteractableState.h"
#include "Armament.h"
#include "Teleport.h"
#include "AntagBehavior.h"
#include "MovingObject.h"
#include "LoadBehavior.h"
#include "DoorBehavior.h"
#include "CameraBehavior.h"
#include "Detected.h"
#include "PoisonVial.h"
#include "CCTV.h"
#include "GuardBehavior.h"
#include "RoomCover.h"
#include "TrashFire.h"
#include "FuseboxKey.h"
#include "Lighter.h"
#include "Inventory.h"
#include "StealthBar.h"
#include "Volume.h"
#include "BrokenDoorBehavior.h"
#include "SoundBehavior.h"
#include "TutorialText.h"


namespace FFunctional
{
  template<typename T>
  static void AddToFactoryWithEngine(ComponentFactory& factory)
  {
    factory.Add(T::Name(),
      [](Engine* engine, Entity& entity, const rapidjson::Value& value)
      {
        T& t = entity.ComponentAdd<T>(engine, entity);
        t.Read(value);
      });
  }

  void SetupComponentFactoryEngine(ComponentFactory& factory)
  {
    AddToFactoryWithEngine<SoundBehavior>(factory);
    AddToFactoryWithEngine<Detected>(factory);
    AddToFactoryWithEngine<VolumeBehavior>(factory);
    AddToFactoryWithEngine<StealthBar>(factory);
    AddToFactoryWithEngine<LoadBehavior>(factory);
    AddToFactoryWithEngine<AttendantBehavior>(factory);
    AddToFactoryWithEngine<PoisonVialBehavior>(factory);
    AddToFactoryWithEngine<PlayerController>(factory);
    AddToFactoryWithEngine<TrashFireBehavior>(factory);
    AddToFactoryWithEngine<RoomCover>(factory);
    AddToFactoryWithEngine<Collider>(factory);
    AddToFactoryWithEngine<MovingObject>(factory);
    AddToFactoryWithEngine<TeleportBehavior>(factory);
    AddToFactoryWithEngine<DoorBehavior>(factory);
    AddToFactoryWithEngine<CameraBehavior>(factory);
    AddToFactoryWithEngine<CCTVBehavior>(factory);
    AddToFactoryWithEngine<GuardBehavior>(factory);
    AddToFactoryWithEngine<AntagBehavior>(factory);
    AddToFactoryWithEngine<FuseboxKeyBehavior>(factory);
    AddToFactoryWithEngine<LighterBehavior>(factory);
    AddToFactoryWithEngine<Inventory>(factory);
    AddToFactoryWithEngine<Armament>(factory);
    AddToFactoryWithEngine<Interactable>(factory);
    AddToFactoryWithEngine<BrokenDoorBehavior>(factory);
    AddToFactoryWithEngine<TutorialText>(factory);

  }
}

