/*!*****************************************************************************
\par filename: Manager.cpp
\par  authors: Ashkon Khalkhali
\par     date: 09/17/2022
\brief
  Templated manager class to contain and update multiple objects
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Manager.h"


namespace FFunctional
{
  template<typename T>
  using TPtr = std::shared_ptr<T>;

  template<typename T>
  inline Manager<T>::Manager(std::function<void(Manager&)> initFunction)
  {
    initFunc_ = initFunction;
  }

  template<typename T>
  inline void Manager<T>::Add(TPtr object)
  {
    NewObjects_.push_back(object);
    //object->Init();
  }

  template<typename T>
  inline void Manager<T>::Init()
  {
    initFunc_(*this);
    for (TPtr object : Objects_)
    {
      object->Init();
  //template<typename T>
  //inline T* Manager<T>::At(unsigned index)
  //{
    }
  }

  //  return Objects_.at(index);
  //}

  template<typename T>
  inline unsigned Manager<T>::Size()
  {
    return (unsigned)Objects_.size() + (unsigned)NewObjects_.size();
  }

  template<typename T>
  inline void Manager<T>::Update(float dt)
  {
    //range based for loops have undefined behavior if the array
    //changes at any time.
      for (int i = 0; i < NewObjects_.size(); i++)
      {
          Objects_.push_back(NewObjects_.at(i));
          Objects_.back()->Init();
    }
      NewObjects_.clear();
    for (int i = 0; i < Objects_.size(); ++i)
    {
      Objects_[i]->Update(dt);
    }
    Kill();
  }

  template<typename T>
  inline void Manager<T>::Delete(TPtr toBeKilled)
  {
    KillList_.push_back(toBeKilled);
  }

  template<typename T>
  inline void Manager<T>::Delete(T* toBeKilled)
  {
    for (unsigned i = 0; i < Objects_.size(); ++i)
    {
      if (Objects_[i].get() == toBeKilled)
      {
        KillList_.push_back(Objects_[i]);
        break;
      }
    }

    for (unsigned i = 0; i < NewObjects_.size(); ++i)
    {
        if (NewObjects_[i].get() == toBeKilled)
        {
            KillList_.push_back(NewObjects_[i]);
            break;
        }
    }
  }

  template<typename T>
  inline void Manager<T>::Clear()
  {
    for (TPtr object : Objects_)
    {
      object->Exit();
    }
    NewObjects_.clear();
    Objects_.clear();
    KillList_.clear();
  }

  template<typename T>
  inline void Manager<T>::Kill()
  {
    for (TPtr& mostWanted : KillList_)
    {
      for (TPtr& object : Objects_)
      {
        if (object.get() == mostWanted.get())
        {
          std::swap(object, Objects_.back());
          Objects_.pop_back();
          break;
        }
      }
      for (TPtr& objectNew : NewObjects_)
      {
          if (objectNew.get() == mostWanted.get())
          {
              std::swap(objectNew, NewObjects_.back());
              NewObjects_.pop_back();
              break;
          }
      }
    }
    KillList_.clear();
  }

  template<typename T>
  inline void Manager<T>::Draw() const
  {
    for (TPtr object : Objects_)
    {
      object->Draw();
    }
  }

  template<typename T>
  inline const std::string& Manager<T>::Name()
  {
    return typeName;
  }

  template<typename T>
  inline std::string_view Manager<T>::GetName() const
  {
    {
      return typeName;
    }
  }

  template<typename T>
  inline void Manager<T>::SetParent(Engine* mainEngine)
  {
    mainEngine_ = mainEngine;
  }

  template<typename T>
  inline Engine* Manager<T>::GetParent() const
  {
    return mainEngine_;
  }

}