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

#ifndef KRATOS_CO_SIM_BASE_IO_H_INCLUDED
#define KRATOS_CO_SIM_BASE_IO_H_INCLUDED

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
#include <unordered_map>

class CoSimulationIO
{

typedef std::unordered_map<std::string, std::vector<double>> DataMap;

public:
    virtual void ImportCouplingInterface(const std::string& rType)
    {
        throw std::runtime_error("\"ImportCouplingInterface\" is not implemented in the derived class!");
    }

    virtual void ExportCouplingInterface(const std::string& rType)
    {
        throw std::runtime_error("\"ExportCouplingInterface\" is not implemented in the derived class!");
    }

    virtual void ImportData(const std::string& rType)
    {
        throw std::runtime_error("\"ImportData\" is not implemented in the derived class!");
    }

    virtual void ExportData(const std::string& rType)
    {
        throw std::runtime_error("\"ExportData\" is not implemented in the derived class!");
    }
};

#endif // KRATOS_CO_SIM_BASE_IO_H_INCLUDED
