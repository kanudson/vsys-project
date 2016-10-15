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

#include <ServerProcess.hpp>
#include <CpuCalculator.hpp>
#include <boost/thread.hpp>

int32_t ServerProcess::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return 1;

    window_ = SDL_CreateWindow("ma4vsys - Mandelbrot",
                     SDL_WINDOWPOS_CENTERED,
                     SDL_WINDOWPOS_CENTERED,
                     screenWidth, screenHeight,
                     0);
    if (!window_)
        return 2;

    render_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!render_)
        return 4;

    return 0;
}

int32_t ServerProcess::run()
{
    std::cout << "running shit" << std::endl;

    for (const std::string& str : args_)
        std::cout << str.c_str() << std::endl;

    processImage();

    keepRunning_ = true;
    do
    {
        auto time = boost::posix_time::milliseconds(5);
        boost::this_thread::sleep(time);

        processEvents();
        SDL_RenderPresent(render_);
    } while (keepRunning_);

    return 42;
}

int32_t ServerProcess::shutdown()
{
    SDL_DestroyRenderer(render_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
    return 0;
}

void ServerProcess::processEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            keepRunning_ = false;
        }
        else if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_ESCAPE))
        {
            keepRunning_ = false;
        }
    }
}

void ServerProcess::processImage()
{
    const int iterations = 0xFF;

    std::cout << "calc...";
    CpuCalculator calc;
    calc.setScreenWidth(screenWidth);
    calc.setScreenHeight(screenHeight);
    calc.setMaxIterations(iterations);
    calc.setOffsetTop(1.0f);
    calc.setOffsetBottom(-1.0f);
    calc.setOffsetLeft(-2.5f);
    calc.setOffsetRight(1.0f);
    calc.calculate();

    std::cout << "processing... ";
    DataVector data = calc.getData();
    for (int y = 0; y < screenHeight; ++y)
        for (int x = 0; x < screenWidth; ++x)
        {
            DataVector::value_type fullcolor = data[(screenWidth * y) + x];

            uint8_t color;
            if (fullcolor > 0x000000FF)
                color = 0xFF;
            else if (fullcolor < 0)
                color = 0;
            else
                color = static_cast<uint8_t>(fullcolor);

            //std::cout << std::to_string(color) << "\n\n";
            SDL_SetRenderDrawColor(render_, color, color, color, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(render_, x, y);
        }

    std::cout << "done!\n";
}
