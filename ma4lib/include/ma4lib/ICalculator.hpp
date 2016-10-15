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

#ifndef ma4lib_ICalculator_h__
#define ma4lib_ICalculator_h__

#include <ma4lib/vsys.hpp>

class ICalculator
{
public:
    virtual void calculate() = 0;

    virtual ~ICalculator()
    {}

    void setScreenWidth(int32_t val)
    {
        screenWidth = val;
    }

    void setScreenHeight(int32_t val)
    {
        screenHeight = val;
    }

    void setOffsetLeft(float val)
    {
        offsetLeft = val;
    }

    void setOffsetRight(float val)
    {
        offsetRight = val;
    }

    void setOffsetTop(float val)
    {
        offsetTop = val;
    }

    void setOffsetBottom(float val)
    {
        offsetBottom = val;
    }

    void setMaxIterations(int32_t val)
    {
        maxIterations = val;
    }

    DataVector getData()
    {
        return data;
    }

protected:
    DataVector data;

    int32_t screenWidth, screenHeight;
    float offsetLeft, offsetRight, offsetTop, offsetBottom;
    int32_t maxIterations;
};

#endif // ma4lib_ICalculator_h__
