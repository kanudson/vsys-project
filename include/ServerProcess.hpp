/*
 * =====================================================================================
 *
 *       Filename:  ServerProcess.hpp
 *
 *    Description:  main process that runs on the Server
 *
 *        Version:  1.0
 *        Created:  10/14/2016 07:09:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paul Thieme (thimepau), kanudson<at>gmail<dot>com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef SERVER_PROCESS_HPP__
#define SERVER_PROCESS_HPP__

#include <vsys.hpp>
#include <IProcess.hpp>

class ServerProcess : public IProcess
{
    public:
        ServerProcess(StringVector& args)
            : IProcess(args)
        {}

        int32_t run() override
        {
            std::cout << "running shit" << std::endl;

            for (auto& str : args_)
                std::cout << str << std::endl;

            return 42;
        }
};

#endif  //  SERVER_PROCESS_HPP__

