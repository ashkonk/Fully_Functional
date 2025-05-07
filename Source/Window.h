/*!*****************************************************************************
\par filename: Window.h
\par  authors: Ashkon Khalkhali
\par     date: 09/11/2022

\brief
  This is the Window class.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once 

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "ISystem.h"
#include "Trace.h"


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
  class Window : public ISystem  /* Window System  * * * * * * */
  {
    private:
      GLFWwindow* mainWindow_; //!< Our Window Variable.
      Engine* parent_;

      int windowWidth;
      int windowHeight;
    public:

      GLFWwindow* GetWindow();

      void Init() override;           //!< Our Window Initialization Function. 

      void Update(float dt) override; //!< Our Window Update Function.

      void Draw() const override;

      void SetParent(Engine* engine) override;

      Engine* GetParent() const override;

      int GetWidth();

      int GetHeight();

      ~Window(); //!< Our Window Destructor.
      // Inherited via ISystem
      virtual std::string_view GetName() const override;

      static std::string_view Name();
  }; /* Window */

}; /* FFunctional */