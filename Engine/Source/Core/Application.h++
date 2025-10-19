// Copyright Vibe Coding. All Rights Reserved.

#pragma once

class IApplication
{
public:
    IApplication();
    virtual ~IApplication();

    int Run();

    virtual void OnBegin() = 0;
    virtual void OnUpdate(float InDeltaTime) = 0;
    // virtual void OnRender() = 0;
    // virtual void OnEnd() = 0;

protected:
    // Graphics* Graphics;
    // Input* Input;

private:
    // SDL_Window *Window;
    bool IsFailed;
};