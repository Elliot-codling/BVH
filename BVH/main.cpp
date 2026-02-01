#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct FloatRect
{
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

struct GameObject
{
	GameObject(std::string _name, FloatRect _boundingBox) {
		name = _name;
		boundingBox = _boundingBox;
	}
	std::string name;
	FloatRect boundingBox;
};

struct Node
{
	Node() = default;

	void CreateLink(Node* _previousNode, Node* _childA, Node* _childB)
	{
		previousNode = _previousNode;
		childA = _childA;
		childB = _childB;
	}

	void DefineGameObjects(std::vector<GameObject*>* gameObjects)
	{
		ptrGameObjects = *gameObjects;
	}

	void DefineBounds(float _left, float _top, float _width, float _height)
	{
		boundingBox.left = _left;
		boundingBox.top = _top;
		boundingBox.width = _width;
		boundingBox.height = _height;
	}

	Node* previousNode = nullptr;
	Node* childA = nullptr;
	Node* childB = nullptr;

	std::vector<GameObject*> ptrGameObjects;
	FloatRect boundingBox;
};

std::vector<GameObject> gameObjects;
std::vector<Node> bvh;
std::vector<Node*> collidedNodes;
FloatRect birdObject = {192, 64, 32, 32};

bool BoxBoxCollision(FloatRect boxA, FloatRect boxB)
{
	if (boxA.left < boxB.left + boxB.width &&
			boxA.left + boxA.width > boxB.left &&
			boxA.top + boxA.height > boxB.top &&
			boxA.top < boxB.top + boxB.height)
	{
		return true;
	}
	return false;

}

void CreateGameObjects()
{
	// Creation of example objects
	gameObjects.emplace_back("circle", FloatRect(0, 0, 64, 64));
	gameObjects.emplace_back("chair", FloatRect(64, 0, 64, 64));

	gameObjects.emplace_back("dino", FloatRect(128, 0, 64, 64));
	gameObjects.emplace_back("square", FloatRect(192, 0, 64, 64));

	gameObjects.emplace_back("chicken", FloatRect(0, 64, 64, 64));
	gameObjects.emplace_back("jockey", FloatRect(64, 64, 64, 64));

	gameObjects.emplace_back("frog", FloatRect(128, 64, 64, 64));
	gameObjects.emplace_back("shark", FloatRect(192, 64, 64, 64));
}

std::vector<GameObject*> CalculateObjectsInNode(FloatRect boundingBox)
{
	std::vector<GameObject*> tempVector;
	// Go through each game object and check if it lies within the specified bounding box
	for (GameObject& object : gameObjects)
	{
		if (!BoxBoxCollision(boundingBox, object.boundingBox))
		{
			// Not within box
			continue;
		}
		tempVector.emplace_back(&object);
	}
	return tempVector;
}

void CreateBVH()
{
	// Creation of the bvh - will be automated
	bvh.emplace_back();		// Master = 0
	bvh.emplace_back();		// 1
	bvh.emplace_back();		// 2
	bvh.emplace_back();		// 3
	bvh.emplace_back();		// 4
	bvh.emplace_back();		// 5
	bvh.emplace_back();		// 6

	// Create the links between nodes
	bvh[0].CreateLink(nullptr, &bvh[1], &bvh[2]);
	bvh[1].CreateLink(&bvh[0], &bvh[3], &bvh[5]);
	bvh[2].CreateLink(&bvh[0], &bvh[4], &bvh[6]);
	bvh[3].CreateLink(&bvh[1], nullptr, nullptr);
	bvh[4].CreateLink(&bvh[2], nullptr, nullptr);
	bvh[5].CreateLink(&bvh[1], nullptr, nullptr);
	bvh[6].CreateLink(&bvh[2], nullptr, nullptr);

	// Calculate the float rects for all the nodes in the bvh - will be automated
	bvh[0].DefineBounds(0, 0, 256, 128);			// Master = 0
	bvh[1].DefineBounds(0, 0, 128, 128);			// 1
	bvh[2].DefineBounds(128, 0, 128, 128);			// 2
	bvh[3].DefineBounds(0, 0, 128, 64);				// 3
	bvh[4].DefineBounds(128, 0, 128, 64);			// 4
	bvh[5].DefineBounds(0, 64, 128, 64);			// 5
	bvh[6].DefineBounds(128, 64, 128, 64);			// 6

	// Calculate what objects are given within the nodes
	for (Node& node : bvh)
	{
		std::vector<GameObject*> tempObjects = CalculateObjectsInNode(node.boundingBox);
		node.DefineGameObjects(&tempObjects);
	}
}

void TraverseBVH(FloatRect boundingBox, Node* currentNode)
{
	// Do not continue if this node is a nullptr
	if (currentNode == nullptr)
	{
		return;
	}

	// Go to child nodes if there are more than 2 objects in this current node
	if (currentNode->ptrGameObjects.size() > 2)
	{
		TraverseBVH(boundingBox, currentNode->childA);
		TraverseBVH(boundingBox, currentNode->childB);
		return;
	}

	if (BoxBoxCollision(boundingBox, currentNode->boundingBox))
	{
		collidedNodes.push_back(currentNode);
	}
}

void CheckCollisionsWithinNodes(FloatRect boundingBox)
{
	std::stringstream out;
	for (Node* node : collidedNodes)
	{
		// Check collisions with object inside of node
		for (GameObject* object : node->ptrGameObjects)
		{
			if (BoxBoxCollision(boundingBox, object->boundingBox))
			{
				std::cout << object->name << "\n";
			}
		}
	}
	std::cout << out.str();
}


// TODO: Will delete later
void CheckCollison(FloatRect collisionBox)
{
	std::stringstream out;
	// Will change, the function will pass in the vector of whats inside that bounding box of the bvh rather than all of the objects
	for (GameObject& object : gameObjects)
	{
		if (BoxBoxCollision(collisionBox, object.boundingBox))
		{
			// Collision has occured
			out << "Collision has occured with object: " << object.name << "\n";
		}
	}

	std::cout << out.str();
}

int main()
{
	CreateGameObjects();
	CreateBVH();

	TraverseBVH(birdObject, &bvh.front());
	CheckCollisionsWithinNodes(birdObject);
	std::cout << std::endl;
	CheckCollison(birdObject);

	return 0;
}