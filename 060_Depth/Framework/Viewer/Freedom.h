#pragma once
#include "Camera.h"

class Freedom : public Camera
{
public:
	Freedom(float moveSpeed = 20.0f, float rotationSpeed = 2.5f);
	~Freedom();

	void Speed(float moveSpeed = 20.0f, float rotationSpeed = 2.5f)
	{
		this->moveSpeed = moveSpeed;
		this->rotationSpeed = rotationSpeed;
	}

	void Update();

private:
	float moveSpeed;
	float rotationSpeed;
};