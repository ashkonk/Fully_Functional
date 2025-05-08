/*!*****************************************************************************
\par filename: Button.h
\par  authors: Mason Dulay
\par     date: 09/09/2022

\brief
  This is the Button file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "Sprite.h"
#include "Input.h"
#include "Window.h"
#include "Behavior.h"
#include "Transform.h"
#include "SceneLoader.h"
#include "Sound.h"
#include <memory>

enum ButtonFunction
{
    NoFunction,
    Quit,
    Pause,
    Play,
    Retry,
    Resume,
    Delete,
    RealQuit,
    CancelQuit,
    VolumeUp,
    VolumeDown,
    Credits,
    Volume,
    Controls,
    Windowed,
    Fullscreen,
    Mute,
    Replay
};

enum Level
{
    Level1,
    Level2,
    Level3,
    Level4,
    Level5,
    Level6
};

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    /*!*************************************************************************
      \brief
        Button class that has access to input, window, and engine as a behavior
    ***************************************************************************/
    class Button : public Behavior<Button> /* Example Component  * * * * */
    {
    private:
        Input* input_;
        Engine* engine_{ nullptr };
        GLFWwindow* window_;
        glm::vec4 baseUV_;
        glm::vec4 vecOffset_;
        int function_;
        std::string sceneDir_;
        bool CursorInside();
        void ChangeScene();
        float offset_;
        Scene* buttonScene;
        bool static_;
        std::string sceneHolder_;
        bool VolumeOpen_;
        std::shared_ptr<Sound> sound_;
        std::shared_ptr<Sound> menuMusic_;

    public:
        Button(Engine* engine, Entity parent, Scene* scene) : Behavior(parent), engine_(engine),
            function_(NoFunction), baseUV_(0), vecOffset_(0), offset_(0), buttonScene(scene),
            static_(false), VolumeOpen_(false)
        {
            input_ = engine->GetSystem<Input>();
            window_ = engine->GetSystem<Window>()->GetWindow();
        }

        void Update(float dt);

        void Read(const rapidjson::Value& value);

        static std::string_view Name();

        void SetSceneDir(std::string Dir);

    }; /* ExampleComponent */

}; /* Normalloc */