#pragma once
#include "Model.h"
#include "aabb.h"

class SceneObject
{
public:
	Model* model;

	SceneObject(Model* model) : mModel(model)
	{
		this->model = model;
		transformMat = glm::mat4(1.f);
		normalLevel = 1;
		mShader = nullptr;
		calculateBoundingBox();
	}

	SceneObject(Model* model, glm::mat4 transformationMatrix, Shader* shader) : mModel(model), transformMat(transformationMatrix), mShader(shader)
	{
		this->model = model;
		normalLevel = 1;
		calculateBoundingBox();
	}

	SceneObject(Model* model, glm::mat4 transformationMatrix, float normLevel, Shader* shader) : mModel(model), transformMat(transformationMatrix), normalLevel(normLevel), mShader(shader)
	{
		this->model = model;
		calculateBoundingBox();
	}

	void setTransform(glm::mat4 transformationMatrix)
	{
		transformMat = transformationMatrix;
	}

	void setNormalLevel(float normLevel)
	{
		normalLevel = normLevel;
	}

	vector<glm::vec3> getVertices()
	{
		return mModel->mvertices;
	}

	vector<glm::vec3> getVerticesInWorld()
	{
		vector<glm::vec3> vertices = getVertices();
		for (int i = 0; i < vertices.size(); ++i)
		{
			glm::vec4 tempVec = glm::vec4(vertices[i], 1);
			vertices[i] = transformMat * tempVec;
		}
		return vertices;
	}

	void calculateBoundingBox()
	{
		vector<glm::vec3> ObjectVertices = getVerticesInWorld();

		glm::vec3 tempMax;
		glm::vec3 tempMin = tempMax = ObjectVertices[0];
		for (int i = 1; i < ObjectVertices.size(); i++)
		{
			if (ObjectVertices[i].x < tempMin.x)
			{
				tempMin.x = ObjectVertices[i].x;
			}
			if (ObjectVertices[i].y < tempMin.y)
			{
				tempMin.y = ObjectVertices[i].y;
			}
			if (ObjectVertices[i].z < tempMin.z)
			{
				tempMin.z = ObjectVertices[i].z;
			}

			if (tempMax.x < ObjectVertices[i].x)
			{
				tempMax.x = ObjectVertices[i].x;
			}
			if (tempMax.y < ObjectVertices[i].y)
			{
				tempMax.y = ObjectVertices[i].y;
			}
			if (tempMax.z < ObjectVertices[i].z)
			{
				tempMax.z = ObjectVertices[i].z;
			}
		}
		boundingBoxWS.mMinPoint = tempMin;
		boundingBoxWS.mMaxPoint = tempMax;

	}

	void draw()
	{
		mShader->setFloat("normalLevel", normalLevel);
		mShader->setMat4("model", transformMat);
		mModel->Draw(*mShader);
	}
	void draw(Shader* usingShader)
	{
		usingShader->setFloat("normalLevel", normalLevel);
		usingShader->setMat4("model", transformMat);
		mModel->Draw(*usingShader);
	}

	aabb getAABB()
	{
		return boundingBoxWS;
	}

	Shader* getShader()
	{
		return mShader;
	}

	Model* getModel()
	{
		return mModel;
	}

	glm::mat4 getModelMatrix()
	{
		return transformMat;
	}

	float getNormalLevel()
	{
		return normalLevel;
	}
private:
	Model* mModel;
	glm::mat4 transformMat;
	float normalLevel;
	Shader* mShader;
	aabb boundingBoxWS;
};