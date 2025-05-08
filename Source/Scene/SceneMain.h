/*!*****************************************************************************
\par filename: SceneMain.h
\par  authors: Jonathan Meyer
\par     date: 09/24/2022

\brief
    This file contains the public declaration of our main scene code.

  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "Scene.h"
#include <memory>

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{
    using EntityPtr = std::shared_ptr<Entity>;
    /*!***************************************************************************
      \brief
        This is our Main Scene class, a container for our main scene.
    *****************************************************************************/
    class SceneMain : public Scene
    {
      private:
        std::shared_ptr<Image> image;
        glm::vec2 position;
        glm::vec2 velocity;

      public:
        void Init();           //!< Our ExampleComponent Initialization Function. 

        void Update(float dt); //!< Our ExampleComponent Update Function.
        
        void Draw() const;
        
        void Exit();           //!< Our ExampleComponent Shutdown Function.
    }; /* ExampleComponent */

}; /* Normalloc */