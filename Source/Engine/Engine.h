/*!*****************************************************************************
\par filename: Engine.h
\par  authors: Ashkon Khalkhali
\par     date: 09/11/2022

\brief
  This file contains the class and public declarations for
  our Engine.
  Copyright © 2022 DigiPen (USA) Corporation.
*******************************************************************************/
#pragma once
#include "ISystem.h"
#include <vector>
#include <chrono>
#include "Trace.h"

namespace FFunctional
{
    class Engine
    {
    public:
        float GetDt() const;

        bool IsRunning() const;

        void SetLastLevel(int lvl);

        int GetLastLevel();

        void Stop();

        void Add(ISystem* system);

        void Init();

        void Update();

        void Draw() const;

        Trace& GetTrace();

        ISystem* GetSystem(std::string_view systemName) const;

        //template argument will be replaced with type where you call it.
        template <typename Sys> 
        Sys* GetSystem() const { 
          return dynamic_cast<Sys*>(GetSystem(Sys::Name()));
        };
        ~Engine();
    private:
        void UpdateDt();
        std::vector<ISystem*> systemList{};
        bool isActive_ = false;
        float dt;
        std::chrono::steady_clock::time_point prevTime_;
        Trace tFile{};
        int lastLevel_ = -3;
    };
}
