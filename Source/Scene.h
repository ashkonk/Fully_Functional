/*!*****************************************************************************
\par filename: Scene.h
\par  authors: Ashkon Khalkhali
\par     date: 09/17/2022

\brief
  Header file for base scene class
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include <vector>
#include <stdlib.h>
// for shared pointers
//points at one thing, whenever all of the shared pointers pointing at the same 
// thing go out of scope, it will free the memory for you
#include <rapidjson/document.h>
#include <memory>
#include "Manager.h"
#include "Entity.h"
#include "ECS.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{
  using EntityPtr = std::shared_ptr<Entity>;
  /*!***************************************************************************
    \brief
      This is the Scene Class, a container for all entities in a level.
  *****************************************************************************/
  class Scene
  {
  private:
    std::string sceneName_;
    ECS sceneEcs_;
    bool Drawing_ = true;
    bool Running_ = true; 
    bool reload_;
    bool destroy_ = false;

    protected:
      std::vector<EntityPtr> entityList_;
      Manager<FFunctional::Scene> *parent_;

    public:
      
      void SetManager(Manager<FFunctional::Scene>* parent);

      Manager<FFunctional::Scene> *GetManager();

      void SetName(const char* fp);

      void Reload();

      void SetDrawing(bool set);

      void Reloading();

      std::string GetName() const;

      void Init();           //!< Our ExampleComponent Initialization Function. 

      void Draw() const;

      // adding entity to a scene.

      void Read(const rapidjson::Value& value);

      void SetRunning(bool set);

      const ECS* getECS() const;
      ECS& getECS();

      void Destroy();

      void ForceDestroy();

      ~Scene();


      void Update(float dt); //!< Our ExampleComponent Update Function.

      void Exit();           //!< Our ExampleComponent Shutdown Function.

      void Stop();

      static std::string_view Name() { return "Scene"; }

      void SetNextScene(std::shared_ptr<FFunctional::Scene> nextScene);

  }; /* ExampleComponent */

}; /* Normalloc */