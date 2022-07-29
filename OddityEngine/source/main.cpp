#include "graphics/graphics.cpp"

Graphics::Engine graphics;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	switch (key) {
	case 300:
		if (action == 1) {
			graphics.fullScreen();
		}
		break;
	}

	printf("%d | %d | %d | %d\n", key, scancode, action, mods);
}

int main(int* argc, char ** argv) {

	graphics.start(400, 400, "Hallo");

	graphics.setKeyCallback(keyCallback);

	bool windowOpen = true;

	do {
		windowOpen = graphics.update();
	} while (windowOpen);

	graphics.stop();
}