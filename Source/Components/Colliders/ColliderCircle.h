/*!*****************************************************************************
\par filename: ColliderCircle.h
\par  authors: Mason Dulay
\par     date: 09/27/2022

\brief
  Code declaration for ColliderCircle
  Copyright 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "Collider.h"
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
	class ColliderCircle : public Collider
	{
	private:
			// Radius of the circle collider.
			float radius_;
	public:
		ColliderCircle(Engine* engine, Entity parent_) : Collider(engine, parent_, ColliderType::Circle),
			radius_(0.0f){}
		// Read properties of a ColliderCircle from a json file
		void Read(const rapidjson::Value&);

		// Get the circle collider's radius.
		float GetRadius() const;

		// Set the circle collider's radius.
		void SetRadius(float radius);

		// CollisionChecks
		bool IsCollidingWithCircle(const ColliderCircle& other, glm::vec3& pos);
		bool isCollidingWithLine(const Collider& other);
		virtual void ResolveCollision(const Collider&, glm::vec3) override;

		// Resolves
		void ResolveLineCollision(const Collider& line, glm::vec3 colPos);
		void ResolveCircleCollision(const Collider& circle, glm::vec3 colPos);


		static std::string_view Name() { return "ColliderCircle"; }

		// Inherited via Collider
	};


	//------------------------------------------------------------------------------
}

