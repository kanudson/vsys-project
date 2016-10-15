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
#include <boost/thread.hpp>

int32_t ServerProcess::init()
{
    const int width = 1280;
    const int height = 720;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return 1;

    window_ = SDL_CreateWindow("ma4vsys - Mandelbrot",
                     SDL_WINDOWPOS_CENTERED,
                     SDL_WINDOWPOS_CENTERED,
                     width, height,
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
    std::cout << "Surface ptr " << surface_ << std::endl;

    for (const std::string& str : args_)
        std::cout << str.c_str() << std::endl;

    std::cout << "calc...";
    for (int y = 0; y < 720; ++y)
        for (int x = 0; x < 1280; ++x)
        {
            processPixel(x, y, 1280, 720,
                         -2.5f, 1.0f, 1.f, -1.0f, 255);
        }
    std::cout << " done!" << std::endl;

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

void ServerProcess::processPixel(int pixelx, int pixely,
                                 int screenWidth, int screenHeight,
                                 float offsetLeft, float offsetTop,
                                 float offsetRight, float offsetBottom, int iterations)
{
    const float stepHorizontal = (offsetRight - offsetLeft) / static_cast<float>(screenWidth);
    const float stepVertical = (offsetTop - offsetBottom) / static_cast<float>(screenHeight);
    const int MAX_ITERATIONS = iterations;

    //  Pixel (0,0) is top left!
    const float re = offsetLeft + (stepHorizontal * pixelx);
    const float im = offsetTop - (stepVertical * pixely);

    //std::cout << pixelx << " : " << pixely << "\n";
    //std::cout << re << " : " << im << "\n";

    float r = 0.0f;
    float i = 0.0f;
    int iteration = 0;

    //  while (x*x + y*y < 2*2  AND  iteration < max_iteration) {
    auto rs = r * r;
    auto is = i * i;
    while (rs + is < 4 && iteration < MAX_ITERATIONS)
    {
        float xtemp = rs - is + re;
        i = 2 * r * i + im;
        r = xtemp;
        ++iteration;

        //  refresh data for next iterations
        rs = r * r;
        is = i * i;
    }

    uint8_t color = iteration;
    //std::cout << std::to_string(color) << "\n\n";
    SDL_SetRenderDrawColor(render_, color, color, color, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(render_, pixelx, pixely);
}
