/*!*****************************************************************************
\par filename: SetupFactory.h
\par  authors: Alijah Rosner
\par     date: 09/09/2022

\brief
  Holds some functions for the set up factory. 
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include "Factory.h"
#include "Entity.h"
#include <rapidjson/document.h>

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{
    using ComponentFactory = Factory<Engine*, Entity&, const rapidjson::Value&>;

    void SetupComponentFactory(ComponentFactory& factory);

    void SetupComponentFactoryEngine(ComponentFactory& factory);
}