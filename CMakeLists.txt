cmake_minimum_required(VERSION 3.0.2)
project(CarnaQt)

set(MAJOR_VERSION   1)
set(MINOR_VERSION   0)
set(PATCH_VERSION   2)

############################################

set(CMAKE_DEBUG_POSTFIX d)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(FULL_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
set(TARGET_NAME ${PROJECT_NAME}-${FULL_VERSION})
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_CAPS)

############################################
# Set default options for this build
############################################

option(BUILD_DOC    "Build and install the API documentation" OFF)
option(BUILD_TEST   "Build the unit tests" OFF)
option(BUILD_DEMO	"Build interactive demos" OFF)

############################################
# Macro that sets variable to default value
# only when the variable isn't defined yet
############################################

macro( option_default_to var_name default_val var_type doc_string )
    if( NOT DEFINED ${var_name} )
        set(${var_name} ${default_val})
    endif()
    set(${var_name} ${${var_name}} CACHE ${var_type} ${doc_string} FORCE)
endmacro()

############################################
# Locate Find<ModuleName>.cmake scripts
############################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/misc/CMake-Modules)

############################################
# Run MediTEC compatiblity script
############################################

add_subdirectory(misc/MediTEC)

############################################
# Define default paths for the installation
############################################

# set default library and header destinations (relative to CMAKE_INSTALL_PREFIX)
option_default_to(INSTALL_LIBRARY_DIR "lib"                       String "Installation directory for libraries")
option_default_to(INSTALL_HEADERS_DIR "include"                   String "Installation directory for headers")
option_default_to(INSTALL_DOC_DIR     "share/doc/${PROJECT_NAME}" String "Installation directory for API documentation")

# set default destination for CMake modules
option_default_to(INSTALL_CMAKE_DIR "${CMAKE_ROOT}/Modules" String "Installation directory for CMake files")

############################################
# Normalize installation paths
# (get rid of Windows-style delimiters)
############################################

file(   TO_CMAKE_PATH   ${INSTALL_LIBRARY_DIR}  INSTALL_LIBRARY_DIR )
file(   TO_CMAKE_PATH   ${INSTALL_HEADERS_DIR}  INSTALL_HEADERS_DIR )
file(   TO_CMAKE_PATH   ${INSTALL_CMAKE_DIR}    INSTALL_CMAKE_DIR )

############################################

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/misc/Version.h.in
                ${CMAKE_CURRENT_SOURCE_DIR}/include/Carna/qt/Version.h @ONLY)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/src/doc/Doxyfile.in
                ${CMAKE_CURRENT_SOURCE_DIR}/src/doc/Doxyfile @ONLY)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/misc/Find${PROJECT_NAME}.cmake.in
                ${CMAKE_CURRENT_BINARY_DIR}/misc/Find${PROJECT_NAME}.cmake @ONLY)

############################################
# Find required dependencies
############################################

# Qt
find_package( Qt5Core QUIET )
find_package( Qt5Widgets QUIET )
find_package( Qt5Gui QUIET )
find_package( Qt5OpenGL QUIET )
find_package( Qt5Xml QUIET )
if(Qt5Core_FOUND AND Qt5Widgets_FOUND AND Qt5Gui_FOUND AND Qt5OpenGL_FOUND AND Qt5Xml_FOUND)
    set(Qt5_FOUND TRUE)
    set(QT_LIBRARIES Qt5::Core Qt5::Widgets Qt5::Gui Qt5::OpenGL Qt5::Xml)
    include_directories(
            ${Qt5Core_INCLUDE_DIRS}
            ${Qt5Widgets_INCLUDE_DIRS}
            ${Qt5Gui_INCLUDE_DIRS}
            ${Qt5OpenGL_INCLUDE_DIRS}
            ${Qt5Xml_INCLUDE_DIRS}
        )
else()
    find_package( Qt4 4.8.0 COMPONENTS QtCore QtGui QtOpenGL QtXml REQUIRED )
    include( ${QT_USE_FILE} )
    add_definitions( ${QT_DEFINITIONS} )
endif()

# GLEW
find_package( GLEW 1.7.0 REQUIRED )
include_directories( ${GLEW_INCLUDE_DIRS} )

# Eigen
find_package( Eigen3 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )

# Carna
find_package( Carna 3.0.0 REQUIRED )
include_directories( ${CARNA_INCLUDE_DIR} )

############################################
# Project
############################################

include_directories(${CMAKE_PROJECT_DIR}include)
set( PUBLIC_QOBJECT_HEADERS
        include/Carna/qt/ExpandableGroupBox.h
        include/Carna/qt/DRRControl.h
        include/Carna/qt/MultiSpanSlider.h
        include/Carna/qt/ColorMapTrackerEditor.h
        include/Carna/qt/WideColorPicker.h
        include/Carna/qt/ColorPicker.h
        include/Carna/qt/ColorMapEditor.h
        include/Carna/qt/DVRControl.h
        include/Carna/qt/VolumeRenderingControl.h
        include/Carna/qt/MIPLayerEditor.h
        include/Carna/qt/MIPControlLayer.h
        include/Carna/qt/MIPControl.h
        include/Carna/qt/WindowingControl.h
)
set( PUBLIC_HEADERS
        ${PUBLIC_QOBJECT_HEADERS}
        include/Carna/qt/Application.h
        include/Carna/qt/CarnaQt.h
        include/Carna/qt/Display.h
        include/Carna/qt/FrameRendererFactory.h
        include/Carna/qt/Version.h
        include/Carna/qt/RenderStageControl.h
        include/Carna/qt/MultiSpanSliderModelViewMapping.h
        include/Carna/qt/MultiSpanSliderTracker.h
        include/Carna/qt/IntSpanPainter.h
        include/Carna/qt/NullIntSpanPainter.h
        include/Carna/qt/ColorMapSpanPainter.h
        include/Carna/qt/ColorMapSpanPainter.h
        include/Carna/qt/ColorMapTracker.h
        include/Carna/qt/ColorPickerPainter.h
        include/Carna/qt/QColorConversion.h
        include/Carna/qt/SpatialListModel.h
        include/Carna/qt/MPRDisplay.h
        include/Carna/qt/MPR.h
    )
include_directories(${CMAKE_PROJECT_DIR}src/include)
set( PRIVATE_QOBJECT_HEADERS
        src/include/Carna/qt/RenderStageControlDetails.h
        src/include/Carna/qt/MIPControlDetails.h
        src/include/Carna/qt/SpatialListModelDetails.h
)
set( PRIVATE_HEADERS
        ${PRIVATE_QOBJECT_HEADERS}
        src/include/Carna/qt/MPRStage.h
        src/include/Carna/qt/MPRDataFeature.h
    )
set( SRC
        src/qt/Application.cpp
        src/qt/Display.cpp
        src/qt/FrameRendererFactory.cpp
        src/qt/RenderStageControl.cpp
        src/qt/DRRControl.cpp
        src/qt/ExpandableGroupBox.cpp
        src/qt/MultiSpanSlider.cpp
        src/qt/MultiSpanSliderTracker.cpp
        src/qt/ColorMapSpanPainter.cpp
        src/qt/ColorMapEditor.cpp
        src/qt/ColorMapTracker.cpp
        src/qt/ColorMapTrackerEditor.cpp
        src/qt/ColorPickerPainter.cpp
        src/qt/WideColorPicker.cpp
        src/qt/ColorPicker.cpp
        src/qt/DVRControl.cpp
        src/qt/VolumeRenderingControl.cpp
        src/qt/MIPLayerEditor.cpp
        src/qt/MIPControlLayer.cpp
        src/qt/MIPControl.cpp
        src/qt/SpatialListModel.cpp
        src/qt/MPRDisplay.cpp
        src/qt/MPRStage.cpp
        src/qt/MPRDataFeature.cpp
        src/qt/MPR.cpp
        src/qt/WindowingControl.cpp
    )
set( FORMS
        ""
    )
set( RESOURCES
	src/res.qrc
    )
set( DOC_SRC
        src/doc/Doxyfile.in
        src/doc/doc_extra.css
        src/doc/doc_main.dox
        src/doc/doc_version_log.dox
    )

############################################

if(Qt5_FOUND)
    QT5_WRAP_CPP( PUBLIC_HEADERS_MOC ${PUBLIC_QOBJECT_HEADERS} )
    QT5_WRAP_CPP( PRIVATE_HEADERS_MOC ${PRIVATE_QOBJECT_HEADERS} )
    QT5_WRAP_UI( FORMS_HEADERS ${FORMS} )
    QT5_ADD_RESOURCES( RESOURCES_RCC ${RESOURCES} )
else(Qt5_FOUND)
    QT4_WRAP_CPP( PUBLIC_HEADERS_MOC ${PUBLIC_QOBJECT_HEADERS} )
    QT4_WRAP_CPP( PRIVATE_HEADERS_MOC ${PRIVATE_QOBJECT_HEADERS} )
    QT4_WRAP_UI( FORMS_HEADERS ${FORMS} )
    QT4_ADD_RESOURCES( RESOURCES_RCC ${RESOURCES} )
endif(Qt5_FOUND)

############################################
# Setup Visual Studio filters
############################################

SOURCE_GROUP( "Headers"
    REGULAR_EXPRESSION .*include/Carna/qt/[^/]*\\.h )

SOURCE_GROUP( "Sources"
    REGULAR_EXPRESSION src/qt/[^/]*\\.cpp )

SOURCE_GROUP( "Miscellaneous"
    REGULAR_EXPRESSION .*\\.\(\(in\)|\(txt\)|\(qrc\)\) )

SOURCE_GROUP( "Miscellaneous\\Documentation"
    REGULAR_EXPRESSION .*\\.\(\(css\)|\(dox\)\) )

SOURCE_GROUP( "Miscellaneous\\Generated Files"
    REGULAR_EXPRESSION build/\(\(.*\\.\(\(cxx\)|\(cpp\)\)\)|\(.*/res/.*\)\) )

############################################
# Exclude all source files from build,
# since we are using unity build.
############################################

set_source_files_properties(    ${SRC}
                                ${PUBLIC_HEADERS_MOC}
                                ${PRIVATE_HEADERS_MOC}
                                ${FORMS_HEADERS}
                                ${RESOURCES_RCC}
                                ${DOC_SRC}
    PROPERTIES
    HEADER_FILE_ONLY TRUE )

############################################
# Write unity build file
############################################

set( UNITY_BUILD_FILE   ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-unitybuild.cpp )

file( REMOVE    ${UNITY_BUILD_FILE} )
file( WRITE     ${UNITY_BUILD_FILE} "// This file is automatically generated by CMake.\n\n" )
file( APPEND    ${UNITY_BUILD_FILE} "#include <Carna/base/glew.h>\n" )

foreach( SOURCE_FILE ${SRC} )
    file( APPEND    ${UNITY_BUILD_FILE} "#include \"${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}\"\n" )
endforeach( SOURCE_FILE )

foreach( QT_GENERATED_SOURCE_FILE ${PUBLIC_HEADERS_MOC} ${PRIVATE_HEADERS_MOC} ${FORMS_HEADERS} ${RESOURCES_RCC} )
	file( APPEND	${UNITY_BUILD_FILE}	"#include \"${QT_GENERATED_SOURCE_FILE}\"\n" )
endforeach( QT_GENERATED_SOURCE_FILE )

############################################
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
############################################

add_library( ${TARGET_NAME} SHARED
            ${SRC}
            ${UNITY_BUILD_FILE}
            ${PUBLIC_HEADERS}
            ${PRIVATE_HEADERS}
            ${PUBLIC_HEADERS_MOC}
            ${PRIVATE_HEADERS_MOC}
            ${FORMS_HEADERS}
            ${RESOURCES_RCC}
            ${DOC_SRC}
        )

add_definitions( -D${PROJECT_NAME_CAPS}_EXPORT -DNOMINMAX )

############################################
# Add dependencies to the linker
############################################

include( "misc/compiler_specific.cmake" )

target_link_libraries( ${TARGET_NAME}
            ${OPENGL_LIBRARIES}
            ${GLEW_LIBRARIES}
            ${CARNA_LIBRARIES}
            ${QT_LIBRARIES}
        )

############################################
# Define installation routines
############################################

install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION ${INSTALL_LIBRARY_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIBRARY_DIR}
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR})

# install headers
install(DIRECTORY ${CMAKE_PROJECT_DIR}include/Carna
        DESTINATION ${INSTALL_HEADERS_DIR})

# also install '.PDB' files on Windows
if( WIN32 )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/bin/${TARGET_NAME}${CMAKE_DEBUG_POSTFIX}.pdb
            DESTINATION ${INSTALL_LIBRARY_DIR}
            OPTIONAL)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/bin/${TARGET_NAME}${CMAKE_DEBUG_POSTFIX}.pdb
            DESTINATION ${INSTALL_LIBRARY_DIR}
            OPTIONAL)
endif( WIN32 )

# install CMake Find<Module> file
install(    FILES ${CMAKE_CURRENT_BINARY_DIR}/misc/Find${PROJECT_NAME}.cmake
            DESTINATION ${INSTALL_CMAKE_DIR})

# install MediTEC-specific files
add_meditec_install_routines()

############################################
# Build interactive demos
############################################

if( BUILD_DEMO )
	add_subdirectory( test/Demos )
endif()

############################################
# Process unit tests
############################################

if( BUILD_TEST )
    add_subdirectory( test )
endif()

############################################
# Doxygen API documentation
############################################

if( BUILD_DOC )
    add_subdirectory( src/doc )
endif()

############################################

