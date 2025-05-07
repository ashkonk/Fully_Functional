/*!*****************************************************************************
\par filename: stub.h
\par  authors: Jonathan Meyer, Ashkon Khalkhali, Alijah Rosner, Mason Dulay
\par     date: 09/09/2022

\brief
Declaration of Tendril Behavior for the Detective Mode state
Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include <string>
#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include <vector>
#include "Component.h"
#include "Engine.h"
#include "Entity.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{

    class TendrilBehavior : public Component
    {
    private:
      Engine* engine_{ nullptr };
      //Location (in world space) of next scene entrance
      glm::vec3 cameraPos_;

    public:
      TendrilBehavior(Engine* engine, Entity parent) : Component(parent),
        engine_(engine), cameraPos_{} {}

      void Init();           //!< Our ExampleComponent Init Function. 

      void Read(const rapidjson::Value&);

      void Update(float dt); //!< Our ExampleComponent Update Function.

      void SetActive(float opacity);

      void Dissipate(float opacity);

      static std::string_view Name() { return "TendrilBehavior"; }

    }; /* ExampleComponent */

}; /* Normalloc */