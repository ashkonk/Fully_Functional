/*!*****************************************************************************
\par filename: Graphics.h
\par  authors: Jonathan Meyer
\par     date: 09/18/2022

\brief
  This file contains the public declarations of our graphics interface.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

# pragma warning(push)
# pragma warning(disable:4201)
// warning C4201: nonstandard extension used : nameless struct/union

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <vector>
#include <memory>
#include <map>
#include "Component.h"
#include "Vision.h"
#include "Occluder.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
    /*!*************************************************************************
      \brief
        Our Vertex struct to pass in any parameters we need for each vertex.
    ***************************************************************************/
    struct Vertex
    {
        glm::vec2 position; //!< the position on screen the vertex is at.
        glm::vec2 uvCoords; //!< the texture UV coordinates of the vertex.
        glm::vec4 color;    //!< the <R,G,B,A> color of the vertex.
    };

    struct Mesh
    {
        GLuint data, count;
    };

    struct Image
    {
        GLuint width, height, bpp, textureID;
        unsigned char* data; //!< raw image data

    public:
        ~Image();
    };


    typedef class ColliderCircle      ColliderCircle;
    typedef class ColliderLine        ColliderLine; 
    typedef struct ColliderLineSegment ColliderLineSegment;
    typedef class Transform Transform;

    /*!*************************************************************************
      \brief
        Our graphics is an interface, rather than a system.
    ***************************************************************************/
    namespace Graphics /* Our Graphics Interface  * * * * * * * * * * * * * * */
    {
        void DrawTriangle(Vertex& vertex0, Vertex& vertex1, Vertex& vertex2);

        Mesh CreateMesh(Vertex* vertexData, GLuint vCount);

        void ClearBackground(glm::vec4 color);

        void DrawMesh(Mesh& mesh);

        void DrawTexturedMesh(Mesh& mesh, Image& texture);

        void DrawShape(const std::vector<glm::vec2>& shape, const glm::mat4& mat);

        std::shared_ptr<Image> LoadImage(const char* filename);

        void DrawImageTransform(const Image& image, const glm::mat4& transform);

        void DrawImageTransformUVs(const Image& image,
          const glm::mat4& transform, const glm::vec4& uvs, float opacity, bool static_);

        void DrawImageTransformUVs_MIX_OVERLAY(const Image& image,
          const glm::mat4& transform, const glm::vec4& uvs, float opacity, bool static_);

        void DrawImage(const Image& image, const glm::vec2& pos);

        void Update(float dt);

        bool AsyncImageReady();

        void Flush();

        void FreeImage(Image image);

        void Init();

        void DrawColliderCircle(const ColliderCircle& collider);

        void DrawColliderLineSeg(const ColliderLineSegment& collider);

        void DrawColliderLine(const ColliderLine& collider);

        void SetCameraTransform(const Transform& transform);

        void ResetCameraTransform();

        const glm::mat4& GetCameraTransform();

        const glm::mat4& GetScreenTransform();

        void SetNodes(const std::map<Component*, TimeNode>& nodes);
        
        void SetVisions(const std::map<Component*, Vision&>& nodes);

        void SetOcclusions(const std::map<Component*, Occluder&>& nodes);

        void UnsetVision(int index);
        
        void UnsetOccluder(int index);


        //void SetOcclusion(const std::map<Component*, Occluder&>& nodes);

        void DrawDilationShader(float relativeTime);
        
        void DrawBackgroundShader();

    }; /* Graphics */

}; /* Normalloc */