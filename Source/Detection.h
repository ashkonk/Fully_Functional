/*!*****************************************************************************
\par filename: Detection.h
\par  authors: Mason Dulay
\par     date: 09/09/2022

\brief
  This is the Detection file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "Component.h"
#include "Graphics.h"

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
    class Detection : public Component
    {
    private:

        // the texture to generate based on given texture
        std::string texture_;
        std::shared_ptr<Image> source_;

        std::string texture2_;
        std::shared_ptr<Image> source2_;

        int mode_;

        // position
        glm::vec3 translation_ = { 0, 0, 0 };
        // scale
        glm::vec3 scale_ = { 0.125f, 0.125f, 0.0f};
        // rotationVec
        glm::vec3 rotationVec_ = { 0, 0, -1 };
        // rotation
        float rotation_ = 0.0f;
        // opacity
        float opacity_ = 1.0f;

    public:

        Detection(Entity parent) : Component(parent), mode_(0) {};

        std::shared_ptr<Image> GetImage() const;

        void Read(const rapidjson::Value& value);

        static std::string_view Name() { return "Detection"; }

        void Update(float dt);

        std::string GetTexture();

        void SetTexture(std::string NewTexture);

        void SetTexture2(std::string NewTexture);

        glm::vec3 GetTranslation();

        glm::vec3 GetScale();

        glm::vec3 GetRotationVec();

        float getRotation();

        float getOpacity();

    }; /* ExampleComponent */

}; /* Normalloc */