#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "di.h"

#define RADIUS      50      // radius of a circle

int main( void )
{ 
    GLFWwindow *window;

    static GLfloat last = 0; 
    static GLfloat lastColorChange = 0;
    GLfloat timer = glfwGetTime();
    GLfloat start;
    start = timer;
    last = timer;
    lastColorChange = timer;
    
    // array of circles 
    Circle circles[5];
    int length = 5;
    int i;
    
    // time to wait before moving the circle 
    GLfloat sleep = 0.01;
    // total running time
    GLfloat total = TOTAL_TIME * 60; // 1 minutes has 60 seconds
    // time to wait before changing color of circles
    GLfloat colorChange = 5.0;
    // index of chosen circle 
    int chosenCircle = 0;
    
    // seeding for random function
    srand(time(NULL));
    
    // initialize all circles in the array
    initCircleArray(circles, length);
    
    glfwSetErrorCallback(error);
    
    if ( !glfwInit( ) ){
        return -1;
    }
    
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "five", NULL, NULL );
    
    if ( !window )
    {
        glfwTerminate( );
        return -1;
    }
    
    // Make the window's context current
    glfwMakeContextCurrent( window );
    
    // set up port of view to use pixel unit 
    glViewport( 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT ); 
    glMatrixMode( GL_PROJECTION ); 
 
    glLoadIdentity( ); 
    glOrtho( 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1 ); 
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( ); 
    
    glfwSetKeyCallback(window, key_callback);
    
    while (timer - start < total
            && !glfwWindowShouldClose(window)) {
        glClear( GL_COLOR_BUFFER_BIT );

        timer = glfwGetTime();
        
        // whether we should change the color 
        if (timer - lastColorChange > colorChange) {
            lastColorChange = timer;    
            // set all circles to blue
            for (i = 0; i < length; i++) {
                circles[i].colour[0] = 0;
                circles[i].colour[1] = 0;
                circles[i].colour[2] = 255;
            }
            // get a random index
            chosenCircle = rand() % length;
            // set green color to the chosen circle
            circles[chosenCircle].colour[0] = 0;
            circles[chosenCircle].colour[1] = 255;
            circles[chosenCircle].colour[2] = 0;
        }
        
        // whether we should move a circle 
        if (timer - last > sleep) {
            last = timer;  
            // update position and draw all circles
            for (i = 0; i < length; i++) {
                moves(circles, length, i);
                drawCircle(circles[i]);
            }
        } else {
            // redraw all circles
            for (i = 0; i < length; i++) {               
                drawCircle(circles[i]);
            }
        }
                
        glfwSwapBuffers( window );        
        glfwPollEvents( );
    }

    glfwDestroyWindow(window);
    glfwTerminate( );
    
    return 0;
}

void moves(Circle circle[], int length, int index){
     
    int direction[] = {1, -1}; 
    Circle c;
    int i;
    GLfloat tmp;
    c = circle[index] ;
    
    // the current circle will have to avoid all previously considered circles 
    for (i = 0; i < index; i++) {  
        // they collide
        if (testCollision(c, circle[i]) == 1) {               
            // 2 circles will travel in opposite vectors, post-collision 
            c.xVector = (c.x - circle[i].x)  / RADIUS * (rand() % DISTANCE);
            c.yVector = (c.y - circle[i].y)  / RADIUS * (rand() % DISTANCE);
            circle[i].xVector = (-1.0) * c.xVector ;
            circle[i].yVector = (-1.0) * c.yVector ;              
        }  
    }
    
    // the circle won't move along a horizontal line 
    while (c.xVector == 0.0) {
        c.xVector = (rand() % DISTANCE) * direction[rand() % 2] ;
    }
    // the circle won't move along a vertical line  
    while (c.yVector == 0.0) {
         c.yVector = (rand() % DISTANCE) * direction[rand() % 2];    
    }
    
    // detect edges 
    // move away from edges
    if (c.y  + RADIUS > SCREEN_HEIGHT 
            || c.y - RADIUS < 0 ) 
    {
        c.yVector *= (-1);
    }
    if (c.x  + RADIUS > SCREEN_WIDTH
            || c.x - RADIUS < 0 ) 
    {
        c.xVector *= (-1);
    }
    
    // update central position
    c.x += c.xVector;    
    c.y += c.yVector;
    
    // set the new altered circle to the current consider circle 
    circle[index] = c; 
}


int testCollision(Circle c1, Circle c2){
    GLfloat distance = sqrt((c1.x - c2.x) * (c1.x - c2.x) 
            + (c1.y - c2.y) * (c1.y - c2.y) );
    if (distance <= c1.radius * 2.0) {
        return 1;
    }
    return 0;
}

void initCircleArray(Circle circle[], int length ){
    int i;
    
    // set position
    circle[0].x = RADIUS;
    circle[1].x = SCREEN_WIDTH - RADIUS;
    circle[2].x = RADIUS;
    circle[3].x = SCREEN_WIDTH - RADIUS;
    circle[4].x = SCREEN_WIDTH / 2;
    
    circle[0].y = SCREEN_HEIGHT - RADIUS;
    circle[1].y = SCREEN_HEIGHT - RADIUS;
    circle[2].y = RADIUS;
    circle[3].y = RADIUS;
    circle[4].y = SCREEN_HEIGHT / 2; 
    
    // initialize circles    
    for (i = 0; i < length; i++) {
        circle[i] = initCircle(circle[i]);  
    } 
    
    // set color of the central circle to green
    circle[4].colour[0] = 0;
    circle[4].colour[1] = 255;
    circle[4].colour[2] = 0; 
}

Circle initCircle(Circle circle){
    Circle c = circle;
    int direction[] = {1, -1}; 
    c.numberOfSides = SIDE_NUM;
    c.radius = RADIUS;
    c.z = 0;
    
    c.xVector = 0.0;
    c.yVector = 0.0;
    
    // set central position
    while (c.xVector == 0.0) {
        c.xVector = (rand() % DISTANCE) * direction[rand() % 2] ;
    }

    while (c.yVector == 0.0) {
         c.yVector = (rand() % DISTANCE) * direction[rand() % 2];    
    }
    // set default color to blue
    c.colour[0] = 0;
    c.colour[1] = 0;
    c.colour[2] = 255;
    return c;
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


void drawCircle( Circle circle )
{
    GLint numberOfVertices = 1 + circle.numberOfSides;
    
    GLfloat doublePi = 2.0f * M_PI;
    
    GLfloat circleVerticesX[numberOfVertices];
    GLfloat circleVerticesY[numberOfVertices];
    GLfloat circleVerticesZ[numberOfVertices];
    GLfloat colour[circle.numberOfSides * 3] ;
   
    
    int i;
    int j;
    for ( i = 0; i < numberOfVertices; i++ )
    {
        // set color to the circle on screen
        j = i * 3;
        colour[j] = circle.colour[0];
        colour[j+1] = circle.colour[1];
        colour[j+2] = circle.colour[2];
        
        circleVerticesX[i] = circle.x + ( circle.radius * cos( i * doublePi / circle.numberOfSides ) );
        circleVerticesY[i] = circle.y + ( circle.radius * sin( i * doublePi / circle.numberOfSides ) );
        circleVerticesZ[i] = circle.z;
    }
    
    GLfloat allCircleVertices[numberOfVertices * 3];
    
    for (  i = 0; i < numberOfVertices; i++ )
    {
        allCircleVertices[i * 3] = circleVerticesX[i];
        allCircleVertices[( i * 3 ) + 1] = circleVerticesY[i];
        allCircleVertices[( i * 3 ) + 2] = circleVerticesZ[i];
    }
    
    glEnableClientState( GL_VERTEX_ARRAY );
    // enable color filling
    glEnableClientState( GL_COLOR_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, allCircleVertices );
    // fill color
    glColorPointer( 3, GL_FLOAT, 0, colour );
    glDrawArrays( GL_TRIANGLE_FAN, 0, numberOfVertices );
    glDisableClientState( GL_COLOR_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );
}

