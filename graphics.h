#ifndef CHIP_8_GRAPHICS_H
#define CHIP_8_GRAPHICS_H

class Graphics
{
public:
    Graphics();
    ~Graphics();

    void initialize();

protected:
    void initializeGraphics();
};

void renderScene();

#endif //CHIP_8_GRAPHICS_H
