/*!*****************************************************************************
\par filename: Physics.h
\par  authors: Alijah Rosner
\par     date: 09/17/2022

\brief
  This is the file for physics implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Component.h"
#include <glm/glm.hpp>
#include <string_view>
#include "Stream.h"
/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{
  /*!***************************************************************************
    \brief
      This is the physics class
  *****************************************************************************/
  class Physics : public Component//: public Component /* Example Component  * * * * * * */
  {

    public:
        Physics(Entity parent) : Component(parent) {};
        Physics(const Physics*& rhs);
        void Read(const rapidjson::Value& value);
        const glm::vec3* GetAcceleration() const; 
        const glm::vec3* GetVelocity() const; 
        float GetRotationVelocity() const; 
        const glm::vec3* GetOldTranslation() const; 
        void AddForce(const glm::vec3* force);
        void SetAcceleration(const glm::vec3* acceleration);
        void SetVelocity(const glm::vec3* velocity);
        void SetRotationalVelocity(float rotationalVelocity);
        void Update(float dt);
        static std::string_view Name() { return "Physics"; }

    private:
        glm::vec3 oldTranslation_ = {};
        glm::vec3 acceleration_ = {};
        glm::vec3 velocity_ = {};
        float rotationalVelocity_ = 0.f; 
  };

}; /* FFunctional */