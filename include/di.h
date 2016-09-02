#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SIDE_NUM    36      // number of 'sides' (or triangles) used to draw the circles  
#define DISTANCE    5       // max length of a movement 
#define TOTAL_TIME  5       // 5 minutes per trial

struct Circle{
    GLfloat x;              // center points
    GLfloat y;
    GLfloat z;
    GLfloat radius;         // circle radius
    GLint numberOfSides;    // number of side
    GLfloat xVector;        // horizontal vecotr
    GLfloat yVector;        // vertical vector
    GLfloat colour[3];      // color of the circle
};

typedef struct Circle Circle;

void drawCircle( Circle circle );

static void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods);

Circle initCircle(Circle circle);
/** 
 * @param circle
 * @return an initialized circle 
 */

void initCircleArray(Circle circle[], int length );
/* 
 * @param circle array
 * @param length        length of the array
 */

int testCollision(Circle c1, Circle c2);
/**
 * @param c1            circle 
 * @param c2            circle
 * @return 0            if they don't collide with each other 
 *         1            if they collide
 */

void moves(Circle circle[], int length, int index);
/** 
 * @param circle array
 * @param length        length of array
 * @param index         index of current considered circle
 */

static void error(int error, const char *desc);
