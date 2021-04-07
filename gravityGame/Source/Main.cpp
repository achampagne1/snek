//includes
#define STB_IMAGE_IMPLEMENTATION //needed for image loading 
#define _USE_MATH_DEFINES
#include "convertToFloat.h"
#include <iostream>
#include <vector> 
#include "delay.h"
#include "map.h"


//function prototypes
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void initWindow();
void destroy();
void update();
void render();
void onStartUp();

//variables
std::shared_ptr<MovementEngine> movementEnginePtr;

int screenWidth = 800;
int screenHeight = 460;
static double limitFPS = 1.0 / 30.0;
double lastTime = glfwGetTime(), timer = lastTime;
double deltaTime = 0, nowTime = 0;
int frames = 0, updates = 0;
bool run = false;
bool step = false;
bool stepMode = false;

//object declerations
GLFWwindow* window;
std::shared_ptr<Delay> delay;
std::shared_ptr<Map> map{ new Map("abc",screenWidth,screenHeight) };

int main(void)
{
    initWindow();
    onStartUp();
    movementEnginePtr = map->getMovementPtr(); //gets movement pointer from the player which is the first object in the models vector
    //adjustDownwardOnStart();
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        nowTime = glfwGetTime();
        deltaTime += (nowTime - lastTime) / limitFPS;
        lastTime = nowTime;
        while (deltaTime >= 1.0) {
            update();
            updates++;
            deltaTime--;
        }
        render();
        frames++;

        if (glfwGetTime() - timer > 1.0) {
            timer++;
            updates = 0, frames = 0;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroy();
}

void onStartUp() {
    srand(time(0)); 
    map->createMap();
}

void render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    map->renderMap();
}

void update() {
    if (run||step) {
        map->updateMap();
        step = false;
    };
}

/*void adjustDownwardOnStart() { //for only adjusting downward on stratup
    std::vector<std::shared_ptr<Model>> references;
    references.push_back(models.at(3)); // expand for other planets
    models.at(1)->calculateGravity(references);
    adjustDownward();
}

void adjustDownward() { //adjusting downward for everything else
    glm::vec2 direction = models.at(1)->getGravityDirection();
    float angleDown = atan2(-1,0); 
    float angle = atan2(direction.y, direction.x); //gets the angle that everything needs to be rotated by
    float angleDifference = angle - angleDown;
    if (angleDifference < 0.0000001)
        return;
    std::shared_ptr<VertexData> player = models.at(1)->getVertexDataPointer(); //player vertex data
    for (int i = 2; i < models.size(); i++) { //loops through all models
        std::shared_ptr<VertexData> temp =models.at(i)->getVertexDataPointer(); //gets model vertex data
        float xDiff = temp->getAvgX() - player->getAvgX();  //gets difference of x
        float yDiff = temp->getAvgY() - player->getAvgY();  //gets difference of y
        float angle2 = atan2(yDiff, xDiff); //you need to extract the angle for a correct calculation
        float magnitude = sqrt(pow(yDiff, 2) + pow(xDiff, 2));  //magnitude is needed for calculating the new rotated position
        angle2 -= angleDifference;
        float newPos[2] = { (magnitude * cos(angle2))+screenWidth/2-temp->getAvgXModel(),(magnitude * sin(angle2))+screenHeight/2-temp->getAvgYModel()};
        models.at(i)->moveWithPosition(newPos);
    }
}*/

void initWindow() {
    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(800, 450, "Gravity Game", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress); //important
    glfwSwapInterval(1);

    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        if (run)
            run = false;
        else
            run = true;
    }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
        if (stepMode)
            stepMode = false;
        else
            stepMode = true;
    }
    if (key == GLFW_KEY_X && action == GLFW_PRESS) {
        step = true;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS){
   
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS){
        movementEnginePtr->setDirection(1);
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS){
        movementEnginePtr->setDirection(3);
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        movementEnginePtr->jump();
    }
    if (action == GLFW_RELEASE) {
        movementEnginePtr->setDirection(4);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void destroy() {
    map->destroyMap();
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

