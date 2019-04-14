//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Author1 Fullname
//                   Author2 Fullname
//


// System includes

// External includes
#include <pybind11/pybind11.h>


// Project includes
#include "includes/define.h"
#include "custom_python/add_custom_utilities_to_python.h"

#include "spaces/ublas_space.h"
#include "linear_solvers/linear_solver.h"

#include "custom_utilities/rans_variable_utils.h"

namespace Kratos {
namespace Python {

void AddCustomUtilitiesToPython(pybind11::module& m)
{
    namespace py = pybind11;

    // typedef UblasSpace<double, CompressedMatrix, Vector> SparseSpaceType;
    // typedef UblasSpace<double, Matrix, Vector> LocalSpaceType;
    // typedef LinearSolver<SparseSpaceType, LocalSpaceType > LinearSolverType;

    py::class_<RansVariableUtils, VariableUtils>(m, "RansVariableUtils")
        .def(py::init<>())
        .def("FixScalarVariableDofs", &RansVariableUtils::FixScalarVariableDofs)
        .def("ClipScalarVariable", &RansVariableUtils::ClipScalarVariable)
        .def("GetNumberOfNegativeScalarValueNodes", &RansVariableUtils::GetNumberOfNegativeScalarValueNodes)
        .def("GetMinimumScalarValue", &RansVariableUtils::GetMinimumScalarValue)
        .def("GetMaximumScalarValue", &RansVariableUtils::GetMaximumScalarValue)
        .def("GetScalarVariableDifferenceNormSquare",
             &RansVariableUtils::GetScalarVariableDifferenceNormSquare)
        .def("GetScalarVariableSolutionNormSquare", &RansVariableUtils::GetScalarVariableSolutionNormSquare)
        .def("CopyNodalSolutionStepVariablesList",
             &RansVariableUtils::CopyNodalSolutionStepVariablesList);
}

} // namespace Python.
} // Namespace Kratos