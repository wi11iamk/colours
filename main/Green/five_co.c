#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#define RADIUS 50
#define CIRCLE_NUM 5

#include "co.h"

int main(void) {

    // for application time calculations
    static GLfloat last = 0;
    GLfloat timer = glfwGetTime();
    GLfloat start;
    start = timer;
    last = timer;

    // array of circles 
    Circle circles[CIRCLE_NUM];
    int length = CIRCLE_NUM;
    int i;
    int currentCircleIndex;

    // time to wait before moving the circle 
    GLfloat sleep = 0.01;
    // total running time
    GLfloat total = TOTAL_TIME * 60; // 1 minutes has 60 seconds

    // output file name
    char text[] = "output.txt";
    // output stream
    FILE * out = fopen(text, "w");
    globalOut = out;
    
    //initialize the global variable
    chosenCircleIndex = 0;
    currentCircleIndex = chosenCircleIndex;
    currentCircle = &circles[currentCircleIndex];

    srand(time(NULL));

    initCircleArray(circles, length);

    glfwSetErrorCallback(error);

    if (!glfwInit() || out == NULL) {
        return -1;
    }

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "five_co", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    glfwSetCursor( window, cursor ); // set to null to reset cursor 

    glfwSetKeyCallback(window, key_callback);

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    while (timer - start < total
            && !glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        timer = glfwGetTime();

        writeToFile(out, circles[currentCircleIndex], timer, window);

        // whether we should change the color 
        if (currentCircleIndex != chosenCircleIndex) { 
            currentCircleIndex = chosenCircleIndex;
            // set all circle to blue
            for (i = 0; i < length; i++) {
                circles[i].colour[0] = 0;
                circles[i].colour[1] = 0;
                circles[i].colour[2] = 255;
            }

            // set green color to the chosen circle
            circles[currentCircleIndex].colour[0] = 0;
            circles[currentCircleIndex].colour[1] = 255;
            circles[currentCircleIndex].colour[2] = 0;

            currentCircle = &circles[currentCircleIndex];   
        }

        // whether we should move the circle 
        if (timer - last > sleep) {
            last = timer;
            for (i = 0; i < length; i++) {
                moves(circles, length, i);
                drawCircle(circles[i]);
            }
        } else {
            for (i = 0; i < length; i++) {
                drawCircle(circles[i]);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    fclose(out);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void playSound() {
    char * sound [] = {"sounds/scale-a6.wav", "sounds/scale-c6.wav", "sounds/scale-c7.wav"
        , "sounds/scale-d6.wav", "sounds/scale-e6.wav", "sounds/scale-f6.wav",
        "sounds/scale-g6.wav", "sounds/scale-h6.wav"};
    int index = rand() % 8;
    ALuint buffer, source;
    ALuint state;

    // Initialize the environment
    alutInit(0, NULL);

    // Capture errors
    alGetError();

    // Load pcm data into buffer
    buffer = alutCreateBufferFromFile(sound[index]);

    // Create sound source (use buffer to fill source)
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);

    // Play
    alSourcePlay(source);

    // Wait for the song to complete
    do {
        alGetSourcei(source, AL_SOURCE_STATE, &state);
    } while (state == AL_PLAYING);

    // Clean up sources and buffers
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);

    // Exit everything
    alutExit();
}

void mouse_button_callback(GLFWwindow* window,
        int button, int action, int mods) {
    int currentCircleIndex = chosenCircleIndex;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // get a random index
        if (isCursorInTheChosenCircle() == 1) {
            do {
                chosenCircleIndex = rand() % CIRCLE_NUM;
            } while (currentCircleIndex == chosenCircleIndex);

            playSound();

            fprintf(globalOut, "___Correct___\n");
        }
        else {
            fprintf(globalOut, "___Incorrect___\n");
        }
    }
}

void writeToFile(FILE * out, Circle circle, double timer, GLFWwindow *window) {

    // temporary position of cursor 
    double xpos;
    double ypos;
    
    // current position of cursor 
    double cX;
    double cY;
    
    // distance from center of chosen circle to the cursor 
    GLfloat distance;

    glfwGetCursorPos(window, &xpos, &ypos);
    
    // we will get update if we move the cursor on the screen         
    cX = xpos;
    cY = SCREEN_HEIGHT - ypos;
    
    // get distance from the cursor to the  center of the circle
    distance = sqrt((cX - circle.x) *
            (cX - circle.x)
            + (cY - circle.y) * (cY - circle.y));
            
    // write data to the file     
    writeData(out, circle.x,
            circle.y, RADIUS,
            cX, cY, timer, distance);
}

int isCursorInTheChosenCircle() {

    // cursor's temporary position 
    double xpos;
    double ypos;
    
    // current position of cursor 
    double cX;
    double cY;
    
    // distance from center of chosen circle to the cursor 
    GLfloat distance;

    // get current position of the cursor 
    glfwGetCursorPos(window, &xpos, &ypos);         
    cX = xpos;
    cY = SCREEN_HEIGHT - ypos;
    
    // get distance from the cursor to the  center of the circle
    distance = sqrt((cX - currentCircle->x) *
            (cX - currentCircle->x)
            + (cY - currentCircle->y) * (cY - currentCircle->y));
            
    if (distance <= currentCircle->radius) {
        return 1;
    }
    return 0;
}

void writeData(FILE * out, GLfloat oX, GLfloat oY, GLfloat radius,
        GLfloat cX, GLfloat cY, double timer,
        GLfloat distance) {
    if (distance <= radius) {
        fprintf(out, "Inside  +++%0.3f: Cursor position: %0.1f %0.1f \n",
                timer, cX, cY);
    } else {
        fprintf(out, "Outside ---%0.3f: Cursor position: %0.1f %0.1f \n",
                timer, cX, cY);
    }

}

void moves(Circle circle[], int length, int index) {

    int direction[] = {1, -1};
    Circle c;
    int i;
    GLfloat tmp;
    c = circle[index];

    // the current circle will have to avoid all previous considered circles 
    for (i = 0; i < index; i++) {
        // they collide
        if (testCollision(c, circle[i]) == 1) {
            // 2 circles will travel in opposite directions, post-collision 
            c.xVector = (c.x - circle[i].x) / RADIUS * (rand() % DISTANCE);
            c.yVector = (c.y - circle[i].y) / RADIUS * (rand() % DISTANCE);
            circle[i].xVector = (-1.0) * c.xVector;
            circle[i].yVector = (-1.0) * c.yVector;
        }
    }

    // the circle  won't move along horizontal line 
    while (c.xVector == 0.0) {
        c.xVector = (rand() % DISTANCE) * direction[rand() % 2];
    }
    // the circle  won't move along vertical line 
    while (c.yVector == 0.0) {
        c.yVector = (rand() % DISTANCE) * direction[rand() % 2];
    }

    // detect and move away from edges
    if (c.y + RADIUS > SCREEN_HEIGHT
            || c.y - RADIUS < 0) {
        c.yVector *= (-1);
    }
    if (c.x + RADIUS > SCREEN_WIDTH
            || c.x - RADIUS < 0) {
        c.xVector *= (-1);
    }

    // update central position
    c.x += c.xVector;
    c.y += c.yVector;

    // set the new altered circle to the current consider circle 
    circle[index] = c;
}

int testCollision(Circle c1, Circle c2) {
    GLfloat distance = sqrt((c1.x - c2.x) * (c1.x - c2.x)
            + (c1.y - c2.y) * (c1.y - c2.y));
    if (distance <= c1.radius * 2.0) {
        return 1;
    }
    return 0;
}

void initCircleArray(Circle circle[], int length) {
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

    // init circles    
    for (i = 0; i < length; i++) {
        circle[i] = initCircle(circle[i]);
    }

    // set  color of the central circle to green
    circle[4].colour[0] = 0;
    circle[4].colour[1] = 255;
    circle[4].colour[2] = 0;
    chosenCircleIndex = 4;
    currentCircle = &circle[4];
}

Circle initCircle(Circle circle) {
    Circle c = circle;
    int direction[] = {1, -1};
    c.numberOfSides = SIDE_NUM;
    c.radius = RADIUS;
    c.z = 0;

    c.xVector = 0.0;
    c.yVector = 0.0;

    // set central position
    while (c.xVector == 0.0) {
        c.xVector = (rand() % DISTANCE) * direction[rand() % 2];
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

static void error(int error, const char *desc) {
    fputs(desc, stderr);
}

static void key_callback(GLFWwindow *w, int key, int scancode,
        int action, int mods) {
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
        glfwSetWindowShouldClose(w, GL_TRUE);
}

void drawCircle(Circle circle) {
    GLint numberOfVertices = 1 + circle.numberOfSides;

    GLfloat doublePi = 2.0f * M_PI;

    GLfloat circleVerticesX[numberOfVertices];
    GLfloat circleVerticesY[numberOfVertices];
    GLfloat circleVerticesZ[numberOfVertices];
    GLfloat colour[circle.numberOfSides * 3];

    int i;
    int j;
    for (i = 0; i < numberOfVertices; i++) {
        // set color to the circle on screen
        j = i * 3;
        colour[j] = circle.colour[0];
        colour[j + 1] = circle.colour[1];
        colour[j + 2] = circle.colour[2];

        circleVerticesX[i] = circle.x + (circle.radius * cos(i * doublePi / circle.numberOfSides));
        circleVerticesY[i] = circle.y + (circle.radius * sin(i * doublePi / circle.numberOfSides));
        circleVerticesZ[i] = circle.z;
    }

    GLfloat allCircleVertices[numberOfVertices * 3];

    for (i = 0; i < numberOfVertices; i++) {
        allCircleVertices[i * 3] = circleVerticesX[i];
        allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
        allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    // enable color filling
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
    // fill color
    glColorPointer(3, GL_FLOAT, 0, colour);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

