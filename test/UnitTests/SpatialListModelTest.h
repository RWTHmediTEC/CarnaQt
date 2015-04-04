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

#include <Carna/qt/CarnaQt.h>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// SpatialListModelTest
// ----------------------------------------------------------------------------------

class SpatialListModelTest : public QObject
{

    Q_OBJECT

private slots:

    /** \brief
      * Called before the first test function is executed.
      */
    void initTestCase();

    /** \brief
      * Called after the last test function is executed.
      */
    void cleanupTestCase();

    /** \brief
      * Called before each test function is executed.
      */
    void init();

    /** \brief
      * Called after each test function is executed.
      */
    void cleanup();

 // ----------------------------------------------------------------------------------
 
    void test_emptyRoot();

    void test_typical();
    
    void test_typicalUpdated();
    
    void test_typicalCleared();

 // ----------------------------------------------------------------------------------
    
private:

    std::unique_ptr< base::Node > root;
    std::unique_ptr< qt::SpatialListModel > listModel;
    
    void verifyListModel( const std::set< std::string >& expectedTags );
    
}; // SpatialListModelTest



}  // namespace Carna :: testing

}  // namespace Carna
