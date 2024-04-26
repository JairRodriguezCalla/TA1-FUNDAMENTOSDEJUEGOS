#include "MainGame.h"
#include <ctime>
#include "Error.h"

MainGame::MainGame() : window(nullptr), time(0.0f), lastRectTime(0.0f) {
    srand(static_cast<unsigned>(std::time(nullptr)));;  // Inicializa la semilla aleatoria
}

MainGame::~MainGame() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void MainGame::init() {
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("nombre_ventana_xd", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

    if (window == nullptr) {
        fatalError("Error al crear la ventana.");
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        fatalError("GLEW no se inicializó correctamente.");
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    initShaders();

    // Inicializa el primer Sprite
    Sprite initialSprite;
    initialSprite.init(-1, -1, 1, 1);
    sprites.push_back(initialSprite);
}

void MainGame::initShaders() {
    program.compileShaders("Shaders/colorShaderVert.txt", "Shaders/colorShaderFrag.txt");
    program.addAtribute("vertexPosition");
    program.addAtribute("vertexColor");
    program.linkShader();
}

void MainGame::draw() {
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.use();

    GLuint timeLocation = program.getUniformLocation("time");
    glUniform1f(timeLocation, time);

    for (const auto& sprite : sprites) {
        sprite.draw();
    }

    program.unuse();
    SDL_GL_SwapWindow(window);
}

void MainGame::update() {
    while (gameState != GameState::EXIT) {
        // Actualiza el tiempo
        time += 0.02;

        // Verifica si es momento de generar un nuevo rectángulo
        if (time - lastRectTime >= 5.0f) {
            // Genera una posición aleatoria dentro de los límites de la ventana
            float randomX = static_cast<float>(rand()) / RAND_MAX * (width - 40);
            float randomY = static_cast<float>(rand()) / RAND_MAX * (height - 40);

            // Crea un nuevo Sprite y agrégalo al vector de Sprites
            Sprite newSprite;
            newSprite.init(randomX, randomY, 40, 40);
            sprites.push_back(newSprite);

            // Actualiza el tiempo de la última generación de un nuevo rectángulo
            lastRectTime = time;
        }

        draw();
        processInput();
    }
}

void MainGame::processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            gameState = GameState::EXIT;
            break;
        case SDL_MOUSEMOTION:
            std::cout << "Posición en X: " << event.motion.x << " | Posición en Y: " << event.motion.y << std::endl;
            break;
        }
    }
}

void MainGame::run() {
    init();
    update();
}
