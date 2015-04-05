#version 330

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

uniform mat4 modelViewProjection;
uniform unsigned int isHorizontal;

layout( location = 0 ) in vec4 inPosition;


// ----------------------------------------------------------------------------------
// Vertex Procedure
// ----------------------------------------------------------------------------------

void main()
{
    vec4 clippingCoordinates = modelViewProjection * inPosition;
    if( isHorizontal > 0u )
    {
        clippingCoordinates.x = inPosition.x;
    }
    else
    {
        clippingCoordinates.y = inPosition.y;
    }
    gl_Position = clippingCoordinates;
}
