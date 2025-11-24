#include "raylib.h"
#include <Game.hpp>

Game::Game() {
	InitWindow(1280, 720, "My Game");
}

void Game::Run() {
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(GRAY);
		EndDrawing();
	}
}

Game::~Game() {
	CloseWindow();
}
