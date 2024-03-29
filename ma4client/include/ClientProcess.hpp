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
#include <queue>

struct MandelbrotHost
{
    std::string ip;
    std::string port;
};

struct Job
{
    int32_t links, rechts, oben, unten;
};

class ClientProcess : public IProcess
{
public:
    ClientProcess(StringVector& args)
        : IProcess(args)
    {}

    int32_t init() override;
    int32_t run() override;
    int32_t shutdown() override;

    void sendBroadcast();

private:
    int factor = 1;
    int screenWidth = 160 * factor;
    int screenHeight = 120 * factor;

    int serverCount = 1;
    int iterations = 255;
    int jobstep = 30;

    float offsetLeft  = -2.5f,
         offsetRight  = 1.0f,
         offsetTop    = 1.0f,
         offsetBottom = -1.0f;

    std::vector<MandelbrotHost> hosts_;
    std::queue<Job> jobs_;
    static void remoteCalc(ClientProcess* obj, MandelbrotHost host, DataVector* data, int threadId, int threadCount);

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

    void createJobs();
    Job getJob();
    std::mutex jobMutex;

    DataVector createImage();

    void processImagePgmAscii(const DataVector& data, std::string filename);
    void processImagePgmBinary(const DataVector& data, std::string filename);
    void processImagePpmBinary(const DataVector& data, std::string filename);
};

#endif // mavsys_ServerProcess_h__
