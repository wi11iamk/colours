#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stddef.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow *window;

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define M_PI 3.141592653

void drawCircle( GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides );

static void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods);

static void error(int error, const char *desc);

int main( void )
{
    // application time 
    static GLfloat last = 0;
    GLfloat delta;
    GLfloat timer = glfwGetTime();
    last = timer;
    
    // circle position and other attribute 
    GLfloat x = SCREEN_WIDTH / 3;
    GLfloat y = SCREEN_HEIGHT / 2;
    GLfloat xStep = 3;
    GLfloat yStep = 3;
    GLfloat radius = 65.0;
    GLfloat lambda = SCREEN_HEIGHT / 4;
    
    // time to wait before  moving the cirle 
    GLfloat sleep = 0.03;

    glfwSetErrorCallback(error);
    
    if ( !glfwInit( ) )
    {
        return -1;
    }
    
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "fx", NULL, NULL );
    
    if ( !window )
    {
        glfwTerminate( );
        return -1;
    }
    
    glfwMakeContextCurrent( window );
     
    glViewport( 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT ); 
    glMatrixMode( GL_PROJECTION ); 
 
    glLoadIdentity( ); 
    glOrtho( 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1 ); 
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( ); 

    glfwSetKeyCallback(window, key_callback);
    
    while ( !glfwWindowShouldClose( window ) )
    {
        glClear( GL_COLOR_BUFFER_BIT );
        
        timer = glfwGetTime();
        
        // whether we should move the circle
        if (timer - last > sleep) {
            last = timer;  
            
            // edge detection  
            if (x + radius > SCREEN_WIDTH ||
                    x - radius < 0.0) {
                xStep *= -1;
            }
            
            if (//y + radius > 3 * SCREEN_HEIGHT / 4        <-- if top/bottom bounds are desired
                    //|| y - radius < SCREEN_HEIGHT / 4     <-- if top/bottom bounds are desired
                    x + radius > SCREEN_WIDTH ||
                    x - radius < 0.0) {
                yStep *= -1;
            }
 
            // change position of circle-center, i.e. we move the circle 
            x += xStep;
            y += yStep * (2 * sin(2 * M_PI * x / lambda)) * 3;
        }

        drawCircle( x, y, 0, radius, 36);

        glfwSwapBuffers( window );        
        glfwPollEvents( );
    }

    glfwDestroyWindow(window);
    glfwTerminate( );
    
    return 0;
}

static void error(int error, const char *desc)
{
    fputs(desc, stderr);
}

static void key_callback(GLFWwindow *w, int key, int scancode, 
        int action, int mods){
        if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
            glfwSetWindowShouldClose(w, GL_TRUE);
}

void drawCircle( GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides )
{
    GLint numberOfVertices = numberOfSides + 1;
    
    GLfloat doublePi = 2.0f * M_PI;
    
    GLfloat circleVerticesX[numberOfVertices];
    GLfloat circleVerticesY[numberOfVertices];
    GLfloat circleVerticesZ[numberOfVertices];
    
    int i;
    for ( i = 0; i < numberOfVertices; i++ )
    {
        circleVerticesX[i] = x + ( radius * cos( i * doublePi / numberOfSides ) );
        circleVerticesY[i] = y + ( radius * sin( i * doublePi / numberOfSides ) );
        circleVerticesZ[i] = z;
    }
    
    GLfloat allCircleVertices[numberOfVertices * 3];
    
    for (  i = 0; i < numberOfVertices; i++ )
    {
        allCircleVertices[i * 3] = circleVerticesX[i];
        allCircleVertices[( i * 3 ) + 1] = circleVerticesY[i];
        allCircleVertices[( i * 3 ) + 2] = circleVerticesZ[i];
    }
    
    glEnableClientState( GL_VERTEX_ARRAY );
    glColor3f(1, 0, 1);
    glVertexPointer( 3, GL_FLOAT, 0, allCircleVertices );
    glDrawArrays( GL_TRIANGLE_FAN, 0, numberOfVertices );
    glDisableClientState( GL_VERTEX_ARRAY );
}

