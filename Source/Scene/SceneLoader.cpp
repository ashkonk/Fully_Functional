/*!*****************************************************************************
\par filename: SceneLoader.cpp
\par  authors: Ashkon Khalkhali
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include <string>
#include "stub.h"
#include "SceneLoader.h"
#include "Stream.h"
#include "Sprite.h"
#include "Audio.h"

namespace FFunctional
{
  //init the first scene. (Placeholder, menu will call LoadScene when play
  //is pressed. Ambrose needs to be read in from a separate json file.
  void SceneInit(Manager<Scene>& sceneManager)
  {
    auto initFile = OpenJson("./Data/SplashScreen0Main.json");
    //auto initFile = OpenJson("./Data/CreditsMain.json");
    auto scene = std::make_shared<Scene>();

   // auto loadFile = OpenJson("./Data/LoadScreenMain.json");
    //auto loadScene = std::make_shared<Scene>();

    //sceneManager.Add(loadScene);
    //loadScene->SetManager(&sceneManager);
    //loadScene->SetName("./Data/LoadScreenMain.json");
    //loadScene->Read(loadFile);
    //SetECSState(loadScene->getECS(), true);

    sceneManager.Add(scene);

    scene->SetManager(&sceneManager);
    scene->SetName("./Data/SplashScreen0Main.json");
    scene->Read(initFile);

    SetECSState(scene->getECS(), true);
  }

  //Loads and initializes a scene from filepath. If scene already exists,
  //function simply returns it.

  std::shared_ptr<Scene> LoadScene(Manager<Scene>& sceneManager, const char* filepath)
  {
    
    auto initFile = OpenJson(filepath);
    auto scene = std::make_shared<Scene>();
    scene->SetManager(&sceneManager);

 /*   Audio& audio = *sceneManager.GetParent()->GetSystem<Audio>();

    std::unordered_map<std::string, std::shared_ptr<Sound>>* library = audio.GetLibrary();

    library->clear();
    rapidjson::Document docu = OpenJson("./Data/Audio/SFX/AllAudio.json");
    audio.CreateAllSounds(docu);*/

    sceneManager.Add(scene);
    scene->SetName(filepath);
    scene->Read(initFile);
    SetECSState(scene->getECS(), true);
    return scene;
  }
  //As of now, sets all sprites to inactive and sets the ecs Active_ to flag
  void SetECSState(ECS& ecs, bool flag)
  {
    auto spriteManager = ecs.GetManager<Sprite>();
    auto& spriteRef = *spriteManager;

    for (unsigned j = 0; j < spriteManager->GetSize(); ++j)
    {
      spriteRef[j].SetActive(flag);
    }
    ecs.SetActive(flag);
  }
}