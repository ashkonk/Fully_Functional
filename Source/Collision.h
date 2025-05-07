/*!*****************************************************************************
\par filename: Collision.h
\par  authors: Ashkon Khalkhali
\par     date: 10/17/2022

\brief
   Plan: implement collider template that works with any collider type, similar
   to state machine.
*******************************************************************************/
#pragma once


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
    class ExampleComponent //: public Component /* Example Component  * * * * */
    {
    private:
        int someExample = 0;   //!< this is an example of a member value.

    public:
        void Init();           //!< Our ExampleComponent Init Function. 

        void Update(float dt); //!< Our ExampleComponent Update Function.

        void Exit();           //!< Our ExampleComponent Shutdown Function.

    }; /* ExampleComponent */

}; /* Normalloc */