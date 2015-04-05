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

#include <Carna/qt/MPRStage.h>
#include <Carna/qt/MPRDisplay.h>
#include <Carna/qt/MPRDataFeature.h>
#include <Carna/base/Mesh.h>
#include <Carna/base/Vertex.h>
#include <Carna/base/ShaderManager.h>
#include <Carna/base/ShaderUniform.h>
#include <Carna/base/Composition.h>
#include <Carna/base/VertexBuffer.h>
#include <Carna/base/IndexBuffer.h>
#include <map>
#include <QFile>
#include <QTextStream>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MPRStage :: Details
// ----------------------------------------------------------------------------------

struct MPRStage::Details
{
    struct VideoResources;
    std::unique_ptr< VideoResources > vr;
    
    Details();
    const base::RenderTask* renderTask;
    static bool shaderSourcedLoaded;
    
    static std::string readResource( const std::string& name );
};


bool MPRStage::Details::shaderSourcedLoaded = false;


MPRStage::Details::Details()
{
    if( !shaderSourcedLoaded )
    {
        base::ShaderManager::instance().setSource( "mpr.vert", readResource( ":/shaders/mpr.vert" ) );
        base::ShaderManager::instance().setSource( "mpr.frag", readResource( ":/shaders/mpr.frag" ) );
        shaderSourcedLoaded = true;
    }
}


std::string MPRStage::Details::readResource( const std::string& name )
{
    QFile srcFile( QString::fromStdString( name ) );
    const bool ok = srcFile.open( QFile::ReadOnly | QFile::Text );
    CARNA_ASSERT_EX( ok, "Failed to open resource file: \"" << name + "\"" );
    QTextStream in( &srcFile );
    const QString text = in.readAll();
    srcFile.close();
    return text.toStdString();
}



// ----------------------------------------------------------------------------------
// MPRStage :: Details :: VideoResources
// ----------------------------------------------------------------------------------

struct MPRStage::Details::VideoResources
{
    VideoResources();
    ~VideoResources();
    
    typedef base::Mesh< base::VertexBase, uint8_t > LineMesh;
    const std::unique_ptr< LineMesh > lineMesh;
    static LineMesh* createLineMesh();
    const base::ShaderProgram& shader;
};


MPRStage::Details::VideoResources::VideoResources()
    : lineMesh( createLineMesh() )
    , shader( base::ShaderManager::instance().acquireShader( "mpr" ) )
{
}


MPRStage::Details::VideoResources::~VideoResources()
{
    base::ShaderManager::instance().releaseShader( shader );
}


MPRStage::Details::VideoResources::LineMesh* MPRStage::Details::VideoResources::createLineMesh()
{
    base::VertexBase vertices[ 2 ];
    uint8_t indices[ 2 ];

    vertices[ 0 ].x = -1;
    vertices[ 0 ].y = -1;
    indices [ 0 ] = 0;

    vertices[ 1 ].x = +1;
    vertices[ 1 ].y = +1;
    indices [ 1 ] = 1;
    
    /* Create vertex buffer.
     */
    typedef base::VertexBuffer< LineMesh::Vertex > VBuffer;
    VBuffer* const vertexBuffer = new VBuffer();
    vertexBuffer->copy( vertices, 2 );
    
    /* Create index buffer.
     */
    typedef base::IndexBuffer< LineMesh::Index > IBuffer;
    IBuffer* const indexBuffer = new IBuffer( base::IndexBufferBase::PRIMITIVE_TYPE_LINES );
    indexBuffer->copy( indices, 2 );

    /* Create the mesh.
     */
    return new LineMesh
        ( new base::Composition< base::VertexBufferBase >( vertexBuffer )
        , new base::Composition< base:: IndexBufferBase >(  indexBuffer ) );
}



// ----------------------------------------------------------------------------------
// MPRStage
// ----------------------------------------------------------------------------------

MPRStage::MPRStage( unsigned int geometryTypePlanes )
    : base::GeometryStage< void >::GeometryStage( geometryTypePlanes )
    , pimpl( new Details() )
{
}


MPRStage::~MPRStage()
{
}


void MPRStage::renderPass
    ( const base::math::Matrix4f& vt
    , base::RenderTask& rt
    , const base::Viewport& vp )
{
    if( pimpl->vr.get() == nullptr )
    {
        pimpl->vr.reset( new Details::VideoResources() );
    }

    /* Configure proper GL state.
     */
    base::RenderState rs;
    rs.setDepthTest( false );
    rs.setDepthWrite( false );

    /* Configure shader.
     */
    rt.renderer.glContext().setShader( pimpl->vr->shader );
    base::ShaderUniform< base::Color >( "color", base::Color::RED ).upload();
    
    /* Do the rendering.
     */
    pimpl->renderTask = &rt;
    base::GeometryStage< void >::renderPass( vt, rt, vp );
    pimpl->renderTask = nullptr;
}


void MPRStage::render( const base::Renderable& renderable )
{
    const base::math::Matrix4f modelViewProjection = pimpl->renderTask->projection * renderable.modelViewTransform();
    const base::math::Vector4f a = modelViewProjection * base::math::Vector4f( -100, -100, 0, 1 );
    const base::math::Vector4f b = modelViewProjection * base::math::Vector4f( +100, +100, 0, 1 );
    if( !base::math::isEqual( a.z(), b.z() ) )
    {
        const base::Color* color = &MPRDisplay::DEFAULT_PLANE_COLOR;
        if( renderable.geometry().hasFeature( ROLE_PLANE_DATA ) )
        {
            MPRDataFeature& feature = static_cast< MPRDataFeature& >( renderable.geometry().feature( ROLE_PLANE_DATA ) );
            color = &feature.color;
        }
    
        base::ShaderUniform< base::math::Matrix4f >( "modelViewProjection", modelViewProjection ).upload();
        base::ShaderUniform< base::Color >( "color", *color ).upload();
        if( base::math::isEqual( a.x(), b.x() ) )
        {
            /* Rendering vertical line.
             */
            base::ShaderUniform< unsigned int >( "isHorizontal", 0 ).upload();
            pimpl->vr->lineMesh->render();
        }
        else
        if( base::math::isEqual( a.y(), b.y() ) )
        {
            /* Rendering horizontal line.
             */
            base::ShaderUniform< unsigned int >( "isHorizontal", 1 ).upload();
            pimpl->vr->lineMesh->render();
        }
    }
}



}  // namespace Carna :: qt

}  // namespace Carna

