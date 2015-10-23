// First new GL program
// Just makes a red triangle

#include "Angel.h"
#include <iostream>
#include <cmath>

using namespace std;

#define PI 3.14159265

typedef vec3 color3;

const int TriPoints = 3;
const int QuadPoints = 4;
const int CirPoints = 500;
const int EllPoints = 500;

//--------------------------------------------------------------------------

void
init( void )
{
    // Specifiy the vertices for a triangle
    vec2 tri_ver[TriPoints] = {
        vec2( 0.3*sin(4*PI/3), 0.6+0.3*cos(4*PI/3) ), vec2( 0.3*sin(0), 0.6+0.3*cos(0) ), vec2( 0.3*sin(2*PI/3), 0.6+0.3*cos(2*PI/3) )
    };

    vec2 quad_ver[6][QuadPoints];
    for (int i = 0; i < 6; i++) {
        quad_ver[i][0] = vec2( (0.6-0.1*i)*cos(PI/4), -0.2+(0.6-0.1*i)*sin(PI/4) );
        quad_ver[i][1] = vec2( (0.6-0.1*i)*cos(3*PI/4), -0.2+(0.6-0.1*i)*sin(3*PI/4) );
        quad_ver[i][2] = vec2( (0.6-0.1*i)*cos(7*PI/4), -0.2+(0.6-0.1*i)*sin(7*PI/4) );
        quad_ver[i][3] = vec2( (0.6-0.1*i)*cos(5*PI/4), -0.2+(0.6-0.1*i)*sin(5*PI/4) );
    }

    vec2 cir_ver[CirPoints];
    for (int i = 0; i < CirPoints; i++)
        cir_ver[i] = vec2( 0.7+0.2*cos(i*2*PI/CirPoints), 0.7+0.2*sin(i*2*PI/CirPoints) );

    vec2 ell_ver[EllPoints];
    for (int i = 0; i < EllPoints; i++)
        ell_ver[i] = vec2( -0.6+0.25*cos(i*2*PI/EllPoints), 0.7+0.25*0.6*sin(i*2*PI/EllPoints) );

    color3 tri_col[TriPoints] = {
        color3( 0, 1, 0 ), color3( 1, 0, 0 ), color3( 0, 0, 1 )
    };

    color3 quad_col[6][QuadPoints];
    for (int i = 0; i < 6; i++)
        if (i % 2 == 0) 
            for (int j = 0; j < QuadPoints; j++)
                quad_col[i][j] = color3( 1, 1, 1 ); 
        else
            for (int j = 0; j < QuadPoints; j++)
                quad_col[i][j] = color3( 0, 0, 0 ); 

    color3 cir_col[CirPoints];
    for (int i = 0; i < CirPoints; i++)
        cir_col[i] = color3( (float)i/(float)CirPoints, 0, 0 );

    color3 ell_col[EllPoints];
    for (int i = 0; i < EllPoints; i++)
        ell_col[i] = color3( 1, 0, 0);

    // Create a vertex array object
    GLuint vao[1];
    glGenVertexArrays( 1, vao );
    glBindVertexArray( vao[0] );
    cout<<"glGenVertexArrays(), glBindVertexArray() called."<<endl;

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    cout<<"glGenBuffer(), glBindBuffer() called."<<endl;
    glBufferData( GL_ARRAY_BUFFER, sizeof(tri_ver)+sizeof(quad_ver)+sizeof(cir_ver)+sizeof(ell_ver)+sizeof(tri_col)+sizeof(quad_col)+sizeof(cir_col)+sizeof(ell_col), tri_ver, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(tri_ver), sizeof(quad_ver), quad_ver );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(tri_ver)+sizeof(quad_ver), sizeof(cir_ver), cir_ver );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(tri_ver)+sizeof(quad_ver)+sizeof(cir_ver), sizeof(ell_ver), ell_ver );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(tri_ver)+sizeof(quad_ver)+sizeof(cir_ver)+sizeof(ell_ver), sizeof(tri_col), tri_col );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(tri_ver)+sizeof(quad_ver)+sizeof(cir_ver)+sizeof(ell_ver)+sizeof(tri_col), sizeof(quad_col), quad_col );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(tri_ver)+sizeof(quad_ver)+sizeof(cir_ver)+sizeof(ell_ver)+sizeof(tri_col)+sizeof(quad_col), sizeof(cir_col), cir_col );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(tri_ver)+sizeof(quad_ver)+sizeof(cir_ver)+sizeof(ell_ver)+sizeof(tri_col)+sizeof(quad_col)+sizeof(cir_col), sizeof(ell_col), ell_col);
    cout<<"glBufferData(), glBufferSubData() called."<<endl;

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    glUseProgram( program );
    cout<<"InitShader(), glUseProgram() called."<<endl;

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc_ver = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc_ver );
    glVertexAttribPointer( loc_ver, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    GLuint loc_col = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( loc_col );
    glVertexAttribPointer( loc_col, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(sizeof(tri_ver)+sizeof(quad_ver)+sizeof(cir_ver)+sizeof(ell_ver)) );
    cout<<"glEnableVertexAttribArray(), glVertexAttribPointer() called."<<endl;

    glClearColor( 0, 0, 0, 0 ); // black background
    cout<<"glClearColor(0,0,0,0) called."<<endl;
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window
    cout<<"glClear() called."<<endl;
    glDrawArrays( GL_TRIANGLES, 0, TriPoints );    // draw the points
    glDrawArrays( GL_TRIANGLE_STRIP, TriPoints, QuadPoints );
    glDrawArrays( GL_TRIANGLE_STRIP, TriPoints + QuadPoints, QuadPoints );
    glDrawArrays( GL_TRIANGLE_STRIP, TriPoints + 2*QuadPoints, QuadPoints );
    glDrawArrays( GL_TRIANGLE_STRIP, TriPoints + 3*QuadPoints, QuadPoints );
    glDrawArrays( GL_TRIANGLE_STRIP, TriPoints + 4*QuadPoints, QuadPoints );
    glDrawArrays( GL_TRIANGLE_STRIP, TriPoints + 5*QuadPoints, QuadPoints );
    glDrawArrays( GL_TRIANGLE_FAN, TriPoints + 6*QuadPoints, CirPoints );
    glDrawArrays( GL_TRIANGLE_FAN, TriPoints + 6*QuadPoints + CirPoints, EllPoints );
    cout<<"glDrawArrays() called."<<endl;
    glFlush();
    cout<<"glFlush() called."<<endl;
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    cout<<"glutInit(&argc,argv) called."<<endl;
    glutInitDisplayMode( GLUT_RGBA );
    cout<<"glutInitDisplayMode(GLUT_RGBA) called."<<endl;
    glutInitWindowSize( 500, 500 );
    cout<<"glutInitWindowSize(500,500) called."<<endl;

    glutCreateWindow( "ICG_hw2" );
    cout<<"glutCreateWindow() called."<<endl;
    glewExperimental=GL_TRUE; 
    glewInit();    
    cout<<"glutInit() called."<<endl;
    init();
    cout<<"My initialization function init() called."<<endl;

    glutDisplayFunc( display );
    cout<<"glutDisplayFunc(display) called."<<endl;
    glutKeyboardFunc( keyboard );
    cout<<"glutKeyboardFunc(keyboard) called."<<endl;

    glutMainLoop();
    cout<<"glutMainLoop() called."<<endl;
    return 0;
}
