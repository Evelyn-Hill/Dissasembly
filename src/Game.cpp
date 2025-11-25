#include "raylib.h"
#include <Game.hpp>
#include <AssetManager.hpp>

Game::Game(int width, int height, std::string title) {
	m_WindowTitle = title;
	SetTraceLogLevel(LOG_ERROR);
	InitWindow(width, height, title.c_str());
}

void Game::Init() {
	// NOTE: I think we should keep VSync mandatory for music sync? -plm
	SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

	// Init asset manager.
	AssetManager::GetInstance();
}

void Game::Run() {
	Texture2D tex = AssetManager::GetInstance()->GetTexture("bgtest");

	while (!WindowShouldClose()) {
		std::string titleFpsAppended =
			std::string(m_WindowTitle + " | FPS: " + std::to_string(GetFPS()));
		SetWindowTitle(titleFpsAppended.c_str());

		BeginDrawing();
		ClearBackground(GRAY);
		DrawTexture(tex, 0, 0, WHITE);
		EndDrawing();
	}
}

void Game::Shutdown() {

}

Game::~Game() {
	CloseWindow();
}
