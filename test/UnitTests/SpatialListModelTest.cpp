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

#include "SpatialListModelTest.h"
#include <Carna/qt/SpatialListModel.h>
#include <Carna/base/Node.h>
#include <Carna/base/Geometry.h>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// SpatialListModelTest
// ----------------------------------------------------------------------------------

void SpatialListModelTest::initTestCase()
{
}


void SpatialListModelTest::cleanupTestCase()
{
}


void SpatialListModelTest::init()
{
    root.reset( new base::Node() );
    listModel.reset( new qt::SpatialListModel( *root ) );
}


void SpatialListModelTest::cleanup()
{
    listModel.reset();
    root.reset();
}


void SpatialListModelTest::test_emptyRoot()
{
    QApplication::processEvents();
    QCOMPARE( listModel->spatials(), static_cast< std::size_t >( 0 ) );
}


void SpatialListModelTest::test_typical()
{
    /* Setup the scene:
     *
     *              root
     *             /  | \
     * (movable) g1  p1  p2
     *                  /  \
     *      (movable) g2    p3
     *                     /  \
     *     (non-movable) g3    g4 (non-movable)
     */
    
    base::Geometry* const g1 = new base::Geometry( 0, "Object 1" );
    root->attachChild( g1 );
    
    root->attachChild( new base::Node() );
    
    base::Node* const p2 = new base::Node();
    root->attachChild( p2 );
    
    base::Geometry* const g2 = new base::Geometry( 0, "Object 2" );
    p2->attachChild( g2 );
    
    base::Node* const p3 = new base::Node( "Object 3" );
    p2->attachChild( p3 );
    
    base::Geometry* const g3 = new base::Geometry( 0 );
    g3->setMovable( false );
    p3->attachChild( g3 );
    
    base::Geometry* const g4 = new base::Geometry( 0 );
    g4->setMovable( false );
    p3->attachChild( g4 );
    
    /* Compose set of expected tags.
     */
    std::set< std::string > expectedTags;
    expectedTags.insert( g1->tag() );
    expectedTags.insert( g2->tag() );
    expectedTags.insert( p3->tag() );
    
    /* Verify the list model.
     */
    verifyListModel( expectedTags );
}


void SpatialListModelTest::test_typicalUpdated()
{
    test_typical();
    
    /* Add another movable geometry to the root node.
     */
    base::Geometry* const gExtra = new base::Geometry( 0, "Extra Object" );
    root->attachChild( new base::Geometry( 0, "Extra Object" ) );
    
    /* Compose set of expected tags.
     */
    std::set< std::string > expectedTags;
    root->visitChildren( true, [&expectedTags]( base::Spatial& spatial )
        {
            if( !spatial.tag().empty() )
            {
                expectedTags.insert( spatial.tag() );
            }
        }
    );
    
    /* Verify the list model.
     */
    QCOMPARE( expectedTags.size(), static_cast< std::size_t >( 4 ) );
    verifyListModel( expectedTags );
}


void SpatialListModelTest::test_typicalCleared()
{
    test_typical();
    
    /* Clear the root node.
     */
    root->deleteAllChildren();
    
    /* Verify the list model.
     */
    QApplication::processEvents();
    QCOMPARE( listModel->spatials(), static_cast< std::size_t >( 0 ) );
}


void SpatialListModelTest::verifyListModel( const std::set< std::string >& expectedTags )
{
    /* Verify number of objects found.
     */
    QApplication::processEvents();
    QCOMPARE( listModel->spatials(), expectedTags.size() );
    
    /* Tick each found tag from the expectations.
     */
    std::set< std::string > tickList( expectedTags.begin(), expectedTags.end() );
    for( std::size_t spatialIdx = 0; spatialIdx < listModel->spatials(); ++spatialIdx )
    {
        const std::string& tag = listModel->spatialAt( spatialIdx ).tag();
        const auto tagItr = tickList.find( tag );
        QVERIFY( tagItr != tickList.end() );
        tickList.erase( tagItr );
    }
    
    /* Verify that all expected tags have been found.
     */
    QVERIFY( tickList.empty() );
}



}  // namespace Carna :: testing

}  // namespace Carna
