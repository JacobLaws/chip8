#include "chip8.h"
#include "graphics.h"
#include <cstdio>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#define ever ;;
#define WINDOW_WIDTH 64
#define WINDOW_HEIGHT 32

int main(int argc, char **argv)
{
    int displayScale  = 14;
    int displayWidth  =  0;
    int displayHeight =  0;

    Chip8 chip8;

    chip8.initialize();
    if(chip8.loadGame() != 0)
        return -1;

    displayWidth  = WINDOW_WIDTH  * displayScale;
    displayHeight = WINDOW_HEIGHT * displayScale;

    glutInit(&argc, argv);

    glutInitWindowPosition(-1, -1);
    glutInitWindowSize(displayWidth, displayHeight);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutCreateWindow("Chip8 Emulator by Jacob Laws");

    glutDisplayFunc(renderScene);

    glutMainLoop();




    /*
    // Main emulation loop
    for(ever)
    {
        // Emulate one cycle
        chip8.emulateCycle();

        // If the draw flag is set, update the screen
        if(chip8.drawFlag)
            chip8.drawGraphics();

        // Store key press state (press and release)
        chip8.setInput();

    }
    */

    return 0;
}

