/*!*****************************************************************************
\par filename: stub.h
\par  authors: Jonathan Meyer, Ashkon Khalkhali, Alijah Rosner, Mason Dulay
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "Component.h"
#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

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

  class LoadBehavior : public Component
  {
  private:
    bool IsReady = false;
    Engine* engine_;
  public:
    LoadBehavior(Engine* engine, Entity parent) : Component(parent), engine_(engine), IsReady(true) { }
     
    void Init();           //!< Our ExampleComponent Init Function. 

    void Read(const rapidjson::Value& value);

    void Update(float dt); //!< Our ExampleComponent Update Function.

    bool CheckReady();

    static std::string_view Name() { return "LoadBehavior"; }

    //oid Open(Entity doorTrigger);

  }; /* ExampleComponent */

}; /* Normalloc */