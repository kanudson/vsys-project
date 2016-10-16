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

#ifndef ma4lib_TimeMeasure_h__
#define ma4lib_TimeMeasure_h__

#include <ma4lib/vsys.hpp>
#include <boost/chrono.hpp>

template <typename timing, typename Func, typename... Args>
timing measureTime(Func fct, Args...args)
{
    auto befor = boost::chrono::high_resolution_clock::now();

    fct(args...);

    auto after = boost::chrono::high_resolution_clock::now();
    auto duration = boost::chrono::duration_cast<timing>(after - befor);
    return duration;
}

//template <typename timing, typename Class, typename Meth, typename... Args>
//timing measureTime(Class& obj, Meth (Class::*mf)(Args...), Args &&...args)
//{
//    auto befor = boost::chrono::high_resolution_clock::now();
//
//    obj.*mf(std::forward<Args>(args)...);
//
//    auto after = boost::chrono::high_resolution_clock::now();
//    auto duration = boost::chrono::duration_cast<timing>(after - befor);
//    return duration.count();
//}

template <typename timing, typename Class, typename Method, typename... Args>
timing measureTime(Class& obj, Method mf, Args... args)
{
    auto befor = boost::chrono::high_resolution_clock::now();

    (obj.*mf)(args...);

    auto after = boost::chrono::high_resolution_clock::now();
    auto duration = boost::chrono::duration_cast<timing>(after - befor);
    return duration;
}

#endif // ma4lib_TimeMeasure_h__
