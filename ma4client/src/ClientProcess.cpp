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
#include <ma4lib/TimeMeasure.hpp>

#include <boost/thread.hpp>
#include <fstream>
#include <algorithm>

#include <boost/array.hpp>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;

const char* pgmFileAscii = "imgascii.pgm";
const char* pgmFileBinary = "imgbin.pgm";

const char* tcpHostname = "localhost";
const char* tcpPort = "40123";

int32_t ClientProcess::init()
{
    return 0;
}

int32_t ClientProcess::run()
{
    std::cout << "running client..." << std::endl;

    boost::asio::io_service ioservice;
    tcp::resolver resolver(ioservice);
    tcp::resolver::query query(tcp::v4(), tcpHostname, tcpPort);
    const tcp::resolver::iterator iter = resolver.resolve(query), end;

    std::cout << "looking for hosts... found:\n";
    for (auto loc = iter; loc != end; loc++)
    {
        boost::asio::ip::tcp::endpoint endpoint = *loc;
        std::cout << "\t" << endpoint << std::endl;
    }

    std::cout << "connecting...\n";
    tcp::socket socket(ioservice);
    boost::asio::connect(socket, iter);

    for (;;)
    {
        boost::array<char, 128> buf;
        boost::system::error_code error;

        size_t len = socket.read_some(boost::asio::buffer(buf), error);
        if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.

        std::cout.write(buf.data(), len);
    }

    return 0;

    for (const std::string& str : args_)
        std::cout << str.c_str() << std::endl;

    auto imgdata = createImage();
    processImagePgmAscii(imgdata, pgmFileAscii);
    processImagePgmBinary(imgdata, pgmFileBinary);

    return 42;
}

int32_t ClientProcess::shutdown()
{
    return 0;
}

DataVector ClientProcess::createImage()
{
    const int iterations = 0xFF;

    std::cout << "createImage()... ";
    //CpuCalculator calc;
    //calc.setScreenWidth(screenWidth);
    //calc.setScreenHeight(screenHeight);
    //calc.setMaxIterations(iterations);
    //calc.setOffsetTop(1.0f);
    //calc.setOffsetBottom(-1.0f);
    //calc.setOffsetLeft(-2.5f);
    //calc.setOffsetRight(1.0f);

    //typedef void (CpuCalculator::*FuncPtr)();
    //constexpr FuncPtr fkt = &CpuCalculator::calculate;
    //auto duration = measureTime<boost::chrono::milliseconds>(calc, fkt);
    //std::cout << "(took " << duration.count() << "ms)\n";

    //return calc.getData();
    DataVector res;
    return res;

}

void ClientProcess::processImagePgmAscii(const DataVector& data, std::string filename)
{
    std::cout << "processImagePgm...";

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
    std::cout << "(took " << duration.count() << "ms)\n";
}

void ClientProcess::processImagePgmBinary(const DataVector& data, std::string filename)
{
    std::cout << "processImagePgm...";

    auto lambda = [&]()
    {
        std::fstream file(filename, std::ios::out);

        file << "P5\n";
        file << screenWidth << "\n";
        file << screenHeight << "\n";
        file << static_cast<int>(255) << "\n";

        std::vector<char> buffer(screenHeight * screenWidth * 2);
        auto iter = buffer.begin();

        for (auto& value : data)
        {
            *iter++ = std::max(std::min(value, 0xFF), 0);
            *iter++ = '\n';
        }

        file.write(buffer.data(), buffer.size());
    };

    auto duration = measureTime<boost::chrono::milliseconds>(lambda);
    std::cout << "(took " << duration.count() << "ms)\n";
}
