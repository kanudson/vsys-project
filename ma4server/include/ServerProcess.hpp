// MIT License
//
// Copyright (c) 2016 Paul Thieme
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef mavsys_ServerProcess_h__
#define mavsys_ServerProcess_h__

#include <ma4lib/vsys.hpp>
#include <ma4lib/IProcess.hpp>

#include <SDL2/SDL.h>

class ServerProcess : public IProcess
{
public:
    ServerProcess(StringVector& args)
        : IProcess(args)
    {}

    int32_t init() override;
    int32_t run() override;
    int32_t shutdown() override;

private:
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    SDL_Window* window_ = nullptr;
    SDL_Renderer* render_ = nullptr;
    SDL_Surface* surface_ = nullptr;
    bool keepRunning_;

    void processEvents();
    void processImage();
};

#endif // mavsys_ServerProcess_h__
