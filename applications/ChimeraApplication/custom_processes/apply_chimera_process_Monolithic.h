// ==============================================================================
//  ChimeraApplication
//
//  License:         BSD License
//                   license: ChimeraApplication/license.txt
//
//  Main authors:    Aditya Ghantasala, https://github.com/adityaghantasala
//
// ==============================================================================
//

#if !defined(KRATOS_CUSTOM_APPLY_CHIMERA_MONOLITHIC_H_INCLUDED)
#define KRATOS_CUSTOM_APPLY_CHIMERA_MONOLITHIC_H_INCLUDED

// System includes

#include <string>
#include <iostream>
#include <algorithm>

// External includes
#include "includes/kratos_flags.h"

#include "utilities/binbased_fast_point_locator.h"

// Project includes

#include "includes/define.h"
#include "processes/process.h"
#include "includes/kratos_flags.h"
#include "includes/model_part.h"
#include "geometries/geometry_data.h"
#include "includes/variables.h"
#include "utilities/math_utils.h"
#include "includes/kratos_parameters.h"

#include "spaces/ublas_space.h"
#include "linear_solvers/linear_solver.h"
#include "solving_strategies/schemes/residualbased_incrementalupdate_static_scheme.h"
#include "solving_strategies/builder_and_solvers/residualbased_block_builder_and_solver.h"
#include "solving_strategies/strategies/residualbased_linear_strategy.h"
#include "elements/distance_calculation_element_simplex.h"

// Application includes
#include "chimera_application_variables.h"
#include "custom_utilities/multipoint_constraint_data.hpp"
#include "custom_processes/custom_calculate_signed_distance_process.h"
#include "custom_hole_cutting_process.h"
#include "custom_utilities/vtk_output.hpp"

namespace Kratos
{

///@name Kratos Globals
///@{

///@}
///@name Type Definitions
///@{

///@}
///@name  Enum's
///@{

///@}
///@name  Functions
///@{

///@}
///@name Kratos Classes
///@{

/// Short class definition.

template <std::size_t TDim>

class ApplyChimeraProcessMonolithic : public Process
{
  public:

	///@name Type Definitions
	///@{

	///@}
	///@name Pointer Definitions
	/// Pointer definition of ApplyChimeraProcessMonolithic
	KRATOS_CLASS_POINTER_DEFINITION(ApplyChimeraProcessMonolithic);
	typedef ProcessInfo::Pointer ProcessInfoPointerType;
	typedef typename BinBasedFastPointLocator<TDim>::Pointer BinBasedPointLocatorPointerType;
	typedef ModelPart::ConditionsContainerType ConditionsArrayType;
	typedef std::pair<std::size_t, std::size_t> SlavePairType;
	typedef Kratos::MpcData::MasterDofWeightMapType MasterDofWeightMapType;
	typedef ProcessInfo ProcessInfoType;
	typedef MpcData::Pointer MpcDataPointerType;
	typedef std::vector<MpcDataPointerType> *MpcDataPointerVectorType;
	typedef Dof<double> DofType;
	typedef std::vector<DofType> DofVectorType;
	typedef MpcData::VariableComponentType VariableComponentType;
	typedef std::size_t IndexType;
	typedef MpcData::VariableType VariableType;
	typedef Node<3> NodeType;
	typedef Kratos::shared_ptr<ModelPart> ModelPartPointer;

	///@}
	///@name Life Cycle
	///@{

	ApplyChimeraProcessMonolithic(ModelPart &MainModelPart, Parameters rParameters) : Process(Flags()), mrMainModelPart(MainModelPart), m_parameters(rParameters)
	{

		Parameters default_parameters(R"(
            {
                "process_name":"chimera",
                                "Chimera_levels" : [
													[{
														"model_part_name":"GENERIC_background",
														"model_part_inside_boundary_name" :"GENERIC_domainboundary"
		            					            }],
				    								 [{
														"model_part_name":"GENERIC_patch_1_1",
														"model_part_inside_boundary_name":"GENERIC_structure_1_1"
		            						     	}],
				    								[{
														"model_part_name":"GENERIC_patch_2_1",
														"model_part_inside_boundary_name":"GENERIC_strcuture2_1"
		            						    	}]
													 ],
                    			"type" : "nearest_element",
                                "IsWeak" : true,
                                "pressure_coupling" : "all",
                                "pressure_coupling_node" : 0.0,
                                "overlap_distance":0.045
            })");

		m_type = m_parameters["type"].GetString();
		m_overlap_distance = m_parameters["overlap_distance"].GetDouble();
		NumberOfLevels = m_parameters["Chimera_levels"].size();

		for (int i =0; i<NumberOfLevels ;i++)
			LevelTable.push_back ( m_parameters["Chimera_levels"][i].size());

		ProcessInfoPointerType info = mrMainModelPart.pGetProcessInfo();
		if (info->GetValue(MPC_DATA_CONTAINER) == NULL)
			info->SetValue(MPC_DATA_CONTAINER, new std::vector<MpcDataPointerType>());

		this->pMpc = MpcDataPointerType(new MpcData(m_type));
		// this->pMpc = Kratos::make_shared<MpcDataType>(m_type);
		this->pHoleCuttingProcess = CustomHoleCuttingProcess::Pointer(new CustomHoleCuttingProcess());
		this->pCalculateDistanceProcess = typename CustomCalculateSignedDistanceProcess<TDim>::Pointer(new CustomCalculateSignedDistanceProcess<TDim>());
		//KRATOS_INFO("my_info") << "some more info" << std::endl;
		this->pMpc->SetName("MPC");
		this->pMpc->SetActive(true);

		MpcDataPointerVectorType mpcDataVector = info->GetValue(MPC_DATA_CONTAINER);
		(*mpcDataVector).push_back(pMpc);
	}

	/// Destructor.
	virtual ~ApplyChimeraProcessMonolithic()
	{
		Clear();
	}

	///@}
	///@name Operators
	///@{

	void operator()()
	{
		Execute();
	}

	///@}
	///@name Operations
	///@{

	virtual void Execute() override
	{
	}

	virtual void Clear()
	{
		pMpc->Clear();
		std::cout << "Monolithic Chimera process is cleared" << std::endl;
	}

	void ExecuteBeforeSolutionLoop() override
	{
	}

	void ExecuteInitializeSolutionStep() override
	{
		KRATOS_TRY;
		// Actual execution of the functionality of this class
		for (ModelPart::ElementsContainerType::iterator it = mrMainModelPart.ElementsBegin(); it != mrMainModelPart.ElementsEnd(); ++it)
			{
				it->SetValue(SPLIT_ELEMENT,false);
			}

		DoChimeraLoop();

		KRATOS_CATCH("");
	}

	void ExecuteFinalizeSolutionStep() override
	{
		Clear();
		//for multipatch
		for (ModelPart::ElementsContainerType::iterator it = mrMainModelPart.ElementsBegin(); it != mrMainModelPart.ElementsEnd(); ++it)
			{
				it->Set(VISITED, false);
				it->SetValue(SPLIT_ELEMENT,false);
			}
	}

	void ExecuteBeforeOutputStep() override
	{
	}

	void ExecuteAfterOutputStep() override
	{
	}

	void ExecuteFinalize() override
	{
	}

	void ApplyMpcConstraint(ModelPart &rBoundaryModelPart, BinBasedPointLocatorPointerType &pBinLocator, MpcDataPointerType pMpc, std::string pressure_coupling)
	{

		//loop over nodes and find the triangle in which it falls, than do interpolation
		array_1d<double, TDim + 1> N;
		const int max_results = 10000;
		typename BinBasedFastPointLocator<TDim>::ResultContainerType results(max_results);
		const int n_boundary_nodes = rBoundaryModelPart.Nodes().size();

		std::size_t counter = 0;

		for (int i = 0; i < n_boundary_nodes; i++)
		{
			ModelPart::NodesContainerType::iterator iparticle = rBoundaryModelPart.NodesBegin() + i;
			Node<3>::Pointer p_boundary_node = *(iparticle.base());

			bool NodeCoupled = false;
			if ((p_boundary_node)->IsDefined(VISITED))
				NodeCoupled = (p_boundary_node)->Is(VISITED);

			typename BinBasedFastPointLocator<TDim>::ResultIteratorType result_begin = results.begin();

			Element::Pointer pElement;

			bool is_found = false;
			is_found = pBinLocator->FindPointOnMesh(p_boundary_node->Coordinates(), N, pElement, result_begin, max_results);

			if (NodeCoupled && is_found)
			{
				//Define master slave relation for velocity
				RemoveMasterSlaveRelationWithNodesAndVariableComponents(pMpc, *p_boundary_node, VELOCITY_X);
				RemoveMasterSlaveRelationWithNodesAndVariableComponents(pMpc, *p_boundary_node, VELOCITY_Y);
				if (TDim == 3)
				{
					//Define master slave relation for velocity
					RemoveMasterSlaveRelationWithNodesAndVariableComponents(pMpc, *p_boundary_node, VELOCITY_Z);
				}

				if (pressure_coupling == "all")
				{
					//Defining master slave relation for pressure
					RemoveMasterSlaveRelationWithNodesAndVariable(pMpc, *p_boundary_node, PRESSURE);
				}
			}

			// Initialise the boundary nodes dofs to 0 at ever time steps

			p_boundary_node->GetDof(VELOCITY_X).GetSolutionStepValue(0) = 0.0;
			p_boundary_node->GetDof(VELOCITY_Y).GetSolutionStepValue(0) = 0.0;
			p_boundary_node->GetDof(VELOCITY_X).GetSolutionStepValue(1) = 0.0;
			p_boundary_node->GetDof(VELOCITY_Y).GetSolutionStepValue(1) = 0.0;

			if (TDim == 3)
			{
				p_boundary_node->GetDof(VELOCITY_Z).GetSolutionStepValue(0) = 0.0;
				p_boundary_node->GetDof(VELOCITY_Z).GetSolutionStepValue(1) = 0.0;
			}

			if (pressure_coupling == "all")
			{	p_boundary_node->GetDof(PRESSURE).GetSolutionStepValue(0) = 0.0;
				p_boundary_node->GetDof(PRESSURE).GetSolutionStepValue(1) = 0.0;}

			if (is_found == true)
			{
				Geometry<Node<3>> &geom = pElement->GetGeometry();

				for (std::size_t i = 0; i < geom.size(); i++)
				{
					//Interpolation of velocity
					p_boundary_node->GetDof(VELOCITY_X).GetSolutionStepValue(0) += geom[i].GetDof(VELOCITY_X).GetSolutionStepValue(0) * N[i];
					p_boundary_node->GetDof(VELOCITY_Y).GetSolutionStepValue(0) += geom[i].GetDof(VELOCITY_Y).GetSolutionStepValue(0) * N[i];

					//Define master slave relation for velocity
					AddMasterSlaveRelationWithNodesAndVariableComponents(pMpc, geom[i], VELOCITY_X, *p_boundary_node, VELOCITY_X, N[i]);
					AddMasterSlaveRelationWithNodesAndVariableComponents(pMpc, geom[i], VELOCITY_Y, *p_boundary_node, VELOCITY_Y, N[i]);
					if (TDim == 3)
					{
						//Interpolation of velocity
						p_boundary_node->GetDof(VELOCITY_Z).GetSolutionStepValue(0) += geom[i].GetDof(VELOCITY_Z).GetSolutionStepValue(0) * N[i];

						//Define master slave relation for velocity
						AddMasterSlaveRelationWithNodesAndVariableComponents(pMpc, geom[i], VELOCITY_Z, *p_boundary_node, VELOCITY_Z, N[i]);
					}

					if (pressure_coupling == "all")
					{
						//Interpolation of pressure
						p_boundary_node->GetDof(PRESSURE).GetSolutionStepValue(0) += geom[i].GetDof(PRESSURE).GetSolutionStepValue(0) * N[i];

						//Defining master slave relation for pressure
						AddMasterSlaveRelationWithNodesAndVariable(pMpc, geom[i], PRESSURE, *p_boundary_node, PRESSURE, N[i]);

						counter++;
					}
				 // end of loop over host element nodes

				// Setting the buffer 1 same buffer 0 
				p_boundary_node->GetDof(VELOCITY_X).GetSolutionStepValue(1) += geom[i].GetDof(VELOCITY_X).GetSolutionStepValue(1) * N[i];
				p_boundary_node->GetDof(VELOCITY_Y).GetSolutionStepValue(1) += geom[i].GetDof(VELOCITY_Y).GetSolutionStepValue(1) * N[i];
				//p_boundary_node->GetDof(VELOCITY_X).GetSolutionStepValue(1) = p_boundary_node->GetDof(VELOCITY_X).GetSolutionStepValue(0);
				//p_boundary_node->GetDof(VELOCITY_Y).GetSolutionStepValue(1) = p_boundary_node->GetDof(VELOCITY_Y).GetSolutionStepValue(0);
				if (TDim == 3)
					p_boundary_node->GetDof(VELOCITY_Z).GetSolutionStepValue(1) += geom[i].GetDof(VELOCITY_Z).GetSolutionStepValue(1) * N[i];
				if (pressure_coupling == "all")
					p_boundary_node->GetDof(PRESSURE).GetSolutionStepValue(1) += geom[i].GetDof(PRESSURE).GetSolutionStepValue(1) * N[i];
				}
			}

			p_boundary_node->Set(VISITED, true);

		} // end of loop over boundary nodes

		if (pressure_coupling == "one")
		{

			Node<3>::Pointer p_boundary_node;
			if (m_parameters["pressure_coupling_node"].GetDouble() == 0.0)
			{
				ModelPart::NodesContainerType::iterator iparticle = rBoundaryModelPart.NodesBegin();
				p_boundary_node = *(iparticle.base());
			}

			else
			{
				std::size_t node_num = m_parameters["pressure_coupling_node"].GetDouble();

				p_boundary_node = rBoundaryModelPart.pGetNode(node_num);
			}

			bool NodeCoupled = false;
			if ((p_boundary_node)->IsDefined(VISITED))
				NodeCoupled = (p_boundary_node)->Is(VISITED);

			if (!NodeCoupled)
			{
				typename BinBasedFastPointLocator<TDim>::ResultIteratorType result_begin = results.begin();

				Element::Pointer pElement;

				bool is_found = false;
				is_found = pBinLocator->FindPointOnMesh(p_boundary_node->Coordinates(), N, pElement, result_begin, max_results);

				//Initialsing pressure to 0 at every time steps
				p_boundary_node->GetDof(PRESSURE).GetSolutionStepValue(0) = 0.0;

				if (is_found == true)
				{
					Geometry<Node<3>> &geom = pElement->GetGeometry();
					for (std::size_t i = 0; i < geom.size(); i++)
					{
						// Interpolation of pressure
						p_boundary_node->GetDof(PRESSURE).GetSolutionStepValue(0) += geom[i].GetDof(PRESSURE).GetSolutionStepValue(0) * N[i];
						// Defining master slave relation for one pressure node
						AddMasterSlaveRelationWithNodesAndVariable(pMpc, geom[i], PRESSURE, *p_boundary_node, PRESSURE, N[i]);
						counter++;
					}
				}
				// Setting the buffer 1 same buffer 0
				p_boundary_node->GetDof(PRESSURE).GetSolutionStepValue(1) = p_boundary_node->GetDof(PRESSURE).GetSolutionStepValue(0);

				std::cout << "Coordinates of node that are pressure coupled" << std::endl;
				std::cout << p_boundary_node->X() << "," << p_boundary_node->Y() << "," << p_boundary_node->Z() << std::endl;
			}
			p_boundary_node->Set(VISITED, true);

		} // end of if (pressure_coupling == "one")

		counter /= TDim + 1;

		std::cout << counter << " pressure nodes from " << rBoundaryModelPart.Name() << " is coupled" << std::endl;
	}

	void ApplyMpcConstraintConservative(ModelPart &rBoundaryModelPart, BinBasedPointLocatorPointerType &pBinLocator, MpcDataPointerType pMpc, std::string pressure_coupling)
	{

		double rtMinvR = 0;
		DofVectorType slaveDofVector;
		double R = 0;

		ApplyMpcConstraint(rBoundaryModelPart, pBinLocator, pMpc, pressure_coupling);
		std::vector<VariableComponentType> dofComponentVector = {VELOCITY_X, VELOCITY_Y, VELOCITY_Z};

		// Calculation of Rt*Minv*R and assignment of nodalnormals to the slave dofs
		for (ModelPart::NodesContainerType::iterator inode = rBoundaryModelPart.NodesBegin(); inode != rBoundaryModelPart.NodesEnd(); ++inode)
		{
			double Minode = inode->FastGetSolutionStepValue(NODAL_MASS);

			for (std::size_t i = 0; i < TDim; i++)
			{

				double rIdof = inode->FastGetSolutionStepValue(NORMAL)[i];
				DofType &slaveDOF = inode->GetDof(dofComponentVector[i]);
				AddNodalNormalSlaveRelationWithDofs(pMpc, inode->GetDof(dofComponentVector[i]), rIdof);
				slaveDofVector.push_back(slaveDOF);
				rtMinvR += (rIdof * rIdof) / Minode;
				R += rIdof;
			}

			AddNodalNormalSlaveRelationWithDofs(pMpc, inode->GetDof(PRESSURE), 0);
		}

		SetRtMinvR(pMpc, rtMinvR);
		std::cout << "RtMRinv of " << rBoundaryModelPart.Name() << rtMinvR << std::endl;

		CalculateConservativeCorrections(mrMainModelPart, pMpc);
		ApplyConservativeCorrections(mrMainModelPart, pMpc);
	}

	static inline void GetBoundingBox(ModelPart &model_part, double *rLowPoint, double *rHighPoint)
    {

		rLowPoint[0] = 1e10;
		rLowPoint[1] = 1e10;
		rLowPoint[2] = 1e10;

		rHighPoint[0] = -1e10;
		rHighPoint[1] = -1e10;
		rHighPoint[2] = -1e10;

		for (auto &r_node : model_part.Nodes())
		{
			rLowPoint[0] = std::min(r_node.X(), rLowPoint[0]);
			rLowPoint[1] = std::min(r_node.Y(), rLowPoint[1]);
			rLowPoint[2] = std::min(r_node.Z(), rLowPoint[2]);

			rHighPoint[0] = std::max(r_node.X(), rHighPoint[0]);
			rHighPoint[1] = std::max(r_node.Y(), rHighPoint[1]);
			rHighPoint[2] = std::max(r_node.Z(), rHighPoint[2]);
		}

    }

	bool BoundingBoxTest(ModelPart &A,ModelPart &B)  //background A and Patch B
	{
		double min_cornerA[3],max_cornerA[3],min_cornerB[3],max_cornerB[3];
		GetBoundingBox(A,min_cornerA,max_cornerA);
		GetBoundingBox(B,min_cornerB,max_cornerB);

		std::cout<<" Bounding box Background"<<min_cornerA[0]<<"::"<<max_cornerA[0]<<std::endl;
		std::cout<<" Bounding box Patch"<<min_cornerB[0]<<"::"<<max_cornerB[0]<<std::endl;

		//int max_dim =GetGeometry().WorkingSpaceDimension();

		ProcessInfo &CurrentProcessInfo = A.GetProcessInfo();

		int idim = CurrentProcessInfo.GetValue(DOMAIN_SIZE);

		for (int i=0;i<idim;i++)
		{
			std::cout<<" checked direction"<<"::"<<i<<std::endl;
			if(min_cornerA[i]<min_cornerB[i]!=true) return false;
			if(max_cornerA[i]>max_cornerB[i]!=true) return false;
		}
		std::cout<<" outside loop "<<std::endl;
		return true;
	}

	void FindOutsideBoundaryOfModelPartGivenInside(ModelPart &rModelPart,ModelPart &rInsideBoundary,ModelPart &rExtractedBoundaryModelPart)
	{
		std::size_t n_nodes = rModelPart.ElementsBegin()->GetGeometry().size();

		if (n_nodes == 3)
		{
			this->pHoleCuttingProcess->ExtractOutsideBoundaryMesh(rInsideBoundary,rModelPart, rExtractedBoundaryModelPart);
		}

		else if (n_nodes == 4)
		{
			this->pHoleCuttingProcess->ExtractOutsideSurfaceMesh(rInsideBoundary,rModelPart, rExtractedBoundaryModelPart);
		}
	}

	void DoChimeraLoop() //selecting patch and background combination for chimera method
	{
		for (ModelPart::ElementsContainerType::iterator it = mrMainModelPart.ElementsBegin(); it != mrMainModelPart.ElementsEnd(); ++it)
		{
			if (!it->Is(VISITED)) //for multipatch
				it->Set(ACTIVE, true);
		}

		for (ModelPart::NodesContainerType::iterator it = mrMainModelPart.NodesBegin(); it != mrMainModelPart.NodesEnd(); ++it)
		{
			it->Set(VISITED, false);
		}

		int MainDomainOrNot = 1 ;

		for(int BG_i= 0; BG_i < NumberOfLevels ;BG_i++) // TODO change the names
		{
			for(int BG_j= 0; BG_j < LevelTable[BG_i];BG_j++)
			{
				for(int patch_i= BG_i+1; patch_i < NumberOfLevels ;patch_i++)
				{
					for(int patch_j= 0; patch_j < LevelTable[patch_i];patch_j++)
					{
						m_background_model_part_name  	=  m_parameters["Chimera_levels" ][BG_i][BG_j]["model_part_name"].GetString();
						m_domain_boundary_model_part_name = m_parameters["Chimera_levels" ][BG_i][BG_j]["model_part_inside_boundary_name"].GetString();
						m_patch_model_part_name       =  m_parameters["Chimera_levels" ][patch_i][patch_j]["model_part_name"].GetString();
						m_patch_inside_boundary_model_part_name = m_parameters["Chimera_levels" ][patch_i][patch_j]["model_part_inside_boundary_name"].GetString();

						std::cout<<"Formulating Chimera for the combination \n background::"<<m_background_model_part_name<<"  \t Patch::"<<m_patch_model_part_name<<std::endl;

						MainDomainOrNot = 1 ;
						if(BG_i == 0) // a check to identify computational Domain boundary
							MainDomainOrNot = -1 ;

						FormulateChimera(MainDomainOrNot);
					}
				}
			}
		}
		std::cout<<"End of chimera loop"<<std::endl;
	}

	//Apply Chimera with or without overlap

	void FormulateChimera(int MainDomainOrNot)
	{

		//rishith : parameters for vtk output
		Parameters parameters= Parameters(R"({
						"result_file_configuration" : {
							"gidpost_flags"       : {
								"GiDPostMode"           : "GiD_PostAscii",
								"WriteDeformedMeshFlag" : "WriteDeformed",
								"WriteConditionsFlag"   : "WriteConditions",
								"MultiFileFlag"         : "SingleFile"
							},
							"file_label"          : "time",
							"output_control_type" : "time",
							"output_frequency"    : 1.0,
							"body_output"         : true,
							"node_output"         : false,
							"skin_output"         : false,
							"plane_output"        : [],
							"nodal_results"       : ["VELOCITY","PRESSURE","DISTANCE"],
							"gauss_point_results" : []
						},
						"point_data_configuration"  : []})" );

		ModelPart &rBackgroundModelPart = mrMainModelPart.GetSubModelPart(m_background_model_part_name);
		ModelPart &rPatchModelPart = mrMainModelPart.GetSubModelPart(m_patch_model_part_name);
		ModelPart &rDomainBoundaryModelPart = mrMainModelPart.GetSubModelPart(m_domain_boundary_model_part_name);
		ModelPart &rPatchInsideBoundaryModelPart = mrMainModelPart.GetSubModelPart(m_patch_inside_boundary_model_part_name);

 		//ModelPart &rPatchBoundaryModelPart = mrMainModelPart.GetSubModelPart(m_patch_boundary_model_part_name);
 //rishith debug
/*
		std::cout<<"printing my background"<<rBackgroundModelPart<<std::endl;
		std::cout<<"printing my domain boundary "<<rDomainBoundaryModelPart<<std::endl;

		std::cout<<"printing my patch"<<rPatchModelPart<<std::endl;
		std::cout<<"printing my patch boundary"<<rPatchBoundaryModelPart<<std::endl;
		std::cout<<"printing my patch inside boundary"<<rPatchInsideBoundaryModelPart<<std::endl;
 */
		this->pBinLocatorForBackground = BinBasedPointLocatorPointerType(new BinBasedFastPointLocator<TDim>(rBackgroundModelPart));
		this->pBinLocatorForPatch = BinBasedPointLocatorPointerType(new BinBasedFastPointLocator<TDim>(rPatchModelPart));

		this->pBinLocatorForBackground->UpdateSearchDatabase();
		this->pBinLocatorForPatch->UpdateSearchDatabase();

		const double epsilon = 1e-12;
		if (m_overlap_distance < epsilon)
		{
			KRATOS_THROW_ERROR("", "Overlap distance should be a positive number \n", "");
		}

		//rishith
		//this->pCalculateDistanceProcess->CalculateSignedDistance(rPatchModelPart, rDomainBoundaryModelPart);

		if (m_overlap_distance > epsilon)
		//if (m_overlap_distance ==10)//rishith
		{
			ModelPartPointer pHoleModelPart = ModelPartPointer(new ModelPart("HoleModelpart"));
			ModelPartPointer pHoleBoundaryModelPart = ModelPartPointer(new ModelPart("HoleBoundaryModelPart"));
			//ModelPartPointer pOutOfDomainPatchModelPart = ModelPartPointer(new ModelPart("OutOfDomainPatch"));
			//ModelPartPointer pOutOfDomainPatchBoundaryModelPart = ModelPartPointer(new ModelPart("OutOfDomainPatchBoundary"));
			ModelPartPointer pModifiedPatchBoundaryModelPart = ModelPartPointer(new ModelPart("ModifiedPatchBoundary"));
			ModelPartPointer pModifiedPatchModelPart = ModelPartPointer(new ModelPart("ModifiedPatch"));

			bool BBoxOverlapTest = BoundingBoxTest(rBackgroundModelPart,rPatchModelPart);
			BBoxOverlapTest = false;

			if(BBoxOverlapTest)
			{
				FindOutsideBoundaryOfModelPartGivenInside(rPatchModelPart,rPatchInsideBoundaryModelPart,*pModifiedPatchBoundaryModelPart);
			}
			else
			{
				std::cout<<" If bounding boxes overlap , find the modified patch boundary"<<std::endl;
				this->pCalculateDistanceProcess->CalculateSignedDistance(rPatchModelPart, rDomainBoundaryModelPart);
			   			//this->pHoleCuttingProcess->RemoveOutOfDomainPatch(rPatchModelPart, *pOutOfDomainPatchModelPart, *pOutOfDomainPatchBoundaryModelPart);
				this->pHoleCuttingProcess->RemoveOutOfDomainPatchAndReturnModifiedPatch(rPatchModelPart,rPatchInsideBoundaryModelPart, *pModifiedPatchModelPart, *pModifiedPatchBoundaryModelPart,MainDomainOrNot);
			}

			//rishith
			VtkOutput VtkOutput_Boundary = VtkOutput(*pModifiedPatchBoundaryModelPart,"nnn",parameters);
			VtkOutput_Boundary.PrintOutput();

			VtkOutput VtkOutput_ModPatch = VtkOutput(*pModifiedPatchModelPart,"nnn",parameters);
			VtkOutput_ModPatch.PrintOutput();

			VtkOutput VtkOutput_DBoundary = VtkOutput(rDomainBoundaryModelPart,"nnn",parameters);
			VtkOutput_DBoundary.PrintOutput();

			//CalculateModifiedPatchBoundary(rPatchBoundaryModelPart, *pOutOfDomainPatchBoundaryModelPart, *pModifiedPatchBoundaryModelPart);
			//this->pCalculateDistanceProcess->CalculateSignedDistance(rBackgroundModelPart, rPatchInsideBoundaryModelPart);
			this->pCalculateDistanceProcess->CalculateSignedDistance(rBackgroundModelPart, *pModifiedPatchBoundaryModelPart);
			this->pHoleCuttingProcess->CreateHoleAfterDistance(rBackgroundModelPart, *pHoleModelPart, *pHoleBoundaryModelPart, m_overlap_distance);

			//*pOutOfDomainPatchBoundaryModelPart  rPatchBoundaryModelPart   *pModifiedPatchBoundaryModelPart
			//for multipatch
			for (ModelPart::ElementsContainerType::iterator it = pHoleModelPart->ElementsBegin(); it != pHoleModelPart->ElementsEnd(); ++it)
				it->Set(VISITED, true);

			CalculateNodalAreaAndNodalMass(*pModifiedPatchBoundaryModelPart, 1);
			CalculateNodalAreaAndNodalMass(*pHoleBoundaryModelPart, -1);

			std::cout<<"Formulate Chimera: Calculated Nodal area and nodal mass "<<std::endl;

			pMpc->SetIsWeak(m_parameters["IsWeak"].GetBool());

			std::string pr_coupling_patch = m_parameters["pressure_coupling"].GetString();
			std::string pr_coupling_background = m_parameters["pressure_coupling"].GetString();

			if (m_type == "nearest_element")
			{
				ApplyMpcConstraint(*pModifiedPatchBoundaryModelPart, pBinLocatorForBackground, pMpc, pr_coupling_patch);
				ApplyMpcConstraint(*pHoleBoundaryModelPart, pBinLocatorForPatch, pMpc, pr_coupling_background);
				std::cout << "Patch boundary coupled with background & HoleBoundary  coupled with patch" << std::endl;
			}

			else if (m_type == "conservative")
			{
				ApplyMpcConstraintConservative(*pModifiedPatchBoundaryModelPart, pBinLocatorForBackground, pMpc, pr_coupling_patch);
				ApplyMpcConstraintConservative(*pHoleBoundaryModelPart, pBinLocatorForPatch, pMpc, pr_coupling_background);
				std::cout << "Patch boundary coupled with background & HoleBoundary  coupled with patch  using conservative approach" << std::endl;
			}
			std::cout<<"Formulate Chimera: Appplied MPCs "<<std::endl;
		}

		//pMpc->GetInfo();

		std::cout<<"End of Formulate Chimera"<<std::endl;
	}

	// This function removes all the boundaries common to first and second but retain rest of the boundaries
	//Below function is required if the inside boundary not given : not used in the implementation
	void CalculateModifiedPatchBoundary(ModelPart &FirstBoundary, ModelPart &SecondBoundary, ModelPart &FirstMinusSecondBoundary)
	{
		std::vector<std::size_t> vector_of_node_ids;
		Properties::Pointer properties = FirstMinusSecondBoundary.rProperties()(0);
		Condition const &rReferenceLineCondition = KratosComponents<Condition>::Get("Condition2D");

		typedef std::unordered_map<std::size_t, std::size_t > hashmap;
		hashmap NodeToDomainMap;
		/*
		Node<3>::Pointer pnode1 = rSurfaceModelPart.Nodes()(original_nodes_order[0]);
		Node<3>::Pointer pnode2 = rSurfaceModelPart.Nodes()(original_nodes_order[1]);

		vector_of_node_ids.push_back(original_nodes_order[0]);
		vector_of_node_ids.push_back(original_nodes_order[1]);

		// Skin edges are added as conditions
         */
		//iterate through all Geometries of first and second and compare for the common edges
		std::size_t id_condition = 1;

		for (ModelPart::ConditionIterator itCondF = FirstBoundary.ConditionsBegin(); itCondF != FirstBoundary.ConditionsEnd(); ++itCondF)
		{
			//Element::GeometryType::GeometriesArrayType edgesF = itCondF->GetGeometry().Edges();

			bool CommonEdgeDetected = false ;

				vector<std::size_t> idsFirst(2);
				// Store node ids
				for (std::size_t i = 0; i < itCondF->GetGeometry().size();i++)
					idsFirst[i] = itCondF->GetGeometry()[i].Id();

				for (ModelPart::ConditionIterator itCondS = SecondBoundary.ConditionsBegin(); itCondS != SecondBoundary.ConditionsEnd(); ++itCondS)
				{
					vector<std::size_t> idsSecond(2);
					for (std::size_t i = 0; i < itCondS->GetGeometry().size();i++)
						idsSecond[i] = itCondS->GetGeometry()[i].Id();


					if ((idsFirst[0] != idsSecond[0]) || (idsFirst[1] != idsSecond[1]))
					{
						Node<3>::Pointer pnodeS1 = SecondBoundary.Nodes()(idsSecond[0]);
						Node<3>::Pointer pnodeS2 = SecondBoundary.Nodes()(idsSecond[1]);

						vector_of_node_ids.push_back(idsSecond[0]);
						vector_of_node_ids.push_back(idsSecond[1]);
						NodeToDomainMap[idsSecond[0]] = 2;
						NodeToDomainMap[idsSecond[1]] = 2;

						Line2D2<Node<3>> lineS(pnodeS1, pnodeS2);
						Condition::Pointer p_conditionS = rReferenceLineCondition.Create(id_condition++, lineS, properties);
						FirstMinusSecondBoundary.Conditions().push_back(p_conditionS);

					}
					else
						CommonEdgeDetected = true;

				}

			if (!CommonEdgeDetected)
			{
				Node<3>::Pointer pnodeF1 = FirstBoundary.Nodes()(idsFirst[0]);
				Node<3>::Pointer pnodeF2 = FirstBoundary.Nodes()(idsFirst[1]);

				vector_of_node_ids.push_back(idsFirst[0]);
				vector_of_node_ids.push_back(idsFirst[1]);

				NodeToDomainMap[idsFirst[0]] = 1;
				NodeToDomainMap[idsFirst[1]] = 1;

				Line2D2<Node<3>> lineF(pnodeF1, pnodeF2);
				Condition::Pointer p_conditionF = rReferenceLineCondition.Create(id_condition++, lineF, properties);
				FirstMinusSecondBoundary.Conditions().push_back(p_conditionF);
			}
		}


		std::set<std::size_t> s(vector_of_node_ids.begin(), vector_of_node_ids.end());
		vector_of_node_ids.assign(s.begin(), s.end());

		for (auto it = vector_of_node_ids.begin(); it != vector_of_node_ids.end(); it++)
		{
			if (NodeToDomainMap[*it] == 1)
			{
				Node<3>::Pointer pnode = FirstBoundary.Nodes()(*it);
				FirstMinusSecondBoundary.AddNode(pnode);
			}
			else
			{
				Node<3>::Pointer pnode2 = SecondBoundary.Nodes()(*it);
				FirstMinusSecondBoundary.AddNode(pnode2);
			}
		}
		std::cout << "First minus Second .........Rishith " << FirstMinusSecondBoundary.GetMesh() << std::endl;
	}

void SetOverlapDistance(double distance)
{
	this->m_overlap_distance = distance;
}

void SetType(std::string type)
{
	KRATOS_TRY
	{
		if ((type != "nearest_element") && (type != "conservative"))
			KRATOS_THROW_ERROR("", "Second argument should be either nearest_element or conservative \n", "");

		this->m_type = type;
		pMpc->SetType(this->m_type);
	}

	KRATOS_CATCH("")
}

void CalculateNodalAreaAndNodalMass(ModelPart &rBoundaryModelPart, int sign)
{
	KRATOS_TRY
	ConditionsArrayType &rConditions = rBoundaryModelPart.Conditions();
	//resetting the normals and calculating centre point

	array_1d<double, 3> zero;
	array_1d<double, 3> centre;
	std::size_t n_nodes = rBoundaryModelPart.Nodes().size();

	zero[0] = 0.0;
	zero[1] = 0.0;
	zero[2] = 0.0;

	centre[0] = 0.0;
	centre[1] = 0.0;
	centre[2] = 0.0;

	for (ConditionsArrayType::iterator it = rConditions.begin();
		 it != rConditions.end(); it++)
	{
		Element::GeometryType &rNodes = it->GetGeometry();
		for (std::size_t in = 0; in < rNodes.size(); in++)
		{
			noalias((rNodes[in]).GetSolutionStepValue(NORMAL)) = zero;
		}
	}

	for (ModelPart::NodesContainerType::iterator inode = rBoundaryModelPart.NodesBegin(); inode != rBoundaryModelPart.NodesEnd(); ++inode)
	{

		centre += inode->Coordinates();
	}

	centre = centre / n_nodes;

	//calculating the normals and storing on the conditions
	array_1d<double, 3> An;
	if (TDim == 2)
	{
		for (ConditionsArrayType::iterator it = rConditions.begin();
			 it != rConditions.end(); it++)
		{
			if (it->GetGeometry().PointsNumber() == 2)
				CalculateNormal2D(it, An, centre, sign);
		}
	}
	else if (TDim == 3)
	{
		array_1d<double, 3> v1;
		array_1d<double, 3> v2;
		for (ConditionsArrayType::iterator it = rConditions.begin();
			 it != rConditions.end(); it++)
		{
			//calculate the normal on the given condition
			if (it->GetGeometry().PointsNumber() == 3)
				CalculateNormal3D(it, An, v1, v2, centre, sign);
		}
	}

	//adding the normals to the nodes
	for (ConditionsArrayType::iterator it = rConditions.begin();
		 it != rConditions.end(); it++)
	{
		Geometry<Node<3>> &pGeometry = (it)->GetGeometry();
		double coeff = 1.00 / pGeometry.size();
		const array_1d<double, 3> &normal = it->GetValue(NORMAL);
		double nodal_mass = MathUtils<double>::Norm3(normal);

		for (std::size_t i = 0; i < pGeometry.size(); i++)
		{
			noalias(pGeometry[i].FastGetSolutionStepValue(NORMAL)) += coeff * normal;
			pGeometry[i].FastGetSolutionStepValue(NODAL_MASS) += coeff * nodal_mass;
		}
	}

	KRATOS_CATCH("")
}

void CalculateNormal2D(ConditionsArrayType::iterator it, array_1d<double, 3> &An, array_1d<double, 3> &centre, int sign)
{
	Geometry<Node<3>> &pGeometry = (it)->GetGeometry();
	array_1d<double, 3> rVector;

	An[0] = pGeometry[1].Y() - pGeometry[0].Y();
	An[1] = -(pGeometry[1].X() - pGeometry[0].X());
	An[2] = 0.00;

	rVector[0] = centre[0] - pGeometry[0].X();
	rVector[1] = centre[1] - pGeometry[0].Y();
	rVector[2] = 0.00;

	array_1d<double, 3> &normal = (it)->GetValue(NORMAL);
	noalias(normal) = An;

	if ((MathUtils<double>::Dot(An, rVector) > 0))
		normal = -1 * normal;

	normal = normal * sign;

	// 				(it)->SetValue(NORMAL,An);
}

void CalculateNormal3D(ConditionsArrayType::iterator it, array_1d<double, 3> &An,
					   array_1d<double, 3> &v1, array_1d<double, 3> &v2, array_1d<double, 3> &centre, int sign)
{
	Geometry<Node<3>> &pGeometry = (it)->GetGeometry();
	array_1d<double, 3> rVector;

	v1[0] = pGeometry[1].X() - pGeometry[0].X();
	v1[1] = pGeometry[1].Y() - pGeometry[0].Y();
	v1[2] = pGeometry[1].Z() - pGeometry[0].Z();

	v2[0] = pGeometry[2].X() - pGeometry[0].X();
	v2[1] = pGeometry[2].Y() - pGeometry[0].Y();
	v2[2] = pGeometry[2].Z() - pGeometry[0].Z();

	rVector[0] = centre[0] - pGeometry[0].X();
	rVector[1] = centre[1] - pGeometry[0].Y();
	rVector[2] = centre[2] - pGeometry[0].Z();

	MathUtils<double>::CrossProduct(An, v1, v2);
	An *= 0.5;

	array_1d<double, 3> &normal = (it)->GetValue(NORMAL);
	noalias(normal) = An;

	if ((MathUtils<double>::Dot(An, rVector) > 0))
		normal = -1 * normal;

	normal = normal * sign;
	// 				noalias((it)->GetValue(NORMAL)) = An;
}

void CalculateConservativeCorrections(ModelPart &r_model_part, MpcDataPointerType pMpc)
{

	double nodalMass;
	std::size_t slaveNodeId;
	std::size_t slaveNodeIdOther;
	std::size_t slaveDofKey;
	std::size_t slaveDofKeyOther;
	double slaveDofValueOther;
	SlavePairType slaveDofMap;
	SlavePairType slaveDofMapOther;
	double RtMinvR = pMpc->RtMinvR;
	double NodalNormalComponent;
	double NodalNormalComponentOther;
	//std::cout << " RtMinvR " << RtMinvR << std::endl;
	std::vector<double> VectorOfconstants;
	std::size_t slaveIndex = 0;

	for (auto slaveMasterDofMap : pMpc->mDofConstraints)
	{
		slaveDofMap = slaveMasterDofMap.first;
		slaveNodeId = slaveDofMap.first;
		slaveDofKey = slaveDofMap.second;
		Node<3> &slaveNode = r_model_part.Nodes()[slaveNodeId];
		Node<3>::DofsContainerType::iterator idof = slaveNode.GetDofs().find(slaveDofKey);
		nodalMass = slaveNode.FastGetSolutionStepValue(NODAL_MASS);
		NodalNormalComponent = pMpc->mSlaveDofToNodalNormalMap[slaveDofMap];
		VectorOfconstants.push_back(0.0);
		for (auto slaveMasterDofMapOther : pMpc->mDofConstraints)
		{

			slaveDofMapOther = slaveMasterDofMapOther.first;
			slaveNodeIdOther = slaveDofMapOther.first;
			slaveDofKeyOther = slaveDofMapOther.second;
			Node<3> &slaveNodeOther = r_model_part.Nodes()[slaveNodeIdOther];
			Node<3>::DofsContainerType::iterator idofOther = slaveNodeOther.GetDofs().find(slaveDofKeyOther);
			slaveDofValueOther = idofOther->GetSolutionStepValue();
			NodalNormalComponentOther = pMpc->mSlaveDofToNodalNormalMap[slaveDofMapOther];
			VectorOfconstants[slaveIndex] -= ((NodalNormalComponent * NodalNormalComponentOther) / (nodalMass * RtMinvR)) * slaveDofValueOther; // correction for zero flux

		} // slaveMasterDofMapOher loop

		slaveIndex++;

	} // slaveMasterDofMap loop

	slaveIndex = 0;

	//Applying correction in the slaveDofValue
	for (auto slaveMasterDofMap : pMpc->mDofConstraints)
	{

		slaveDofMap = slaveMasterDofMap.first;
		slaveNodeId = slaveDofMap.first;
		slaveDofKey = slaveDofMap.second;
		Node<3> &slaveNode = r_model_part.Nodes()[slaveNodeId];
		Node<3>::DofsContainerType::iterator idof = slaveNode.GetDofs().find(slaveDofKey);
		std::size_t slaveEquationId = idof->EquationId();

		pMpc->mSlaveEquationIdConstantsMap[slaveEquationId] = VectorOfconstants[slaveIndex];

		slaveIndex++;

	} // slaveMasterDofMap loop

	//std::cout << "Conservative correction to the velocity field applied" << std::endl;
	std::cout << "Conservative Correction of " << pMpc->mName << " is calculated" << std::endl;
}

void ApplyConservativeCorrections(ModelPart &r_model_part, MpcDataPointerType pMpc)
{

	for (auto slaveMasterDofMap : pMpc->mDofConstraints)
	{
		SlavePairType slaveDofMap = slaveMasterDofMap.first;
		std::size_t slaveNodeId = slaveDofMap.first;
		std::size_t slaveDofKey = slaveDofMap.second;
		NodeType &node = r_model_part.Nodes()[slaveNodeId];
		Node<3>::DofsContainerType::iterator it = node.GetDofs().find(slaveDofKey);
		std::size_t slaveEquationId = it->EquationId();

		it->GetSolutionStepValue(0) += pMpc->mSlaveEquationIdConstantsMap[slaveEquationId];
		it->GetSolutionStepValue(1) += pMpc->mSlaveEquationIdConstantsMap[slaveEquationId];
	}

	std::cout << "Conservative Correction of " << pMpc->mName << " is applied" << std::endl;
}

// Functions which use two variable components

/**
		Applies the MPC condition using two nodes, one as master and other as slave, and with the given weight
		@arg MasterNode
        @arg MasterVariable
        @arg SlaveNode
        @arg SlaveVariable
        @arg weight
		*/
void AddMasterSlaveRelationWithNodesAndVariableComponents(MpcDataPointerType pMpc, Node<3> &MasterNode, VariableComponentType &MasterVariable, Node<3> &SlaveNode, VariableComponentType &SlaveVariable, double weight, double constant = 0.0)
{
	SlaveNode.Set(SLAVE);
	DofType &pointerSlaveDOF = SlaveNode.GetDof(SlaveVariable);
	DofType &pointerMasterDOF = MasterNode.GetDof(MasterVariable);
	AddMasterSlaveRelationWithDofs(pMpc, pointerSlaveDOF, pointerMasterDOF, weight, constant);
}

void AddMasterSlaveRelationWithNodeIdsAndVariableComponents(MpcDataPointerType pMpc, IndexType MasterNodeId, VariableComponentType &MasterVariable, IndexType SlaveNodeId, VariableComponentType &SlaveVariable, double weight, double constant = 0.0)
{
	Node<3> &SlaveNode = mrMainModelPart.Nodes()[SlaveNodeId];
	Node<3> &MasterNode = mrMainModelPart.Nodes()[MasterNodeId];
	SlaveNode.Set(SLAVE);
	DofType &pointerSlaveDOF = SlaveNode.GetDof(SlaveVariable);
	DofType &pointerMasterDOF = MasterNode.GetDof(MasterVariable);
	AddMasterSlaveRelationWithDofs(pMpc, pointerSlaveDOF, pointerMasterDOF, weight, constant);
}

// Functions with use two variables
void AddMasterSlaveRelationWithNodesAndVariable(MpcDataPointerType pMpc, Node<3> &MasterNode, VariableType &MasterVariable, Node<3> &SlaveNode, VariableType &SlaveVariable, double weight, double constant = 0.0)
{
	SlaveNode.Set(SLAVE);
	DofType &pointerSlaveDOF = SlaveNode.GetDof(SlaveVariable);
	DofType &pointerMasterDOF = MasterNode.GetDof(MasterVariable);
	AddMasterSlaveRelationWithDofs(pMpc, pointerSlaveDOF, pointerMasterDOF, weight, constant);
}

void AddMasterSlaveRelationWithNodeIdsAndVariable(MpcDataPointerType pMpc, IndexType MasterNodeId, VariableType &MasterVariable, IndexType SlaveNodeId, VariableType &SlaveVariable, double weight, double constant = 0.0)
{
	Node<3> &SlaveNode = mrMainModelPart.Nodes()[SlaveNodeId];
	Node<3> &MasterNode = mrMainModelPart.Nodes()[MasterNodeId];
	SlaveNode.Set(SLAVE);
	DofType &pointerSlaveDOF = SlaveNode.GetDof(SlaveVariable);
	DofType &pointerMasterDOF = MasterNode.GetDof(MasterVariable);
	AddMasterSlaveRelationWithDofs(pMpc, pointerSlaveDOF, pointerMasterDOF, weight, constant);
}


// REMOVE


    // Remove constraints
    void RemoveMasterSlaveRelationWithNodesAndVariableComponents(MpcDataPointerType pMpc, Node<3> &SlaveNode, VariableComponentType &SlaveVariable)
    {
        SlaveNode.Set(SLAVE);
        DofType &pointerSlaveDOF = SlaveNode.GetDof(SlaveVariable);
        RemoveMasterSlaveRelationWithDofs(pMpc,pointerSlaveDOF);
    }

    void RemoveMasterSlaveRelationWithNodeIdsAndVariableComponents(MpcDataPointerType pMpc, IndexType SlaveNodeId, VariableComponentType &SlaveVariable)
    {
        Node<3> &SlaveNode = mrMainModelPart.Nodes()[SlaveNodeId];
        SlaveNode.Set(SLAVE, false);
        DofType &pointerSlaveDOF = SlaveNode.GetDof(SlaveVariable);
        RemoveMasterSlaveRelationWithDofs(pMpc,pointerSlaveDOF);
    }

    // Functions with use two variables
    void RemoveMasterSlaveRelationWithNodesAndVariable(MpcDataPointerType pMpc, Node<3> &SlaveNode, VariableType &SlaveVariable)
    {
        SlaveNode.Set(SLAVE);
        DofType &pointerSlaveDOF = SlaveNode.GetDof(SlaveVariable);
        RemoveMasterSlaveRelationWithDofs(pMpc,pointerSlaveDOF);
    }

    void RemoveMasterSlaveRelationWithNodeIdsAndVariable(MpcDataPointerType pMpc, IndexType SlaveNodeId, VariableType &SlaveVariable)
    {
        Node<3> &SlaveNode = mrMainModelPart.Nodes()[SlaveNodeId];
        SlaveNode.Set(SLAVE, false);
        DofType &pointerSlaveDOF = SlaveNode.GetDof(SlaveVariable);
        RemoveMasterSlaveRelationWithDofs(pMpc,pointerSlaveDOF);
    }



// Default functions
/**
		Applies the MPC condition using DOFs, one as master and other as slave, and with the given weight
		@arg slaveDOF
        @arg masterDOF
        @arg weight
		*/
void AddMasterSlaveRelationWithDofs(MpcDataPointerType pMpc, DofType slaveDOF, DofType masterDOF, double masterWeight, double constant = 0.0)
{
	pMpc->AddConstraint(slaveDOF, masterDOF, masterWeight, constant);
}

void RemoveMasterSlaveRelationWithDofs(MpcDataPointerType pMpc,DofType slaveDOF)
{
	pMpc->RemoveConstraint(slaveDOF);
}


void AddNodalNormalSlaveRelationWithDofs(MpcDataPointerType pMpc, DofType slaveDOF, double nodalNormalComponent = 0.0)
{
	pMpc->AddNodalNormalToSlaveDof(slaveDOF, nodalNormalComponent);
}

/**
		Activates the constraint set or deactivates
		@arg isActive true/false
		*/
void SetActive(bool isActive = true)
{
	pMpc->SetActive(isActive);
}

void SetRtMinvR(MpcDataPointerType pMpc, double value)
{

	pMpc->RtMinvR = value;
}

void PrintGIDMesh(ModelPart &rmodel_part)
{
	std::ofstream myfile;
	myfile.open(rmodel_part.Name() + ".post.msh");
	myfile << "MESH \"leaves\" dimension 2 ElemType Line Nnode 2" << std::endl;
	myfile << "# color 96 96 96" << std::endl;
	myfile << "Coordinates" << std::endl;
	myfile << "# node number coordinate_x coordinate_y coordinate_z  " << std::endl;

	for (std::size_t i = 0; i < rmodel_part.Nodes().size(); i++)
	{
		ModelPart::NodesContainerType::iterator iparticle = rmodel_part.NodesBegin() + i;
		Node<3>::Pointer p_node = *(iparticle.base());
		myfile << p_node->Id() << "  " << p_node->Coordinates()[0] << "  " << p_node->Coordinates()[1] << "  " << p_node->Coordinates()[2] << std::endl;
	}

	myfile << "end coordinates" << std::endl;
	myfile << "elements" << std::endl;
	myfile << "# element node_1 node_2 material_number" << std::endl;

	for (ConditionsArrayType::iterator it = rmodel_part.Conditions().begin();
		 it != rmodel_part.Conditions().end(); it++)
	{

		myfile << it->Id() << "  ";
		for (std::size_t i = 0; i < it->GetGeometry().PointsNumber(); i++)
			myfile << (it->GetGeometry()[i]).Id() << "  ";

		myfile << std::endl;
	}

	myfile << "end elements" << std::endl;
}

virtual std::string Info() const override
{
	return "ApplyChimeraProcessMonolithic";
}

/// Print information about this object.
virtual void PrintInfo(std::ostream &rOStream) const override
{
	rOStream << "ApplyChimeraProcessMonolithic";
}

/// Print object's data.
virtual void PrintData(std::ostream &rOStream) const override
{

	std::cout << "\nNumber of slave nodes :: " << std::endl;
	pMpc->GetInfo();
}

///@}
///@name Friends
///@{

///@}

protected:
///@name Protected static Member Variables
///@{

///@}
///@name Protected member Variables
///@{

///@}
///@name Protected Operators
///@{

///@}
///@name Protected Operations
///@{

///@}
///@name Protected  Access
///@{

///@}
///@name Protected Inquiry
///@{

///@}
///@name Protected LifeCycle
///@{

///@}

private:
///@name Static Member Variables
///@{

///@}
///@name Member Variables
///@{

//ModelPart &mrBackGroundModelPart;
//ModelPart &mrPatchSurfaceModelPart;
BinBasedPointLocatorPointerType pBinLocatorForBackground; // Template argument 3 stands for 3D case
BinBasedPointLocatorPointerType pBinLocatorForPatch;

//for monolithic
MpcDataPointerType pMpc;

CustomHoleCuttingProcess::Pointer pHoleCuttingProcess;
typename CustomCalculateSignedDistanceProcess<TDim>::Pointer pCalculateDistanceProcess;
ModelPart &mrMainModelPart;
double m_overlap_distance;
int NumberOfLevels;
std::vector<int> LevelTable;

Parameters m_parameters;
std::string m_background_model_part_name;
std::string m_patch_boundary_model_part_name;
std::string m_domain_boundary_model_part_name;
std::string m_patch_inside_boundary_model_part_name;
std::string m_patch_model_part_name;
std::string m_type;

// epsilon
//static const double epsilon;

///@}
///@name Private Operators
///@{

///@}
///@name Private Operations
///@{

///@}
///@name Private  Access
///@{

///@}
///@name Private Inquiry
///@{

///@}
///@name Un accessible methods
///@{

/// Assignment operator.
ApplyChimeraProcessMonolithic &operator=(ApplyChimeraProcessMonolithic const &rOther);

/// Copy constructor.
//CustomExtractVariablesProcess(CustomExtractVariablesProcess const& rOther);

///@}
}; // Class CustomExtractVariablesProcess

} // namespace Kratos.

#endif // KRATOS_CUSTOM_EXTRACT_VARIABLES_PROCESS_H_INCLUDED  defined
