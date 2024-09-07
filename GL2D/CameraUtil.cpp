#include "CameraUtil.h"

void Camera::CalculateASPECT() {
	ASPECT = 1.0 * WIDTH / HEIGHT;
	rect.lx = -1.0 * ASPECT;
	rect.ly = -1.0;
	rect.rx = 1.0 * ASPECT;
	rect.ry = 1.0;

	PREV_WIDTH = WIDTH;
	PREV_HEIGHT = HEIGHT;
}

void Camera::Init() {
	CalculateASPECT();
	SetCamera();
}

void Camera::SetCamera(bool Flag) {
	using namespace glm;

	if(PREV_WIDTH != WIDTH || PREV_HEIGHT != HEIGHT)
		CalculateASPECT();

	CamPos = vec3(0.0f, 0.0f, 1.0f);
	CamDirection = vec3(0.0f, 0.0f, 0.0f);
	CamUp = vec3(0.0f, 1.0f, 0.0f);

	if (Flag)
		StaticMode = true;

	else {
		ViewMatrix = mat4(1.0f);
		ViewMatrix = lookAt(CamPos, CamDirection, CamUp);
		StaticMode = false;
	}
}

void Camera::ProcessTransform(bool UseTextShader) {
	if (StaticMode) {
		ViewMatrix = glm::mat4(1.0f);
		Projection = glm::mat4(1.0f);
		ViewMatrix = lookAt(CamPos, CamDirection, CamUp);
		Projection = glm::ortho((ASPECT * -1.0f), (ASPECT * 1.0f), -1.0f, 1.0f, -100.0f, 100.0f);
	}
	else {
		Projection = glm::mat4(1.0f);
		Projection = glm::ortho((ASPECT * -1.0f) / ZoomValue, (ASPECT * 1.0f) / ZoomValue, -1.0f / ZoomValue, 1.0f / ZoomValue, -100.0f, 100.0f);
	}

	if (UseTextShader) {
		ProjectionLocation = glGetUniformLocation(TextShader, "projection");
		glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, &Projection[0][0]);

		ViewLocation = glGetUniformLocation(TextShader, "view");
		glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, &ViewMatrix[0][0]);

		ViewPosLocation = glGetUniformLocation(TextShader, "viewPos");
		glUniform3f(ViewPosLocation, CamPos.x, CamPos.y, CamPos.z);
	}

	else {
		ProjectionLocation = glGetUniformLocation(ImageShader, "projection");
		glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, &Projection[0][0]);

		ViewLocation = glGetUniformLocation(ImageShader, "view");
		glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, &ViewMatrix[0][0]);

		ViewPosLocation = glGetUniformLocation(ImageShader, "viewPos");
		glUniform3f(ViewPosLocation, CamPos.x, CamPos.y, CamPos.z);
	}
}

void Camera::Move(GLfloat X, GLfloat Y) {
	ViewMatrix = translate(ViewMatrix, glm::vec3(X, Y, 0.0));
}

void Camera::Rotation(GLfloat Rotation) {
	ViewMatrix = rotate(ViewMatrix, glm::radians(Rotation), glm::vec3(0.0, 0.0, 1.0));
}

void Camera::Zoom(ZoomOpt Type, GLfloat Value) {
	switch (Type) {
	case ZoomOpt::In:
		ZoomValue = ZoomValue / (1.0f - Value);
		break;

	case ZoomOpt::Out:
		ZoomValue = ZoomValue * (1.0f - Value);
		break;
	}
}

void Camera::ChangeZoom(GLfloat Value) {
	ZoomValue = Value;
}

GLfloat DivZoom(GLfloat Value) {
	return Value / camera.ZoomValue;
}