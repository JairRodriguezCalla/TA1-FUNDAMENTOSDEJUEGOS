#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include "Sprite.h"
#include "GLS_Program.h"
#include <vector>

enum class GameState {
    PLAY,
    EXIT
};

class MainGame {
private:
    SDL_Window* window;
    GLS_Program program;
    std::vector<Sprite> sprites;  // Vector de Sprites para almacenar m�ltiples rect�ngulos
    float lastRectTime = 0.0f;  // Tiempo desde la �ltima generaci�n de un nuevo rect�ngulo
    GameState gameState = GameState::PLAY;
    int width = 800;
    int height = 600;
    float time = 0.0f;

    void init();
    void processInput();
    void initShaders();
    void draw();
    void update();

public:
    MainGame();
    ~MainGame();
    void run();
};