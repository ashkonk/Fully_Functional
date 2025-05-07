/*!*****************************************************************************
\par filename: Manager.h
\par  authors: Ashkon Khalkhali
\par     date: 09/17/2022
\brief
  Templated manager class to contain and update multiple objects
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "ISystem.h"
#include "Engine.h"

/*!*****************************************************************************
  \brief
    Our Perfectly Normal Fully Functional Game Engine Namespace.
*******************************************************************************/
namespace FFunctional /* The Perfectly Normal Full Functional Game Engine * * * */
{
  /*!***************************************************************************
    \brief
      This is an example of a component class.
  *****************************************************************************/
  template<typename T> class Manager : public ISystem
  {
    using TPtr = std::shared_ptr<T>;

    private:
      std::vector<TPtr> Objects_;
      std::vector<TPtr> KillList_;
      std::vector<TPtr> NewObjects_;
      Engine* mainEngine_;
      std::function<void(Manager&)> initFunc_;
      inline const static std::string typeName = T::Name().data() + std::string("Manager");
      bool runFrontOnly;

    public: 
      //construct manager
      Manager(std::function<void(Manager&)> initFunction);
  
      void Add(TPtr object);

      void Init() override; 

      TPtr At(unsigned index)
      {
          if (index >= Objects_.size())
          {
              return NewObjects_.at(index - Objects_.size());
         }
        return Objects_.at(index);
      }

      unsigned Size();

      void Update(float dt) override;

      void Delete(TPtr toBeKilled);

      void Delete(T* toBeKilled);
 
      void Clear();

      void Kill();

      // Inherited via ISystem
      virtual void Draw() const override;
      
      static const std::string& Name();

      virtual std::string_view GetName() const override;
      
      virtual void SetParent(Engine* mainEngine) override;

      
      virtual Engine* GetParent() const override;

  }; /* ExampleComponent */
}; /* Normalloc */

#include "Manager.cpp"