/*!*****************************************************************************
\par filename: QuickMaths.h
\par  authors: Alijah Rosner
\par     date: 09/09/2022

\brief
  Allows for Vec2 usage. 
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include <glm/matrix.hpp>

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{
    void Vector2DScaleAdd(glm::vec3* pResult, const glm::vec3* pVec0, const glm::vec3* pVec1, float scale);
    void Vector2DScaleSub(glm::vec3* pResult, const glm::vec3* pVec0, const glm::vec3* pVec1, float scale);
    void VectorSet(glm::vec3* pResult, float x, float y);
    float Vector2DToAngleRad(const glm::vec3* pVec);

}; /* Normalloc */