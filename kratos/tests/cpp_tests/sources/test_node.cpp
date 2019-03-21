//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                     Kratos default license: kratos/license.txt
//
//  Main authors:    Pooyan Dadvand
//                   Vicente Mataix Ferrandiz
//
//

// Project includes
#include "containers/model.h"
#include "testing/testing.h"
#include "includes/model_part.h"

namespace Kratos {
    namespace Testing {

    typedef Node<3> NodeType;

    /**
     *  Here the assigment operator is test
     */

    KRATOS_TEST_CASE_IN_SUITE(NodeAssignOperator, KratosCoreFastSuite)
    {
        Model current_model;
        ModelPart& model_part = current_model.CreateModelPart("test");
        model_part.AddNodalSolutionStepVariable(DISTANCE);
        model_part.AddNodalSolutionStepVariable(VELOCITY);

        auto p_node = model_part.CreateNewNode(1, 1., 0, 0);

        p_node->FastGetSolutionStepValue(DISTANCE) = 12.1;
        p_node->FastGetSolutionStepValue(VELOCITY_X) = 32.4;
        p_node->Set(ACTIVE, true);

        NodeType copy_of_node(2,1,0,0);
        copy_of_node = *p_node;

        KRATOS_CHECK_EQUAL(copy_of_node.Id(), 1);
        KRATOS_CHECK_DOUBLE_EQUAL(copy_of_node.FastGetSolutionStepValue(DISTANCE), 12.1);
        KRATOS_CHECK_DOUBLE_EQUAL(copy_of_node.FastGetSolutionStepValue(VELOCITY_X), 32.4);
        KRATOS_CHECK_DOUBLE_EQUAL(copy_of_node.FastGetSolutionStepValue(VELOCITY_Y), 0.00);
        KRATOS_CHECK_DOUBLE_EQUAL(copy_of_node.FastGetSolutionStepValue(VELOCITY_Z), 0.00);
        KRATOS_CHECK(copy_of_node.Is(ACTIVE));
    }

    /**
     *  Here the clone operator is test
     */
    KRATOS_TEST_CASE_IN_SUITE(NodeCloneOperator, KratosCoreFastSuite)
    {
        Model current_model;
        ModelPart& model_part = current_model.CreateModelPart("test");
        model_part.AddNodalSolutionStepVariable(DISTANCE);
        model_part.AddNodalSolutionStepVariable(VELOCITY);

        auto p_node = model_part.CreateNewNode(1, 1., 0, 0);

        p_node->FastGetSolutionStepValue(DISTANCE) = 12.1;
        p_node->FastGetSolutionStepValue(VELOCITY_X) = 32.4;
        p_node->Set(ACTIVE, true);

        NodeType::Pointer p_clone_of_node = p_node->Clone();

        KRATOS_CHECK_EQUAL(p_clone_of_node->Id(), 1);
        KRATOS_CHECK_DOUBLE_EQUAL(p_clone_of_node->FastGetSolutionStepValue(DISTANCE), 12.1);
        KRATOS_CHECK_DOUBLE_EQUAL(p_clone_of_node->FastGetSolutionStepValue(VELOCITY_X), 32.4);
        KRATOS_CHECK_DOUBLE_EQUAL(p_clone_of_node->FastGetSolutionStepValue(VELOCITY_Y), 0.00);
        KRATOS_CHECK_DOUBLE_EQUAL(p_clone_of_node->FastGetSolutionStepValue(VELOCITY_Z), 0.00);
        KRATOS_CHECK(p_clone_of_node->Is(ACTIVE));
    }

    /**
     *  Here the GetDof position
     */
    KRATOS_TEST_CASE_IN_SUITE(NodeGetDofPosition, KratosCoreFastSuite)
    {
        Model current_model;
        ModelPart& model_part = current_model.CreateModelPart("test");
        model_part.AddNodalSolutionStepVariable(VELOCITY);

        auto p_node = model_part.CreateNewNode(1, 1., 0, 0);
        p_node->AddDof(VELOCITY_X);
        p_node->AddDof(VELOCITY_Y);
        p_node->AddDof(VELOCITY_Z);

        // GetDofPosition
        KRATOS_CHECK_EQUAL(p_node->GetDofPosition(VELOCITY_X), 0);
        KRATOS_CHECK_EQUAL(p_node->GetDofPosition(VELOCITY_Y), 1);
        KRATOS_CHECK_EQUAL(p_node->GetDofPosition(VELOCITY_Z), 2);

        // GetDof with position
        KRATOS_CHECK((p_node->GetDof(VELOCITY_X, 0)).GetVariable() == VELOCITY_X);
        KRATOS_CHECK((p_node->GetDof(VELOCITY_Y, 1)).GetVariable() == VELOCITY_Y);
        KRATOS_CHECK((p_node->GetDof(VELOCITY_Z, 2)).GetVariable() == VELOCITY_Z);

        // GetDof with position not check
        KRATOS_CHECK((p_node->FastGetDof(VELOCITY_X, 0)).GetVariable() == VELOCITY_X);
        KRATOS_CHECK((p_node->FastGetDof(VELOCITY_Y, 1)).GetVariable() == VELOCITY_Y);
        KRATOS_CHECK((p_node->FastGetDof(VELOCITY_Z, 2)).GetVariable() == VELOCITY_Z);
    }
}  // namespace Testing.
}  // namespace Kratos.
