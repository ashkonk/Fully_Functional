/*!*****************************************************************************
\par filename: Trace.cpp
\par  authors: Mason Dulay
\par     date: 09/21/2022

\brief
  Code implementation for the Init function of trace
  Copyright � 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "Trace.h"
#include "Window.h"
#include "Input.h"


namespace FFunctional
{
    void Trace::Init()
    {
        // open a file to write to
        traceFile.open("Trace.txt");

        // if the file fails to open, print failed to console
        if (!traceFile.is_open())
        {
            printf("Failed to open trace file\n");
        }
    }
}