############################################
# MediTEC Compatibility Script
# ------------------------------------------
# The script does nothing unless the
# environmental variable MEDITEC_LIBS is set
#-------------------------------------------
# Leonid Kostrykin, 12.03.2015
############################################

if( DEFINED ENV{MEDITEC_LIBS} )

	# normalize paths so we can compare them
	get_filename_component(MEDITEC_LIBS $ENV{MEDITEC_LIBS} ABSOLUTE)
	set(MEDITEC_LIBS ${MEDITEC_LIBS} PARENT_SCOPE)
	
	# adjust CMAKE_MODULE_PATH so that proper dependency versions are found
	list(APPEND CMAKE_MODULE_PATH "${MEDITEC_LIBS}/Carna/3.0")

	# export CMAKE_MODULE_PATH to parent scope
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)

	# print warning if CMAKE_INSTALL_PREFIX isn't set to %MEDITEC_LIBS%
	if( NOT CMAKE_INSTALL_PREFIX STREQUAL ${MEDITEC_LIBS} )
		message(WARNING "\n**WARNING** CMAKE_INSTALL_PREFIX is not set to %MEDITEC_LIBS%\n"
		                  "    CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}\n"
		                  "    %MEDITEC_LIBS%: ${MEDITEC_LIBS}\n")
	else()
		# set default installation paths (relative to CMAKE_INSTALL_PREFIX)
		# now so they won't be set to different default values later
		set(INSTALL_LIBRARY_DIR "${PROJECT_NAME}/${FULL_VERSION}/bin"     PARENT_SCOPE)
		set(INSTALL_HEADERS_DIR "${PROJECT_NAME}/${FULL_VERSION}/include" PARENT_SCOPE)
		set(INSTALL_CMAKE_DIR   "${PROJECT_NAME}/${FULL_VERSION}"         PARENT_SCOPE)
	endif()
	
	# find GLEW headers
	list(APPEND CMAKE_INCLUDE_PATH "$ENV{MEDITEC_LIBS}/GLEW/1.7.0/include")
	find_path(GLEW_INCLUDE_DIR GL/glew.h)
	
	# find GLEW libraries
	list(APPEND CMAKE_LIBRARY_PATH "$ENV{MEDITEC_LIBS}/GLEW/1.7.0/bin")
	find_library(GLEW_LIBRARY NAMES glew32-1.7.0 PATH_SUFFIXES lib64)
	
	# find Eigen headers
	find_path(	EIGEN3_INCLUDE_DIR
				Eigen/Core
				PATHS ENV MEDITEC_LIBS
				PATH_SUFFIXES "Eigen/3.0.5/include"
				DOC "Eigen3 headers")

endif( DEFINED ENV{MEDITEC_LIBS} )

# installs MediTEC-specific files
function( add_meditec_install_routines )
	if( DEFINED ENV{MEDITEC_LIBS} )
		if( CMAKE_INSTALL_PREFIX STREQUAL ${MEDITEC_LIBS} )
			configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/misc/MediTEC/configure.cmake.in
							${CMAKE_CURRENT_BINARY_DIR}/misc/MediTEC/configure.cmake @ONLY )
			
			configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/misc/MediTEC/install.bat.in
							${CMAKE_CURRENT_BINARY_DIR}/misc/MediTEC/install.bat @ONLY )
			
			install(FILES
						${CMAKE_CURRENT_BINARY_DIR}/misc/MediTEC/configure.cmake
						${CMAKE_CURRENT_BINARY_DIR}/misc/MediTEC/install.bat
					DESTINATION "${PROJECT_NAME}/${FULL_VERSION}")
			
			configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/misc/MediTEC/Find${PROJECT_NAME}.cmake.in
							${CMAKE_CURRENT_BINARY_DIR}/misc/MediTEC/Find${PROJECT_NAME}.cmake @ONLY )
			
			install(FILES ${CMAKE_CURRENT_BINARY_DIR}/misc/MediTEC/Find${PROJECT_NAME}.cmake
					DESTINATION "${PROJECT_NAME}/${MAJOR_VERSION}.${MINOR_VERSION}")
			
			install(	TARGETS	${TARGET_NAME}
						RUNTIME DESTINATION	${MEDITEC_LIBS}/bin)
		endif()
	endif()
endfunction()
