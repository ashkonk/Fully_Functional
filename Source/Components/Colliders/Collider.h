/*!*****************************************************************************
\par filename: Collider.h
\par  authors: Jonathan Meyer
\par     date: 11/14/2022

\brief
  Code declaration for ColliderCircle
	Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "Component.h"
#include "StateMachine.h"
#include "Engine.h"
#include "rapidjson/document.h"
#include <glm/gtc/constants.hpp>
#include <glm/mat3x3.hpp>
#include <unordered_map>

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

namespace FFunctional
{

	std::pair<bool, glm::vec2> LineCollisionDetect(const glm::vec2& a_0,
	const glm::vec2& b_0, const glm::vec2& a_1, const glm::vec2& b_1);

	bool ShapeCollisionDetect(const std::vector<glm::vec2>& shape_a,
		const std::vector<glm::vec2>& shape_b, const glm::mat4& mat_0,
		const glm::mat4& mat_1);

	class Collider : public Component
	{
	public:
		Collider(Entity parent):
		Component(parent), engine_(NULL), shape(), offset(), radius() { };

		Collider(Engine* engine, Entity parent):
		Component(parent), engine_(engine), shape(), offset() { };

		~Collider();

		void Read(const rapidjson::Value& value);

		void GenerateCircleCol(float radius, unsigned segments);

		void GenerateBoxCol(float width, float height);

		bool CheckCollision(Collider& rhs, bool callSignal = true);

		void CollisionSignal(bool check, const Collider& rhs);

		void Draw() const;

		void Update(float dt);

		bool IsStatic() const;
		bool IsEnabled() const;
		bool IsSolid() const;
		
		void SetStatic(bool staticC);
		void SetEnabled(bool enabled);
		void SetSolid(bool solid);

		bool GetCheck();

		std::string GetTag();

		glm::vec2 GetOffset();


		static std::string_view Name() { return "Collider"; }

	private:
		Engine* engine_;
		std::unordered_map<EntityID, bool> collisions;
		std::vector<glm::vec2> shape;
		std::vector<glm::vec2> belts;
		float radius;
		glm::vec3 offset;
		std::string tag_; 
		bool colliding_;

		bool static_ = false, solid_ = true, enabled_ = true;
	};
}