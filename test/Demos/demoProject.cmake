cmake_minimum_required(VERSION 3.0.2)

############################################
# It is important to use the root binary directory,
# so that the proper LIB from the tested project is found
# and the EXE is placed in the same directory as the DLL.
############################################
set( CMAKE_CURRENT_BINARY_DIR ${CMAKE_BINARY_DIR} )
############################################

set( CMAKE_DEBUG_POSTFIX d )
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin )

set( TARGET_NAME	${PROJECT_NAME}-${FULL_VERSION} )
############################################



############################################
# Find dependencies and add them later to the linker (see below)
############################################

# Qt
find_package( Qt5Core QUIET )
find_package( Qt5Widgets QUIET )
find_package( Qt5Gui QUIET )
find_package( Qt5OpenGL QUIET )
if(Qt5Core_FOUND AND Qt5Widgets_FOUND AND Qt5Gui_FOUND AND Qt5OpenGL_FOUND)
    set(Qt5_FOUND TRUE)
    set(QT_LIBRARIES Qt5::Core Qt5::Widgets Qt5::Gui Qt5::OpenGL)
    include_directories(
            ${Qt5Core_INCLUDE_DIRS}
            ${Qt5Widgets_INCLUDE_DIRS}
            ${Qt5Gui_INCLUDE_DIRS}
            ${Qt5OpenGL_INCLUDE_DIRS}
        )
else()
    find_package( Qt4 4.8.0 COMPONENTS QtCore QtGui QtOpenGL REQUIRED )
    include( ${QT_USE_FILE} )
    add_definitions( ${QT_DEFINITIONS} )
endif()

# Boost
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.48.0 COMPONENTS iostreams)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

# GLEW
find_package( GLEW 1.7.0 REQUIRED )
include_directories( ${GLEW_INCLUDE_DIRS} )

# Carna
find_package( Carna 3.0.0 REQUIRED )
include_directories( ${CARNA_INCLUDE_DIR} )

# Eigen
find_package( Eigen3 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )

# CarnaQt
include_directories( ${CMAKE_SOURCE_DIR}/include )
link_directories( ${CMAKE_BINARY_DIR}/bin )

############################################
# Project
############################################

include_directories(${CMAKE_PROJECT_DIR}src)
include_directories(${CMAKE_PROJECT_DIR}../../Tools)
set( HEADERS
        ${HEADERS}
		../../Tools/HUGZSceneFactory.h
		../../Tools/HUIO.h
		../../Tools/TestScene.h
	)
set( QOBJECT_HEADERS
		""
	)
set( SRC
        ${SRC}
		../../Tools/TestScene.cpp
		../../Tools/HUGZSceneFactory.cpp
	)
set( FORMS
		""
	)
set( RESOURCES
		""
	)

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

if(Boost_FOUND)
	
    unset( RESOURCES_RCC )
    if(Qt5_FOUND)
        QT5_WRAP_CPP( QOBJECT_HEADERS_MOC ${QOBJECT_HEADERS} )
        QT5_WRAP_UI( FORMS_HEADERS ${FORMS} )
        QT5_ADD_RESOURCES( RESOURCES_RCC ${RESOURCES} )
    else(Qt5_FOUND)
        QT4_WRAP_CPP( QOBJECT_HEADERS_MOC ${QOBJECT_HEADERS} )
        QT4_WRAP_UI( FORMS_HEADERS ${FORMS} )
        QT4_ADD_RESOURCES( RESOURCES_RCC ${RESOURCES} )
    endif(Qt5_FOUND)

    add_definitions(
            -DNOMINMAX
            -DBOOST_IOSTREAMS_NO_LIB
            -D_SCL_SECURE_NO_WARNINGS
            -DEXPECTED_MAJOR_VERSION=${MAJOR_VERSION}
            -DEXPECTED_MINOR_VERSION=${MINOR_VERSION}
            -DEXPECTED_RELEASE_VERSION=${PATCH_VERSION}
            -DSOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../.."
    	)
    remove_definitions( -DCARNAQT_EXPORT )
    
    add_executable( ${TARGET_NAME} WIN32
            ${SRC}
            ${HEADERS}
            ${QOBJECT_HEADERS}
            ${QOBJECT_HEADERS_MOC}
            ${FORMS_HEADERS}
            ${RESOURCES_RCC}
    	)
    
    include( "../../../misc/compiler_specific.cmake" )
    
    target_link_libraries( ${TARGET_NAME}
            ${OPENGL_LIBRARIES}
            ${GLEW_LIBRARIES}
            ${QT_LIBRARIES}
            ${CARNA_LIBRARIES}
            ${Boost_LIBRARIES}
            optimized   CarnaQt-${FULL_VERSION}
            debug       CarnaQt-${FULL_VERSION}${CMAKE_DEBUG_POSTFIX}
    	)
    
    if(MSVC)
        set_target_properties(
                ${TARGET_NAME} PROPERTIES
                WIN32_EXECUTABLE YES
                LINK_FLAGS "/ENTRY:mainCRTStartup"
    	    )
    endif(MSVC)

else(Boost_FOUND)

	message(WARNING
		"\nBoost not found, skipping ${PROJECT_NAME}.\n"
		  "You don't have to worry about this unless you need "
		  "an executable demo project within your CarnaQt "
		  "development environment, i.e. when you are going to "
		  "work on CarnaQt.\n")

endif(Boost_FOUND)
