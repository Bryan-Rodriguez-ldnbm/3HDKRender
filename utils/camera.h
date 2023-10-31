#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum CameraDirection  {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW			= -90.0f;
const float PITCH		= 0.0f;
const float SPEED		= 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM		= 45.0f;

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 worldUp;

	float Yaw;
	float Pitch;

	float speed;
	float mouseSensitivity;
	float zoom;

	Camera
	(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH
	) :
		Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		speed(SPEED),
		mouseSensitivity(SENSITIVITY),
		zoom(ZOOM)
	{
		Position = position;
		worldUp = up;
		Yaw = yaw;
		Pitch = pitch;

		updateCameraVectors();
	}

	Camera
	(
		float x,
		float y,
		float z,
		float upX,
		float upY,
		float upZ,
		float yaw,
		float pitch
	) :
		Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		speed(SPEED),
		mouseSensitivity(SENSITIVITY),
		zoom(ZOOM)
	{
		Position = glm::vec3(x, y, z);
		worldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;

		updateCameraVectors();
	}

	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void processKeyboard(CameraDirection direction, float dTime)
	{
		float velocity = speed * dTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}

	void processMouseMovement(float xOffset, float yOffset, GLboolean constraintPitch = true)
	{
		xOffset *= mouseSensitivity;
		yOffset *= mouseSensitivity;

		Yaw	  += xOffset;
		Pitch += yOffset;

		if (constraintPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		updateCameraVectors();
	}

	void processMouseScroll(float yOffset)
	{
		zoom -= (float)yOffset;
		if (zoom < 1.0f)
			zoom = 1.0f;
		if (zoom > 45.0f)
			zoom = 45.0f;
	}

private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, worldUp));
		Up    = glm::normalize(glm::cross(Right, Front));
	}
};

#endif