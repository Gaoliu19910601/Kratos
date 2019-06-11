from __future__ import print_function, absolute_import, division  # makes KratosMultiphysics backward compatible with python 2.6 and 2.7

# Importing the Kratos Library
import KratosMultiphysics
import KratosMultiphysics.CompressiblePotentialFlowApplication as KCPFApp

# Importing the base class
from KratosMultiphysics.FluidDynamicsApplication.fluid_solver import FluidSolver

class PotentialFlowFormulation(object):
    def __init__(self, formulation_settings):
        self.element_name = None
        self.condition_name = None

        if formulation_settings.Has("element_type"):
            element_type = formulation_settings["element_type"].GetString()
            if element_type == "incompressible":
                self._SetUpIncompressibleElement(formulation_settings)
            elif element_type == "embedded_incompressible":
                self._SetUpEmbeddedIncompressibleElement(formulation_settings)
        else:
            raise RuntimeError("Argument \'element_type\' not found in formulation settings.")


    def _SetUpIncompressibleElement(self, formulation_settings):
        default_settings = KratosMultiphysics.Parameters(r"""{
            "element_type": "incompressible"
        }""")
        formulation_settings.ValidateAndAssignDefaults(default_settings)

        self.element_name = "IncompressiblePotentialFlowElement"
        self.condition_name = "PotentialWallCondition"

    def _SetUpEmbeddedIncompressibleElement(self, formulation_settings):
        default_settings = KratosMultiphysics.Parameters(r"""{
            "element_type": "embedded_incompressible"
        }""")
        formulation_settings.ValidateAndAssignDefaults(default_settings)

        self.element_name = "EmbeddedIncompressiblePotentialFlowElement"
        self.condition_name = "PotentialWallCondition"

def CreateSolver(model, custom_settings):
    return PotentialFlowSolver(model, custom_settings)

class PotentialFlowSolver(FluidSolver):

    def _ValidateSettings(self, settings):
        # Defaul settings string in json format
        default_settings = KratosMultiphysics.Parameters(r'''{
            "solver_type": "potential_flow_solver",
            "model_part_name": "PotentialFluidModelPart",
            "domain_size": -1,
            "model_import_settings": {
                "input_type": "mdpa",
                "input_filename": "unknown_name"
            },
            "material_import_settings": {
                "materials_filename": "unknown_materials.json"
            },
            "formulation": {
                "element_type": "imcompressible_potential"
            },
            "maximum_iterations": 10,
            "echo_level": 0,
            "relative_tolerance": 1e-5,
            "absolute_tolerance": 1e-9,
            "compute_reactions": false,
            "reform_dofs_at_each_step": false,
            "calculate_solution_norm": false,
            "linear_solver_settings": {
                "solver_type": "amgcl"
            },
            "formulation": {
                "element_type": "incompressible"
            },
            "volume_model_part_name": "volume_model_part",
            "skin_parts":[""],
            "no_skin_parts": [""],
            "move_mesh_flag": false
        }''')

        settings.ValidateAndAssignDefaults(default_settings)
        return settings

    def __init__(self, model, custom_settings):

        super(PotentialFlowSolver, self).__init__(model, custom_settings)

        # There is only a single rank in OpenMP, we always print
        self._is_printing_rank = True

        # Set the element and condition names for the replace settings
        self.formulation = PotentialFlowFormulation(self.settings["formulation"])
        self.element_name = self.formulation.element_name
        self.condition_name = self.formulation.condition_name
        self.min_buffer_size = 1

        # Construct the linear solvers
        import KratosMultiphysics.python_linear_solver_factory as linear_solver_factory
        self.linear_solver = linear_solver_factory.ConstructSolver(self.settings["linear_solver_settings"])

    def AddVariables(self):
        # Degrees of freedom
        self.main_model_part.AddNodalSolutionStepVariable(KCPFApp.VELOCITY_POTENTIAL)
        self.main_model_part.AddNodalSolutionStepVariable(KCPFApp.AUXILIARY_VELOCITY_POTENTIAL)
        # Embedded variables
        self.main_model_part.AddNodalSolutionStepVariable(KCPFApp.GEOMETRY_DISTANCE)
        # Kratos variables
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.FLAG_VARIABLE) # Required for variational_distance_process
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.DISTANCE)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.DISTANCE_GRADIENT)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.NODAL_H) # Required for modify_distance_process
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.VELOCITY) # Required for modify_distance_process
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.PRESSURE) # Required for modify_distance_process

    def AddDofs(self):
        KratosMultiphysics.VariableUtils().AddDof(KCPFApp.VELOCITY_POTENTIAL, self.main_model_part)
        KratosMultiphysics.VariableUtils().AddDof(KCPFApp.AUXILIARY_VELOCITY_POTENTIAL, self.main_model_part)
        KratosMultiphysics.VariableUtils().AddDof(KratosMultiphysics.PRESSURE, self.main_model_part)
        KratosMultiphysics.VariableUtils().AddDof(KratosMultiphysics.VELOCITY_X, self.main_model_part)
        KratosMultiphysics.VariableUtils().AddDof(KratosMultiphysics.VELOCITY_Y, self.main_model_part)
        KratosMultiphysics.VariableUtils().AddDof(KratosMultiphysics.VELOCITY_Z, self.main_model_part)

    def Initialize(self):
        time_scheme = KratosMultiphysics.ResidualBasedIncrementalUpdateStaticScheme()
        # TODO: Rename to self.strategy once we upgrade the base FluidDynamicsApplication solvers
        self.solver = KratosMultiphysics.ResidualBasedLinearStrategy(
            self.GetComputingModelPart(),
            time_scheme,
            self.linear_solver,
            self.settings["compute_reactions"].GetBool(),
            self.settings["reform_dofs_at_each_step"].GetBool(),
            self.settings["calculate_solution_norm"].GetBool(),
            self.settings["move_mesh_flag"].GetBool())

        (self.solver).SetEchoLevel(self.settings["echo_level"].GetInt())
        (self.solver).Initialize()

    def AdvanceInTime(self, current_time):
        raise Exception("AdvanceInTime is not implemented. Potential Flow simulations are steady state.")
