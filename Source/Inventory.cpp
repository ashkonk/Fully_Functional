/*!*****************************************************************************
\par filename: Inventory.cpp
\par  authors: Mason Dulay
\par     date: 11/29/2022

\brief
  This is the Inventory file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "Inventory.h"
#include "PlayerController.h"

namespace FFunctional
{
  void Inventory::Update(float dt)
  {
    dt;
    if (GetParent().GetParent()->GetActive())
    {
      PlayerController pController = GetParent().GetParent()->ComponentGetFirst<PlayerController>();

      std::unordered_map<std::string, Entity&>* inventory = pController.GetInventory();

      // slots are all taken by default
      bool allTaken = true;

      // check if there are any open slots
      for (int i = 0; i < inventorySlots_.size(); ++i)
      {
        // if there is an open slot set all taken and break
        if (inventorySlots_[i].taken_ == false)
        {
          allTaken = false;
          break;
        }
      }

      if (allTaken)
      {
        return;
      }

      if (inventory->contains("PoisonVial") && PoisonVial_ == false)
      {
        // find open slot
        for (int i = 0; i < inventorySlots_.size(); ++i)
        {
          // if there is an open slot set all taken and break
          if (inventorySlots_[i].taken_ == false)
          {
            // set poison vial translation using the slot
            Entity pVial(GetParent().GetParent(), GetParent().GetParent()->EntityCreate());
            rapidjson::Document docu = OpenJson("./Data/PoisonVial.json");
            pVial.Read(docu, engine_);
            pVial.ComponentGet<Transform>().SetTranslation(&inventorySlots_[i].translation_);
            inventorySlots_[i].taken_ = true;
            break;
          }
        }
        PoisonVial_ = true;
      }

      if (inventory->contains("FuseboxKey") && FuseboxKey_ == false)
      {
        // find open slot
        for (int i = 0; i < inventorySlots_.size(); ++i)
        {
          // if there is an open slot set all taken and break
          if (inventorySlots_[i].taken_ == false)
          {
            // set FuseboxKey translation using the slot
            Entity fKey(GetParent().GetParent(), GetParent().GetParent()->EntityCreate());
            rapidjson::Document docu = OpenJson("./Data/FuseboxKey.json");
            fKey.Read(docu, engine_);
            fKey.ComponentGet<Transform>().SetTranslation(&inventorySlots_[i].translation_);
            inventorySlots_[i].taken_ = true;
            break;
          }
        }
        FuseboxKey_ = true;
      }

      if (inventory->contains("Lighter") && Lighter_ == false)
      {
        // find open slot
        for (int i = 0; i < inventorySlots_.size(); ++i)
        {
          // if there is an open slot set all taken and break
          if (inventorySlots_[i].taken_ == false)
          {
            // set poison Lighter translation using the slot
            Entity lighter(GetParent().GetParent(), GetParent().GetParent()->EntityCreate());
            rapidjson::Document docu = OpenJson("./Data/Lighter.json");
            lighter.Read(docu, engine_);
            lighter.ComponentGet<Transform>().SetTranslation(&inventorySlots_[i].translation_);
            inventorySlots_[i].taken_ = true;
            break;
          }
        }
        Lighter_ = true;
      }
    }

  }

  void Inventory::Read(const rapidjson::Value&)
  {
    glm::vec3 slot1 = { -0.425f, -0.35f, 0.0f };
    glm::vec3 slot2 = { -0.35f, -0.35f, 0.0f };
    glm::vec3 slot3 = { -0.27, -0.35f, 0.0f };

    Slot s1;
    s1.translation_ = slot1;
    s1.taken_ = false;

    Slot s2;
    s2.translation_ = slot2;
    s2.taken_ = false;

    Slot s3;
    s3.translation_ = slot3;
    s3.taken_ = false;

    inventorySlots_.push_back(s1);
    inventorySlots_.push_back(s2);
    inventorySlots_.push_back(s3);
  }
}