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
#include <ma4lib/TimeMeasure.hpp>
#include <ma4lib/CpuCalculator.hpp>
#include <UdpServer.hpp>
#include <TcpServer.hpp>

#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <fstream>
#include <algorithm>

using boost::asio::ip::tcp;
constexpr int HOSTPORT = 40123;

const char* pgmFileAscii = "imgascii.pgm";
const char* pgmFileBinary = "imgbin.pgm";

int32_t ServerProcess::init()
{
    return 0;
}

int32_t ServerProcess::run()
{
    uint16_t tcpport = REQUEST_PORT_BEGIN + std::atoi(args_[1].c_str());

    UdpServer udp(ioservice_, tcpport);
    TcpServer tcp(ioservice_, tcpport);
 
    std::cout << "server is up (" << tcpport << "), waiting for connections..." << std::endl;
    ioservice_.run();

    //tcp::acceptor acceptor(ioservice_, tcp::endpoint(tcp::v4(), HOSTPORT));
    //for (;;)
    //{
    //    //  wait for clients to connect
    //    tcp::socket socket(ioservice_);
    //    acceptor.accept(socket);

    //    processRequest(std::move(socket));
    //}

    return 42;
}

int32_t ServerProcess::shutdown()
{
    return 0;
}

void ServerProcess::processRequest(boost::asio::ip::tcp::socket socket)
{
    boost::array<char, 12> buf;
    boost::system::error_code ignoredError;

    int bytesRead = 0;
    do 
    {
        bytesRead += socket.read_some(boost::asio::buffer(buf), ignoredError);
    } while (bytesRead < 12);

    float re = *(reinterpret_cast<float*>(&(*buf.data())));
    float im = *(reinterpret_cast<float*>(&(*buf.data()) + 4));
    int32_t iterations = *(reinterpret_cast<int32_t*>(&(*buf.data()) + 8));

    CpuCalculator calc;
    calc.setMaxIterations(iterations);
    auto result = calc.calculate(re, im);

    std::string msg = std::to_string(result);
    boost::asio::write(socket, boost::asio::buffer(msg), ignoredError);
    //std::cout << "got re\t" << re << "\tgot im\t" << im << "\tres is\t" << result << "\tused " << iterations << "\n";
}
