/*!**************************************************************************!*\
\par filename: Renderer.h
\par  authors: Jonathan Meyer
\par     date: 09/18/2022

\brief
  This file contains the public declarations of our renderer system.
  Copyright © 2022 DigiPen (USA) Corporation.
\*!**************************************************************************!*/
#pragma once

#include <queue>
#include "Engine.h"
#include "ISystem.h"
#include "Graphics.h"


/*!**************************************************************************!*\
\brief
  Our Perfectly Normal Fully Functional Game Engine Namespace.
\*!**************************************************************************!*/
namespace FFunctional
{


  /*!************************************************************************!*\
  \brief
    Our renderer is an ISystem attached to our ECS engine. attached to our
    lower-level Graphics interface (this class is essentially glue logic).
  \*!************************************************************************!*/
  class Renderer : public ISystem
  {
  private:
    Engine* parent_;            //!< A handle to our systems parent ECS engine.

  public:
    /* ISystem Interface Functions. * * * * * * * * * * * * * * * * * * * * * */
    void Init() override;           //!< Initialize Our ISystem.

    void Update(float dt) override; //!< Update Our ISystem.

    void Draw() const override;     //!< Begin Drawing Our Scenes.

      

    /* Renderer Access Functions. * * * * * * * * * * * * * * * * * * * * * * */
    virtual std::string_view GetName() const override;   //!< ISystem Access.

    static std::string_view Name() {return "Renderer";}  //!< Renderer  Name.

    void SetParent(Engine* mainEngine) override;     //!< Set our parent ECS.

    Engine* GetParent() const override;              //!< Get our parent ECS.



    /* Class Management Functions.  * * * * * * * * * * * * * * * * * * * * * */
    ~Renderer();  //!< Our Renderer cleanup & deconstructor.
  };
};
