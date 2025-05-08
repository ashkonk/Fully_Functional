/*!*****************************************************************************
\par filename: Factory.h
\par  authors: Alijah Rosner
\par     date: 09/22/2022

\brief
  This is the stub file for code implimentation.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include <cassert>
#include <string_view>
#include <functional>
#include <unordered_map>
#include <utility>
#include "ISystem.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{
  //These are the same thing

  //typedef void(*GameStateUpdateFunction)(float dt);
  //GameStateUpdateFunction CStyleFunction;

  //std::function<void(float dt)> CppStyleFunction;

/*!***************************************************************************
  \brief
    This is an example of a component class.
*****************************************************************************/

  template<typename ... Args>

  class Factory : public ISystem
  {

  public:

    using construct_function_t = std::function<void(Factory&)>;
    using create_function_t = std::function<void(Args&&...)>;

    Factory(construct_function_t on_construct)
    {
      on_construct(*this);
    }


    void Add(std::string_view id, create_function_t function)
    {
      //looking through functions to make sure nobody has registered
      //one before. Safety wrap around class creation.
      assert(functions_.find(std::string(id)) == functions_.end());
      functions_[std::string(id)] = function;
    }

    void Create(std::string_view id, Args... args)
    {
      auto iterator = functions_.find(std::string(id));

      if (iterator != functions_.end())
      {
        iterator->second(std::forward<Args>(args)...);
      }
    }

    static std::string_view Name() { return typeName; }

    virtual Engine* GetParent() const override
    {
      return parent_;
    }

  private:
    std::unordered_map<std::string, create_function_t> functions_;
    Engine* parent_;

    inline const static std::string typeName = "Factory";

    // Inherited via ISystem
    virtual void Init() override {}
    virtual void Update(float) override {}
    virtual void Draw() const override {}

    virtual std::string_view GetName() const override
    {
      return Name();
    }

    virtual void SetParent(Engine* mainEngine) override
    {
      parent_ = mainEngine;
    }


  };

}; /* FFunctional */