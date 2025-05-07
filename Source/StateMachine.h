/*!*****************************************************************************
\par filename: StateMachine.h
\par  authors: Mason Dulay
\par     date: 09/21/2022

\brief
  This is the declarations for the behavior header 
  Copyright 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include <istream>
#include "Component.h"
#include "Entity.h"
#include "Stream.h"
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{

  struct IStateMachine
  {
    virtual void OnInit() = 0;
    virtual void OnUpdate(float dt) = 0;
    virtual void OnExit() = 0;
    virtual void Read(const rapidjson::Value& value) = 0;

  };

    /*!***************************************************************************
      \brief
        This is an example of a component class.
    *****************************************************************************/
    template <typename T>
    class StateMachine : public IStateMachine /* Example Component  * * * * * * */
    {
    public:
        void Update(float dt); //!< Our ExampleComponent Update Function.

        virtual void OnInit() = 0;

        virtual void OnUpdate(float dt) = 0;

        virtual void OnExit() = 0;

        virtual void Read(const rapidjson::Value& value) = 0;
        
        T GetCurrState() const;

        void SetNextState(T state);
    
    private:
        T stateNext_{-1}; //!< next state of behavior
        T stateCurr_{-1}; //!< current state of behavior
    }; /* ExampleComponent */

}; /* Normalloc */

#include "StateMachine.cpp"