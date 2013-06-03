#
# CLAPACK & CBLAS
# (for Windows)
#

SET(CLAPACK_INCLUDE_SEARCH_PATH	${CLAPACK_ROOT}/include)
if(WIN64)
  SET(CLAPACK_LIB_SEARCH_PATH	${CLAPACK_ROOT}/lib/x64 ${CLAPACK_ROOT}/lib)
else(WIN64)
  SET(CLAPACK_LIB_SEARCH_PATH	${CLAPACK_ROOT}/lib)
endif(WIN64)

FIND_PATH(BLAS_INCLUDE_PATH blaswrap.h ${CLAPACK_INCLUDE_SEARCH_PATH})
FIND_PATH(CLAPACK_INCLUDE_PATH clapack.h ${CLAPACK_INCLUDE_SEARCH_PATH})
FIND_PATH(F2C_INCLUDE_PATH f2c.h ${CLAPACK_INCLUDE_SEARCH_PATH})

FIND_LIBRARY(BLAS_LIBRARY blas ${CLAPACK_LIB_SEARCH_PATH})
FIND_LIBRARY(CLAPACK_LIBRARY lapack ${CLAPACK_LIB_SEARCH_PATH})
FIND_LIBRARY(F2C_LIBRARY libf2c ${CLAPACK_LIB_SEARCH_PATH})

IF(BLAS_INCLUDE_PATH AND CLAPACK_INCLUDE_PATH AND F2C_INCLUDE_PATH AND BLAS_LIBRARY AND CLAPACK_LIBRARY AND F2C_LIBRARY)
	SET(CLAPACK_INCLUDE_DIR ${BLAS_INCLUDE_PATH} ${CLAPACK_INCLUDE_PATH} ${F2C_INCLUDE_PATH})
	SET(CLAPACK_LIBRARY ${BLAS_LIBRARY} ${CLAPACK_LIBRARY}  ${F2C_LIBRARY})
	SET(HAVE_CLAPACK YES)
	find_package_handle_standard_args(LAPACK DEFAULT_MSG CLAPACK_LIBRARY)
	find_package_handle_standard_args(BLAS DEFAULT_MSG BLAS_LIBRARY)
	find_package_handle_standard_args(F2C DEFAULT_MSG F2C_LIBRARY)
ENDIF(BLAS_INCLUDE_PATH AND CLAPACK_INCLUDE_PATH AND F2C_INCLUDE_PATH AND BLAS_LIBRARY AND CLAPACK_LIBRARY AND F2C_LIBRARY)

IF(LAPACK_FOUND AND BLAS_FOUND AND F2C_FOUND)
	SET(CLAPACK_FOUND TRUE)
	MESSAGE(STATUS "CLAPACK found.")
ELSE(LAPACK_FOUND AND BLAS_FOUND AND F2C_FOUND)
	MESSAGE(FATAL_ERROR "Could not find CLAPACK.")
ENDIF(LAPACK_FOUND AND BLAS_FOUND AND F2C_FOUND)