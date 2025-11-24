#include <raylib.h>
#include <string>

class Game {
public:
	Game(int width, int height, std::string title);
	~Game();

	void Init();
	void Run();
	void Shutdown();
private:
	std::string m_WindowTitle;
};
