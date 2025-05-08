/*!*****************************************************************************
\par filename: Collider.cpp
\par  authors: Jonathan Meyer
\par     date: 11/14/2022

\brief
  Code implementation for Collider
  Copyright 2022 DigiPen (USA) Corporation.
*******************************************************************************/

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "Transform.h"
#include "Physics.h"
#include "Collider.h"
#include "Manager.h"
#include "Scene.h"
#include "AttendantA.h"

/* the types of colliders we want to be able to create. * * * * * * * * * * * */
#include "StateMachine.h"
#include <utility>
#include "iostream"
#include "Graphics.h"
#include <algorithm>
#include "Behavior.h"
#include "Trace.h"

//------------------------------------------------------------------------------


namespace FFunctional
{
  std::pair<bool, glm::vec2> LineCollisionDetect(const glm::vec2& a_0,
  const glm::vec2& b_0, const glm::vec2& a_1, const glm::vec2& b_1)
  {
    /* get the a normal vector from each line.  * * * * * * * * * * * * * * * */
    glm::vec2 norm_0 = b_0 - a_0, norm_1 = b_1 - a_1;
    norm_0 = glm::normalize(glm::vec2(norm_0.y, -norm_0.x));
    norm_1 = glm::normalize(glm::vec2(norm_1.y, -norm_1.x));

    /* perform a SAT-projection to find any intersections.  * * * * * * * * * */
    float projA_0 = glm::dot(a_0, norm_1) - glm::dot(a_1, norm_1);
    float projB_0 = glm::dot(b_0, norm_1) - glm::dot(a_1, norm_1);

    /* proj_0 is line 0 projected relative to line 1, proj_1 is vice versa. * */
    float projA_1 = glm::dot(a_1, norm_0) - glm::dot(a_0, norm_0);
    float projB_1 = glm::dot(b_1, norm_0) - glm::dot(a_0, norm_0);

    /* if they intersect at every angle, then they are colliding. * * * * * * */
    bool areColliding = (abs(projA_0) + abs(projB_0) > abs(projA_0 + projB_0));
    areColliding &= (abs(projA_1) + abs(projB_1) > abs(projA_1 + projB_1));

    
    /* store our collision test result in this variable.  * * * * * * * * * * */
    std::pair<bool, glm::vec2> colliderResult(areColliding, glm::vec2(0, 0));

    /* if we are colliding, then where? * * * * * * * * * * * * * * * * * * * */
    
    if (areColliding) [[unlikely]]
    {
      /* finding our point of intersection. * * * * * * * * * * * * * * * * * */
      float u_1 = (glm::dot(a_1, norm_1) - glm::dot(a_0, norm_1));
      u_1      /= (glm::dot(b_0, norm_1) - glm::dot(a_0, norm_1));

      /* set our result to include the point of incident. * * * * * * * * * * */
      colliderResult.second =
      glm::vec2(a_0.x*(1-u_1) + b_0.x*u_1, a_0.y * (1-u_1) + b_0.y*u_1);
    }

    return colliderResult; /* return the result of our collision test.  * * * */
  }

  bool ShapeCollisionDetect(const std::vector<glm::vec2>& width_a,
  const std::vector<glm::vec2>& shape_a, const std::vector<glm::vec2>& shape_b,
  const glm::mat4& mat_0, const glm::mat4& mat_1)
  {
    /* we want to use SAT-proj. using the normal of each line in shape A. * * */
    for (size_t i = 0; i < shape_b.size(); ++i)
    {
      /* we are testing if this vertex is intersecting any lines. * * * * * * */
      glm::vec4 u_1(shape_b.at(i), 0.0f, 1.0f);
      const glm::vec2 a_1 = glm::vec2(mat_1 * u_1);
      bool isColliding = true; /* assume this point intersects. * * * * * * * */

      /* we want to test intersection of every vertex in the other shape. * * */
      for (size_t j = 0; j < shape_a.size(); ++j)
      {
        /* we want the points a_0, b_0 from each line in the shape. * * * * * */
        glm::vec4 u_0(shape_a.at(j), 0.0f, 1.0f);
        glm::vec4 v_0(shape_a.at((j + 1) % shape_a.size()), 0.0f, 1.0f);
        const glm::vec2 a_0 = glm::vec2(mat_0 * u_0);
        const glm::vec2 b_0 = glm::vec2(mat_0 * v_0);

        /* we want to get the normal of this side/line-segment. * * * * * * * */
        glm::vec2 norm_0 = glm::normalize(glm::vec2(b_0.y-a_0.y, a_0.x-b_0.x));
        glm::vec2 bounds(width_a.at(j)); /* bounds on left and right. * * * * */

        /* this is our projection of our vertice relative to the normal.  * * */
        float projA_1 = glm::dot(a_1, norm_0) - glm::dot(a_0, norm_0);

        /* if they intersect at every angle, then they are colliding. * * * * */
        isColliding &= (projA_1 >= bounds.x && projA_1 <= bounds.y);

        if (!isColliding)
          break;
      }

      if (isColliding)
        return isColliding;
    }

    /* return whether or not the colliders are colliding/intersecting.  * * * */
    return false;
  }






  bool CircleCollisionDetect(float rad_0, float rad_1, glm::vec2& pos_0,
  glm::vec2& pos_1)
  {
    return (glm::distance(pos_0, pos_1) - (rad_0 + rad_1) < 0);
  }







  void Collider::Read(const rapidjson::Value& value)
  {
    enabled_ = true;
    radius = 0.0f;

    if (value.HasMember("Static"))
      static_ = value["Static"].GetBool();

    if (value.HasMember("Enabled"))
      enabled_ = value["Enabled"].GetBool();

    if (value.HasMember("Solid"))
      solid_ = value["Solid"].GetBool();

    if (value.HasMember("Offset"))
      JsonConvert(value["Offset"], offset);


    if (value.HasMember("Tag"))
        tag_ = value["Tag"].GetString();

    /* if we specify the radius, we want to set it. * * * * * * * * * * * */
    if (value.HasMember("Radius"))
      radius = value["Radius"].GetFloat();

    if (value.HasMember("Type"))
    {
      std::string type = value["Type"].GetString();

      if (!type.compare("Circle")) /* our collider is a circle. * * * * * * * */
      {
        
        GenerateCircleCol(radius, 16); /* generate our circle collider. * * * */
      }
      if (!type.compare("Box")) /* our collider is a rectangle. * * * * * * * */
      {
        /* if we specify the <w*h>, we want to set it.  * * * * * * * * * * * */
        float width = value.HasMember("Width") ? value["Width"].GetFloat() : 1;
        float hgt = value.HasMember("Height") ? value["Height"].GetFloat() : 1;

        GenerateBoxCol(width, hgt); /* generate our Rectangle collider. * * * */
      }
      if (!type.compare("Shape")) /* our collider is a rectangle. * * * * * * * */
      {
        auto mesh = value["Mesh"].GetArray();

        shape.clear();
        for (rapidjson::SizeType k = 0; k < mesh.Size(); k++)
        {
          auto arr = mesh[k].GetArray();
          shape.push_back(glm::vec2(arr[0].GetFloat(), arr[1].GetFloat()));
        }
      }
    }
    glm::mat4 translate = glm::translate(glm::mat4(1), offset);

    for (unsigned i = 0; i < shape.size(); ++i)
      shape.at(i) = translate * glm::vec4(shape.at(i), 0, 1);


    // build SAT width

    float lhs = 0;
    float rhs = 0;


    Transform& lhsTransform = GetParent().ComponentGet<Transform>();
    const glm::mat4& mat_0 = *lhsTransform.GetMatrix();

    /* we want to test intersection of every vertex in the other shape. * * */
    for (size_t j = 0; j < shape.size(); ++j)
    {
      /* we want the points a_0, b_0 from each line in the shape. * * * * * */
      glm::vec4 u_0(shape.at(j), 0.0f, 1.0f);
      glm::vec4 v_0(shape.at((j + 1) % shape.size()), 0.0f, 1.0f);
      const glm::vec2 a_0 = glm::vec2(mat_0 * u_0);
      const glm::vec2 b_0 = glm::vec2(mat_0 * v_0);

      /* we want to get the normal of this side/line-segment. * * * * * * * */
      glm::vec2 norm_0 = glm::normalize(glm::vec2(b_0.y - a_0.y, a_0.x - b_0.x));

      for (unsigned k = 0; k < shape.size(); ++k)
      {
        /* we want the points a_0, b_0 from each line in the shape. * * * * */
        glm::vec4 v_2(shape.at(k), 0.0f, 1.0f);
        const glm::vec2 a_2 = glm::vec2(mat_0 * v_2);

        /* this is our projection of our vertice relative to the normal.  * */
        float projV_0 = glm::dot(a_2, norm_0) - glm::dot(a_0, norm_0);

        /* assign our bounds the the vertex if its farther away.  * * * * * */

        rhs = (projV_0 >= rhs || k == 0) ? projV_0 : rhs;
        lhs = (projV_0 <= lhs || k == 0) ? projV_0 : lhs;
      }
      belts.push_back(glm::vec2(lhs, rhs));
    }


    for (unsigned i = 0; i < shape.size(); ++i)
      if (radius < glm::length(shape.at(i)))
        radius = glm::length(shape.at(i));
  }

  Collider::~Collider()
  {

  }

  bool Collider::IsStatic() const
  {
    return static_;
  }

  bool Collider::IsEnabled() const
  {
    return enabled_;
  }

  bool Collider::IsSolid() const
  {
    return solid_;
  }

  void Collider::SetStatic(bool staticC)
  {
    static_ = staticC;
  }

  void Collider::SetEnabled(bool enabled)
  {
    enabled_ = enabled;
  }

  void Collider::SetSolid(bool solid)
  {
    solid_ = solid;
  }

  bool Collider::GetCheck()
  {
      return colliding_;
  }

  std::string Collider::GetTag()
  {
      return tag_;
  }

  glm::vec2 Collider::GetOffset()
  {
    return offset;
  }


  bool Collider::CheckCollision(Collider& rhs, bool callSignal)
  {

    if (!enabled_ || !rhs.IsEnabled())
      return false;

    Transform& lhsTransform = GetParent().ComponentGet<Transform>();
    Transform& rhsTransform = rhs.GetParent().ComponentGet<Transform>();

    if ((&lhsTransform) && (&rhsTransform))
    {
      glm::vec2 lhsPos = glm::vec2(*lhsTransform.GetTranslation());
      glm::vec2 rhsPos = glm::vec2(*lhsTransform.GetTranslation());

      glm::vec2 lhsScl = glm::vec2(*lhsTransform.GetScale());
      glm::vec2 rhsScl = glm::vec2(*lhsTransform.GetScale());

      bool check =
      CircleCollisionDetect(radius*(glm::length(lhsScl)/sqrtf(2)), rhs.radius*(glm::length(rhsScl)/sqrtf(2)), lhsPos, rhsPos);
      
      if (check)
      {
        const glm::mat4& lhsMat = *lhsTransform.GetMatrix();
        const glm::mat4& rhsMat = *rhsTransform.GetMatrix();

        check  = ShapeCollisionDetect(belts, shape, rhs.shape, lhsMat, rhsMat);
        check |= ShapeCollisionDetect(rhs.belts, rhs.shape, shape, rhsMat, lhsMat);
        
      }

      if (callSignal) [[likely]]
      {
        CollisionSignal(check, rhs);
        rhs.CollisionSignal(check, *this);
      }
      colliding_ = check; 
      return check;
    }
    colliding_ = false; 
    return false;
  }

  void Collider::CollisionSignal(bool check, const Collider& rhs)
  {
    Transform& lhsTransform = GetParent().ComponentGet<Transform>();

    /* update our collision checking table. * * * * * * * * * * * * * * * * */
    bool lastCheck = false;
    if (collisions.find(rhs.GetParent().GetEntity()) != collisions.end())
    {
      lastCheck = collisions.find(rhs.GetParent().GetEntity())->second;
      if (!check)
        collisions.erase(rhs.GetParent().GetEntity());
    }

    if (check)
    {
      collisions[rhs.GetParent().GetEntity()] = check;

      if (!static_ && rhs.IsSolid())
      {
        /* worlds' worst collision resolution.  * * * * * * * * * * * * * * * */
        //Getting physics that doesn't exist
        Physics& lhsPhysics = GetParent().ComponentGet<Physics>();
        lhsTransform.SetTranslation(lhsPhysics.GetOldTranslation());
      }
    }
    //because we have an ECS, behaviors move around in memory. Can't hold behaviors.
    if (lastCheck != check)
    {
      /* collision state has changed. * * * * * * * * * * * * * * * * * * * */
      if (check)
      {
        std::vector<Signal> signals = GetParent().GetSignals("EnterCollider");
        for (int i = 0; i < signals.size(); ++i)
        {
          Entity testEnt = Entity(signals.at(i).parentEcs, signals.at(i).parent);
          void* comp = testEnt.ComponentGetByName(signals.at(i).componentName);
          if (comp)
          {
            //engine_->GetTrace().Message(signals.at(i).componentName, "has entered a collision.");
            ((IBehavior*)comp)->EnterCollision2D(*this, rhs);
          }
        }
      }
      else
      {
        std::vector<Signal> signals = GetParent().GetSignals("ExitCollider");
        for (int i = 0; i < signals.size(); ++i)
        {
          Entity testEnt = Entity(signals.at(i).parentEcs, signals.at(i).parent);
          void* comp = testEnt.ComponentGetByName(signals.at(i).componentName);
          if (comp)
          {
            //engine_->GetTrace().Message(signals.at(i).componentName, "has exited a collision.");
            ((IBehavior*)comp)->ExitCollision2D(*this, rhs);
          }
        }
      }
    }
  }

  //Update checks every collider against every other atm...
  void Collider::Update(float dt)
  {
    dt;

    /* if we cant even get access to the engine we might as well leave. * * * */
    if (!engine_ || !shape.size())
      return;

    /* get our scene manager so we can iterate through scenes.  * * * * * * * */
    Manager<Scene>* sceneManager;
    sceneManager = (Manager<Scene>*)engine_->GetSystem("SceneManager");

    /* if we cant even get the scene manager we might as well leave.  * * * * */
    if (!sceneManager)
      return;

    unsigned sceneIndex = 0; /* our scene by index. * * * * * * * * * * * * * */
    
    /* we will iterater and search for our scene. * * * * * * * * * * * * * * */
    for (unsigned i = 0; i < sceneManager->Size(); ++i)
    {
      std::shared_ptr<Scene> currentScene = sceneManager->At(i);
      const ECS& ecs = currentScene->getECS();
      if (ecs.HasManager<Collider>())
      {
        ComponentManagerPtr<Collider> colliders = ecs.GetManager<Collider>();
          if (colliders && colliders->Has(GetParent().GetEntity()))
          {
            sceneIndex = i;
            break;
          }

      }
      /* ignore any scenes without our collider.  * * * * * * * * * * * * */
    }

    /* we found our scene we want to find collisions in.  * * * * * * * * */
    std::shared_ptr<Scene> currentScene = sceneManager->At(sceneIndex);
    const ECS& ecs = currentScene->getECS();
    if (ecs.HasManager<Collider>())
    {
        ComponentManagerPtr<Collider> colliders = ecs.GetManager<Collider>();

        /* we only need to compare against colliders ordered after ours.  */
        size_t startIndex = 1;
        startIndex += colliders->GetComponentIndex(GetParent().GetEntity());

        for (unsigned i = (unsigned)startIndex; i < colliders->GetSize(); ++i)
        {
          /* we've finally made it, time to do collision checks.  * * * * * * * * */
          if (((*colliders)[i]).GetParent().GetEntity() != GetParent().GetEntity())
            CheckCollision((*colliders)[i]);
        }
    }
  }

  void Collider::GenerateCircleCol(float radius_, unsigned segments)
  {
    shape.clear();
    for (float f = 0.0f; f < 1.0f; f += 1.0f / float(segments))
    {
      glm::vec2 vertex(0, 0);
      vertex.x = radius_ * cosf(f * glm::two_pi<float>());
      vertex.y = radius_ * sinf(f * glm::two_pi<float>());

      shape.push_back(vertex);
    }
  }

  void Collider::GenerateBoxCol(float width, float height)
  {
    shape.clear();

    shape.push_back(glm::vec2(-width / 2, -height / 2));
    shape.push_back(glm::vec2(-width / 2,  height / 2));
    shape.push_back(glm::vec2( width / 2,  height / 2));
    shape.push_back(glm::vec2( width / 2, -height / 2));
  }

  void Collider::Draw() const
  {
    Transform& lhsTransform = GetParent().ComponentGet<Transform>();

    if ((&lhsTransform) && enabled_)
      Graphics::DrawShape(shape, *lhsTransform.GetMatrix());
  }
}

