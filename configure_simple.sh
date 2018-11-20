# Set variables
export KRATOS_BUILD_TYPE="Debug"

export KRATOS_SRC="."
export KRATOS_BLD="./build"

export KRATOS_INT_APP="${KRATOS_INT_APP}FluidDynamicsApplication;"
export KRATOS_INT_APP="${KRATOS_INT_APP}ExternalSolversApplication;"
export KRATOS_INT_APP="${KRATOS_INT_APP}FSIapplication;"

export KRATOS_EXT_APP="${KRATOS_EXT_APP}/home/roigcarlo/KratosExternalApplications/ExternalFSIApplication;"

# Clean
clear
rm -rf "${KRATOS_BLD}/${KRATOS_BUILD_TYPE}/cmake_install.cmake"
rm -rf "${KRATOS_BLD}/${KRATOS_BUILD_TYPE}/CMakeCache.txt"
rm -rf "${KRATOS_BLD}/${KRATOS_BUILD_TYPE}/CMakeFiles"

# Configure
cmake -H"${KRATOS_SRC}" -B"${KRATOS_BLD}/${KRATOS_BUILD_TYPE}"

# Buid
# cmake --build "${KRATOS_BLD}/${KRATOS_BUILD_TYPE}" -- -j4 install

## Optional ##

# COTIRE - Let's you enable unity builds for Kratos and applications using cotrie
# -DUSE_COTIRE=ON

