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
#include <ma4lib/ICalculator.hpp>

class ClientProcess : public IProcess
{
public:
    ClientProcess(StringVector& args)
        : IProcess(args)
    {}

    int32_t init() override;
    int32_t run() override;
    int32_t shutdown() override;

private:
    static constexpr int factor = 10;
    const int screenWidth = 320 * factor;
    const int screenHeight = 240 * factor;

    //  1080p is ~2.25x of 720p
    //
    //  ~1050ms calculating
    //  ~525ms processing
    //  2,073,600
    //

    //  ~470ms
    //  921,600
    //const int screenWidth = 1280;
    //const int screenHeight = 720;

    bool keepRunning_;

    DataVector createImage();

    void processImagePgmAscii(const DataVector& data, std::string filename);
    void processImagePgmBinary(const DataVector& data, std::string filename);
    void processImagePpmBinary(const DataVector& data, std::string filename);
};

#endif // mavsys_ServerProcess_h__
