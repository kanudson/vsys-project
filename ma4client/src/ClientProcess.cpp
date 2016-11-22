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

int32_t ClientProcess::init()
{
    if (useSdl)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
            return 1;

        window_ = SDL_CreateWindow("ma4vsys - Mandelbrot",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   screenWidth, screenHeight,
                                   0);
        if (!window_)
            return 2;

        render_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
        if (!render_)
            return 4;
    }
    return 0;
}

int32_t ClientProcess::run()
{
    std::cout << "running client, connecting..." << std::endl;

    boost::asio::io_service ioservice;

    tcp::resolver resolver(ioservice);
    tcp::resolver::query query(tcp::v4(), "localhost", "40123");
    const tcp::resolver::iterator iter = resolver.resolve(query), end;

    for (auto loc = iter; loc != end; loc++)
    {
        boost::asio::ip::tcp::endpoint endpoint = *loc;
        std::cout << endpoint << std::endl;
    }

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

    //  nop nop nop
    return 0;

    for (const std::string& str : args_)
        std::cout << str.c_str() << std::endl;

    auto imgdata = createImage();
    processImagePgmAscii(imgdata, pgmFileAscii);
    processImagePgmBinary(imgdata, pgmFileBinary);

    if (useSdl)
    {
        //processImageSdl(imgdata);
        keepRunning_ = false;
        do
        {
            auto time = boost::posix_time::milliseconds(5);
            boost::this_thread::sleep(time);

            processEvents();
            SDL_RenderPresent(render_);
        } while (keepRunning_);
    }

    return 42;
}

int32_t ClientProcess::shutdown()
{
    if (useSdl)
    {
        SDL_DestroyRenderer(render_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }
    return 0;
}

void ClientProcess::processEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            keepRunning_ = false;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                keepRunning_ = false;
                break;

            case SDLK_s:
                {
                    auto data = createImage();
                    processImageSdl(data);
                }
                break;

            case SDLK_p:
                {
                    auto data = createImage();
                    processImagePgmAscii(data, pgmFileAscii);
                }

            default:
                break;
            }
        }
    }
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

void ClientProcess::processImageSdl(const DataVector& data)
{
    std::cout << "processImageSdl... ";

    //  casted into a lambda to enable easier time measuring
    auto lambda = [&]()
    {
        for (int y = 0; y < screenHeight; ++y)
            for (int x = 0; x < screenWidth; ++x)
            {
                DataVector::value_type fullcolor = data[(screenWidth * y) + x];

                uint8_t color;
                if (fullcolor > 0x000000FF)
                    color = 0xFF;
                else if (fullcolor < 0)
                    color = 0;
                else
                    color = static_cast<uint8_t>(fullcolor);

                //std::cout << std::to_string(color) << "\n\n";
                SDL_SetRenderDrawColor(render_, color, color, color, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(render_, x, y);
            }
    };

    auto duration = measureTime<boost::chrono::milliseconds>(lambda);
    std::cout << "(took " << duration.count() << "ms)\n";
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
