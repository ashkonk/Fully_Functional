/*!*****************************************************************************
\par filename: SceneMain.cpp
\par  authors: Jonathan Meyer
\par     date: 09/24/2022

\brief
    This file contains the implementation of our main scene code.

  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Renderer.h"
#include "Input.h"
#include "GLFW/glfw3.h"
#include "SceneMain.h"
#include <iostream>

namespace FFunctional
{
    void SceneMain::Init()
    {
        /* Load in a sample image to test rendering.  * * * * * * * * * * * * */
        image = Graphics::LoadImage("test.png");
        
        /* initalize the objects' position and velocity.  * * * * * * * * * * */
        position = glm::vec2(0.0f);
        velocity = glm::vec2(0.0f);
    }

    void SceneMain::Update(float dt)
    {
        const float speed = 10.0f;
        const double dampening = 10.0f;
        
        Input *input = (Input*)(GetManager()->GetParent()->GetSystem("Input"));
        if (input->GetKeyState(GLFW_KEY_W) == Keystate::Down)
            velocity.y += speed * dt;
        if (input->GetKeyState(GLFW_KEY_A) == Keystate::Down)
            velocity.x -= speed * dt;
        if (input->GetKeyState(GLFW_KEY_S) == Keystate::Down)
            velocity.y -= speed * dt;
        if (input->GetKeyState(GLFW_KEY_D) == Keystate::Down)
            velocity.x += speed * dt;

        velocity *= glm::pow(glm::euler<double>(), dampening * dt);
        position += velocity * dt;

        if (input->GetKeyState(GLFW_KEY_F1) == Keystate::Down)
        {
          SetNextScene(std::shared_ptr<FFunctional::Scene>(new SceneMain()));
        }
    }

    void SceneMain::Draw() const
    {
        /* Clear the background at the beginning of drawing to the buffer. * * */
        Graphics::ClearBackground(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        
        /* Draw our sample image for testing rendering.  * * * * * * * * * * * */
        Graphics::DrawImage(*image, position);
    }

    void SceneMain::Exit()
    {
        //Graphics::FreeImage(image);
        //!< since entities are Shared Ptrs no cleanup necessary
    }
}