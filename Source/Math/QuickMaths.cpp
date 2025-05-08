/*!*****************************************************************************
\par filename: QuickMaths.cpp
\par  authors: Alijah Rosner
\par     date: 09/09/2022

\brief
  Provides some functions for Vec2s. 
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include "stub.h"
#include "QuickMaths.h"

namespace FFunctional
{
    void Vector2DScaleAdd(glm::vec3* pResult, const glm::vec3* pVec0, const glm::vec3* pVec1, float scale)
    {
        pResult->x = (pVec0->x * scale) + pVec1->x; 
        pResult->y = (pVec0->y * scale) + pVec1->y; 
    }

    void Vector2DScaleSub(glm::vec3* pResult, const glm::vec3* pVec0, const glm::vec3* pVec1, float scale)
    {
        pResult->x = (pVec0->x * scale) - pVec1->x;
        pResult->y = (pVec0->y * scale) - pVec1->y; 
    }

    void VectorSet(glm::vec3* pResult, float x, float y)
    {
        pResult->x = x; 
        pResult->y = y; 
    }
    float Vector2DToAngleRad(const glm::vec3* pVec)
    {
      float x = pVec->x;
      float y = pVec->y;

      float angle = atan2f(y, x);
      return angle;
    }
}