/*!*****************************************************************************
\par filename: Window.cpp
\par  authors: Ashkon Khalkhali
\par     date: 09/11/2022

\brief
  CPP File for Window System. contains key callback function and polls window events
  on update.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include <GL/glew.h>
#include <cassert>
#include <vector>
#include <iostream>

#include "Window.h"
#include "Engine.h"
#include "Graphics.h"
#include "Input.h"
#include "SceneLoader.h"
#include "PlayerController.h"

namespace FFunctional
{
    static void KeyChecker(GLFWwindow* mainWindow, int key, int, int action, int);
    static void MouseChecker(GLFWwindow* mainWindow, double xpos, double ypos);
    static void ButtonChecker(GLFWwindow* mainWindow, int button, int action, int);

    int windowWidth;
    int windowHeight;

  GLFWwindow* Window::GetWindow()
  {
    return mainWindow_;
  }

  void Window::Init()
    {
        windowWidth = 1920*3/4;
        windowHeight = 1080*3/4;

        #ifdef _DEBUG
            bool windowInit = glfwInit();
            assert(windowInit && "GLFW Init failed");
        #else
            glfwInit();
        #endif // _DEBUG
        
            

        //GLFW Version 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        mainWindow_ = glfwCreateWindow(windowWidth, windowHeight, "Ambrose Doppler The Atlas Unlimited", nullptr, nullptr);
        assert(mainWindow_ && "Window Creation failed");

        assert(glewInit() && "GLEW Init failed");
        glfwMakeContextCurrent(mainWindow_);

        glfwGetFramebufferSize(mainWindow_, &windowWidth, &windowHeight);


        glViewport(0, 0, windowWidth, windowHeight);
        glfwSetWindowUserPointer(mainWindow_, parent_);
        glfwSetKeyCallback(mainWindow_, KeyChecker);
        glfwSetCursorPosCallback(mainWindow_, MouseChecker);
        glfwSetMouseButtonCallback(mainWindow_, ButtonChecker);
        //CAN MESS WITH THIS TO SET CUSTOM CURSOR (GAMEOBJECT)
        std::shared_ptr<Image> cursor = Graphics::LoadImage("Assets/UI_Cursor_001.PNG");
        GLFWimage cursorImage;
        cursorImage.width = cursor->width;
        cursorImage.height = cursor->height;
        cursorImage.pixels = cursor->data;
        GLFWcursor* cursorglfw = glfwCreateCursor(&cursorImage, 0, 0);
        glfwSetCursor(mainWindow_, cursorglfw);
        glfwSetInputMode(mainWindow_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetWindowAspectRatio(mainWindow_, 16, 9);
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(mainWindow_, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);


    }

    static void MouseChecker(GLFWwindow* mainWindow, double xpos, double ypos)
    {
        Engine* mainEngine = (Engine*)glfwGetWindowUserPointer(mainWindow);
        Input* input = mainEngine->GetSystem<Input>();
        input->SetCursorPos(xpos, ypos);
    }

    static void ButtonChecker(GLFWwindow* mainWindow, int button, int action, int)
    {
        Engine* mainEngine = (Engine*)glfwGetWindowUserPointer(mainWindow);
        Input* input = mainEngine->GetSystem<Input>();
        std::vector<Keystate>& buttonList = input->GetAllButtons();
        if (action == GLFW_PRESS)
        {
            buttonList[button] = Keystate::Triggered;
        }
        if (action == GLFW_RELEASE)
        {
            buttonList[button] = Keystate::Released;
        }
    }

    static void KeyChecker(GLFWwindow* mainWindow, int key, int, int action, int)
    {
        Engine* mainEngine = (Engine*)glfwGetWindowUserPointer(mainWindow);
        Input* input = mainEngine->GetSystem<Input>();
        std::vector<Keystate>& keyList = input->GetAllKeys();

        if (action == GLFW_PRESS)
        {
            keyList[key] = Keystate::Triggered;
        }
        if (action == GLFW_RELEASE)
        {
            keyList[key] = Keystate::Released;
        }
    }

    float timer = 0.0f, dtSum = 0.0f;
    int frames = 0;

    void Window::Update(float dt)
    {
        glfwPollEvents();
        if (glfwWindowShouldClose(mainWindow_))
        {
            GetParent()->Stop();
        }


        int newWindowWidth = 0, newWindowHeight = 0;
        glfwGetFramebufferSize(mainWindow_, &newWindowWidth, &newWindowHeight);


        if (newWindowWidth != windowWidth || newWindowHeight != windowHeight)
        {
            windowWidth = newWindowWidth;
            windowHeight = newWindowHeight;
            glViewport(0, 0, windowWidth, windowHeight);
        }

        frames++;
        dtSum += dt;
        
        if (timer == 0 || int(timer + dt) > int(timer))
        {
            float fps = 1.0f / (dtSum / float(frames));
            char buff[64];

            frames = 0;
            dtSum = 0;

            sprintf_s(buff, 64, "%2.2f fps | Ambrose Doppler The Atlas Unlimited", fps);
            glfwSetWindowTitle(mainWindow_, buff);


        }
        timer += dt;
    }

    void Window::Draw() const
    {

    }

    void Window::SetParent(Engine* engine)
    {
      parent_ = engine;
    }

    Engine* Window::GetParent() const
    {
      return parent_;
    }

    int Window::GetWidth()
    {
        return windowWidth;
    }

    int Window::GetHeight()
    {
        return windowHeight;
    }

    Window::~Window()
    {
        glfwDestroyWindow(mainWindow_);
        glfwTerminate();
    }

    std::string_view Window::GetName() const
    {
        return Name();
    }

    std::string_view Window::Name()
    {
      return "Window";
    }

}