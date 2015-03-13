############################################
# Doxygen API documentation
############################################

find_package( Doxygen REQUIRED )
if( DOXYGEN_FOUND )
	add_custom_target(	doc ALL
						${DOXYGEN_EXECUTABLE} src/doc/Doxyfile
						WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../..
						COMMENT "Generating API documentation with Doxygen" VERBATIM )
	install(	DIRECTORY	${CMAKE_CURRENT_SOURCE_DIR}/../../doc/html
				DESTINATION	${INSTALL_DOC_DIR} )
endif( DOXYGEN_FOUND )

############################################
