#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480 //window resolution
#define PI 3.1415926
#define PI2 PI/2
#define PI3 (3*PI)/2

GLfloat px = SCREEN_WIDTH / 2;
GLfloat py = SCREEN_HEIGHT / 2;
GLfloat pdx, pdy, pa;

void keyCallback(GLFWwindow* window, int key, int scancode,
    int action, int mods);

/// <summary>
/// Draw Map
/// </summary>
int mapX = 8, mapY = 8, mapS = 64;
GLuint maps[] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
};

void drawMap2D()
{
    int x, y, xo, yo;
    for (y = 0; y < mapY; y++)
    {
        for (x = 0; x < mapX; x++)
        {
            if(maps[y*mapX+x] == 1)
            {
                glColor3f(1, 1, 1);
            }
            else
                glColor3f(0, 0, 0);

            xo = x * mapS; yo = y * mapS;
            glBegin(GL_QUADS);
            glVertex2i(xo +1, yo +1);
            glVertex2i(xo +1, yo+mapS -1);
            glVertex2i(xo+mapS -1, yo+mapS-1);
            glVertex2i(xo+mapS -1, yo +1);
            glEnd();
        }
    }
}


void drawRays3D()
{
    int r, mx, my, mp, dof;
    GLfloat rx, ry, ra, xo, yo;
    ra = pa;

    for (r = 0; r < 1; r++)
    {
        //------Horizontal Line----
        /*dof = 0;
        float aTan = -1 / tan(ra);
        if (ra > PI) { 
            ry = (((int)py >> 6) << 6) - 0.0001; 
            rx = (py - ry) * aTan + px;
            yo = -64;
            xo = -yo * aTan;
        }
        if (ra < PI) {
            ry = (((int)py >> 6) << 6) + 64;
            rx = (py - ry) * aTan + px;
            yo = 64;
            xo = -yo * aTan;
        }
        if (ra == 0 || ra == PI) { rx = px;ry = py;dof = 8; }
        while (dof < 8)
        {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapX + mx;
            if (mp >0 && mp < mapX * mapY && maps[mp] == 1)
            {
                dof = 8;
            }
            else {
                rx += xo;ry += yo;dof += 1;
            }
        }
        glColor3f(0, 1, 0);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2i(px, py);
        glVertex2i(rx, ry);
        glEnd();*/

        //-------Check for vertical lines -------
        dof = 0;
        float nTan = -tan(ra);
        if (ra > PI2 && ra< PI3) {
            rx = (((int)px >> 6) << 6) - 0.0001;
            ry = (px - rx) * nTan + py;
            xo = -64;
            yo = -xo * nTan;
        }
        if (ra < PI2 || ra>PI3) {
            rx = (((int)px >> 6) << 6) + 64;
            ry = (px - rx) * nTan + py;
            xo = 64;
            yo = -xo * nTan;
        }
        if (ra == 0 || ra == PI) { rx = px;ry = py;dof = 8; }
        while (dof < 8)
        {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapX + mx;
            if (mp >0 && mp < mapX * mapY && maps[mp] == 1)
            {
                dof = 8;
            }
            else {
                rx += xo;ry += yo;dof += 1;
            }
        }
        glColor3f(0, 1, 0);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2i(px, py);
        glVertex2i(rx, ry);
        glEnd();
    }
}

//init runs before rendering only once
void init()
{
    pdx = cos(pa) * 5;
    pdy = sin(pa) * 5;
}

void drawPlayer()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glPointSize(10);
    GLfloat pointVertex[] = { px, py };
    glVertexPointer(2, GL_FLOAT, 0, pointVertex);
    glDrawArrays(GL_POINTS, 0, 1);
    glDisableClientState(GL_VERTEX_ARRAY);

    //Draw Line
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2i(px, py);
    glVertex2i(px + pdx * 5, py + pdy * 5);
    glEnd();
}


int main(void)
{
    init();

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, keyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        drawMap2D();

        drawPlayer();
        drawRays3D();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode,
    int action, int mods)
{
    /*if (key == GLFW_KEY_A && px > 2)
        px -= 2;
    else if (key == GLFW_KEY_D && px < SCREEN_WIDTH - 2)
        px += 2;
    else if (key == GLFW_KEY_S && py > 2)
        py -= 2;
    else if (key == GLFW_KEY_W && py < SCREEN_HEIGHT - 2)
        py += 2;*/

    if (key == GLFW_KEY_A)
    {
        pa -= 0.1;
        if (pa < 0)
            pa += 2 * PI;
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    else if (key == GLFW_KEY_D)
    {
        pa += 0.1;
        if (pa > 2*PI)
            pa -= 2 * PI;
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    else if (key == GLFW_KEY_S)
    {
        px -= pdx;
        py -= pdy;
    }
    else if (key == GLFW_KEY_W)
    {
        px += pdx;
        py += pdy;
    }
}