// Copyright Vibe Coding. All Rights Reserved.

#include <chrono>
#include <cstdlib>

#include "Application.h++"

IApplication::IApplication() : IsFailed(false)
{
    //TODO: Create window and input here
}

IApplication::~IApplication()
{
    // delete Input;
    // delete Graphics;
}

int IApplication::Run()
{
    if (IsFailed)
    {
        system("Pause");
        return 1;
    }

    using Time = std::chrono::high_resolution_clock;
    using Seconds = std::chrono::duration<float>;

    OnBegin();

    auto LastTick = Time::now();

    while (true /*!Input->IsWillQuit()*/)
    {
        // Input->Update();
        static uint32_t count = 0;
        ++count;
        if (count > 100)
            break;

        auto CurrentTick = Time::now();
        Seconds DeltaTime = CurrentTick - LastTick;
        LastTick = CurrentTick;

        OnUpdate(DeltaTime.count());

        // Graphics->Begin();
        // OnRender();
        // Graphics->End();
    }

    return 0;
}