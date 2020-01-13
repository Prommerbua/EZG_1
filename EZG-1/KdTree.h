#pragma once
#include "Model.h"
#include "aabb.h"
#include "scene.h"

struct KDSplit
{
	vector<glm::vec3> leftSplit;
	vector<glm::vec3> rightSplit;
};

struct node
{
	node(vector<glm::vec3> vertices)
	{
		this->vertices = vertices;
		this->calcNodeAABB();
	}

	int axis;
	float axisPosition;
	node* left;
	node* right;
	bool leaf = false;
	aabb* AABB;

	vector<glm::vec3> vertices;

	void calcNodeAABB()
	{
		if (vertices.empty()) 
		{
			this->AABB = nullptr;
			return;
		}
		aabb* bb = new aabb(vertices[0], vertices[0]);

		for (auto v : vertices) {
			bb->include(v);
		}

		AABB = bb;
	}

	float medianPos(int axis)
	{
		vector<float> vertexOnAxisPosition;

		switch (axis)
		{
		case 0:
			for (glm::vec3 v : vertices)
			{
				vertexOnAxisPosition.emplace_back(v.x);
			}
			break;
		case 1:
			for (glm::vec3 v : vertices)
			{
				vertexOnAxisPosition.emplace_back(v.y);
			}
			break;
		case 2:
			for (glm::vec3 v : vertices)
			{
				vertexOnAxisPosition.emplace_back(v.z);
			}
			break;
		}

		std::nth_element(vertexOnAxisPosition.begin(), vertexOnAxisPosition.begin() + vertexOnAxisPosition.size() / 2, vertexOnAxisPosition.end());
		return vertexOnAxisPosition[vertexOnAxisPosition.size() / 2];
	}

	//get next vertices
	vector<glm::vec3> getObjsInAABB(aabb* containingAABB)
	{
		vector<glm::vec3> tempVert;
		for (auto vertex : vertices)
		{
			if (containingAABB->containsPoint(vertex))
			{
				tempVert.emplace_back(vertex);
			}
		}
		return tempVert;
	}

	KDSplit getKDSplit(float splitPosition)
	{
		KDSplit kdSplit;

		for (glm::vec3 v : vertices) {
			switch (axis) {
			case 0:
				if (v.x < splitPosition) kdSplit.leftSplit.push_back(v);
				else kdSplit.rightSplit.push_back(v);
				break;
			case 1:
				if (v.y < splitPosition) kdSplit.leftSplit.push_back(v);
				else kdSplit.rightSplit.push_back(v);
				break;
			case 2:
				if (v.z < splitPosition) kdSplit.leftSplit.push_back(v);
				else kdSplit.rightSplit.push_back(v);
				break;
			}
		}

		return kdSplit;
	}
};



class kd_tree
{
public:
	kd_tree() {  }
	kd_tree(Scene masterScene)
	{
		vector<glm::vec3> vertices = masterScene.getAllVertices();
		root = new node(vertices);
		//create node
		//root->vertices.insert(root->vertices.end(), vertices.begin(), vertices.end());
		make_tree(root, 0);
	}

	node* root;
private:

	void make_tree(node* current, int depth)
	{
		if (current->vertices.size() < 50 || depth > 10)
		{
			current->left = nullptr;
			current->right = nullptr;
			current->leaf = true;
			return;
		}

		current->axis = current->AABB->getMaxExtentAxis();
		current->axisPosition = current->medianPos(current->axis);


		KDSplit split = current->getKDSplit(current->axisPosition);

		node* left = new node(split.leftSplit);
		node* right = new node(split.rightSplit);


		++depth;
		current->left = left;
		current->right = right;
		current->leaf = false;
		make_tree(left, depth);
		make_tree(right, depth);
		return;
	}
};