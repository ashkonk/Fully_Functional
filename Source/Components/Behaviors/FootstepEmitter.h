/*!*****************************************************************************
\par filename: FootstepEmitter.h
\par  authors: Mason Dulay
\par     date: 09/09/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "Component.h"
#include "glm/vec3.hpp"
#include "Graphics.h"

typedef enum Foot
{
    left,
    right
} Foot;

typedef struct Particle
{
    // the remaining lifetime particle has
    float lifetime;
    // which foot the particle represents
    int foot;
    // opacity
    float opacity_;
    // position
    glm::vec3 translation_ = {0, 0, 0};
    // scale
    glm::vec3 scale_ = {};
    // rotationVec
    glm::vec3 rotationVec = { 0, 0, -1 };
    // rotation
    float rotation_ = 0.0f;
} Particle;

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    /*!*************************************************************************
      \brief
        This is an example of a component class.
    ***************************************************************************/
    class FootstepEmitter : public Component
    {
    private:

        // the texture to generate based on given texture
        std::string texture_;
        std::shared_ptr<Image> source_;

        // vector to hold all particles
        std::vector<Particle> Pvec_;
        
        // to track which foot you are on
        bool footCurr_;

        // spawning
        float timer_;       //<- read in
        float spawnrate_;     //<- read in
        float spawn_;   //<- initialized

        // helper functions
        Particle PCreate(float lifetime, int foot);

    public:

        FootstepEmitter(Entity parent) : Component(parent), footCurr_(true), spawn_(0.0f), spawnrate_(0.0f), timer_(0.0f) {};

        std::shared_ptr<Image> GetImage() const;

        void Read(const rapidjson::Value& value);

        static std::string_view Name() { return "FootstepEmitter"; }

        void Update(float dt);

        std::string GetTexture();

        void SetTexture(std::string NewTexture);

        std::vector<Particle>& GetPVec();

    }; /* ExampleComponent */

}; /* Normalloc */