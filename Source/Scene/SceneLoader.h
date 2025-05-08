/*!*****************************************************************************
\par filename: stub.h
\par  authors: Jonathan Meyer, Ashkon Khalkhali, Alijah Rosner, Mason Dulay
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include "Manager.h"
#include "Scene.h"
#include "EntityID.h"


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
  void SceneInit(Manager<Scene>& sceneManager);
  std::shared_ptr<Scene> LoadScene(Manager<Scene>& sceneManager, const char* filepath);
  //tell necessary managers within ecs to active/inactive
  void SetECSState(ECS& newEcs, bool flag);
}; /* Normalloc */