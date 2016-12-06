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

#include <ma4lib/RemoteCalculator.hpp>

#include <boost/array.hpp>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;

struct RemoteCalculatorImpl
{
    boost::asio::io_service service;
    tcp::resolver::iterator endpoint;
};

RemoteCalculator::RemoteCalculator(std::string host, std::string port)
    :host_(host)
    ,port_(port)
    ,impl_(std::make_shared<RemoteCalculatorImpl>())
{
    tcp::resolver resolver(impl_->service);
    tcp::resolver::query query(tcp::v4(), host_, port_);
    impl_->endpoint = resolver.resolve(query);
}

void RemoteCalculator::calculate()
{
    data.resize(screenWidth * screenHeight);
    calculateAllData();
}

int32_t RemoteCalculator::calculate(float re, float im)
{
    auto& ioservice = impl_->service;
    auto& endpoint = impl_->endpoint;

    tcp::socket socket(ioservice);
    boost::asio::connect(socket, endpoint);
    boost::system::error_code ignoredError;

    //  send request
    float valbuffer[2] = { re, im };
    boost::asio::write(socket, boost::asio::buffer(valbuffer, sizeof(valbuffer)), ignoredError);

    std::stringstream ss;
    for (;;)
    {
        boost::array<char, 128> buf;
        boost::system::error_code error;

        size_t len = socket.read_some(boost::asio::buffer(buf), error);
        if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.

        ss.write(buf.data(), len);
    }
    //std::cout << re << " x " << im << "\tcalculate() result: " << ss.str() << std::endl;

    int32_t result;
    ss >> result;
    return result;
}

void RemoteCalculator::calculateAllData()
{
    for (int y = 0; y < screenHeight; ++y)
    {
        //std::cout << (y * 100) / screenHeight << "% done...\n";
        for (int x = 0; x < screenWidth; ++x)
        {
            auto iteration = iter_mandel(x, y);
            data[(screenWidth * y) + x] = iteration;
        }
    }
}

int32_t RemoteCalculator::iter_mandel(int cre, int cim)
{
    const float stepHorizontal = (offsetRight - offsetLeft) / static_cast<float>(screenWidth);
    const float stepVertical = (offsetTop - offsetBottom) / static_cast<float>(screenHeight);

    //  Pixel (0,0) is top left!
    const float re = offsetLeft + (stepHorizontal * cre);
    const float im = offsetTop - (stepVertical * cim);

    auto result = calculate(re, im);
    return result;
}
