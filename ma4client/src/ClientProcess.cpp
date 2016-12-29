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

#include <ClientProcess.hpp>
#include <ma4lib/CpuCalculator.hpp>
#include <ma4lib/RemoteCalculator.hpp>
#include <ma4lib/TimeMeasure.hpp>

#include <boost/thread.hpp>
#include <fstream>
#include <algorithm>
#include <thread>
#include <future>

#include <boost/array.hpp>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;
using boost::asio::ip::udp;

const char* pgmFileAscii = "imgascii.pgm";
const char* pgmFileBinary = "imgbin.pgm";
const char* ppmFileColor = "imgcolor.ppm";

const char* tcpHostname = "localhost";
const char* tcpPort = "40123";

int32_t ClientProcess::init()
{
    return 0;
}

int32_t ClientProcess::run()
{
    for (const std::string& str : args_)
        std::cout << str.c_str() << std::endl;

    sendBroadcast();
    auto imgdata = createImage();
    //processImagePgmAscii(imgdata, pgmFileAscii);
    processImagePgmBinary(imgdata, pgmFileBinary);
    processImagePpmBinary(imgdata, ppmFileColor);

    return 42;
}

int32_t ClientProcess::shutdown()
{
    return 0;
}

void ClientProcess::sendBroadcast()
{
    std::string ipAddrStr = "192.168.2.255";
    try
    {
        boost::asio::io_service io_service;
        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), ipAddrStr, std::to_string(BROADCAST_PORT));
        udp::endpoint receiver_endpoint = *resolver.resolve(query);

        udp::socket socket(io_service);
        socket.open(udp::v4());

        boost::array<char, 1> send_buf = { 0 };
        socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

        for (std::size_t i = 0; i < 3; ++i)
        {
            boost::array<char, 128> recv_buf;
            udp::endpoint sender_endpoint;
            size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

            std::cout.write(recv_buf.data(), len);
            std::cout << "\n" << sender_endpoint.address().to_string() << "\n";

            //  convert buf to string
            std::stringstream ss;
            ss.write(recv_buf.data(), len);

            MandelbrotHost host = {sender_endpoint.address().to_string(), ss.str()};
            hosts_.push_back(host);
        }
        std::cout << "\n\n";
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

}

void ClientProcess::remoteCalc(ClientProcess* obj, MandelbrotHost host, DataVector* data, int threadId, int threadCount)
{
    assert(threadId < threadCount);

    RemoteCalculator calc(host.ip, host.port);

    int heightBlock = obj->screenHeight / threadCount;
    int start = heightBlock * threadId;
    int end = heightBlock * (threadId + 1);

    if (threadId + 1 == threadCount)
    {
        end = obj->screenHeight;
    }

    for (int y = start; y < end; ++y)
    {
        std::cout << "thread #" << threadId << "\t" << ((y - start) * 100) / heightBlock << "% done...\n";
        for (int x = 0; x < obj->screenWidth; ++x)
        {
            const float stepHorizontal = (obj->offsetRight - obj->offsetLeft) / static_cast<float>(obj->screenWidth);
            const float stepVertical = (obj->offsetTop - obj->offsetBottom) / static_cast<float>(obj->screenHeight);

            //  Pixel (0,0) is top left!
            const float re = obj->offsetLeft + (stepHorizontal * x);
            const float im = obj->offsetTop - (stepVertical * y);

            auto result = calc.calculate(re, im);
            (*data)[(obj->screenWidth * y) + x] = result;
        }
    }
}

DataVector ClientProcess::createImage()
{
    const int maxThreads = 3;
    const int iterations = 0xFF;
    DataVector data;
    data.resize(screenWidth * screenHeight);

    std::vector<std::thread> threads(maxThreads);
    for (int i = 0; i < maxThreads; ++i)
    {
        std::thread th(&ClientProcess::remoteCalc, this, hosts_[i], &data, i, maxThreads);
        threads[i] = std::move(th);
    }

    for (auto& thread : threads)
        thread.join();

    //remoteCalc(this, hosts_[0], data, 0, 1);

    return data;

    RemoteCalculator calc("localhost", std::to_string(REQUEST_PORT_BEGIN + 1));
    calc.setScreenWidth(screenWidth);
    calc.setScreenHeight(screenHeight);
    calc.setMaxIterations(iterations);
    calc.setOffsetTop(1.0f);
    calc.setOffsetBottom(-1.0f);
    calc.setOffsetLeft(-2.5f);
    calc.setOffsetRight(1.0f);

    auto lambda = [&]()
    {
        calc.calculate();
    };

    //  laeuft nicht bei gcc-5.2?!
    //  deshalb mit lambda Funktion
    //typedef void (CpuCalculator::*FuncPtr)();
    //constexpr FuncPtr fkt = &CpuCalculator::calculate;
    //auto duration = measureTime<boost::chrono::milliseconds>(calc, fkt);

    auto duration = measureTime<boost::chrono::milliseconds>(lambda);
    std::cout << "createImage()... (took " << duration.count() << "ms)\n";

    return calc.getData();
}

void ClientProcess::processImagePgmAscii(const DataVector& data, std::string filename)
{
    //  logic inside lambda for measureTime<> template
    auto lambda = [&]()
    {
        std::fstream file(filename, std::ios::out);
        std::stringstream ss;

        //  fill buffer with data
        ss << "P2\n";
        ss << screenWidth << "\n";
        ss << screenHeight << "\n";
        ss << static_cast<int>(255) << "\n";
        for (auto& value : data)
            ss << value << "\n";

        file << ss.str();
    };

    auto duration = measureTime<boost::chrono::milliseconds>(lambda);
    std::cout << "processImagePgmAscii... (took " << duration.count() << "ms)\n";
}

void ClientProcess::processImagePgmBinary(const DataVector& data, std::string filename)
{
    //  logic inside lambda for measureTime<> template
    auto lambda = [&]()
    {
        std::fstream file(filename, std::ios::out | std::ios::binary);

        file << "P5\n";
        file << screenWidth << "\n";
        file << screenHeight << "\n";
        file << static_cast<int>(255) << " ";

        std::vector<char> buffer(screenHeight * screenWidth);
        auto iter = buffer.begin();

        //  TODO funktioniert noch nicht
        //  bild passt ganz und gar nicht

        for (auto& value : data)
        {
            *iter++ = std::max(std::min(value, 0xFF), 0);
            //*iter++ = '\n';
        }

        file.write(buffer.data(), buffer.size());
    };

    auto duration = measureTime<boost::chrono::milliseconds>(lambda);
    std::cout << "processImagePgmBinary... (took " << duration.count() << "ms)\n";
}

void ClientProcess::processImagePpmBinary(const DataVector& data, std::string filename)
{
    std::cout << "processImagePpmAscii()";

    //  logic inside lambda for measureTime<> template
    auto lambda = [&]()
    {
        std::fstream file(filename, std::ios::out | std::ios::binary);

        //  fill buffer with data
        file << "P6\n";
        file << screenWidth << " ";
        file << screenHeight << "\n";
        file << static_cast<int>(255) << "\n";

        std::vector<char> buffer(screenHeight * screenWidth * 3);
        auto iter = buffer.begin();

        for (auto& value : data)
        {
            //  TODO maxIterations konfigurierbar machen
            if (value >= 255)
            {
                *iter++ = 0xff;
                *iter++ = 0xff;
                *iter++ = 0xff;
            }
            else
            {
                //  rewrote this without modulo operand
                //*iter++ = value % 255;
                //*iter++ = value % 32;
                //*iter++ = value % 4;

                *iter++ = value & 0xFF;
                *iter++ = value % 0x1F;
                *iter++ = value % 0x03;
            }
        }
        file.write(buffer.data(), buffer.size());
    };

    auto duration = measureTime<boost::chrono::milliseconds>(lambda);
    std::cout << "processImagePgm... (took " << duration.count() << "ms)\n";
}
