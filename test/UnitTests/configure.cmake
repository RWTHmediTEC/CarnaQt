include_directories( ${CMAKE_PROJECT_DIR}UnitTests )

list( APPEND TESTS
		SpatialListModelTest
	)

list( APPEND TESTS_QOBJECT_HEADERS
		UnitTests/SpatialListModelTest.h
	)

list( APPEND TESTS_HEADERS
		""
	)

list( APPEND TESTS_SOURCES
		UnitTests/SpatialListModelTest.cpp
	)
