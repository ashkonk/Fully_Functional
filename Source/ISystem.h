/*!*****************************************************************************
\par filename: ISystem.h
\par  authors: Ashkon Khalkhali
\par     date: 09/11/2022

\brief
  Virtual struct containing all system functions
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include <string_view>

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Fully Functional Game Engine * * * */
{
  class Engine;
  /*!***************************************************************************
    \brief
      This is the Systems Virtual Interface
  *****************************************************************************/
  class ISystem // 
  {
  public:

      virtual void Init() = 0; //!< Init system

      virtual void Update(float dt) = 0; //!< Update system

      virtual void Draw() const = 0;  //!< Draw system 
      
      virtual std::string_view GetName() const = 0;  //!< Get system name

      virtual void SetParent(Engine* mainEngine) = 0;

      virtual Engine* GetParent() const = 0;

      virtual ~ISystem() {};

  }; /* System base struct */

};