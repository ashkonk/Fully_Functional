/*!**************************************************************************!*\
\par filename: Renderer.cpp
\par  authors: Jonathan Meyer
\par     date: 09/18/2022

\brief
  This file contains the implimentation for our renderer system.
  Copyright © 2022 DigiPen (USA) Corporation.
\*!**************************************************************************!*/
#include <glm/gtc/matrix_transform.hpp> /* matrix transform math include. * * */
#include               <glm/matrix.hpp> /* matrix datatype  math include. * * */
#include                     <iostream> /* cout   printing  stdl include. * * */

#include        "Renderer.h" /* Our Renderer  ISystem   public declaration. * */
#include          "Window.h" /* Our Window    ISystem   public declaration. * */
#include           "Input.h" /* Our Input     ISystem   public declaration. * */
#include           "Scene.h" /* Our Scene ECS Container public declaration. * */

#include          "Sprite.h" /* Our Sprite    Component public declaration. * */
#include       "Transform.h" /* Our Transform Component public declaration. * */
#include        "Collider.h" /* Our Collider  Component public declaration. * */
#include          "Camera.h" /* Our Camera    Component public declaration. * */
#include "FootstepEmitter.h" /* Our Emmiter   Component public declaration. * */
#include "ParticleEmitter.h"
#include       "Detection.h" /* Our Detection Component public declaration. * */
#include          "Button.h" /* Our Button UI Component public declaration. * */
#include  "FadeController.h"

#define NOOP() ((void)(0))   /* Dumb Macro. * * * * * * * * * * * * * * * * * */



/*!**************************************************************************!*\
\brief
  Our Perfectly Normal Fully Functional Game Engine Namespace.
\*!**************************************************************************!*/
namespace FFunctional
{
  static bool inputTrigger  = false; /*!< temp  for input  toggle behavior. * */
  static bool collisionMode = false; /*!< Assume debug draw is off on init. * */
  static bool timeDebugMode =  true; /*!< debug time  dilation not visible. * */
  static bool timeDbTrigger = false; /*!< temp for  input toggle  behavior. * */



  /*!************************************************************************!*\
  \brief
    Initialize our Renderer ISystem class to get ready to draw to the screen.

  \return
    This function does not return anything.
  \*!************************************************************************!*/
  void Renderer::Init()
  {
      Graphics::Init(); /* Graphics needs to initialize before anything else. */
      collisionMode = false, inputTrigger = false; /* init resets debug draw. */
  }



  /*!************************************************************************!*\
  \brief
    Update our Renderer and flush our new buffer to the screen.

  \param dt
    the duration of time that has passed since our last update call.

  \return
    This function does not return anything.
  \*!************************************************************************!*/
  void Renderer::Update(float dt)
  {
    Window* window(parent_->GetSystem<Window>()); /* update window w/ handle. */

    /* flush our newly drawn buffer to the screen.  * * * * * * * * * * * * * */
    (window)? /*then*/ glfwSwapBuffers(window->GetWindow()): /*else*/ (NOOP());    
    Graphics::Update(dt); /* Call our backend interface to update it. * * * * */
  }



  /*!************************************************************************!*\
  \brief
    Begin doing draw calls for every visible entity in our active scenes.

  \return
    This function does not return anything.
  \*!************************************************************************!*/
  void Renderer::Draw() const
  {
    /* get a handle to our ECS engine so that we can access its contents. * * */
    const Engine* engine = GetParent();

    if (!engine) /* if our handle isn't valid, return early.  * * * * * * * * */
    {
      /* Flush all of the graphics to the screen. * * * * * * * * * * * * * * */
      Graphics::Flush();
      return;
    }

    /* get a handle to our scene manager so that we can access our scenes.  * */
    Manager<Scene>* scenes = (Manager<Scene>*)engine->GetSystem("SceneManager");

    if (!scenes) /* if our handle isn't valid, return early.  * * * * * * * * */
    {
      /* Flush all of the graphics to the screen. * * * * * * * * * * * * * * */
      Graphics::Flush();
      return;
    }

    Input* input = parent_->GetSystem<Input>();

    if (input->GetKeyState(GLFW_KEY_MINUS) == Keystate::Triggered)
        collisionMode = !collisionMode;

    //inputTrigger = (input->GetKeyState(GLFW_KEY_MINUS) == Keystate::Up);

    if (input->GetKeyState(GLFW_KEY_O) == Keystate::Triggered)
        timeDebugMode = !timeDebugMode;

    /* iterate through all of our active scenes in the scen manager.  * * * * */
    for (unsigned i = 0; i < scenes->Size(); ++i)
    {
      /* we cant have it deallocate while drawing our scene.  * * * * * * * * */
      std::shared_ptr<Scene> currentScene = scenes->At(i);

      /* get a reference to the ECS container for our current scene.  * * * * */
      const ECS& ecs = currentScene->getECS();

      /* get access to our list of cameras in the scene.  * * * * * * * * * * */
      ComponentManagerPtr<Camera> cameraMgr = ecs.GetManager<Camera>();
      
      /* we need to get a handle for the best camera we can find. * * * * * * */
      Camera* targetCam = NULL;

      /* if there are any camera in our scene, tell our graphics interface. * */
      if (cameraMgr)
      {
        /* find the camera with the highest priority. * * * * * * * * * * * * */
        for (unsigned j = 0; j < cameraMgr->GetSize(); j++)
          if (targetCam == NULL || (*cameraMgr)[j] < *targetCam)
            targetCam = &(*cameraMgr)[j];
      }

      if (targetCam) /* if we have found a camera to reference. * * * * * * * */
      {
        /* Get access to the transform attached to our Camera entity. * * * * */
        Transform& transform = targetCam->GetParent().ComponentGet<Transform>();
        Graphics::SetCameraTransform(transform);
      }
      else /* otherwise we set camera transform to a default matrix.  * * * * */
        Graphics::ResetCameraTransform();

      bool gameScene = true;

      /* get access to our list of cameras in the scene.  * * * * * * * * * * */
      ComponentManagerPtr<Button> buttonMgr = ecs.GetManager<Button>();

      /* if there are any camera in our scene, tell our graphics interface. * */
      if (buttonMgr)
        gameScene = !buttonMgr->GetSize();

      /* Access all of the sprite components in our Scene.  * * * * * * * * * */
      ComponentManagerPtr<Sprite> spriteMgr = ecs.GetManager<Sprite>();
      
      if (spriteMgr)
      {
        std::priority_queue<Drawable> zOrderedList;

        for (unsigned j = 0; j < spriteMgr->GetSize(); j++)
            zOrderedList.push(Drawable(j, (*spriteMgr)[j].GetBottomAnchor()));

        for (unsigned j = 0; j < spriteMgr->GetSize(); j++)
        {

          const Sprite& comp = (*spriteMgr)[zOrderedList.top().GetIndex()];
          auto& spriteRef = *spriteMgr;
          //sorryjon
          if (spriteRef[j].GetActive() == true)
          {
            zOrderedList.pop();
            

            


            Entity parentEntity = comp.GetParent();

            if (parentEntity.Has<FadeControllerBehavior>())
              continue;

            Transform& transform = (parentEntity).ComponentGet<Transform>();


            const glm::mat4 mat = *transform.GetMatrix();

            // get the UVs for our texture.
            glm::vec4 uvs = comp.GetUVs();

            if (comp.GetText() == "")
            {
              if (comp.GetOverlay())
              {
                Graphics::DrawImageTransformUVs_MIX_OVERLAY(
                  *comp.GetImage().get(), mat, uvs, comp.GetOpacity(),
                  comp.GetStatic()
                );
              }
              else
              {
                Graphics::DrawImageTransformUVs(
                  *comp.GetImage().get(), mat, uvs, comp.GetOpacity(),
                  comp.GetStatic()
                );
              }
            }
            else
            {
                //write the text
                Transform trans = comp.GetParent().ComponentGet<Transform>();
                // get the matrix of transform
                glm::mat4 matrix = *trans.GetMatrix();
                // create offset matrix
                glm::mat4 offset(1);
                // generate translation matrix from transformscale->x and 0.0f into offset matrix
                offset = glm::translate(offset, glm::vec3(trans.GetScale()->x, 0.0f, 0.0f));
                // iterate through buffer
                std::string localString = comp.GetText();
                const char* localText = localString.c_str();
                while (*localText)
                {
                    // convert letter to uvs and draw
                    unsigned int frameCount = (*localText) - ' ';
                    glm::vec4 temp = comp.GetTextUVs(frameCount);
                    Graphics::DrawImageTransformUVs(*comp.GetImage().get(), matrix, temp, comp.GetOpacity(), comp.GetStatic());
                    
                    // concat matrix and offset
                    matrix = offset * matrix;
                    
                    // increment localText
                    ++localText;
                }
            }
          }
        }
      }

      
      
      //timeDbTrigger = input->GetKeyState(GLFW_KEY_O) == Keystate::Up;




      ComponentManagerPtr<Collider> collMgr = ecs.GetManager<Collider>();

      if (collMgr && collisionMode)
        for (unsigned j = 0; j < collMgr->GetSize(); j++)
          (*collMgr)[j].Draw();


      // footstep manager
      ComponentManagerPtr<FootstepEmitter> stepMgr = ecs.GetManager<FootstepEmitter>();
      if (stepMgr)
      {
          for (unsigned k = 0; k < stepMgr->GetSize(); ++k)
          {
              FootstepEmitter& tempStep = (*stepMgr)[k];
              glm::vec4 uvs = glm::vec4{ 0, 0, 1, 1 }; // default
              std::vector<Particle> tempVec = tempStep.GetPVec();
              for (int j = 0; j < tempVec.size(); ++j)
              {
                  glm::mat4 scale = glm::scale(glm::mat4(1), tempVec.at(j).scale_);
                  glm::mat4 rotation = glm::rotate(glm::mat4(1), tempVec.at(j).rotation_, tempVec.at(j).rotationVec);
                  glm::mat4 translate = glm::translate(glm::mat4(1), tempVec.at(j).translation_);
                  glm::mat4 matrix_ = translate * rotation * scale;
                  Graphics::DrawImageTransformUVs(*tempStep.GetImage().get(), matrix_, uvs, tempVec.at(j).opacity_, false);
              }
          }
      }

      // particle manager
      ComponentManagerPtr<ParticleEmitter> particleMgr = ecs.GetManager<ParticleEmitter>();
      if (particleMgr)
      {
          for (unsigned k = 0; k < particleMgr->GetSize(); ++k)
          {
              ParticleEmitter& tempParticle = (*particleMgr)[k];
              glm::vec4 uvs = glm::vec4{ 0, 0, 1, 1 }; // default
              std::vector<pEmit> tempVec = tempParticle.GetPVec();
              for (int j = 0; j < tempVec.size(); ++j)
              {
                  glm::mat4 scale = glm::scale(glm::mat4(1), tempVec.at(j).scale_);
                  glm::mat4 rotation = glm::rotate(glm::mat4(1), tempVec.at(j).rotation_, tempVec.at(j).rotationVec);
                  glm::mat4 translate = glm::translate(glm::mat4(1), tempVec.at(j).translation_);
                  glm::mat4 matrix_ = translate * rotation * scale;
                  Graphics::DrawImageTransformUVs(*tempParticle.GetImage().get(), matrix_, uvs, tempVec.at(j).opacity_, false);
              }
          }
      }

      // detection symbols manager
      ComponentManagerPtr<Detection> detectMgr = ecs.GetManager<Detection>();
      if (detectMgr)
      {
          for (unsigned k = 0; k < detectMgr->GetSize(); ++k)
          {
              Detection& tempDet = (*detectMgr)[k];
              glm::vec4 uvs = glm::vec4{ 0, 0, 1, 1 }; // default
              glm::mat4 scale = glm::scale(glm::mat4(1), tempDet.GetScale());
              glm::mat4 rotation = glm::rotate(glm::mat4(1), tempDet.getRotation(), tempDet.GetRotationVec());
              glm::mat4 translate = glm::translate(glm::mat4(1), tempDet.GetTranslation());
              glm::mat4 matrix_ = translate * rotation * scale;
              Graphics::DrawImageTransformUVs(*tempDet.GetImage().get(), matrix_, uvs, tempDet.getOpacity(), false);
          }
      }


      //draw time dilation shader
      
      if (timeDebugMode && gameScene)
      {
        Graphics::SetNodes(Component::GetNodes());
        //Graphics::DrawDilationShader(1.0f);

        if (targetCam)
          Graphics::DrawDilationShader(targetCam->GetDilation(false));
        else
          Graphics::DrawDilationShader(1.0f);

        if (ecs.HasManager<FadeControllerBehavior>())
        {
          FadeControllerBehavior& fade = ecs.GetManager<FadeControllerBehavior>()->GetFirstComponent();

          const Sprite& comp = fade.GetParent().ComponentGet<Sprite>();
          Transform& transform = fade.GetParent().ComponentGet<Transform>();

          const glm::mat4 mat = *transform.GetMatrix();

          // get the UVs for our texture.
          glm::vec4 uvs = comp.GetUVs();

          Graphics::DrawImageTransformUVs(
            *comp.GetImage().get(), mat, uvs, comp.GetOpacity(),
            comp.GetStatic()
          );
        }

      }
    }
    Vision::WipeMinDistance();

    /* Flush all of the graphics to the screen.  * * * * * * * * * * * * * */
    Graphics::Flush();
  }



  Renderer::~Renderer()
  {

  }



  void Renderer::SetParent(Engine* engine)
  {
      parent_ = engine;
  }



  Engine* Renderer::GetParent() const
  {
      return parent_;
  }



  std::string_view Renderer::GetName() const
  {
      return Name();
  }
};