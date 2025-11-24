#include <Game.hpp>

int main() {
	constexpr int width = 1280;
	constexpr int height = 720;
	const std::string title = std::string("Dissasembly Line");

	Game* g = new Game(width, height, title);
	g->Init();
	g->Run();
	g->Shutdown();

	delete g;
}
