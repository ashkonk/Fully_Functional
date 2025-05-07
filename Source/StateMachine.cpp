/*!*****************************************************************************
\par filename: StateMachine.cpp
\par  authors: Mason Dulay
\par     date: 09/21/2022

\brief
  Code implementation for behaviors 
  Copyright 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "StateMachine.h"
#include "Collider.h"

#define UNREFERENCED_PARAMETER(P) P

namespace FFunctional
{
    template <typename T>
    // updates the behavior based on "flags"
    inline void StateMachine<T>::Update(float dt) //!< Our ExampleComponent Update Function.
    {
        if (stateCurr_ != stateNext_)
        {
            OnExit();
            stateCurr_ = stateNext_; 
            OnInit();
        }
        OnUpdate(dt);
    }

    template <typename T>
    T StateMachine<T>::GetCurrState() const
    {
        return stateCurr_;
    }

    template <typename T>
    void StateMachine<T>::SetNextState(T state)
    {
        stateNext_ = state;
    };

}