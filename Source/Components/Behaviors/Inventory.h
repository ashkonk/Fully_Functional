/*!*****************************************************************************
\par filename: Inventory.h
\par  authors: Mason Dulay
\par     date: 11/29/2022

\brief
  This is the Inventory file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "Component.h"

typedef struct Slot
{
    glm::vec3 translation_;
    bool taken_;
}Slot;

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    /*!*************************************************************************
      \brief
        This is an example of a component class.
    ***************************************************************************/
    class Inventory : public Component
    {
    private:
        Engine* engine_{ nullptr };
        std::vector<Slot> inventorySlots_;
        bool PoisonVial_;
        bool FuseboxKey_;
        bool Lighter_;

    public:
        Inventory(Engine* engine, Entity parent) : Component(parent), engine_(engine), PoisonVial_(false), FuseboxKey_(false), Lighter_(false)
        {}

        void Init() {};

        void Update(float dt); 

        void Read(const rapidjson::Value& value);

        static std::string_view Name() { return "Inventory"; }
    }; /* ExampleComponent */

}; /* Normalloc */
