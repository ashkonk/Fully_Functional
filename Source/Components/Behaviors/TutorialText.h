/*!*****************************************************************************
\par filename: TutorialText.h
\par  authors: Mason Dulay
\par     date: 09/09/2022

\brief
  This is the TutorialText file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "Behavior.h"
#include "Sound.h"


/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    /*!*************************************************************************
      \brief
        This is an example of a component class.
    ***************************************************************************/

    class TutorialText : public Behavior<TutorialText> /* Example Component  * * * * */
    {
    private:
        Engine* engine_;
        bool dissolving_; 
        bool loadingIn_; 
        bool canResolve_;

        bool eventBool = false;
        float distThresh = 0.5f;
        float cutsceneDur_ = 0.0f;
        float durationCap_ = 2.0f; 

        std::shared_ptr<Sound> sound_;

    public:

        TutorialText(Engine* engine, Entity parent) : engine_(engine), Behavior(parent), dissolving_(false), loadingIn_(false), canResolve_(true) {}

        void EnterCollision2D(const Collider& self, const Collider& other);
        void ExitCollision2D(const Collider& self, const Collider& other);

        void Update(float dt); //!< Our ExampleComponent Update Function.
        
        void Read(const rapidjson::Value& value);

        static std::string_view Name() { return "TutorialText"; };

    }; /* ExampleComponent */

}; /* Normalloc */