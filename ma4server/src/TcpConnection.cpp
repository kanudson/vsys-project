// MIT License
//
// Copyright (c) 2016 Kanudson
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

#include <TcpConnection.hpp>
#include <ma4lib/CpuCalculator.hpp>

void TcpConnection::start()
{
    boost::array<char, 12> buf;
    boost::system::error_code ignoredError;

    int bytesRead = 0;
    do
    {
        //  TODO buf + bytesRead beim erneuten read_some
        bytesRead += socket_.read_some(boost::asio::buffer(buf), ignoredError);
    } while (bytesRead < 12);

    float re = *(reinterpret_cast<float*>(&(*buf.data())));
    float im = *(reinterpret_cast<float*>(&(*buf.data()) + 4));
    int32_t iterations = *(reinterpret_cast<int32_t*>(&(*buf.data()) + 8));

    CpuCalculator calc;
    calc.setMaxIterations(iterations);
    auto result = calc.calculate(re, im);

    std::string msg = std::to_string(result);
    boost::asio::write(socket_, boost::asio::buffer(msg), ignoredError);
    std::cout << "got re\t" << re << "\tgot im\t" << im << "\tres is\t" << result << "\tused " << iterations << "\n";
}

void TcpConnection::handleRecieve()
{}

void TcpConnection::handleWrite()
{
}