#include "graphics/graphics.cpp"
#include "character/movement.h"
#include "character/character.cpp"
#include "input/input.h"

Graphics::Engine graphics;
Character character;

void fullscreen() {
	graphics.fullScreen();
}

std::vector<Vertex> tempVertices(3);
uint32_t tvertex = 0;
std::vector<uint32_t> tempIndices(3);
uint32_t tindex = 0;
uint32_t index = 0;

void addVertex() {
	tempVertices = {
			{{0.1f * -character.movement.dirN.x + character.movement.pos.x + character.movement.dir.x, -0.1f + character.movement.dir.y, 0.1f * -character.movement.dirN.z + character.movement.pos.z + character.movement.dir.z}, {1.0f, 0.0f, 0.0f}},
			{{0.1f * character.movement.dirN.x + character.movement.pos.x + character.movement.dir.x, -0.1f + character.movement.dir.y, 0.1f * character.movement.dirN.z + character.movement.pos.z + character.movement.dir.z}, {0.0f, 1.0f, 0.0f}},
			{{character.movement.pos.x + character.movement.dir.x, 0.1f + character.movement.dir.y, character.movement.pos.z + character.movement.dir.z}, {0.0f, 0.0f, 1.0f}},
	};

	tempIndices = {
		3 + index * 3, 4 + index * 3, 5 + index * 3
	};

	index++;

	graphics.vulkan.add(tempVertices, tempIndices);

	/*
	Vertex vertex;
	vertex.pos = character.movement.pos + character.movement.dir;
	vertex.color = { 1.0f, 1.0f, 1.0f };
	tempVertices[index] = vertex;
	tempIndices[index] = tindex++;

	std::cout << glm::to_string(vertex.pos) << std::endl;

	if (index++ >= 2) {
		graphics.vulkan.addVertex(tempVertices);
		graphics.vulkan.addIndex(tempIndices);
		index = 0;
		printf("Added Triangle\n");
	}
	*/
}

int main(int argc, char ** argv) {

	graphics.start(800, 800, "Hallo");

	Input::setup(&fullscreen, &character, &addVertex);

	graphics.setKeyCallback(Input::keyCallback);
	graphics.setCursorPositionCallback(Input::cursorCallback);
	graphics.setMouseButtonCallback(Input::mouseButtonCallback);
	graphics.setScrollCallback(Input::scrollCallback);

	character.setPos(2.0f, 2.0f, 2.0f);
	character.setVelocity(2.0f, 2.0f, 2.0f);

	bool windowOpen = true;

	do {
		character.update();
		windowOpen = graphics.update(character.getMovement());
	} while (windowOpen);

	graphics.stop();
}