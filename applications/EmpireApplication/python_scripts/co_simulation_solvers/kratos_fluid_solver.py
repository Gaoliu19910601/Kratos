from __future__ import print_function, absolute_import, division  # makes KratosMultiphysics backward compatible with python 2.6 and 2.7

# Importing the Kratos Library
import KratosMultiphysics
import KratosMultiphysics.FluidDynamicsApplication

# Importing the base class
from kratos_base_field_solver import KratosBaseFieldSolver

# Other imports
from fluid_dynamics_analysis import FluidDynamicsAnalysis

def CreateSolver(cosim_solver_settings, level):
    return KratosFluidSolver(cosim_solver_settings, level)

class KratosFluidSolver(KratosBaseFieldSolver):
    def _CreateAnalysisStage(self):
        return FluidDynamicsAnalysis(self.model, self.project_parameters)