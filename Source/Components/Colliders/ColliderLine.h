/*!*****************************************************************************
\par filename: ColliderLine.h
\par  authors: Mason Dulay, Ashkon Khalkhali
\par     date: 09/27/2022

\brief
  Code declaration for ColliderLine
  Copyright 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "Component.h"
#include "Collider.h"
#include "ColliderCircle.h" 
#include "glm/vec3.hpp"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
  /*!*************************************************************************
    \brief
      This is an example of a component class.
  ***************************************************************************/
  typedef struct ColliderLineSegment
  {
    // A single line segment (P0 and P1).
    glm::vec3	point[2];
  } ColliderLineSegment;

  class ColliderLine : public Collider /* Example Component  * * * * */
  {
  private:


    // The number of line segments in the list.
    unsigned int		lineCount_;

    // The individual line segments.
    // (NOTE: Make sure to allocate enough memory for all line segments!)
    ColliderLineSegment	lineSegments_[20];

    //TODO: MAKE SEGMENT COUNT DERIVED FROM JSON

  public:

    ColliderLine(Engine* engine, Entity parent_) : Collider(engine, parent_, ColliderType::Line), 
      lineCount_(0), lineSegments_() {}

    inline int GetLineCount() const { return (int)lineCount_; }

    inline const ColliderLineSegment* GetSegment() const { return lineSegments_; }

    // read in properties of a ColliderLine from a json file
    void Read(const rapidjson::Value&);

    // Add a line segment to the line collider's line segment list.
    void AddLineSegment(const glm::vec3 p0, const glm::vec3 p1);

    // Check for collision between a line collider and a circle collider.
    bool IsCollidingWithCircle(ColliderCircle& circle, glm::vec3& colPos);

    static std::string_view Name() { return "ColliderLine"; }

    // Inherited via Collider
    virtual void ResolveCollision(const Collider& collider, glm::vec3 vec) override;

  }; /* ExampleComponent */

}; /* Normalloc */

//------------------------------------------------------------------------------

