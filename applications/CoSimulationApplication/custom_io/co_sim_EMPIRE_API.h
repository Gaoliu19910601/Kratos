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

#ifndef KRATOS_CO_SIM_EMPIRE_API_H_INCLUDED
#define KRATOS_CO_SIM_EMPIRE_API_H_INCLUDED

/*
This file has the same API as EMPIRE (defined in "EMPIRE_API.h"), hence it can be included instead of EMPIRE
It is used FileIO for data-exchange, in VTK-format
Note:
- This file cannot have Kratos-includes, because it is also included in other codes!
- This file is intended to be header-only, such that other codes do not have to link against a library
- Requires c++11 to compile
*/

// System includes
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <unordered_map>

namespace EMPIRE_API_helpers {

// Some options that can be configured
static const std::string ConvergenceSignalFileName = "EMPIRE_convergence_signal.dat";
static const std::string TempFilePreString = ".";
static const bool VtkUseBinary = false;
static const bool PrintTiming = false;
static const int EchoLevel = 0;

static bool FileExists(const std::string& rFileName)
{
    std::ifstream infile(rFileName);
    return infile.good(); // no need to close manually
}

static std::string GetTempFileName(const std::string& rFileName)
{
    // wrapped in a function such that it could be changed easily (e.g. if files are in a folder)
    return TempFilePreString + rFileName;
}

static void WaitForFile(const std::string& rFileName)
{
    if (EchoLevel>0) std::cout << "Waiting for file: \"" << rFileName << "\"" << std::endl;
    while(!FileExists(rFileName)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // wait 0.5s before next check
        if (EchoLevel>1) std::cout << "    Waiting" << std::endl;
    }
    if (EchoLevel>0) std::cout << "Found file: \"" << rFileName << "\"" << std::endl;
}

static void RemoveFile(const std::string& rFileName)
{
    if (std::remove(rFileName.c_str()) != 0) {
        std::cout << "Warning: \"" << rFileName << "\" could not be deleted!" << std::endl;
    }
}

static void MakeFileVisible(const std::string& rFinalFileName)
{
    if (std::rename(GetTempFileName(rFinalFileName).c_str(), rFinalFileName.c_str()) != 0) {
        std::cout << "Warning: \"" << rFinalFileName << "\" could not be made visible!" << std::endl;
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

static void SendArray(const std::string& rFileName, int sizeOfArray, double *data)
{
    std::ofstream output_file;
    output_file.open(EMPIRE_API_helpers::GetTempFileName(rFileName));
    EMPIRE_API_helpers::CheckStream(output_file, rFileName);

    // TODO write size in first line?

    for (int i=0; i<sizeOfArray-1; ++i) {
        output_file << data[i] << " ";
    }
    output_file << data[sizeOfArray-1]; // outside to not have trailing whitespace

    output_file.close();
    EMPIRE_API_helpers::MakeFileVisible(rFileName);
}

static void ReceiveArray(const std::string& rFileName, int sizeOfArray, double *data)
{
    EMPIRE_API_helpers::WaitForFile(rFileName);

    std::ifstream input_file(rFileName);
    EMPIRE_API_helpers::CheckStream(input_file, rFileName);

    for (int i=0; i<sizeOfArray; ++i) {
        input_file >> data[i];
    }

    EMPIRE_API_helpers::RemoveFile(rFileName);
}

static int GetVtkCellType(const int NumberOfNodes)
{
    if (NumberOfNodes == 1) {
        return 1;
    } else if (NumberOfNodes == 2) {
        return 3;
    } else if (NumberOfNodes == 3) {
        return 5;
    } else if (NumberOfNodes == 4) {
        return 9;
    } else {
        std::stringstream err_msg;
        err_msg << "Unsupported number of nodes/element: " << NumberOfNodes;
        throw std::runtime_error(err_msg.str());
    }
}

} // namespace EMPIRE_API_helpers

/***********************************************************************************************
 * \brief Establishes the necessary connection with the Emperor
 ***********/
static void EMPIRE_API_Connect(char* inputFileName)
{
    std::cout << "Called \"EMPIRE_API_Connect\" which is no longer necessary and can be removed" << std::endl;
}

/***********************************************************************************************
 * \brief Get user defined text by the element name in the XML input file
 * \param[in] elementName name of the XML element
 * \return user defined text
 ***********/
static char *EMPIRE_API_getUserDefinedText(char *elementName)
{
    std::cout << "Called \"EMPIRE_API_getUserDefinedText\" with \"" << elementName << "\" which is no longer working and can be removed" << std::endl;
    return ""; // TODO this gives a warning, find better solution
}

/***********************************************************************************************
 * \brief Send the mesh to the server
 * \param[in] name name of the mesh
 * \param[in] numNodes number of nodes
 * \param[in] numElems number of elements
 * \param[in] nodes coordinates of all nodes
 * \param[in] nodeIDs IDs of all nodes
 * \param[in] numNodesPerElem number of nodes per element
 * \param[in] elems connectivity table of all elements
 ***********/
static void EMPIRE_API_sendMesh(char *name, int numNodes, int numElems, double *nodes, int *nodeIDs, int *numNodesPerElem, int *elems)
{
    const std::string file_name("EMPIRE_mesh_" + std::string(name) + ".vtk");

    std::ofstream output_file;
    output_file.open(EMPIRE_API_helpers::GetTempFileName(file_name));
    EMPIRE_API_helpers::CheckStream(output_file, file_name);

    // write file header
    output_file << "# vtk DataFile Version 4.0\n";
    output_file << "vtk output\n";
    if (EMPIRE_API_helpers::VtkUseBinary) {
        output_file << "BINARY\n";
    } else {
        output_file << "ASCII\n";
    }
    output_file << "DATASET UNSTRUCTURED_GRID\n\n";

    // write nodes
    int vtk_id = 0;
    std::unordered_map<int, int> node_vtk_id_map;
    output_file << "POINTS " << numNodes << " float\n";
    for (int i=0; i<numNodes; ++i) {
        output_file << nodes[i*3] << " " << nodes[i*3+1] << " " << nodes[i*3+2] << "\n";
        node_vtk_id_map[nodeIDs[i]] = vtk_id++;
    }
    output_file << "\n";

    // write cells connectivity
    int cell_list_size = 0;
    for (int i=0; i<numElems; ++i) {
        cell_list_size += numNodesPerElem[i] + 1;
    }

    int counter=0;
    output_file << "CELLS " << numElems << " " << cell_list_size << "\n";
    for (int i=0; i<numElems; ++i) {
        const int num_nodes_elem = numNodesPerElem[i];
        output_file << num_nodes_elem << " ";
        for (int j=0; j<num_nodes_elem; ++j) {
            output_file << node_vtk_id_map.at(elems[counter++]);
            if (j<num_nodes_elem-1) output_file << " ";
        }
        output_file << "\n";
    }

    output_file << "\n";

    // write cell types
    output_file << "CELL_TYPES " << numElems << "\n";
    for (int i=0; i<numElems; ++i) {
        output_file << EMPIRE_API_helpers::GetVtkCellType(numNodesPerElem[i]) << "\n";
    }

    output_file.close();
    EMPIRE_API_helpers::MakeFileVisible(file_name);
}

/***********************************************************************************************
 * \brief Recieve mesh from the server
 * \param[in] name name of the mesh
 * \param[in] numNodes number of nodes
 * \param[in] numElems number of elements
 * \param[in] nodes coordinates of all nodes
 * \param[in] nodeIDs IDs of all nodes
 * \param[in] numNodesPerElem number of nodes per element
 * \param[in] elems connectivity table of all elements
 ***********/
static void EMPIRE_API_recvMesh(char *name, int *numNodes, int *numElems, double **nodes, int **nodeIDs, int **numNodesPerElem, int **elem)
{
    const std::string file_name("EMPIRE_mesh_" + std::string(name) + ".vtk");

    EMPIRE_API_helpers::WaitForFile(file_name);

    std::ifstream input_file(file_name);
    EMPIRE_API_helpers::CheckStream(input_file, file_name);

    // reading file
    std::string current_line;
    bool nodes_read = false;

    while (std::getline(input_file, current_line)) {
        // reading nodes
        if (current_line.find("POINTS") != std::string::npos) {
            if (nodes_read) throw std::runtime_error("The nodes were read already!");
            nodes_read = true;

            current_line = current_line.substr(current_line.find("POINTS") + 6); // removing "POINTS"
            std::istringstream line_stream(current_line);
            line_stream >> *numNodes;

            // allocating memory for nodes
            // note that this has to be deleted by the client!
            *nodes = new double[(*numNodes) * 3];
            *nodeIDs = new int[*numNodes];

            for (int i=0; i<*numNodes*3; ++i) {
                input_file >> (*nodes)[i];
            }

            for (int i=0; i<*numNodes; ++i) {
                (*nodeIDs)[i] = i+1; // Node Ids have an offset of 1 from Kratos to VTK
            }
        }

        // reading elements
        if (current_line.find("CELLS") != std::string::npos) {
            if (!nodes_read) throw std::runtime_error("The nodes were not yet read!");

            int num_nodes_per_elem, node_id, cell_list_size;
            current_line = current_line.substr(current_line.find("CELLS") + 6); // removing "CELLS"
            std::istringstream line_stream(current_line);
            line_stream >> *numElems;
            line_stream >> cell_list_size;

            // allocating memory for elements
            // note that this has to be deleted by the client!
            *numNodesPerElem = new int[*numElems];
            *elem = new int[cell_list_size-*numElems];

            int counter=0;
            for (int i=0; i<*numElems; ++i) {
                input_file >> num_nodes_per_elem;
                (*numNodesPerElem)[i] = num_nodes_per_elem;
                for (int j=0; j<num_nodes_per_elem; ++j) {
                    input_file >> node_id;
                    (*elem)[counter++] = node_id+1; // Node Ids have an offset of 1 from Kratos to VTK
                }
            }
            break; // no further information reading required => CELL_TYPES are not used here
        }
    }

    EMPIRE_API_helpers::RemoveFile(file_name);
}

/***********************************************************************************************
 * \brief Send data field to the server
 * \param[in] name name of the field
 * \param[in] sizeOfArray size of the array (data field)
 * \param[in] dataField the data field to be sent
 ***********/
static void EMPIRE_API_sendDataField(char *name, int sizeOfArray, double *dataField)
{
    const std::string file_name("EMPIRE_datafield_" + std::string(name) + ".dat");

    EMPIRE_API_helpers::SendArray(file_name, sizeOfArray, dataField);
}

/***********************************************************************************************
 * \brief Receive data field from the server
 * \param[in] name name of the field
 * \param[in] sizeOfArray size of the array (data field)
 * \param[out] dataField the data field to be received
 ***********/
static void EMPIRE_API_recvDataField(char *name, int sizeOfArray, double *dataField)
{
    const std::string file_name("EMPIRE_datafield_" + std::string(name) + ".dat");

    EMPIRE_API_helpers::ReceiveArray(file_name, sizeOfArray, dataField);
}

/***********************************************************************************************
 * \brief Send signal to the server
 * \param[in] name name of the signal
 * \param[in] sizeOfArray size of the array (signal)
 * \param[in] signal the signal
 ***********/
static void EMPIRE_API_sendSignal_double(char *name, int sizeOfArray, double *signal)
{
    const std::string file_name("EMPIRE_signal_" + std::string(name) + ".dat");

    EMPIRE_API_helpers::SendArray(file_name, sizeOfArray, signal);
}

/***********************************************************************************************
 * \brief Receive signal from the server
 * \param[in] name name of the signal
 * \param[in] sizeOfArray size of the array (signal)
 * \param[in] signal the signal
 ***********/
static void EMPIRE_API_recvSignal_double(char *name, int sizeOfArray, double *signal)
{
    const std::string file_name("EMPIRE_signal_" + std::string(name) + ".dat");

    EMPIRE_API_helpers::ReceiveArray(file_name, sizeOfArray, signal);
}

/***********************************************************************************************
 * \brief Receive the convergence signal of an loop
 * \return 1 means convergence, 0 means non-convergence
 ***********/
static int EMPIRE_API_recvConvergenceSignal()
{
    EMPIRE_API_helpers::WaitForFile(EMPIRE_API_helpers::ConvergenceSignalFileName);

    std::ifstream input_file(EMPIRE_API_helpers::ConvergenceSignalFileName);
    EMPIRE_API_helpers::CheckStream(input_file, EMPIRE_API_helpers::ConvergenceSignalFileName);

    int signal;
    input_file >> signal;

    if (!(signal == 0 || signal == 1)) {
        std::stringstream err_msg;
        err_msg << "Read an invalid convergence signal: " << signal << ", can only be 0 for non-convergence or 1 for convergence";
        throw std::runtime_error(err_msg.str());
    }

    EMPIRE_API_helpers::RemoveFile(EMPIRE_API_helpers::ConvergenceSignalFileName);

    return signal;
}

/***********************************************************************************************
 * \brief Send the convergence signal of an loop
 * \param[in] signal 1 means convergence, 0 means non-convergence
 ***********/
static void EMPIRE_API_sendConvergenceSignal(int signal)
{
    if (!(signal == 0 || signal == 1)) {
        std::stringstream err_msg;
        err_msg << "Input can only be 0 for non-convergence or 1 for convergence, called with: " << signal;
        throw std::runtime_error(err_msg.str());
    }

    std::ofstream output_file;
    output_file.open(EMPIRE_API_helpers::GetTempFileName(EMPIRE_API_helpers::ConvergenceSignalFileName));
    EMPIRE_API_helpers::CheckStream(output_file, EMPIRE_API_helpers::ConvergenceSignalFileName);

    output_file << signal;

    output_file.close();
    EMPIRE_API_helpers::MakeFileVisible(EMPIRE_API_helpers::ConvergenceSignalFileName);
}

/***********************************************************************************************
 * \brief Performs disconnection and finalization operations to the Emperor
 ***********/
static void EMPIRE_API_Disconnect()
{
    std::cout << "Called \"EMPIRE_API_Disconnect\" which is no longer necessary and can be removed" << std::endl;
}

#endif /* KRATOS_CO_SIM_EMPIRE_API_H_INCLUDED */
