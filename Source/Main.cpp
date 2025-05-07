/*!*****************************************************************************
\par filename: Main.cpp
\par  authors: Ashkon Khalkhali, Jonathan Meyer, Mason Dulay, Alijah Rosner
\par     date: 09/11/2022

\brief
  Main function to manage engine.
  Copyright � 2022 DigiPen (USA) Corporation.
*******************************************************************************/

 #ifdef NDEBUG
 #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
 #endif


#define GLFW_INCLUDE_NONE

#include <string_view>
#include <GLFW/glfw3.h>
#include "Factory.h"
#include "Engine.h"
#include "Window.h"
#include "Input.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "Entity.h"
#include "SetupFactory.h"
#include "SceneLoader.h"
#include "Audio.h"
#include "Sound.h"
#include "Trace.h"

int main(void)
{
    using namespace FFunctional;
    using SceneManager = Manager<Scene>;

    Engine mainEngine;
    mainEngine.Add(new FFunctional::Audio);
    mainEngine.Add(new Input);
    mainEngine.Add(new Window);
    mainEngine.Add(new SceneManager(SceneInit));
    mainEngine.Add(new ComponentFactory(SetupComponentFactory));
    mainEngine.Add(new Renderer);

    mainEngine.Init();




    glfwSetInputMode(mainEngine.GetSystem<Window>()->GetWindow(), GLFW_STICKY_KEYS, GLFW_FALSE);


    while (mainEngine.IsRunning())
    {
        mainEngine.Update();
        mainEngine.Draw();
    }
    return 0;
}