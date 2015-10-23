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
const int PentaPoints = 5;
const int TotalTri = 729;

int ip = 0;

//--------------------------------------------------------------------------

void 
triangle(vec2 a, vec2 b, vec2 c, vec2 points[])
{
    points[ip++] = a;
    points[ip++] = b;
    points[ip++] = c;
}

//--------------------------------------------------------------------------

void
divide_triangle(vec2 a, vec2 b, vec2 c, vec2 points[], int m = 5)
{
    vec2 ab, ac, bc;
    if (m > 0)
    {
        ab = (a+b)/2;
        ac = (a+c)/2;
        bc = (b+c)/2;
        divide_triangle(a, ac, ab, points, m-1);
        divide_triangle(c, bc, ac, points, m-1);
        divide_triangle(b, ab, bc, points, m-1);
    }
    else triangle(a, b, c, points);
}

//--------------------------------------------------------------------------

void
init( void )
{
    // Specifiy the vertices for a triangle
    vec2 tri_ori[TriPoints] = {
        vec2( 0.5*sin(4*PI/3), 0.4+0.5*cos(4*PI/3) ), vec2( 0.5*sin(0), 0.4+0.5*cos(0) ), vec2( 0.5*sin(2*PI/3), 0.4+0.5*cos(2*PI/3) )
    };
    
    vec2 tri_ver[TotalTri];
    divide_triangle( tri_ori[0], tri_ori[1], tri_ori[2], tri_ver);

    vec2 quad_ver[QuadPoints] = {
        vec2( -0.5+0.5*sin(0), -0.5+0.5*cos(0) ), vec2( -0.5+0.5*sin(PI/2), -0.5+0.5*cos(PI/2) ), vec2( -0.5+0.5*sin(-PI/2), -0.5+0.5*cos(-PI/2) ), vec2( -0.5+0.5*sin(PI), -0.5+0.5*cos(PI) )
    };

    vec2 penta_ver[PentaPoints + 2];
    penta_ver[0] = vec2( 0.5, -0.5 );
    for (int i = 1; i <= PentaPoints+1; i++)
        penta_ver[i] = vec2( 0.5+0.5*sin((i-1)*2*PI/5), -0.5+0.5*cos((i-1)*2*PI/5) );

    color3 tri_col[TotalTri];
    for (int i = 0; i < TotalTri; i++)
        tri_col[i] = color3( 0, 1, 1);

    color3 quad_col[QuadPoints] = {
        color3( 0, 1, 1 ), color3( 1, 1, 0 ), color3( 1, 1, 0 ), color3( 0, 1, 1 )
    };

    color3 penta_col[PentaPoints + 2] = {
        color3( 1, 1, 0 ), color3( 1, 0, 1 ), color3( 0, 1, 1 ), color3( 1, 0, 0 ), color3( 0, 1, 0 ), color3( 0, 0, 1 ), color3( 1, 0, 1 )
    };
    

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

    glBufferData( GL_ARRAY_BUFFER, sizeof(tri_ver)+sizeof(quad_ver)+sizeof(penta_ver)+sizeof(tri_col)+sizeof(quad_col)+sizeof(penta_col), tri_ver, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(tri_ver), sizeof(quad_ver), quad_ver );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(tri_ver)+sizeof(quad_ver), sizeof(penta_ver), penta_ver );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(tri_ver)+sizeof(quad_ver)+sizeof(penta_ver), sizeof(tri_col), tri_col );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(tri_ver)+sizeof(quad_ver)+sizeof(penta_ver)+sizeof(tri_col), sizeof(quad_col), quad_col );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(tri_ver)+sizeof(quad_ver)+sizeof(penta_ver)+sizeof(tri_col)+sizeof(quad_col), sizeof(penta_col), penta_col );
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
                           BUFFER_OFFSET(sizeof(tri_ver)+sizeof(quad_ver)+sizeof(penta_ver)) );
    cout<<"glEnableVertexAttribArray(), glVertexAttribPointer() called."<<endl;

    glClearColor( 1, 1, 1, 1 ); // black background
    cout<<"glClearColor(1,1,1,1) called."<<endl;
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window
    cout<<"glClear() called."<<endl;
    glDrawArrays( GL_TRIANGLES, 0, TotalTri );    // draw the points
    glDrawArrays( GL_TRIANGLE_STRIP, TotalTri, QuadPoints );
    glDrawArrays( GL_TRIANGLE_FAN, TotalTri+QuadPoints, PentaPoints+2 );
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

    glutCreateWindow( "ICG_hw2_part2" );
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
