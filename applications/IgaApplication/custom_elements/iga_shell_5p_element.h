#if !defined(KRATOS_IGA_SHELL_5P_ELEMENT_H_INCLUDED )
#define  KRATOS_IGA_SHELL_5P_ELEMENT_H_INCLUDED


// System includes
#include "utilities/math_utils.h"

// External includes

// Project includes

// Application includes
#include "iga_application_variables.h"

#include "custom_elements/base_discrete_element.h"
#include "custom_utilities/geometry_utilities/iga_geometry_utilities.h"



namespace Kratos
{
///@name Kratos Classes
///@{
/// Short class definition.
/** Kirchhoff-Love Shell. Optimized for Isogeometric Analysis by Kiendl et al. .
*/
class IgaShell5pElement
    : public BaseDiscreteElement
{
public:
    ///@name Type Definitions
    ///@{
    /// Counted pointer of IgaShell5pElement
    KRATOS_CLASS_POINTER_DEFINITION(IgaShell5pElement);
    ///@}
    ///@name Life Cycle
    ///@{
    /// Default constructor.
    // Constructor using an array of nodes
    IgaShell5pElement(IndexType NewId, GeometryType::Pointer pGeometry)
        : BaseDiscreteElement(NewId, pGeometry)
    {};
    // Constructor using an array of nodes with properties
    IgaShell5pElement(IndexType NewId, GeometryType::Pointer pGeometry, PropertiesType::Pointer pProperties)
        : BaseDiscreteElement(NewId, pGeometry, pProperties)
    {};

    // default constructor necessary for serialization
    IgaShell5pElement() : BaseDiscreteElement() {};

    /// Destructor.
    virtual ~IgaShell5pElement() override
    {};

    /**
    * @brief Creates a new element
    * @param NewId The Id of the new created element
    * @param pGeom The pointer to the geometry of the element
    * @param pProperties The pointer to property
    * @return The pointer to the created element
    */
    Element::Pointer Create(
        IndexType NewId,
        GeometryType::Pointer pGeom,
        PropertiesType::Pointer pProperties
    ) const override
    {
        return Kratos::make_shared<IgaShell5pElement>(
            NewId, pGeom, pProperties);
    };

    Element::Pointer Create(
        IndexType NewId,
        NodesArrayType const& ThisNodes,
        PropertiesType::Pointer pProperties
    ) const override
    {
        return Kratos::make_shared< IgaShell5pElement >(
            NewId, GetGeometry().Create(ThisNodes), pProperties);
    };

    ///@}
    ///@name Operations
    ///@{

    /**
    * Called to initialize the element.
    * Must be called before any calculation is done
    */
    void Initialize() override;

    /**
    * This functions calculates both the RHS and the LHS
    * @param rLeftHandSideMatrix: The LHS
    * @param rRightHandSideVector: The RHS
    * @param rCurrentProcessInfo: The current process info instance
    * @param CalculateStiffnessMatrixFlag: The flag to set if compute the LHS
    * @param CalculateResidualVectorFlag: The flag to set if compute the RHS
    */
    void CalculateAll(
        MatrixType& rLeftHandSideMatrix,
        VectorType& rRightHandSideVector,
        ProcessInfo& rCurrentProcessInfo,
        const bool CalculateStiffnessMatrixFlag,
        const bool CalculateResidualVectorFlag
    ) override;


    /**
    * @brief Sets on rResult the ID's of the element degrees of freedom
    * @param rResult The vector containing the equation id
    * @param rCurrentProcessInfo The current process info instance
    */
    void EquationIdVector(
        EquationIdVectorType& rResult,
        ProcessInfo& rCurrentProcessInfo
    ) override;

    /**
    * @brief Sets on rElementalDofList the degrees of freedom of the considered element geometry
    * @param rElementalDofList The vector containing the dof of the element
    * @param rCurrentProcessInfo The current process info instance
    */
    void GetDofList(
        DofsVectorType& rElementalDofList,
        ProcessInfo& rCurrentProcessInfo
    ) override;

    /**
    * This function provides the place to perform checks on the completeness of the input.
    * It is designed to be called only once (or anyway, not often) typically at the beginning
    * of the calculations, so to verify that nothing is missing from the input
    * or that no common error is found.
    * @param rCurrentProcessInfo
    */
    int Check(const ProcessInfo& rCurrentProcessInfo) override;

    std::string Info() const override
    {
        std::stringstream buffer;
        buffer << "IgaShell5pElement #" << Id();
        KRATOS_WATCH(GetValue(SHAPE_FUNCTION_VALUES));
        KRATOS_WATCH(GetValue(SHAPE_FUNCTION_LOCAL_DERIVATIVES));
        KRATOS_WATCH(GetValue(SHAPE_FUNCTION_LOCAL_SECOND_DERIVATIVES));
        KRATOS_WATCH(GetValue(INTEGRATION_WEIGHT));
        return buffer.str();
    }

    /// Print information about this object.

    void PrintInfo(std::ostream& rOStream) const override
    {
        rOStream << "IgaShell5pElement #" << Id();
    }

    ///@}

protected:

private:
    ///@name Static Member Variables
    ///@{
    ///@name Operations
    ///@{
    void CalculateMetric( MetricVariables& metric ) override;


    /**
    * This functions updates the constitutive variables
    * @param rActualMetric: The actual metric
    * @param rThisConstitutiveVariables: The constitutive variables to be calculated
    * @param rValues: The CL parameters
    * @param ThisStressMeasure: The stress measure considered
    */
    void CalculateConstitutiveVariables(
        MetricVariables& rActualMetric,
        ConstitutiveVariables& rThisConstitutiveVariablesMembrane,
        ConstitutiveVariables& rThisConstitutiveVariablesCurvature,
        ConstitutiveLaw::Parameters& rValues,
        const ConstitutiveLaw::StressMeasure ThisStressMeasure
    );
    ///@}

    ///@}
    ///@name Serialization
    ///@{

    friend class Serializer;

    void save(Serializer& rSerializer) const override
    {
        KRATOS_SERIALIZE_SAVE_BASE_CLASS(rSerializer, Element)
    }

    void load(Serializer& rSerializer) override
    {
        KRATOS_SERIALIZE_LOAD_BASE_CLASS(rSerializer, Element)
    }

    ///@}

};     // Class IgaShell5pElement
///@}

}  // namespace Kratos.

#endif // KRATOS_MESHLESS_IGA_SHELL_5P_ELEMENT_H_INCLUDED  defined