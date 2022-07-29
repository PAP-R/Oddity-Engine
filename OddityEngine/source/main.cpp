#include "graphics/graphics.cpp"

int main(int* argc, char ** argv) {
	Graphics::Engine graphics;

	graphics.start(400, 400, "Hallo");

	bool windowOpen = true;

	do {
		windowOpen = graphics.update();
	} while (windowOpen);

	graphics.stop();
}