/*!*****************************************************************************
\par filename: Graphics.cpp
\par  authors: Jonathan Meyer
\par     date: 09/18/2022

\brief
  This file contains the implimentation for our graphics interface.
  Copyright � 2022 DigiPen (USA) Corporation.
*******************************************************************************/

#include <thread>
#include "Graphics.h"
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include "GL/glew.h"
#include <iostream>
#include "Transform.h"
#include "Collider.h"
#include "Transform.h"
#include "mutex"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Prototype shaders

const char* vertShader =
R"VERT(
#version 330 core

layout (location = 0) in vec2 inCoords;
layout (location = 1) in vec2 inTexUVs;
layout (location = 2) in vec4 inColors;

out vec2 TexCoords;
out vec4 Colors;

uniform mat4 camera;
uniform mat4 transform;
uniform float opacity;

void main(void)
{
    gl_Position = (camera * transform * vec4(inCoords, 1, 1)) * vec4(2, 2, 1, 1);
    TexCoords = inTexUVs;
    Colors = inColors*vec4(1, 1, 1, opacity);
}

)VERT";


const char* vertLinesShader =
R"VERTLINES(
#version 330 core

layout (location = 0) in vec2 inCoords;
layout (location = 1) in vec2 inTexUVs;
layout (location = 2) in vec4 inColors;

out vec2 TexCoords;
out vec4 Colors;

uniform mat4 camera;
uniform mat4 transform;

uniform vec2 point_a;
uniform vec2 point_b;

void main(void)
{
    gl_Position = (camera * transform * vec4(mix(point_a, point_b, float(gl_VertexID)), 1, 1)) * vec4(2, 2, 1, 1);
    TexCoords = inTexUVs;
    Colors = inColors;
}

)VERTLINES";

const char* vertTimeShader =
R"VERTTIME(
#version 330 core

layout (location = 0) in vec2 inCoords;
layout (location = 1) in vec2 inTexUVs;
layout (location = 2) in vec4 inColors;

out vec2 TexCoords;
out vec4 Colors;

uniform mat4 camera;
uniform float aspectRatio;

uniform vec2  cameraScl;
uniform vec2  cameraOff;
uniform float opacity;

void main(void)
{
    gl_Position = vec4(2*inCoords, 1, 1);
    TexCoords = inTexUVs;

    vec4 coords = camera * vec4(inCoords*vec2(cameraScl.x, cameraScl.y), 0, 1);

    Colors = vec4(inCoords*vec2(cameraScl.x, cameraScl.y) + cameraOff, 0, opacity);
}

)VERTTIME";


const char* fragShader =
R"FRAG(
#version 330 core

in vec2 TexCoords;
in vec4 Colors;

out vec4 outColor;

uniform sampler2D inTexture;
uniform float time;

void main(void)
{
    //outColor = mix(Colors, texture(inTexture, TexCoords), 0.5f);
    //outColor = Colors*texture(inTexture, TexCoords);

    vec4 color = texture(inTexture, TexCoords);
    float strength = 16.0;


    float x = (TexCoords.x + 4.0 ) * (TexCoords.y + 4.0 ) * (time * 10.0);
	  vec4 grain = vec4(mod((mod(x, 13.0) + 1.0) * (mod(x, 123.0) + 1.0), 0.01)-0.005) * strength;


    if(abs(TexCoords.x - 0.5) < 0.002)
        color = vec4(0.0);

    if(TexCoords.x > 0.5)
    {
    	grain = 1.0 - grain;
		  outColor = vec4(1,0,0,1)*(color * grain);
    }
    else
    {
		  outColor = vec4(1,0,0,1)*(color + grain);
    }
}

)FRAG";


const char* fragLinesShader =
R"FRAGLINES(
#version 330 core

in vec2 TexCoords;
in vec4 Colors;

out vec4 outColor;

void main(void)
{
    outColor = vec4(1,0,0,1);
}

)FRAGLINES";


/* ref-url: https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83 */
/* used the first two functions under 'Perlin Noise' and modified them.       */
/* accordingly.                                                               */
const char* fragTimeShader =
R"FRAGTIME(
#version 330 core

in vec2 TexCoords;
in vec4 Colors;

out vec4 outColor;

uniform float aspectRatio;

uniform vec2  coords[8];
uniform float radius[8];
uniform float curve[8];
uniform float rate[8];
uniform int nodeCount;

uniform vec2 visionCoord[8];
uniform vec3 visionExtra[8];
uniform int  visionCount;


uniform vec4 occludeTrans[64];
uniform int  occludeCount;

uniform float playerDist;

uniform float time;

uniform float relativeTime;


float chromaConeSense(float wavelength, float steep, float offset)
{
  float exponent = (wavelength*2 + offset);

  exponent = -steep*exponent*exponent;

  return exp(exponent);
}

float reScale(float f)
{
  return (2.0f/(1.0f + exp(-f))) - 1;
}

vec3 chromaScale(float f)
{
  float wave = 1 - f;

  float r = chromaConeSense(wave, 8, -1.2);
  float g = chromaConeSense(wave, 6, -0.9);
  float b = chromaConeSense(wave, 8, -0.61);

  return vec3(r, g, b);
}



float rand(vec2 seed)
{
	return fract(cos(dot(vec2( 19.3769, 49.867), seed.xy)) * 32859.1092);
}

float noise(vec2 coords, float scale)
{
	vec2 newCoords = floor(coords * scale);
	vec2 bilinear  = mod(coords, 1.0f / scale) * scale;
	bilinear = (1.0f - cos(3.141592f * bilinear)) / 2.0f;

	float tl = rand(newCoords + vec2(0.0f, 0.0f));
	float tr = rand(newCoords + vec2(1.0f, 0.0f));
	float bl = rand(newCoords + vec2(0.0f, 1.0f));
	float br = rand(newCoords + vec2(1.0f, 1.0f));

	float t = mix(tl, tr, bilinear.x);
	float b = mix(bl, br, bilinear.x);

	return mix(t, b, bilinear.y);
}

float evaluate(vec2 pos)
{
    float dial = 1.0f;

    for (int i = 0; i < nodeCount; ++i)
    {
        float x = distance(pos.xy, coords[i]);
        x = mix(x*0.9, x*1.1, noise(normalize(pos.xy - coords[i]) + vec2(cos(0.01f*time), sin(0.01f*time)), 4.0f));
        float y = clamp(1 - pow(x/radius[i], curve[i]), 0, 10);

        dial = dial*(1 - y) + rate[i]*y;
    }
    return dial;
}


float rectSDF(vec2 coords, vec2 size)
{
    vec2 edge = max(abs(coords) - size/2, vec2(0.0f));

    float outside = length(edge);
    float inside = min(max(edge.x, edge.y), 0.0f);

    //return 0.5f;

    return outside + inside;
}

float SDF(vec2 coords)
{
  float sdf = 1e10;

  for (int i = 0; i < occludeCount; ++i)
  {
      sdf = min(sdf, rectSDF(coords - occludeTrans[i].xy, occludeTrans[i].zw));
  }

  return sdf;
}

vec2 raymarch(vec2 source, vec2 dir)
{
  vec2 point = source;

  for (int i = 0; i < 64; ++i)
  {
    point = point + SDF(point)*dir;
  }

  return point;
}

float visibility(vec2 coords, vec2 source, vec3 view)
{
  float rad = view.x;
  float dir = view.y;
  float fov = view.z;

  //raymarch from the vision cone to coords if within the vision angle

  //if the raymarch distance to coords is shorter than the distance between
  // the vision cone and the coords. then it is in shadow, otherwise it is not.

  vec2 point = source;

  float dist = distance(source, coords);

  if (dist < rad)
  {
      vec2 coordDir = normalize(coords - source);
      float product = dot(coordDir, vec2(cos(radians(dir)), sin(radians(dir))));

      if (product >= cos(radians(fov/2)))
      {
          point = raymarch(source, coordDir);
      }

  }

  return float(int((sign(distance(source, point) - dist + 0.05f) + 1)/2))*(1 - pow(min(dist, rad)/rad, 4.0f));
}



void main(void)
{
    float scale = reScale(evaluate(Colors.xy)/relativeTime);
    float visible = 0.0f;

    for (int i = 0; i < visionCount; ++i)
    {
        visible = max(visible, visibility(Colors.xy, visionCoord[i], visionExtra[i]));
    }

    float dist = 1-clamp(playerDist, 0.0, 1.0);
    outColor = vec4(mix(chromaScale(scale), mix(vec3(1.0, 0.8, 0.0), vec3(1.0f, 0.0f, 0.0f), dist), visible*0.5f), mix(Colors.w*distance(scale, 0.5f), 0.33f, visible));
}

)FRAGTIME";

/* ref-url: https://www.shadertoy.com/view/4sXSWs                             */
/* used grain function as reference when writing our noise shader.            */
/* my implementation removes seed artifacts, and works for our use case.      */
const char* animShader =
R"ANIM(
#version 330 core


in vec2 TexCoords;
in vec4 Colors;

out vec4 outColor;

uniform sampler2D inTexture;
uniform vec4 uvs;
uniform float time;

void main(void)
{
    vec2 coords = TexCoords*uvs.zw + uvs.xy;

    vec4 color = texture(inTexture, coords);
    float strength = 10.0/pow((color.r + color.g + color.b)/3 + 1, 3);

    float x = (sin(coords.x*1000)) * (cos(coords.y*1000)) * ((sin(time) + 1.1)/2.2 * 10.0);
	  vec4 grain = vec4(mod((mod(x, 13.0) + 1.0) * (mod(x, 123.0) + 1.0), 0.01)-0.005) * strength;

    outColor = Colors*vec4(color.rgb + grain.rgb, color.w);
}

)ANIM";



/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * */
{
  /*!*************************************************************************
    \brief
      Our graphics is an interface, rather than a system.
  ***************************************************************************/
  namespace Graphics /* Our Graphics Interface  * * * * * * * * * * * * * * */
  {
    static std::vector<std::shared_ptr<Image>> loadedImages;
    static std::unordered_map<std::string, std::shared_ptr<Image>> imageMap;
    static std::mutex loadedImagesMutex;
    static GLuint shaderDefault;
    static GLuint shaderLines;
    static GLuint shaderDilation;

    static Mesh quad;
    static Mesh line;

    glm::mat4 cameraTransform;
    glm::mat4 cameraRealMat;

    glm::vec2 cameraScale;
    glm::vec2 cameraOffset;

    static int totalImages = 0;
    static int totalLoaded = 0;

    GLint viewport[4];

    float time = 0;

    /* returns 0 when didnt compile correctly, otherwise shaderID */
    static GLuint MakeShaders(const char* vShader, const char* fShader)
    {
      /* Compile the vertex shader program */
      GLint vertStrLen = (GLint)strlen(vShader);

      GLuint vertexStage = glCreateShader(GL_VERTEX_SHADER);

      glShaderSource(vertexStage, 1, &vShader, &vertStrLen);
      glCompileShader(vertexStage);

      GLint success;
      glGetShaderiv(vertexStage, GL_COMPILE_STATUS, &success);

      if (!success)
      {
        std::cout << "oh noes! v-shader couldnt compile!" << std::endl;
        return 0;
      }

      /* Compile the fragment shader program */
      GLint fragStrLen = (GLint)strlen(fShader);

      GLuint fragmentStage = glCreateShader(GL_FRAGMENT_SHADER);

      glShaderSource(fragmentStage, 1, &fShader, &fragStrLen);
      glCompileShader(fragmentStage);

      glGetShaderiv(fragmentStage, GL_COMPILE_STATUS, &success);

      if (!success)
      {
        std::cout << "oh noes! f-shader couldnt compile!" << std::endl;
        return 0;
      }

      GLuint shaderID = glCreateProgram();
      glAttachShader(shaderID, vertexStage);
      glAttachShader(shaderID, fragmentStage);



      /* link the shader program together */

      glLinkProgram(shaderID);

      glGetProgramiv(shaderID, GL_LINK_STATUS, &success);

      if (!success)
      {
        std::cout << "oh noes! your shader couldnt link!" << std::endl;
        return 0;
      }

      glDeleteShader(vertexStage);
      glDeleteShader(fragmentStage);

      return shaderID;
    }

    Mesh CreateMesh(Vertex* vertexData, GLuint vCount)
    {
      GLuint meshID, meshVertexBufferID;

      /* make the vertex array */
      glGenVertexArrays(1, &meshID);
      glBindVertexArray(meshID);

      /* make the buffers that is passed to the shaders */
      glGenBuffers(1, &meshVertexBufferID);
      glBindBuffer(GL_ARRAY_BUFFER, meshVertexBufferID);

      /* pass in the vertex data */
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vCount, vertexData, GL_STATIC_DRAW);


      /* vertex attributes... */
      GLint positionAttribute = 0, uvsAttribute = 1, colorAttribute = 2;

      glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, false, sizeof(Vertex), 0); //Position

      glVertexAttribPointer(uvsAttribute, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(glm::vec2))); //UVS

      glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, false, sizeof(Vertex), (void*)(2 * sizeof(glm::vec2))); //Color

      glEnableVertexAttribArray(positionAttribute);
      glEnableVertexAttribArray(uvsAttribute);
      glEnableVertexAttribArray(colorAttribute);

      const GLuint unbindAll = 0;
      glBindVertexArray(unbindAll);

      return Mesh{ meshID, vCount };
    }

    void Init()
    {
      // Disable culling (triangles seen from behind will be drawn)
      glDisable(GL_CULL_FACE);

      cameraTransform = glm::inverse(glm::mat4(1.0f));

      // Enable blending for normal transparency
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      // make sure glew has initialized
      GLenum err = glewInit();

      if (!err) //if successful, start building shaders.
      {
        shaderDefault = MakeShaders(vertShader, animShader);
        shaderLines = MakeShaders(vertLinesShader, fragLinesShader);
        shaderDilation = MakeShaders(vertTimeShader, fragTimeShader);

        Vertex triangle[6] =
        {
            Vertex{ glm::vec2{-0.5f, -0.5f}, glm::vec2{0.0f, 1.0f}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}},
            Vertex{ glm::vec2{ 0.5f, -0.5f}, glm::vec2{1.0f, 1.0f}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}},
            Vertex{ glm::vec2{-0.5f,  0.5f}, glm::vec2{0.0f, 0.0f}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}},
            Vertex{ glm::vec2{ 0.5f, -0.5f}, glm::vec2{1.0f, 1.0f}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}},
            Vertex{ glm::vec2{-0.5f,  0.5f}, glm::vec2{0.0f, 0.0f}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}},
            Vertex{ glm::vec2{ 0.5f,  0.5f}, glm::vec2{1.0f, 0.0f}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}}
        };

        Vertex lines[2] =
        {
            Vertex{ glm::vec2{0.0f, 0.0f}, glm::vec2{0.0f, 0.0f}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}},
            Vertex{ glm::vec2{1.0f, 0.0f}, glm::vec2{0.0f, 0.0f}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}}
        };

        line = CreateMesh(lines, 2);
        quad = CreateMesh(triangle, 6);
      }
    }

    void DrawImage(const Image& image, const glm::vec2& pos)
    {
      glm::mat4 result = glm::mat4(1.0f);

      float ratio = float(image.height) / image.width;
      ratio *= (float(viewport[3]) / viewport[2]);

      result[2][0] = pos[0];
      result[2][1] = pos[1];
      result[0][0] = ratio;

      DrawImageTransform(image, result);
    }

    void DrawImageTransform(const Image& image, const glm::mat4& transform)
    {
      //we want to use our shader program.
      glUseProgram(shaderDefault);

      //pass in our mesh.
      glBindVertexArray(quad.data);

      //pass in our texture.
      glBindTexture(GL_TEXTURE_2D, image.textureID);

      //get transform uniform from our shader
      GLint transformLoc;
      transformLoc = glGetUniformLocation(shaderDefault, "transform");
      GLint timeLoc = glGetUniformLocation(shaderDefault, "time");
      glUniform1f(timeLoc, time);

      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

      //get our GPU to draw the mesh.
      glDrawArrays(GL_TRIANGLES, 0, quad.count);

      //unbind our mesh.
      glBindVertexArray(0);

      //unbind our shader.
      glUseProgram(0);
    }



    void DrawImageTransformUVs(const Image& image,
      const glm::mat4& transform, const glm::vec4& uvs, float opacity, bool static_)
    {
      //we want to use our shader program.
      glUseProgram(shaderDefault);

      //pass in our mesh.
      glBindVertexArray(quad.data);

      //pass in our texture.
      glBindTexture(GL_TEXTURE_2D, image.textureID);

      //get transform uniform from our shader
      GLint transformLoc, cameraLoc, opacityLoc;
      transformLoc = glGetUniformLocation(shaderDefault, "transform");
      cameraLoc = glGetUniformLocation(shaderDefault, "camera");
      opacityLoc = glGetUniformLocation(shaderDefault, "opacity");
      GLint timeLoc = glGetUniformLocation(shaderDefault, "time");

      glUniform1f(timeLoc, time);

      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
      if (static_)
      {
        glm::mat4 defaultMat = glm::mat4(1.0f);
        glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(defaultMat));
      }
      else
      {
        glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(cameraTransform));
      }


      glUniform1f(opacityLoc, opacity);

      //get transform uniform from our shader
      GLint uvsLoc;
      uvsLoc = glGetUniformLocation(shaderDefault, "uvs");

      glUniform4fv(uvsLoc, 1, glm::value_ptr(uvs));

      //get our GPU to draw the mesh.
      glDrawArrays(GL_TRIANGLES, 0, quad.count);

      //unbind our mesh.
      glBindVertexArray(0);

      //unbind our shader.
      glUseProgram(0);

      glBindTexture(GL_TEXTURE_2D, 0);
    }

    void DrawImageTransformUVs_MIX_OVERLAY(const Image& image,
      const glm::mat4& transform, const glm::vec4& uvs, float opacity, bool static_)
    {
      //glEnable(GL_BLEND);
      //glBlendFuncSeparate(GL_ZERO, GL_SRC_COLOR, GL_ONE, GL_ZERO);
      //glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
      glEnable(GL_BLEND);
      glBlendFuncSeparate(GL_DST_COLOR, GL_SRC_COLOR, GL_ONE, GL_ONE);
      glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);

      DrawImageTransformUVs(image, transform, uvs, opacity, static_);

      //glDisable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void DrawLine(const glm::vec2 point_a, const glm::vec2 point_b, const glm::mat4& transform)
    {
      //we want to use our shader program.
      glUseProgram(shaderLines);

      //pass in our mesh.
      glBindVertexArray(line.data);

      //get transform uniform from our shader
      GLint pointALoc, pointBLoc, transformLoc, cameraLoc, opacityLoc;
      pointALoc = glGetUniformLocation(shaderLines, "point_a");
      pointBLoc = glGetUniformLocation(shaderLines, "point_b");
      transformLoc = glGetUniformLocation(shaderLines, "transform");
      cameraLoc = glGetUniformLocation(shaderLines, "camera");
      opacityLoc = glGetUniformLocation(shaderDefault, "opacity");

      glUniform2fv(pointALoc, 1, glm::value_ptr(point_a));
      glUniform2fv(pointBLoc, 1, glm::value_ptr(point_b));

      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
      glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(cameraTransform));
      glUniform1f(opacityLoc, 1.0f);

      //get our GPU to draw the mesh.
      glDrawArrays(GL_LINES, 0, line.count);

      //unbind our mesh.
      glBindVertexArray(0);

      //unbind our shader.
      glUseProgram(0);
    }

    void DrawShape(const std::vector<glm::vec2>& shape, const glm::mat4& mat)
    {
      for (size_t i = 0; i < shape.size(); ++i)
        DrawLine(shape.at(i), shape.at((i + 1) % shape.size()), mat);
    }

    void ClearBackground(glm::vec4 color)
    {
      //set background color and clear the screen
      glClearColor(color.x, color.y, color.z, color.w);
      glClear(GL_COLOR_BUFFER_BIT);
    }

    void DrawMesh(Mesh& mesh)
    {
      //we want to use our shader program.
      glUseProgram(shaderDefault);

      //pass in our mesh.
      glBindVertexArray(mesh.data);

      GLint timeLoc = glGetUniformLocation(shaderDefault, "time");
      glUniform1f(timeLoc, time);

      //get our GPU to draw the mesh.
      glDrawArrays(GL_TRIANGLES, 0, mesh.count);

      //unbind our mesh.
      glBindVertexArray(0);

      //unbind our shader.
      glUseProgram(0);
    }

    void DrawTexturedMesh(Mesh& mesh, Image& texture)
    {
      //we want to use our shader program.
      glUseProgram(shaderDefault);

      //pass in our mesh.
      glBindVertexArray(mesh.data);

      GLint timeLoc = glGetUniformLocation(shaderDefault, "time");
      glUniform1f(timeLoc, time);

      //pass in our texture.
      glBindTexture(GL_TEXTURE_2D, texture.textureID);

      //get our GPU to draw the mesh.
      glDrawArrays(GL_TRIANGLES, 0, mesh.count);

      //unbind our mesh.
      glBindVertexArray(0);

      //unbind our shader.
      glUseProgram(0);
    }

    void Flush()
    {
      //flush to the screen.
      glFlush();
    }

    bool AsyncImageReady()
    {
      if (totalImages == totalLoaded)
      {
        return true;
      }
      return false;
    }

    void VerifyImageIntegrity()
    {
      //double buffering
      if (loadedImages.size() == 0)
      {
        return;
      }
      loadedImagesMutex.lock();
      std::vector<std::shared_ptr<Image>> loadedImagesCopy = loadedImages;
      loadedImages.clear();
      loadedImagesMutex.unlock();

      for (const auto& image : loadedImagesCopy)
      {
        glGenTextures(1, &image->textureID);

        glBindTexture(GL_TEXTURE_2D, image->textureID);

        /* make it into a texture */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);

        /* set texture filtering and wrapping */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        /* unbind our texture. */
        glBindTexture(GL_TEXTURE_2D, 0);
        totalLoaded++;
      }
    }

    void Update(float dt)
    {
      VerifyImageIntegrity();
      glGetIntegerv(GL_VIEWPORT, viewport);

      /* Alijah's Code */
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      /* End of Alijah's Code */

      time = time + dt + 0.78f;
      //time = fmodf(time + dt, 60.0f) + 0.78f;

      GLint timeLoc = glGetUniformLocation(shaderDefault, "time");
      glUniform1f(timeLoc, fmodf(time, 2.0f));
    }



    std::shared_ptr<Image> LoadImageAsync(const char* filename)
    {

      const auto& imageIter = imageMap.find(filename);
      if (imageIter != imageMap.end())
      {
        return imageIter->second;
      }

      std::shared_ptr<Image> newImage = std::make_shared<Image>();
      imageMap.emplace(filename, newImage);

      auto imageLoad = [](std::shared_ptr<Image> image, std::string filename) {

        /* load in the image*/
        image->data = stbi_load(filename.c_str(),
          (int*)&image->width, (int*)&image->height, (int*)&image->bpp, 4
        );

        if (image->data == nullptr)
        {
          std::cout << "oh noes! Image <" << filename;
          std::cout << "> did not load correctly!" << std::endl;
        }
        //only allow one image at a time
        loadedImagesMutex.lock();
        loadedImages.push_back(image);
        loadedImagesMutex.unlock();
      };

      //args:: Function, args for function
      std::thread imageThread(imageLoad, newImage, filename);
      imageThread.detach();
      return newImage;
    }

    std::shared_ptr<Image> LoadImage(const char* filename)
    {
      totalImages += 1;
      return LoadImageAsync(filename);

      //std::shared_ptr<Image> image = std::make_shared<Image>();

      ///* load in the image*/
      //image->data = stbi_load(filename,
      //  (int*)&image->width, (int*)&image->height, (int*)&image->bpp, 4
      //);

      //if (image->data == nullptr)
      //{
      //  std::cout << "oh noes! Image <" << filename;
      //  std::cout << "> did not load correctly!" << std::endl;
      //}

      //glGenTextures(1, &image->textureID);

      //glBindTexture(GL_TEXTURE_2D, image->textureID);

      ///* make it into a texture */
      //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);

      ///* set texture filtering and wrapping */
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      ///* unbind our texture. */
      //glBindTexture(GL_TEXTURE_2D, 0);

      ////stbi_image_free(image->data);
      ////image->data = nullptr;

      //return image;
    }



    void FreeImage(Image image)
    {
      //glDeleteTextures(1, &image.textureID);
      //stbi_image_free(image.data);
    }

    void SetCameraTransform(const Transform& transform)
    {
      glm::vec3 newScale = (*transform.GetScale());


      cameraScale = glm::vec2(newScale);
      cameraOffset = *transform.GetTranslation();

      newScale.x = 1.0f / newScale.x;
      newScale.y = 1.0f / newScale.y;
      newScale.z = 1.0f / newScale.z;

      cameraTransform = glm::scale(glm::mat4(1.0f), newScale);


      cameraTransform = glm::rotate(cameraTransform, -transform.GetRotation(), { 0, 0, -1 });
      cameraTransform = glm::translate(cameraTransform, (*transform.GetTranslation() * glm::vec3(-1, -1, -1)));

      cameraRealMat = glm::scale(glm::mat4(1.0f), *transform.GetScale());
      cameraRealMat = glm::rotate(cameraRealMat, transform.GetRotation(), { 0, 0, 1 });
      cameraRealMat = glm::translate(cameraRealMat, *transform.GetTranslation());

    }

    void ResetCameraTransform()
    {
      cameraTransform = glm::mat4(1.0f);
    }

    const glm::mat4& GetCameraTransform()
    {
      return cameraRealMat;
    }

    const glm::mat4& GetScreenTransform()
    {
      return cameraTransform;
    }

    const int max_nodes = 8;

    glm::vec2 coords[max_nodes];
    float radius[max_nodes];
    float curve[max_nodes];
    float rate[max_nodes];
    int nodeCount = 0;

    void SetNodes(const std::map<Component*, TimeNode>& nodes)
    {
      /* clamp to shader max. */
      nodeCount = int(nodes.size());
      nodeCount = (nodeCount > max_nodes) ? max_nodes : nodeCount;

      int i = 0;
      for (auto const& [c, t] : nodes)
      {
        if (i >= nodeCount)
          break;

        coords[i] = t.pos;
        radius[i] = t.radius;
        curve[i] = t.curve;
        rate[i] = t.rate;

        ++i;
      }
    }




    const int max_visors = 8;

    glm::vec2 visionPos[max_visors];
    glm::vec3 vision[max_visors];
    int visionCount = 0;

    const int max_occluders = 64;
    glm::vec4 occluders[max_occluders];
    int occludeCount = 0;

    void SetVisions(const std::map<Component*, Vision&>& nodes)
    {
      //nodes;
      visionCount = int(nodes.size());
      visionCount = (nodeCount > max_visors) ? max_visors : visionCount;

      int i = 0;
      for (auto const& [c, t] : nodes)
      {
        if (i >= visionCount)
          break;

        visionPos[i] = t.GetPos();
        vision[i] = t.GetFrustrum();
        ++i;
      }
    }


    void SetOcclusions(const std::map<Component*, Occluder&>& nodes)
    {
      //nodes;
      occludeCount = int(nodes.size());
      occludeCount = (occludeCount > max_occluders) ? max_occluders : occludeCount;

      int i = 0;
      for (auto const& [c, t] : nodes)
      {
        if (i >= occludeCount)
          break;

        occluders[i] = t.GetDimensions();
        ++i;
      }
    }

    void UnsetVision(int index)
    {
      for (int i = index + 1; i < visionCount; ++i)
      {
        visionPos[i - 1] = visionPos[i];
        vision[i - 1] = vision[i];
      }
      visionCount--;
    }

    void UnsetOccluder(int index)
    {
      for (int i = index + 1; i < occludeCount; ++i)
        occluders[i - 1] = occluders[i];

      occludeCount--;
    }


    void DrawDilationShader(float relativeTime)
    {
      float dist = Vision::GetMinDistance();

      //we want to use our shader program.
      glUseProgram(shaderDilation);

      //get transform uniform from our vertex shader
      GLint cameraLoc, opacityLoc, aspectLoc, scaleLoc, camPosLoc;
      cameraLoc = glGetUniformLocation(shaderDilation, "camera");
      opacityLoc = glGetUniformLocation(shaderDilation, "opacity");
      aspectLoc = glGetUniformLocation(shaderDilation, "aspectRatio");
      scaleLoc = glGetUniformLocation(shaderDilation, "cameraScl");
      camPosLoc = glGetUniformLocation(shaderDilation, "cameraOff");

      glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(cameraRealMat));
      glUniform1f(opacityLoc, 1.0f);
      glUniform1f(aspectLoc, 16.0f / 9.0f);
      glUniform2fv(scaleLoc, 1, (float*)(&cameraScale));
      glUniform2fv(camPosLoc, 1, (float*)(&cameraOffset));

      //get transform uniform from our fragment shader
      GLint coordsLoc, radiusLoc, curveLoc, rateLoc, nodeLoc, timeLoc, time2Loc;
      GLint distLoc;
      coordsLoc = glGetUniformLocation(shaderDilation, "coords");
      radiusLoc = glGetUniformLocation(shaderDilation, "radius");
      curveLoc = glGetUniformLocation(shaderDilation, "curve");
      rateLoc = glGetUniformLocation(shaderDilation, "rate");
      nodeLoc = glGetUniformLocation(shaderDilation, "nodeCount");
      timeLoc = glGetUniformLocation(shaderDilation, "relativeTime");
      time2Loc = glGetUniformLocation(shaderDilation, "time");
      distLoc = glGetUniformLocation(shaderDilation, "playerDist");

      glUniform2fv(coordsLoc, nodeCount, (float*)(&coords));
      glUniform1fv(radiusLoc, nodeCount, (float*)(&radius));
      glUniform1fv(curveLoc, nodeCount, (float*)(&curve));
      glUniform1fv(rateLoc, nodeCount, (float*)(&rate));
      glUniform1i(nodeLoc, nodeCount);
      glUniform1f(timeLoc, relativeTime);
      glUniform1f(time2Loc, time);
      glUniform1f(distLoc, dist);

      GLint vExtraLoc, vCoordLoc, vCountLoc;
      vCoordLoc = glGetUniformLocation(shaderDilation, "visionCoord");
      vExtraLoc = glGetUniformLocation(shaderDilation, "visionExtra");
      vCountLoc = glGetUniformLocation(shaderDilation, "visionCount");

      glUniform2fv(vCoordLoc, visionCount, (float*)(&visionPos));
      glUniform3fv(vExtraLoc, visionCount, (float*)(&vision));
      glUniform1i(vCountLoc, visionCount);

      GLint oCoordLoc, oCountLoc;
      oCoordLoc = glGetUniformLocation(shaderDilation, "occludeTrans");
      oCountLoc = glGetUniformLocation(shaderDilation, "occludeCount");

      glUniform4fv(oCoordLoc, occludeCount, (float*)(&occluders));
      glUniform1i(oCountLoc, occludeCount);

      //pass in our mesh.
      glBindVertexArray(quad.data);

      //get our GPU to draw the mesh.
      glDrawArrays(GL_TRIANGLES, 0, quad.count);

      //unbind our mesh.
      glBindVertexArray(0);

      //unbind our shader.
      glUseProgram(0);
    }

  }; /* Graphics */

  Image::~Image()
  {
    glDeleteTextures(1, &textureID);
    stbi_image_free(data);
  }

}; /* Normalloc */
