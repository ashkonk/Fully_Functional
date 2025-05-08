/*!*****************************************************************************
\par filename: Input.cpp
\par  authors: Ashkon Khalkhali
\par     date: 09/16/2022
\brief
  Header file for Input System. Checks key states set by GLFW and sets keys respectively.
  Contains a vector list of all keys and their current state.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include "glm/vec2.hpp"
#include "ISystem.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{
  /*!***************************************************************************
    \brief
      This is an example of a component class.

  *****************************************************************************/
  //make an enum to determine trigger or released,
  enum class Keystate {

    Triggered = 0,
    Released,
    Up,
    Down
  };

  class Input : public ISystem
  {
  public:
      void Init();           //!< Our ExampleComponent Initialization Function. 

      void Update(float dt); //!< Handle the state of keys

      void Exit();           //!< Our ExampleComponent Shutdown Function.

      Keystate GetKeyState(int key) const;

      Keystate GetButtonState(int button) const;

      virtual void Draw() const override;

      virtual std::string_view GetName() const override;

      static std::string_view Name();

      virtual void SetParent(Engine* mainEngine) override;

      virtual Engine* GetParent() const override;
      
      std::vector<Keystate>& GetAllKeys();
      std::vector<Keystate>& GetAllButtons();

      void SetCursorPos(double xPos, double yPos);

      const glm::vec2 GetCursorPos();

  private:
    Engine* parent_;

    std::vector<Keystate> AllKeys_;
    std::vector<Keystate> AllButtons_;

    float cursorXpos;
    float cursorYpos;

  }; /* ExampleComponent */

}; /* Normalloc */