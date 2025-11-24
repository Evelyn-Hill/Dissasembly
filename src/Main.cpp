#include <raylib.h>
#include <raymath.h>
#include <string>

int main() {
	SetTraceLogLevel(LOG_WARNING);
	std::string title = "Dissasembly Line";
	InitWindow(1280, 720, title.c_str());

	// NOTE: We should keep VSync on for audio sync reasons? -Plum
	SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

	while (!WindowShouldClose()) {
		SetWindowTitle(std::string(title + " - FPS: " + std::to_string(GetFPS())).c_str());

		BeginDrawing();
			ClearBackground(GRAY);
		EndDrawing();
	}
	
	CloseWindow();
}
