#pragma once
#include "Definitions.h"

class QuadTree
{
public:
	QuadTree();
	QuadTree(const std::vector<Hitbox*>& objects);

	void BuildQuadTree(const std::vector<Hitbox*>& objects);
	void CreateChildrenNodes(const std::vector<Hitbox*>& objects, Node* node);

	Node* GetRoot() { return root; }

private:
	Node* root;
};

