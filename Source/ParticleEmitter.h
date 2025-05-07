/*!*****************************************************************************
\par filename: ParticleEmitter.h
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

typedef struct pEmit
{
    // speed of particles
    glm::vec3 speed_;
    // the remaining lifetime particle has
    float lifetime;
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
} pEmit;

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
    class ParticleEmitter : public Component
    {
    private:

        // the texture to generate based on given texture
        std::string texture_;
        std::shared_ptr<Image> source_;

        // vector to hold all particles
        std::vector<pEmit> Pvec_;

        // spawning
        float timer_;       //<- read in
        float spawnrate_;     //<- read in
        float spawn_;   //<- initialized
        float limitX;   //<- range limit in x-direction
        float limitY;  //<- range limit in y-direction
        bool emit; //<- whether or not to emit particles
        float maxDuration_; 
        float emitterTimer_; 
        glm::vec3 scale;
        // helper functions
        pEmit PCreate(float lifetime);
        glm::vec3 RandomPoint();

    public:

        ParticleEmitter(Entity parent) : Component(parent), spawn_(0.0f), spawnrate_(0.0f), timer_(0.0f), limitX(0.0f), limitY(0.0f), emit(false), maxDuration_(0), emitterTimer_(0), scale(0) {};

        std::shared_ptr<Image> GetImage() const;

        void Read(const rapidjson::Value& value);

        static std::string_view Name() { return "ParticleEmitter"; }

        void Update(float dt);

        std::string GetTexture();

        void SetTexture(std::string NewTexture);

        std::vector<pEmit>& GetPVec();

        void SetEmit(int state);

    }; /* ExampleComponent */

}; /* Normalloc */