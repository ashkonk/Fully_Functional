/*!*****************************************************************************
\par filename: Stream.h
\par  authors: Alijah Rosner
\par     date: 09/09/2022

\brief
  This is the stream header file. 
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include <rapidjson/document.h>
#include <string_view>
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Fully Functional Game Engine * * * */
{
    rapidjson::Document OpenJson(std::string_view fileName);

    void JsonConvert(const rapidjson::Value& value, glm::vec3& vec);
    void JsonConvert(const rapidjson::Value& value, glm::vec4& vec);
    void JsonConvertString(const rapidjson::Value& value, std::string& string);
};