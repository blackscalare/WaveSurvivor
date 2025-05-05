#include "QuadTree.h"

#define MAX_OBJECTS 50

QuadTree::QuadTree()
{
	root = nullptr;
}

QuadTree::QuadTree(const std::vector<Hitbox*>& objects)
{
	BuildQuadTree(objects);
}

// TODO: This has to be done very often, create an add function
void QuadTree::BuildQuadTree(const std::vector<Hitbox*>& objects)
{
	root = new Node();
	// root rectangle covers entire map
	root->rect = Rectangle({ 0, 0, WORLD_WIDTH, WORLD_HEIGHT });

	if (objects.size() > MAX_OBJECTS) {
		CreateChildrenNodes(objects, root);
	}
	else {
		// If all objects fit in 
		root->objects.insert(root->objects.end(), objects.begin(), objects.end());
	}
}

void QuadTree::CreateChildrenNodes(const std::vector<Hitbox*>& objects, Node* node)
{
}
