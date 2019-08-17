// KRATOS  / ___|___/ ___|(_)_ __ ___  _   _| | __ _| |_(_) ___  _ ___
//        | |   / _ \___ \| | '_ ` _ \| | | | |/ _` | __| |/ _ \| '_  |
//        | |__| (_) |__) | | | | | | | |_| | | (_| | |_| | (_) | | | |
//         \____\___/____/|_|_| |_| |_|\__,_|_|\__,_|\__|_|\___/|_| |_|
//
//  License:		 BSD License
//                   license: CoSimulationApplication/license.txt
//
//  Main authors:    Philipp Bucher
//

#ifndef KRATOS_CO_SIM_IO_HELPERS_H_INCLUDED
#define KRATOS_CO_SIM_IO_HELPERS_H_INCLUDED

/*
This file has the same API as EMPIRE (defined in "EMPIRE_API.h"), hence it can be included instead of EMPIRE
It is used FileIO for data-exchange, in VTK-format
Note:
- This file cannot have Kratos-includes, because it is also included in other codes!
- This file is intended to be header-only, such that other codes do not have to link against a library
- Requires c++11 to compile
- All functions have to be static, otherwise linking errors occur if it is included in multiple files
*/

// System includes
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>

namespace CoSimulationIOHelpers {

#define CO_SIM_IO_LOG(conditional) if(conditional) std::cout << "[CoSimIO] "

static double ElapsedSeconds(const std::chrono::steady_clock::time_point& rStartTime)
{
    using namespace std::chrono;
    return duration_cast<duration<double>>(steady_clock::now() - rStartTime).count();
}

static void SleepForMilliseconds(const int SecondsToSleep)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(SecondsToSleep));
}

static bool FileExists(const std::string& rFileName)
{
    std::ifstream infile(rFileName);
    return infile.good(); // no need to close manually
}

static std::string GetTempFileName(const std::string& rFileName)
{
    return "." + rFileName;
}

static void WaitForFile(const std::string& rFileName)
{
    CO_SIM_IO_LOG(1) << "Waiting for file: \"" << rFileName << "\"" << std::endl;
    while(!FileExists(rFileName)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // wait 0.5s before next check
        CO_SIM_IO_LOG(3) << "    Waiting" << std::endl;
    }
    CO_SIM_IO_LOG(1) << "Found file: \"" << rFileName << "\"" << std::endl;
}

static void RemoveFile(const std::string& rFileName)
{
    if (std::remove(rFileName.c_str()) != 0) {
        CO_SIM_IO_LOG(0) << "Warning: \"" << rFileName << "\" could not be deleted!" << std::endl;
    }
}

static void MakeFileVisible(const std::string& rFinalFileName)
{
    if (std::rename(GetTempFileName(rFinalFileName).c_str(), rFinalFileName.c_str()) != 0) {
        CO_SIM_IO_LOG(0) << "Warning: \"" << rFinalFileName << "\" could not be made visible!" << std::endl;
    }
}

template <typename T>
static void CheckStream(const T& rStream, const std::string& rFileName)
{
    if (!rStream.is_open()) {
        std::stringstream err_msg;
        err_msg << rFileName << " could not be opened!";
        throw std::runtime_error(err_msg.str());
    }
}
} // namespace CoSimulationIOHelpers

#endif // KRATOS_CO_SIM_IO_HELPERS_H_INCLUDED
