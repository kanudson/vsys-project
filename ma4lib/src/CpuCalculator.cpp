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

#include <ma4lib/CpuCalculator.hpp>

void CpuCalculator::calculate()
{
    data.resize(screenWidth * screenHeight);

    run();
    //run_openmp();
}

int32_t CpuCalculator::calculate(float re, float im)
{
    float r = 0.0f;
    float i = 0.0f;
    int32_t iteration = 0;

    //  while (x*x + y*y < 2*2  AND  iteration < max_iteration) {
    auto rs = r * r;
    auto is = i * i;
    while (rs + is < 4 && iteration < maxIterations)
    {
        float xtemp = rs - is + re;
        i = 2 * r * i + im;
        r = xtemp;
        ++iteration;

        //  refresh data for next iterations
        rs = r * r;
        is = i * i;
    }

    return iteration;
}

void CpuCalculator::run()
{
    for (int y = 0; y < screenHeight; ++y)
        for (int x = 0; x < screenWidth; ++x)
        {
            auto iteration = iter_mandel(x, y);
            data[(screenWidth * y) + x] = iteration;
        }
}

void CpuCalculator::run_openmp()
{
#pragma omp parallel for schedule(static, 40)
    for (int y = 0; y < screenHeight; ++y)
        for (int x = 0; x < screenWidth; ++x)
        {
            auto iteration = iter_mandel(x, y);
            data[(screenWidth * y) + x] = iteration;
        }
}

int32_t CpuCalculator::iter_mandel(int cre, int cim)
{
    const float stepHorizontal = (offsetRight - offsetLeft) / static_cast<float>(screenWidth);
    const float stepVertical = (offsetTop - offsetBottom) / static_cast<float>(screenHeight);

    //  Pixel (0,0) is top left!
    const float re = offsetLeft + (stepHorizontal * cre);
    const float im = offsetTop - (stepVertical * cim);

    auto result = calculate(re, im);
    return result;
}
