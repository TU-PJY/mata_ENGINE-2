#include "TransformUtil.h"

void Transform::Move(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
	Matrix = translate(Matrix, glm::vec3(X, Y, 0.0));
}

void Transform::Rotate(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(0.0, 0.0, 1.0));
}

void Transform::Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
	Matrix = scale(Matrix, glm::vec3(X, Y, 1.0));
}

void Transform::RotateV(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(1.0, 0.0, 0.0));
}

void Transform::RotateH(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(0.0, 1.0, 0.0));
}

void Transform::Flip(glm::mat4& Matrix, FlipDir FlipOption) {
	if (FlipOption != static_cast<FlipDir>(-1)) {
		switch (FlipOption) {
		case FlipDir::H:
			Matrix = rotate(Matrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
			break;

		case FlipDir::V:
			Matrix = rotate(Matrix, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
			break;
		}
	}
}

void Transform::MatchAspect(glm::mat4& Matrix, GLfloat ImageWidth, GLfloat ImageHeight) {
	if (ImageWidth > 0 && ImageHeight > 0) {
		if (ImageWidth > ImageHeight)
			Matrix = scale(Matrix, glm::vec3(1.0, ImageHeight / ImageWidth, 1.0));
		else if (ImageWidth < ImageHeight)
			Matrix = scale(Matrix, glm::vec3(ImageWidth / ImageHeight, 1.0, 1.0));
	}
}
