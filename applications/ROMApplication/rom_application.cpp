//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    @{KRATOS_APP_AUTHOR}
//


// System includes


// External includes


// Project includes
#include "rom_application.h"
#include "rom_application_variables.h"


namespace Kratos {

KratosROMApplication::KratosROMApplication():
    KratosApplication("ROMApplication")
    {}

void KratosROMApplication::Register()
{
     // calling base class register to register Kratos components
     KratosApplication::Register();
     KRATOS_INFO("") << "Initializing KratosROMApplication..." << std::endl;

  KRATOS_REGISTER_VARIABLE( DOF_1 )
  KRATOS_REGISTER_VARIABLE( DOF_2 )
  KRATOS_REGISTER_VARIABLE( ScalarVariable )
  KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS( VectorVariable )

}
}  // namespace Kratos.
