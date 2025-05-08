/*!*****************************************************************************
\par filename: Trace.h
\par  authors: Mason Dulay
\par     date: 09/21/2022

\brief
  This is the declaration and implementations for Trace
  Copyright � 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include <iostream>
#include <fstream>

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{
  /*!***************************************************************************
    \brief
      This is an example of a component class.
  *****************************************************************************/

  class Trace  //: public ISystem
  {
    private:
        std::ofstream traceFile; //!< the Trace file stream

    public:
      void Init();           //!< Initialize file

      // templatized Message for Trace
      template <typename... Types>
      void Message(Types... vars)
      {
          if (traceFile.is_open())
          {
              (traceFile <<  ... << vars) << std::endl;
          }
      }
      //!< Function to print to file

  }; /* ExampleComponent */

}; /* Normalloc */