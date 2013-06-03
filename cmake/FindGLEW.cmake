#
# GLEW
#

if(WIN64)
  FIND_LIBRARY(GLEW_LIBRARY glew32 GLEW ${GLEW_ROOT}/lib/x64 ${GLEW_ROOT}/lib)
else(WIN64)  
  FIND_LIBRARY(GLEW_LIBRARY glew32 GLEW ${GLEW_ROOT}/lib /usr/lib /usr/local/lib /usr/lib64 /usr/local/lib64)
endif(WIN64)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG GLEW_LIBRARY)
