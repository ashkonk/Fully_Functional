 /*!*****************************************************************************
\par filename: ColliderCircle.h
\par  authors: Mason Dulay, Ashkon Khalkhali
\par     date: 09/27/2022

\brief
  Code implementation for ColliderCircle
  Copyright 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "ColliderCircle.h"
#include "ColliderLine.h"
#include "Physics.h"
#include "Collider.h"
#include "Entity.h"
#include "Transform.h"
#include "glm/vector_relational.hpp"

//------------------------------------------------------------------------------

namespace FFunctional {

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

  void ColliderCircle::Read(const rapidjson::Value& value)
  {
    radius_ = value["Radius"].GetFloat();

    SetResolve(true);
    SetStatic(false);

    if (value.HasMember("Resolve"))
    {
      SetResolve(value["Resolve"].GetBool());
    }
    if (value.HasMember("Static"))
    {
      SetStatic(value["Static"].GetBool());
    }
  }

  // Get the circle collider's radius.
  float ColliderCircle::GetRadius() const
  {
    return this->radius_;
  }

  // Set the circle collider's radius.
  void ColliderCircle::SetRadius(float radius)
  {
    this->radius_ = radius;
  }

  // Check for collision between two circle colliders.
  bool ColliderCircle::IsCollidingWithCircle(const ColliderCircle& other, glm::vec3& pos)
  {

    // get and verify both transforms 
    Transform& first = GetParent().ComponentGet<Transform>();
    Transform& second = other.GetParent().ComponentGet<Transform>();


    // get the radius' and add them together to get the distance that means the objects are colliding
    float radius1 = radius_;
    float radius2 = other.radius_;
    float distanceCheck = radius1 + radius2;
    // get vectors so we can see the distance between the two objects using vector2Ddistance
    const glm::vec3* pVec1 = first.GetTranslation();
    const glm::vec3* pVec2 = second.GetTranslation();
    float actualDistance = glm::distance(*pVec1, *pVec2);
    // if the distance between the two points is less than the radius' combined then they are colliding
    if (actualDistance <= distanceCheck)
    {
      glm::vec3 dist = (*pVec1 - *pVec2);
      //dist = glm::normalize(dist)*radius2;
     
      pos = (*pVec2 + glm::normalize(dist) * radius2);
      //pos = (*pVec2 - *pVec1) / 2.0f;
      return true;
    }

    return false;
  }

  bool ColliderCircle::isCollidingWithLine(const Collider& other)
  {
    //Line Data
    const ColliderLine& lineObject = (const ColliderLine&)other;
    const ColliderLineSegment* lineSegments = lineObject.GetSegment();
    int lineCount = lineObject.GetLineCount();

    //Circle Data
    Entity circleObject = GetParent();
    Physics circlePhysics = circleObject.ComponentGet<Physics>();
    Transform circleTransform = circleObject.ComponentGet<Transform>();

    //// start
    const glm::vec3* Bs = circlePhysics.GetOldTranslation();
    //// current
    const glm::vec3* Be = circleTransform.GetTranslation();
    //// intersection
    glm::vec3 Bi = glm::vec3(0);
    //// reflection
    glm::vec3 Br = glm::vec3(0);

    glm::vec3 p0;
    glm::vec3 p1;
    glm::vec3 direction = glm::vec3(0);

    direction.x = Be->x - Bs->x;
    direction.y = Be->y - Bs->y;

    glm::vec3 e = glm::vec3(0);
    glm::vec3 n = glm::vec3(0);

    float a;
    float b;
    float c;
    float ti;

    glm::vec3 tiSubVec = glm::vec3(0);

    glm::vec3 i = glm::vec3(0);
    glm::vec3 s = glm::vec3(0);
    glm::vec3 sDouble = glm::vec3(0);
    glm::vec3 r = glm::vec3(0);


    Transform& t = other.GetParent().ComponentGet<Transform>();

    glm::mat4& mat1 = *t.GetMatrix();

    for (int j = 0; j < lineCount; ++j)
    {
      p0 = lineSegments[j].point[0];
      p1 = lineSegments[j].point[1];
      
      p0 = glm::vec3(mat1 * glm::vec4(p0, 1));
      p1 = glm::vec3(mat1 * glm::vec4(p1, 1));

      e.x = p1.x - p0.x;
      e.y = p1.y - p0.y;
      n.x = e.y;
      n.y = -e.x;
      n = glm::normalize(n);

      if (glm::dot(n, direction) == 0)
      {
        continue;
      }
      if (((glm::dot(*Bs, n) <= glm::dot(n, p0)) && (glm::dot(*Be, n) < glm::dot(n, p0))))
      {
        continue;
      }
      if ((glm::dot(*Bs, n) <= glm::dot(n, p0)) && (glm::dot(*Be, n) > glm::dot(n, p0)))
      {
        continue;
      }
      a = glm::dot(n, p0);
      b = glm::dot(n, *Bs);
      c = glm::dot(n, direction);
      ti = (a - b) / c;

      Bi.x = (direction.x * ti) + Bs->x;
      Bi.y = (direction.y * ti) + Bs->y;
      tiSubVec.x = Bi.x - p0.x;
      tiSubVec.y = Bi.y - p0.y;

      s.x = -e.x;
      s.y = -e.y;

      if (glm::dot(e, tiSubVec) < 0)
      {
        continue;
      }
      //	Vector2DSub(&tiSubVec, &Bi, &p1);
      //	if (Vector2DDotProduct(&s, &tiSubVec) < 0)
      //	{
      //		continue;
      //	}
      tiSubVec.x = Bi.x - p1.x;
      tiSubVec.y = Bi.y - p1.y;
      if (glm::dot(s, tiSubVec) < 0)
      {
        continue;
      }
      return true;
    }
    //return false;
    return false;
  }

  void ColliderCircle::ResolveLineCollision(const Collider& col, glm::vec3)
  {
    Entity circleObject = GetParent();
    const ColliderLine& colLine = static_cast<const ColliderLine&>(col);


    Physics circlePhysics = circleObject.ComponentGet<Physics>();
    Transform circleTransform = circleObject.ComponentGet<Transform>();
    //// start
    const glm::vec3* Bs = circlePhysics.GetOldTranslation();
    //// current
    const glm::vec3* Be = circleTransform.GetTranslation();
    glm::vec3 Bi = glm::vec3(0);
    //// reflection
    glm::vec3 Br = glm::vec3(0);

    //glm::vec3 p0;
    //glm::vec3 p1;
    //Vector2D direction = { 0.0f, 0.0f };
    glm::vec3 direction = *Be - *Bs;

    //Vector2DSub(&direction, &Be, &Bs);
    direction.x = Be->x - Bs->x;
    direction.y = Be->y - Bs->y;

    //Vector2D e = { 0.0f, 0.0f };
    glm::vec3 e = glm::vec3(0);
    //Vector2D n = { 0.0f, 0.0f };
    glm::vec3 n = glm::vec3(0);

    float a;
    float b;
    float c;
    float ti;

    //Vector2D tiSubVec = { 0.0f, 0.0f };
    glm::vec3 tiSubVec = glm::vec3(0);


    glm::vec3 v = *Be - *Bs;
    //Vector2D i = { 0.0f, 0.0f };
    glm::vec3 i = glm::vec3(0);
    //Vector2D s = { 0.0f, 0.0f };
    glm::vec3 s = glm::vec3(0);
    //Vector2D sDouble = { 0.0f, 0.0f };
    glm::vec3 sDouble = glm::vec3(0);
    //Vector2D r = { 0.0f, 0.0f };
    glm::vec3 r = glm::vec3(0);
    //	Vector2DSub(&i, &Be, &Bi);


    Transform& t = colLine.GetParent().ComponentGet<Transform>();

    glm::mat4& mat1 = *t.GetMatrix();

    /*for (int j = 0; j < colLine.GetLineCount(); j++)
    {
        glm::vec3 p0 = colLine.GetSegment()[j].point[0];
        glm::vec3 p1 = colLine.GetSegment()[j].point[1];

        p0 = glm::vec3(mat1 * glm::vec4(p0, 1));
        p1 = glm::vec3(mat1 * glm::vec4(p1, 1));

        e = p1 - p0;
        n = glm::normalize(glm::vec3(e.y*1, e.x*-1, 1));

        if (glm::dot(n, v) == 0)
        {
            continue;
        }


    }*/

    for (int j = 0; j < colLine.GetLineCount(); ++j)
    {
        glm::vec3 p0 = colLine.GetSegment()[j].point[0];
        glm::vec3 p1 = colLine.GetSegment()[j].point[1];

        p0 = glm::vec3(mat1 * glm::vec4(p0, 1));
        p1 = glm::vec3(mat1 * glm::vec4(p1, 1));

        e.x = p1.x - p0.x;
        e.y = p1.y - p0.y;
        n.x = e.y;
        n.y = -e.x;
        n = glm::normalize(n);

        if (glm::dot(n, direction) == 0)
        {
            continue;
        }
        if (((glm::dot(*Bs, n) <= glm::dot(n, p0)) && (glm::dot(*Be, n) < glm::dot(n, p0))))
        {
            continue;
        }
        if ((glm::dot(*Bs, n) <= glm::dot(n, p0)) && (glm::dot(*Be, n) > glm::dot(n, p0)))
        {
            continue;
        }
        a = glm::dot(n, p0);
        b = glm::dot(n, *Bs);
        c = glm::dot(n, direction);
        ti = (a - b) / c;

        Bi.x = (direction.x * ti) + Bs->x;
        Bi.y = (direction.y * ti) + Bs->y;
        tiSubVec.x = Bi.x - p0.x;
        tiSubVec.y = Bi.y - p0.y;

        s.x = -e.x;
        s.y = -e.y;

        if (glm::dot(e, tiSubVec) < 0)
        {
            continue;
        }
        //	Vector2DSub(&tiSubVec, &Bi, &p1);
        //	if (Vector2DDotProduct(&s, &tiSubVec) < 0)
        //	{
        //		continue;
        //	}
        tiSubVec.x = Bi.x - p1.x;
        tiSubVec.y = Bi.y - p1.y;
        if (glm::dot(s, tiSubVec) < 0)
        {
            continue;
        }
        break;
    }


    i.x = Be->x - Bi.x;
    i.y = Be->y - Bi.y;
    //	Vector2DScale(&s, &n, Vector2DDotProduct(&i, &n));
    float scalar = glm::dot(i, n);
    s.x = n.x * scalar;
    s.y = n.y * scalar;
    //	Vector2DScale(&sDouble, &s, 2.0f);
    sDouble.x = s.x * 2.0f;
    sDouble.y = s.y * 2.0f;
    //	Vector2DSub(&r, &i, &sDouble);
    r.x = i.x - sDouble.x;
    r.y = i.y - sDouble.y;
    //	Vector2DAdd(&Br, &Bi, &r);
    Br.x = Bi.x + r.x;
    Br.y = Bi.y + r.y;
    //	TransformSetTranslation(circleTrans, &Br);
    circleTransform.SetTranslation(&Br);

    //	float rotation = Vector2DToAngleRad(&r);
    float rotation = atan2f(r.y, r.x);
    //	Vector2D circleVel = *PhysicsGetVelocity(circlePhys);
    glm::vec3 circleVel = *circlePhysics.GetVelocity();
    //	float speed = Vector2DLength(&circleVel);
    float speed = glm::length(circleVel);

    //	Vector2DNormalize(&r, &r);
    r = glm::normalize(r);
    //	Vector2DScale(&circleVel, &r, speed);
    circleVel.x = r.x * speed;
    circleVel.y = r.y * speed;
    //	PhysicsSetVelocity(circlePhys, &circleVel);
    circlePhysics.SetVelocity(&circleVel);
    //	TransformSetRotation(circleTrans, rotation);
    circleTransform.SetRotation(rotation);
  }

  void ColliderCircle::ResolveCircleCollision(const Collider& circle, glm::vec3 colPos)
  {
    circle;
    Transform& myTransform = GetParent().ComponentGet<Transform>();

    if (&myTransform != NULL)
    {
        glm::vec3 vel = colPos - *myTransform.GetTranslation();
        float scale = glm::length(vel) - GetRadius();
        
        vel = *myTransform.GetTranslation() + glm::normalize(vel)*scale;
        myTransform.SetTranslation(&vel);
    }
  }

  void ColliderCircle::ResolveCollision(const Collider& collider, glm::vec3 colPos)
  {
    if (!GetStatic() && collider.GetResolve())
    {
        if (collider.GetType() == ColliderType::Line)
        {
            ResolveLineCollision(collider, colPos);
        }
        if (collider.GetType() == ColliderType::Circle)
        {
            ResolveCircleCollision(collider, colPos);
        }
    }
  }
}

