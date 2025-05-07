/*!*****************************************************************************
\par filename: Engine.cpp
\par  authors: Ashkon Khalkhali
\par     date: 09/11/2022

\brief
  Engine class to update systems and manage dt
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#include "Engine.h"
#include "Manager.h"

namespace FFunctional
{
    float Engine::GetDt() const
    {
        return dt;
    }

    bool Engine::IsRunning() const
    {
        return isActive_;
    }

    void Engine::SetLastLevel(int lvl)
    {
      lastLevel_ = lvl;
    }

    int Engine::GetLastLevel()
    {
      return lastLevel_;
    }

    void Engine::Stop()
    {
        isActive_ = false;
    }

    void Engine::Add(ISystem* system)
    {
        //Add new system to back of vector
        system->SetParent(this);
        systemList.push_back(system);
    }

    void Engine::Init()
    {
        tFile = Trace();
        tFile.Init();
        //range based for loop
        for (ISystem* system : systemList)
        {
            system->Init();
        }
        isActive_ = true;

        prevTime_ = std::chrono::steady_clock::now();
    }

    Trace& Engine::GetTrace()
    {
        return tFile;
    }

    void Engine::Update()
    {
        UpdateDt();
        for (ISystem* system : systemList)
        {
            system->Update(dt);
        }
    }

    void Engine::Draw() const
    {
        for (const ISystem* system : systemList)
        {
            system->Draw();
        }
    }
    ISystem* Engine::GetSystem(std::string_view systemName) const
    {
      for (ISystem* system : systemList)
      {
        if (system->GetName() == systemName)
        {
          return system;
        } 
      }
      return nullptr;
    }

    // destroy all systems from back to front
    Engine::~Engine()
    {
        auto systemBegin = systemList.rbegin();
        auto systemEnd = systemList.rend();
        while (systemBegin != systemEnd)
        {
            delete (*systemBegin);
            ++systemBegin;
        }
        // fudge it , we're done!
    }

    //Private

    void Engine::UpdateDt()
    {
        auto currTime = std::chrono::steady_clock::now();

        dt = std::chrono::duration_cast<std::chrono::duration<float>>(currTime - prevTime_).count();
        prevTime_ = currTime;

        dt = std::clamp(dt, 0.0f, 0.05f);
    }
}
