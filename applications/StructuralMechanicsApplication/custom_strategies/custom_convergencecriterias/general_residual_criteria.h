//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Natalia Saiapova
//                   Philipp Bucher
//


#if !defined(KRATOS_GENERAL_RESIDUAL_CRITERIA_H_INCLUDED )
#define  KRATOS_GENERAL_RESIDUAL_CRITERIA_H_INCLUDED

/*
Here is a list of files that you can look at for reference:
- applications/StructuralMechanicsApplication/custom_strategies/custom_convergencecriterias/displacement_and_other_dof_criteria.h
- applications/StructuralMechanicsApplication/custom_strategies/custom_convergencecriterias/residual_displacement_and_other_dof_criteria.h
- applications/FluidDynamicsApplication/custom_strategies/convergence_criteria/vel_pr_criteria.h
- applications/trilinos_application/custom_strategies/convergencecriterias/trilinos_displacement_criteria.h
- applications/trilinos_application/custom_strategies/convergencecriterias/trilinos_up_criteria.h
- kratos/solving_strategies/convergencecriterias/displacement_criteria.h
- kratos/solving_strategies/convergencecriterias/residual_criteria.h


*/

// System includes


// External includes


// Project includes
#include "includes/define.h"
#include "solving_strategies/convergencecriterias/convergence_criteria.h"
#include "includes/model_part.h"


namespace Kratos
{
  ///@addtogroup ApplicationNameApplication
  ///@{

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
  /** Detail class definition.
  */
template<class TSparseSpace,
         class TDenseSpace>
class GeneralResidualCriteria : public ConvergenceCriteria< TSparseSpace, TDenseSpace >
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of GeneralResidualCriteria
    KRATOS_CLASS_POINTER_DEFINITION(GeneralResidualCriteria);

    typedef ConvergenceCriteria< TSparseSpace, TDenseSpace > BaseType;

    typedef TSparseSpace SparseSpaceType;

    typedef typename BaseType::TDataType TDataType;

    typedef typename BaseType::DofsArrayType DofsArrayType;

    typedef typename BaseType::TSystemMatrixType TSystemMatrixType;

    typedef typename BaseType::TSystemVectorType TSystemVectorType;

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor.
    GeneralResidualCriteria(
        TDataType NewRatioTolerance,
        TDataType AlwaysConvergedNorm)
        : ConvergenceCriteria< TSparseSpace, TDenseSpace >(),
          mRatioTolerance(NewRatioTolerance),
          mAlwaysConvergedNorm(AlwaysConvergedNorm),
          mInitialResidualIsSet(false) { }

    /// Destructor.
    virtual ~GeneralResidualCriteria(){}


    ///@}
    ///@name Operators
    ///@{


    ///@}
    ///@name Operations
    ///@{

    //Criterias that need to be called after getting the solution
    bool PostCriteria(
        ModelPart& rModelPart,
        DofsArrayType& rDofSet,
        const TSystemMatrixType& A,
        const TSystemVectorType& Dx,
        const TSystemVectorType& b
    ) override
    {
        /*if (TSparseSpace::Size(b) != 0) //if we are solving for something
        {

            if (mInitialResidualIsSet == false)
            {
                mInitialResidualNorm = TSparseSpace::TwoNorm(b);
                mInitialResidualIsSet = true;

                //KRATOS_INFO(" Initial Residual ") << mInitialResidualNorm <<std::endl;
            }

            TDataType ratio;
            mCurrentResidualNorm = TSparseSpace::TwoNorm(b);

            if(mInitialResidualNorm == 0.00)
            {
                ratio = 0.00;
            }

            else
            {
                ratio = mCurrentResidualNorm/mInitialResidualNorm;
            }

            //KRATOS_INFO(" Current Residual ") << mCurrentResidualNorm << " ratio: "<< ratio << std::endl;

	    double b_size = TSparseSpace::Size(b);
	    TDataType absolute_norm = (mCurrentResidualNorm/b_size);

            if (rModelPart.GetCommunicator().MyPID() == 0)
            {
                if (this->GetEchoLevel() >= 1)
                {
                    std::cout << "RESIDUAL CRITERION :: Ratio = "<< ratio  << ";  Norm = " << absolute_norm << std::endl;
                }
            }

            rModelPart.GetProcessInfo()[CONVERGENCE_RATIO] = ratio;
            rModelPart.GetProcessInfo()[RESIDUAL_NORM] = absolute_norm;

            if (ratio <= mRatioTolerance || absolute_norm < mAlwaysConvergedNorm)
            {
                if (rModelPart.GetCommunicator().MyPID() == 0)
                {
                    if (this->GetEchoLevel() >= 1)
                    {
                        std::cout << "Convergence is achieved" << std::endl;
                    }
                }
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return true;
        }*/
        return false;
    }

    void Initialize(
        ModelPart& rModelPart
    ) override
    {
        BaseType::mConvergenceCriteriaIsInitialized = true;
    }

    void InitializeSolutionStep(
        ModelPart& rModelPart,
        DofsArrayType& rDofSet,
        const TSystemMatrixType& A,
        const TSystemVectorType& Dx,
        const TSystemVectorType& b
    ) override
    {
        mInitialResidualIsSet = false;
    }

    void FinalizeSolutionStep(
        ModelPart& rModelPart,
        DofsArrayType& rDofSet,
        const TSystemMatrixType& A,
        const TSystemVectorType& Dx,
        const TSystemVectorType& b
    ) override
    {

    }

    /**
     * This function is designed to be called once to perform all the checks needed
     * on the input provided. Checks can be "expensive" as the function is designed
     * to catch user's errors.
     * @param rModelPart
     * @return 0 all ok
     */
    int Check(ModelPart& rModelPart) override
    {
        KRATOS_TRY

        BaseType::Check(rModelPart);

        // TODO check if the variables that are being checked here are in the ModelPart!
        // TODO check if the variables that are being checked here Dofs!

        return 0;
        KRATOS_CATCH("");
    }


    ///@}
    ///@name Access
    ///@{


    ///@}
    ///@name Inquiry
    ///@{


    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    virtual std::string Info() const
    {
std::stringstream buffer;
    buffer << "GeneralResidualCriteria" ;
    return buffer.str();
    }

    /// Print information about this object.
    virtual void PrintInfo(std::ostream& rOStream) const {rOStream << "GeneralResidualCriteria";}

    /// Print object's data.
    virtual void PrintData(std::ostream& rOStream) const {}


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

    TDataType mRatioTolerance;

    TDataType mAlwaysConvergedNorm;

    bool mInitialResidualIsSet;

    TDataType mInitialResidualNorm;

    TDataType mCurrentResidualNorm;

    TDataType mReferenceDispNorm;


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
    GeneralResidualCriteria& operator=(GeneralResidualCriteria const& rOther){}

    /// Copy constructor.
    GeneralResidualCriteria(GeneralResidualCriteria const& rOther){}


    ///@}

}; // Class GeneralResidualCriteria

///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{


// /// input stream function
// inline std::istream& operator >> (std::istream& rIStream,
//                 GeneralResidualCriteria& rThis){}

// /// output stream function
// inline std::ostream& operator << (std::ostream& rOStream,
//                 const GeneralResidualCriteria& rThis)
// {
//     rThis.PrintInfo(rOStream);
//     rOStream << std::endl;
//     rThis.PrintData(rOStream);

//     return rOStream;
// }
///@}

///@} addtogroup block

}  // namespace Kratos.

#endif // KRATOS_GENERAL_RESIDUAL_CRITERIA_H_INCLUDED  defined


