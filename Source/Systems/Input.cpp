/*.........................................................................................../*!*****************************************************************************
\par filename: Input.cpp
\par  authors: Ashkon Khalkhali
\par     date: 09/16/2022
\brief
  Cpp file for Input System. Checks key states set by GLFW and sets keys respectively.
  Copyright © 2022 DigiPen (USA) Corporation.

*******************************************************************************/
#include <GLFW/glfw3.h>
#include "Input.h"
#include "Window.h"
#include "Engine.h"

// state machine isah, glfw doesnt give key state.

namespace FFunctional
{

  void Input::Init()
  {
    glfwSetInputMode(GetParent()->GetSystem<Window>()->GetWindow(), GLFW_STICKY_KEYS, GLFW_FALSE);
    AllKeys_.resize(GLFW_KEY_LAST + 1);
    AllButtons_.resize(GLFW_MOUSE_BUTTON_LAST + 1);
    //Engine* mainEngine = GetParent();
    //Window* window = mainEngine->GetSystem<Window>();
    //GLFWwindow* mainWindow = window->GetWindow();

    for (int i = 0; i < GLFW_KEY_LAST + 1; ++i)
    {
      AllKeys_[i] = Keystate::Up;
    }
    for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST + 1; ++i)
    {
      AllButtons_[i] = Keystate::Up;
    }
  }

  void Input::Update(float dt)
  {

    dt;

    for (int i = 0; i < GLFW_KEY_LAST + 1; ++i)
    {
      if (AllKeys_[i] == Keystate::Triggered)
      {
        AllKeys_[i] = Keystate::Down;
      }
      if (AllKeys_[i] == Keystate::Released)
      {
        AllKeys_[i] = Keystate::Up;
      }
    }

    for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST + 1; ++i)
    {
      if (AllButtons_[i] == Keystate::Triggered)
      {
        AllButtons_[i] = Keystate::Down;
      }
      if (AllButtons_[i] == Keystate::Released)
      {
        AllButtons_[i] = Keystate::Up;
      }
    }

  }

  void Input::Exit()
  {

  }

  Keystate Input::GetKeyState(int key) const
  {
    return AllKeys_[key];
  }

  Keystate Input::GetButtonState(int button) const
  {
    return AllButtons_[button];
  }

  void Input::Draw() const
  {

  }

  std::string_view Input::GetName() const
  {
    return Name();
  }

  std::string_view Input::Name()
  {
    return "Input";
  }

  void Input::SetParent(Engine* mainEngine)
  {
    //why are you like this @me
    parent_ = mainEngine;
  }

  Engine* Input::GetParent() const
  {
    return parent_;
  }

  std::vector<Keystate>& Input::GetAllKeys()
  {
    return AllKeys_;
  }

  std::vector<Keystate>& Input::GetAllButtons()
  {
    return AllButtons_;
  }

  void Input::SetCursorPos(double xPos, double yPos)
  {
    cursorXpos = (float)xPos;
    cursorYpos = (float)yPos;
  }

  const glm::vec2 Input::GetCursorPos() {
    glm::vec2 cursorPos = { (float)cursorXpos, (float)cursorYpos };
    return cursorPos;
  }
}