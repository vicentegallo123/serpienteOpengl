#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#define ARRIBA 0
#define ABAJO 1
#define IZDA 2
#define DCHA 3

class Cuerpo {
public:
    float x, y;
};

class Snake {
public:
    Cuerpo serpiente[20 * 20], comida, frente;
    int longitud, milisegundos, direccion;
    bool colision;
    GLuint texture[1]; // Array para la textura

    void Iniciar() {
        longitud = 3;
        milisegundos = 100;
        direccion = ARRIBA;
        colision = 0;

        serpiente[0].x = serpiente[0].y = serpiente[1].y = serpiente[2].y = 0.0f;
        serpiente[1].x = -1.0f;
        serpiente[2].x = -2.0f;

        do {
            comida.x = (float)(rand() % (40)) - 20.0f;
            comida.y = (float)(rand() % (40)) - 20.0f;
        } while (comida.y == 0.0f || comida.x == -2.0f || comida.x == -1.0f || comida.x == 0.0f);
    }

    void Pintar() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[0]); // Utilizar la textura

        glBegin(GL_QUADS);
        for (int i = 0; i < longitud; i++) {
            glTexCoord2f(0.0f, 0.0f); glVertex2f(serpiente[i].x, serpiente[i].y);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(serpiente[i].x + 1.0f, serpiente[i].y);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(serpiente[i].x + 1.0f, serpiente[i].y + 1.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(serpiente[i].x, serpiente[i].y + 1.0f);
        }
        glEnd();

        glDisable(GL_TEXTURE_2D);

        // Comida
        glColor3f(1.0f, 0.0f, 0.0f);
        glRectf(comida.x, comida.y, comida.x + 1.0f, comida.y + 1.0f);
        glColor3f(0.5f, 0.0f, 0.0f);
        glRectf(comida.x + 0.2f, comida.y + 0.2f, comida.x + 0.8f, comida.y + 0.8f);

        // Objetos adicionales
        glColor3f(0.0f, 1.0f, 0.0f);
        glRectf(5.0f, 5.0f, 6.0f, 6.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glRectf(-10.0f, -10.0f, -9.0f, -9.0f);
    }

    void colocarNuevaComida() {
        int esp_libre, libre, m;
        libre = 0;
        esp_libre = 0;
        while (!esp_libre) {
            libre = 0;
            esp_libre = 0;
            comida.x = (float)(rand() % (40)) - 20.0f;
            comida.y = (float)(rand() % (40)) - 20.0f;

            for (m = 0; m < longitud; m++) {
                if (serpiente[m].x != comida.x && serpiente[m].y != comida.y) libre++;
            }
            if (libre == longitud) esp_libre = 1;
        }
    }

    bool Avanzar() {
        switch (direccion) {
            case ARRIBA: frente.y = frente.y + 1.0f; break;
            case ABAJO: frente.y = frente.y - 1.0f; break;
            case IZDA: frente.x = frente.x - 1.0f; break;
            case DCHA: frente.x = frente.x + 1.0f; break;
        }

        for (int p = 1; p < longitud; p++) if (serpiente[0].x == serpiente[p].x && serpiente[0].y == serpiente[p].y) colision = 1;
        if (frente.y > 19.0f || frente.y < -20.0f || frente.x > 19.0f || frente.x < -20.0f) colision = 1;
        else {
            if (frente.x == comida.x && frente.y == comida.y) {
                longitud++;
                colocarNuevaComida();
            }
            for (int p = longitud - 1; p >= 0; p--) {
                serpiente[p + 1].x = serpiente[p].x;
                serpiente[p + 1].y = serpiente[p].y;
            }
            serpiente[0].x = frente.x;
            serpiente[0].y = frente.y;
        }
        if (!colision) return 1;
        else {
            std::cout << "Game Over! Press 'r' to restart." << std::endl;
            return 0;
        }
    }
} snake;

void LoadTexture() {
    // Temporary storage for one texture
    unsigned char textureData[64][64][4];

    // Create a simple checkerboard texture
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            int c = (((i&0x8)==0)^((j&0x8)==0))*255;
            textureData[i][j][0] = (GLubyte) c;
            textureData[i][j][1] = (GLubyte) c;
            textureData[i][j][2] = (GLubyte) c;
            textureData[i][j][3] = (GLubyte) 255;
        }
    }

    glGenTextures(1, snake.texture); // Generate the texture
    glBindTexture(GL_TEXTURE_2D, snake.texture[0]); // Bind the texture

    // Specify the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

    // Set the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

float zoom = 1.0f;
float translateX = 0.0f;
float translateY = 0.0f;

void PintarEscena() {
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(translateX, translateY, 0.0f);
    glScalef(zoom, zoom, 1.0f);

    snake.Pintar();

    glutSwapBuffers();
}

void ReProyectar(int w, int h) {
    GLfloat formato;

    if (h == 0) h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    formato = (GLfloat)w / (GLfloat)h;
    if (w <= h) glOrtho(-20.0f, 20.0f, -20.0f / formato, 20.0f / formato, 1.0f, -1.0f);
    else glOrtho(-20.0f * formato, 20.0f * formato, -20.0f, 20.0f, 1.0f, -1.0f);
}

void ControlFlechas(int key, int x, int y) {
    if (key == GLUT_KEY_UP) if (snake.direccion != ABAJO) snake.direccion = ARRIBA;
    if (key == GLUT_KEY_DOWN) if (snake.direccion != ARRIBA) snake.direccion = ABAJO;
    if (key == GLUT_KEY_LEFT) if (snake.direccion != DCHA) snake.direccion = IZDA;
    if (key == GLUT_KEY_RIGHT) if (snake.direccion != IZDA) snake.direccion = DCHA;
}

void Mover(int value) {
    if (snake.Avanzar()) glutTimerFunc(snake.milisegundos, Mover, 1);
}

void MouseControl(int button, int state, int x, int y) {
    if (button == 3 && state == GLUT_DOWN) zoom += 0.1f; // Scroll up
    if (button == 4 && state == GLUT_DOWN) zoom -= 0.1f; // Scroll down
}

void MouseMotion(int x, int y) {
    static int lastX = -1;
    static int lastY = -1;

    if (lastX != -1 && lastY != -1) {
        translateX += (x - lastX) / 100.0f;
        translateY -= (y - lastY) / 100.0f;
    }

    lastX = x;
    lastY = y;
}

void ReiniciarJuego() {
    snake.Iniciar();
    snake.colision = 0;
    glutTimerFunc(snake.milisegundos, Mover, 1);
}

void ControlTeclado(unsigned char key, int x, int y) {
    if (key == 'r') {
        ReiniciarJuego();
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("proyecto");

    srand(time(0));

    snake.Iniciar();
    LoadTexture();

    glutReshapeFunc(ReProyectar);
    glutDisplayFunc(PintarEscena);
    glutTimerFunc(snake.milisegundos, Mover, 1);
    glutSpecialFunc(ControlFlechas);
    glutIdleFunc(PintarEscena);
    glutMouseFunc(MouseControl);
    glutMotionFunc(MouseMotion);
    glutKeyboardFunc(ControlTeclado);

    glutMainLoop();
    return 0;
}
