#include <List>
#include <string>
#include <vector>

struct FloatRect {
	FloatRect() = default;
	FloatRect(float _left, float _top, float _width, float _height) {
		left = _left;
		top = _top;
		width = _width;
		height = _height;
	}

	float left = 0;
	float top = 0;
	float width = 0;
	float height = 0;
};

struct GameObject {
	GameObject(std::string _name, FloatRect _boundingBox) {
		name = _name;
		boundingBox = _boundingBox;
	}
	std::string name;
	FloatRect boundingBox;
};

struct Node {
	Node(Node* _previousNode, Node* childNodeA, Node* childNodeB, std::vector<GameObject>& gameObjects)
	{
		previousNode = _previousNode;
		childA = childNodeA;
		childB = childNodeB;
		ptrGameObjects = &gameObjects;
	}
	Node* previousNode = nullptr;
	Node* childA = nullptr;
	Node* childB = nullptr;

	std::vector<GameObject>* ptrGameObjects;
	FloatRect boundingBox;
};

std::vector<GameObject> gameObjects;
std::vector<Node> bvh;

int main() {
	// Creation of example obbjects
	gameObjects.emplace_back("circle", FloatRect( 0, 0, 64, 64 ));
	gameObjects.emplace_back("chair", FloatRect( 64, 0, 64, 64));
	gameObjects.emplace_back("dino", FloatRect( 128, 0, 64, 64));
	gameObjects.emplace_back("square", FloatRect( 192, 0, 64, 64));
	gameObjects.emplace_back("chicken", FloatRect( 0, 64, 64, 64));
	gameObjects.emplace_back("jockey", FloatRect( 64, 64, 64, 64));
	gameObjects.emplace_back("frog", FloatRect( 128, 64, 64, 64));
	gameObjects.emplace_back("shark", FloatRect( 192, 64, 64, 64));

	// Creation of the bvh
	bvh.emplace_back(nullptr, &bvh[1], &bvh[2], gameObjects);		// Master = 0
	bvh.emplace_back(&bvh[0], &bvh[3], &bvh[5], gameObjects);		// 1
	bvh.emplace_back(&bvh[0], &bvh[4], &bvh[6], gameObjects);		// 2
	bvh.emplace_back(&bvh[1], nullptr, nullptr, gameObjects);		// 3
	bvh.emplace_back(&bvh[2], nullptr, nullptr, gameObjects);		// 4
	bvh.emplace_back(&bvh[1], nullptr, nullptr, gameObjects);		// 5
	bvh.emplace_back(&bvh[2], nullptr, nullptr, gameObjects);		// 6

	// Next step: Calculate the float rects for all the nodes in the bvh

	return 0;
}