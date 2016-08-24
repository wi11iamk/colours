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
GLFWcursor *cursor;

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define M_PI 3.141592653
#define GLFW_HAND_CURSOR   0x00036004

void drawCircle( GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides );

void writeData(FILE * out, GLfloat oX, GLfloat oY, GLfloat radius,
                GLfloat cX, GLfloat cY, double timer,
                GLfloat distance);
/* 
 * out      --> output stream
 * oX       --> center of the circle X
 * oY       --> center of the circle Y
 * radius   --> radius of the circle 
 * cX       --> cursor position   
 * cY       --> cursor position      
 * timer    --> current time
 * distance --> distance from cursor to center of the circle 
 */

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
    GLfloat xStep = 5;
    GLfloat radius = 60.0;
    
    // time to wait before  moving the cirle 
    GLfloat sleep = 0.03;
    
    char text[] = "output.txt";
    FILE * out = fopen(text, "w");
    
    // temporary position of cursor
    double xpos;
    double ypos;
    
    // current position of cursor 
    double cX;
    double cY;
     
    GLfloat distance;
    
    glfwSetErrorCallback(error);
    
    if ( !glfwInit( ) 
            || out == NULL)
    {
        return -1;
    }
    
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "ln", NULL, NULL );
    
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
    
    cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    glfwSetCursor( window, cursor ); // set to null to reset cursor 
    glfwGetCursorPos(window, &cX, &cY);
    
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
 
            // change position of circle-center, i.e. we move the circle 
            x += xStep;
        }

        drawCircle( x, y, 0, radius, 36);
        
        //update cursor position
        glfwGetCursorPos(window, &xpos, &ypos);
        cX = xpos;
        cY = SCREEN_HEIGHT - ypos;            
        distance = sqrt((cX - x) * (cX - x) + (cY - y) * (cY - y));    
        writeData(out, x, y, radius, cX, cY, timer, distance);            

        glfwSwapBuffers( window );        
        glfwPollEvents( );
    }
 
    fclose(out);
    glfwDestroyWindow(window);
    glfwDestroyCursor(cursor);

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

void writeData(FILE * out, GLfloat oX, GLfloat oY, GLfloat radius,
                GLfloat cX, GLfloat cY, double timer,
                GLfloat distance){
    if (distance <= radius) {
        fprintf(out,"+++%0.3f: Cursor position: xPos:%f yPos:%f \n",
               timer, cX, cY);
    } else {
        fprintf(out,"---%0.3f: Cursor position: xPos:%f yPos:%f \n",
               timer, cX, cY);
    }
    
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

