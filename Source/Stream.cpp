/*!*****************************************************************************
\par filename: Stream.cpp
\par  authors: Alijah Rosner
\par     date: 09/09/2022

\brief
  This is the implementation for stream
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once

#include "Stream.h"
#include <fstream>
#include <rapidjson/istreamwrapper.h>
#include <map>
#include <unordered_map>
#include "glm/vec4.hpp"
#include <string_view>

namespace FFunctional
{
    rapidjson::Document OpenJson(std::string_view fileName)
    {
        /* Generic rapidjson document */
        rapidjson::Document document;

        /* Input file stream */
        std::ifstream inputStream(fileName.data());

        if (inputStream)
        {
            /* Wraps the input file stream for reading */
            rapidjson::IStreamWrapper wrapper(inputStream);

            /* Constructs tree */
            document.ParseStream(wrapper);
        }
        return document;
    }

    void JsonConvert(const rapidjson::Value& value, glm::vec3& vec)
    {
        auto preConvert = value.GetArray();

        for (int i = 0; i < 3; i++)
        {
            vec[i] = preConvert[i].GetFloat();
        }
    }

    void JsonConvert(const rapidjson::Value& value, glm::vec4& vec)
    {
      auto preConvert = value.GetArray();

      for (int i = 0; i < 4; i++)
      {
        vec[i] = preConvert[i].GetFloat();
      }
    }

    void JsonConvertString(const rapidjson::Value & value, std::string& string)
    {
        string = value.GetString();
    }

}
