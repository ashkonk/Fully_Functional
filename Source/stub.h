/*!*****************************************************************************
\par filename: stub.h
\par  authors: Jonathan Meyer, Ashkon Khalkhali, Alijah Rosner, Mason Dulay
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
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