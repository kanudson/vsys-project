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

#ifndef mavsys_vsys_h__
#define mavsys_vsys_h__

//  C++ Headers
#include <vector>
#include <algorithm>
#include <thread>
#include <future>
#include <memory>
#include <iostream>

//  C Headers
#include <cstdint>

//  3rd Party
#include <boost/property_tree/ini_parser.hpp>

typedef std::vector<std::string> StringVector;
typedef std::vector<int32_t> DataVector;

extern const char* CONFIG_SERVER_FILENAME;
extern const char* CONFIG_CLIENT_FILENAME;

StringVector parseArguments(int argc, char* argv[]);

boost::property_tree::ptree ReadServerConfig();
boost::property_tree::ptree ReadClientConfig();

constexpr int BROADCAST_PORT = 13337;
constexpr int REQUEST_PORT_BEGIN = 13340;

#endif // mavsys_vsys_h__
