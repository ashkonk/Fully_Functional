/*!*****************************************************************************
\par filename: ColliderLine.cpp
\par  authors: Mason Dulay, Ashkon Khalkhali
\par     date: 09/27/2022

\brief
  Code implementation for ColliderLine
  Copyright 2022 DigiPen (USA) Corporation.
	USED FOR VISION CONES
*******************************************************************************/

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "ColliderCircle.h"
#include "ColliderLine.h"
#include "Collider.h"
#include "Entity.h"
#include "ECS.h"
#include "Transform.h"
#include "Physics.h"
#include "glm/vector_relational.hpp"

#define UNREFERENCED_PARAMETER(P) P
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
	namespace FFunctional
	{

		//------------------------------------------------------------------------------
		// Public Variables:
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Read the properties of a ColliderLine component from a file.
		void ColliderLine::Read(const rapidjson::Value& value)
		{
			lineCount_ = 0;

			if (value.HasMember("Resolve"))
			{
				SetResolve(value["Resolve"].GetBool());
			}
			if (value.HasMember("Static"))
			{
				SetStatic(value["Static"].GetBool());
			}
			// reading in line segment? maybe by x and y individually?

			if (value.HasMember("Points"))
			{
				auto points = value["Points"].GetArray();
				lineCount_ = points.Size();

				for (rapidjson::SizeType i = 0; i < 20 && i < points.Size(); i++)
				{
					auto point = points[i].GetArray();
					lineSegments_[i].point[0].x = point[0].GetFloat();
					lineSegments_[i].point[0].y = point[1].GetFloat();
					lineSegments_[i].point[1].x = point[2].GetFloat();
					lineSegments_[i].point[1].y = point[3].GetFloat();
				}
			}
		}

		// Add a line segment to the line collider's line segment list.
		void ColliderLine::AddLineSegment(const glm::vec3 p0, const glm::vec3 p1)
		{
			this->lineSegments_[this->lineCount_].point[0] = p0;
			this->lineSegments_[this->lineCount_].point[1] = p1;
			this->lineCount_++;
		}

		// Check for collision between a line collider and a circle collider.
		bool ColliderLine::IsCollidingWithCircle(ColliderCircle& circle, glm::vec3& colPos)
		{
			Entity circleObject = circle.GetParent();

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


			Transform& t = GetParent().ComponentGet<Transform>();

			glm::mat4& mat1 = *t.GetMatrix();

			for (int j = 0; j < (int)this->lineCount_; ++j)
			{
				p0 = this->lineSegments_[j].point[0];
				p1 = this->lineSegments_[j].point[1];

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

				tiSubVec.x = Bi.x - p1.x;
				tiSubVec.y = Bi.y - p1.y;

				if (glm::dot(s, tiSubVec) < 0)
				{
					continue;
				}
				colPos = Bi;
				return true;
			}
			return false;
		}
		void ColliderLine::ResolveCollision(const Collider& collider, glm::vec3)
		{ 
			if (!GetStatic() && collider.GetResolve())
			{
				//DO NOTHING XD	
			}
		}

		//------------------------------------------------------------------------------
	}