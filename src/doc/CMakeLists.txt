############################################
# Doxygen API documentation
############################################

find_package( Doxygen REQUIRED )
if( DOXYGEN_FOUND )
	add_custom_target(	doc ALL
						${DOXYGEN_EXECUTABLE} 
						WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/doc
						COMMENT "Generating API documentation with Doxygen" VERBATIM )
	install(	DIRECTORY	${CMAKE_CURRENT_SOURCE_DIR}/doc
				DESTINATION	${INSTALL_DIR} )
endif( DOXYGEN_FOUND )

############################################
