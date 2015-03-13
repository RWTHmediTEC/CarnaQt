/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#pragma once

#include <QDebug>
#include <QtGlobal>
#include <iostream>

namespace Carna
{

/** \namespace
  * Carna::testing
  *
  * \brief
  * This namespace contains the test suite components.
  *
  * \section TestSuite Test Suite
  *
  * The test suite is built upon QTestLib. It is integrated into CMake not via CTest
  * but as an executable. All files belonging to the test suite are listed within
  * <code>test/CMakeLists.txt</code>. For each class under test there is a test
  * class, that consists of a pair of header and implementation files. The CMake
  * project is set up to build a single executable for the whole test suite.
  *
  * When executed, the the test suite returns \c 0 if and only if all tests pass.
  *
  * \subsection TestSuite_TestTypes Unit tests and module tests
  *
  * Considering test types, the test suite currently differs among unit tests and
  * module tests. For each test type there is a single <code>configure.cmake</code>
  * file. This is where the test cases are specified:
  *
  * - Module tests are specified within
  *   <code>test/ModuleTests/configure.cmake</code>.
  * - Unit tests are specified within <code>test/UnitTests/configure.cmake</code>.
  *
  * \subsection TestSuite_EnlistingTests Defining the test files
  *
  * In order to add a new test, simply choose the proper test type and modify the
  * corresponding <code>configure.cmake</code> file.
  *
  *Each consists of the sections below.
  *
  * - The variable <b>TESTS</b> lists the class names of all test classes. If your
  *   class <code>MyClass</code> was tested by the test class named <code>MyClassTest
  *   </code>, you would have <code>MyClassTest</code> listed here.
  * - The variable <b>TESTS_QOBJECT_HEADERS</b> lists the header files where the test
  *   classes are defined. QTestLib requires each test class to be derived from
  *   <code>QObject</code>.
  * - If you had any additional header files that do not contain the <code>Q_OBJECT
  *   </code> macro, they needed to be listed within <b>TESTS_HEADERS</b>.
  * - <b>APPEND TESTS_SOURCES</b> lists the test classes implementation files.
  *
  * \subsection TestSuite_MiscellaneousFiles Miscellaneous files
  *
  * There are two ways of specifying the test suite's additional files that do not
  * define any test classes. If these files are only required by a certain test type,
  * they should be enlisted within the <b>TESTS_QOBJECT_HEADERS</b> or
  * <b>TESTS_HEADERS</b> variable of the corresponding <code> configure.cmake</code>
  * file, depending on whether they do use the <code>Q_OBJECT</code> macro or not.
  * The other option is to enlist them within the <code>test/CMakeLists.txt</code>
  * file, beneath the same variables. This way they become available to each test
  * type.
  */
namespace testing
{



// ----------------------------------------------------------------------------------
// EXPECT_ASSERTION_FAILURE
// ----------------------------------------------------------------------------------

#define EXPECT_ASSERTION_FAILURE( expression ) \
    try \
    { \
        expression; \
        QFAIL( "Expected assertion failure did not raise upon expression." ); \
    } \
    catch( const Carna::base::AssertionFailure& ) \
    { \
    }



}  // namespace Carna :: testing

}  // namespace Carna

using namespace Carna::testing;
