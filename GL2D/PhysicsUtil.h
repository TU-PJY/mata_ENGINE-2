#pragma once
#include "GLHeader.h"
#include <cmath>

class PhysicsUtil {
private:
	GLfloat GravityAcc{};
	GLfloat MoveAcc{};
	bool FallingState{};

public:
	void Fall(GLfloat& Position, GLfloat Gravity, float FT);
	bool IsOnFloor(GLfloat Position, GLfloat FloorPosition);
	void LandFloor(GLfloat& Position, GLfloat FloorHeight);
	void SetFallingState();
	void AddGravityAcc(GLfloat AccValue);
	void BounceFloor(GLfloat& Position, GLfloat FloorHeight, GLfloat RebounceValue, GLfloat Threshold);
	void BounceWall(int& MoveDirection, GLfloat& Speed, GLfloat RebounceValue);
	void LerpAcc(GLfloat& Position, GLfloat& Speed, int MoveDirection, GLfloat Dest, GLfloat AccValue, float FT);
	void LerpDcc(GLfloat& Position, GLfloat& Speed, GLfloat Friction, float FT);
	void LinearAcc(GLfloat& Position, GLfloat& Speed, int MoveDirection, GLfloat Dest, GLfloat AccValue, float FT);
	void LinearDcc(GLfloat& Position, GLfloat& Speed, GLfloat Friction, float FT);
	void MoveUniform(GLfloat& Position, int& MoveDirection, GLfloat Speed, float FT);
};