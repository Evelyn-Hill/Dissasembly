#include <Game.hpp>

int main() {
	Game* g = new Game();
	g->Run();
	delete g;
}
