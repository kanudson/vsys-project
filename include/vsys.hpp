/*
 * =====================================================================================
 *
 *       Filename:  vsys.hpp
 *
 *    Description:  Includes headers used in VSys project.
 *
 *        Version:  1.0
 *        Created:  10/11/2016 08:31:25 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paul Thieme (thimepau), kanudson<at>gmail<dot>com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef VSYS_HPP__

#include <vector>
#include <algorithm>
#include <thread>
#include <future>
#include <memory>
#include <iostream>

#include <cstdint>

typedef std::vector<std::string> StringVector;

StringVector parseArguments(int argc, char* argv[]);

#endif  //  VSYS_HPP__

