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

#ifndef UdpServer_h__
#define UdpServer_h__

#include <ma4lib/vsys.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>

class UdpServer
{
public:
    UdpServer(boost::asio::io_service& ioservice, const int port, const int replyport)
        :port_(port)
        ,reply_(std::to_string(replyport))
        ,ioservice_(ioservice)
        ,socket_(ioservice, 
                 boost::asio::ip::udp::endpoint(
                 boost::asio::ip::udp::v4(), port))
    {
        startRecieve();
        std::cout << "udp running on " << port_ << " for tcp port " << replyport << std::endl;
    }

private:
    void startRecieve()
    {
        //  start receiving on udp port
        //  register callback function 'handleRecieve' with 'this' object
        socket_.async_receive_from(
            boost::asio::buffer(recvBuffer_),
            endpoint_,
            boost::bind(&UdpServer::handleRecieve,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }

    void handleRecieve(const boost::system::error_code& error, std::size_t bytesTransferred)
    {
        if (!error || error == boost::asio::error::message_size)
        {
            socket_.async_send_to(boost::asio::buffer(reply_),
                                  endpoint_,
                                  boost::bind(&UdpServer::handleSend,
                                  this,
                                  boost::asio::placeholders::error,
                                  boost::asio::placeholders::bytes_transferred));

            startRecieve();
        }
    }

    void handleSend(const boost::system::error_code& error, std::size_t bytesTransferred)
    {
        std::cout << "send broadcast reply to someone\n";
    }

    const int port_;
    const std::string reply_;
    boost::asio::io_service& ioservice_;
    boost::asio::ip::udp::socket socket_;

    //  temporary data for incoming connections
    boost::asio::ip::udp::endpoint endpoint_;
    boost::array<char, 10> recvBuffer_;
};

#endif // UdpServer_h__
