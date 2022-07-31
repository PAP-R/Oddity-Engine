#include "graphics/graphics.cpp"
#include "character/movement.h"
#include "character/character.cpp"
#include "input/input.h"

Graphics::Engine graphics;
Character character;

void fullscreen() {
	graphics.fullScreen();
}

int main(int* argc, char ** argv) {

	graphics.start(400, 400, "Hallo");

	Input::setup(&fullscreen, &character);

	graphics.setKeyCallback(Input::keyCallback);
	graphics.setCursorPositionCallback(Input::cursorCallback);
	graphics.setMouseButtonCallback(Input::mouseButtonCallback);

	character.setPos(2.0f, 2.0f, 2.0f);

	bool windowOpen = true;

	do {
		character.update();
		windowOpen = graphics.update(character.getMovement());
	} while (windowOpen);

	graphics.stop();
}