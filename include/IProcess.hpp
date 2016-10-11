/*
 * =====================================================================================
 *
 *       Filename:  IProcess.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/11/2016 08:39:47 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paul Thieme (thimepau), kanudson<at>gmail<dot>com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <vsys.hpp>

/*
 * =====================================================================================
 *        Class:  IProcess
 *  Description:  
 * =====================================================================================
 */
class IProcess
{
    public:
        /* ====================  LIFECYCLE     ======================================= */
        virtual ~IProcess() {}

        /* ====================  ACCESSORS     ======================================= */
        inline auto getArguments()
        {
            return args_;
        }

        static std::vector<std::string> parseArguments(int argc, char* argv[])
        {
            std::vector<std::string> args(argc);

            for (std::size_t i = 0; i < argc; ++i)
            {
                args[i] = std::string(argv[i]);
            }

            return args;
        }

        /* ====================  MUTATORS      ======================================= */
        virtual int32_t run() = 0;

        /* ====================  OPERATORS     ======================================= */

    protected:
        /* ====================  METHODS       ======================================= */
        IProcess(std::vector<std::string> args)
            :
                args_(args)
        {}

        /* ====================  DATA MEMBERS  ======================================= */
        const std::vector<std::string> args_;

    private:
        /* ====================  METHODS       ======================================= */
        IProcess(const IProcess&) = delete;
        IProcess(const IProcess&&) = delete;
        
        /* ====================  DATA MEMBERS  ======================================= */

}; /* -----  end of class IProcess  ----- */

