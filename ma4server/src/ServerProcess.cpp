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
#include <CpuCalculator.hpp>

#include <boost/thread.hpp>
#include <boost/asio.hpp>
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
    std::cout << "server is up, waiting for connections..." << std::endl;

    tcp::acceptor acceptor(ioservice_, tcp::endpoint(tcp::v4(), HOSTPORT));
    for (;;)
    {
        tcp::socket socket(ioservice_);
        acceptor.accept(socket);

        std::string msg = "hello world frpm server";
        boost::system::error_code ignoredError;
        boost::asio::write(socket, boost::asio::buffer(msg), ignoredError);
    }

    return 42;
}

int32_t ServerProcess::shutdown()
{
    return 0;
}
