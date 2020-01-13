#pragma once
#include "sceneObject.h"

class Scene
{
public:


	Scene() {};

	vector<SceneObject> getSceneObjects()
	{
		return mSceneObjects;
	}

	void addSceneObject(SceneObject sceneObj)
	{
		mSceneObjects.emplace_back(sceneObj);
	}

	void addSceneObject(Model* model, glm::mat4 transMat, float normalLevel, Shader* usingShader)
	{
		auto* temp = new SceneObject(model, transMat, normalLevel, usingShader);
		mSceneObjects.emplace_back(*temp);
	}

	void draw() {
		for (SceneObject so : mSceneObjects)
		{
			so.draw();
		}
	}

	void draw(Shader* usingShader) {
		for (SceneObject so : mSceneObjects)
		{
			so.draw(usingShader);
		}
	}

	vector<aabb> getAllAABB()
	{
		vector<aabb> tempVec;
		for (SceneObject so : mSceneObjects)
		{
			tempVec.emplace_back(so.getAABB());
		}
		return tempVec;
	}

	vector<glm::vec3> getAllVertices()
	{
		vector<glm::vec3> vertices;

		for (SceneObject sO : mSceneObjects)
		{
			vector<glm::vec3> tempVertices = sO.getVerticesInWorld();
			vertices.reserve(vertices.size() + tempVertices.size()); // preallocate memory
			vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
		}
		return vertices;
	}


private:
	vector<SceneObject> mSceneObjects;
};