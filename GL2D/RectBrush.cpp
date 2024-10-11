#include "RectBrush.h"
#include "ImageUtil.h"
#include "CameraUtil.h"
#include "TransformUtil.h"
#include "SystemResources.h"
#include <cmath>

void LineRectBrush::SetRenderType(RenderType Opt) {
	Type = Opt;
}

void LineRectBrush::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void LineRectBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (GLfloat)R;
	Color.g = (1.0f / 255.0f) * (GLfloat)G;
	Color.b = (1.0f / 255.0f) * (GLfloat)B;
}

void LineRectBrush::Draw(GLfloat X, GLfloat Y, GLfloat SizeX, GLfloat SizeY, GLfloat Width, GLfloat RotationValue, GLfloat Transparency) {
	TransparencyValue = Transparency;
	DrawLine(X, Y, 0.0, SizeY / 2.0, SizeX + Width, Width, RotationValue);
	DrawLine(X, Y, 0.0, -SizeY / 2.0, SizeX + Width, Width, RotationValue);
	DrawLine(X, Y, -SizeX / 2.0, 0.0, Width, SizeY + Width, RotationValue);
	DrawLine(X, Y, SizeX / 2.0, 0.0, Width, SizeY + Width, RotationValue);
}

void LineRectBrush::DrawLine(GLfloat X, GLfloat Y, GLfloat OffsetX, GLfloat OffsetY, GLfloat Width, GLfloat Height, GLfloat RotationValue) {
	TranslateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);

	Transform::Move(TranslateMatrix, X, Y);
	Transform::Rotate(TranslateMatrix, RotationValue);
	Transform::Move(TranslateMatrix, OffsetX, OffsetY);
	Transform::Scale(ScaleMatrix, Width, Height);
	Render();
}

void LineRectBrush::Render() {
	camera.SetCamera(Type);

	glUseProgram(ImageShader);
	camera.PrepareRender(ShaderType::Image);

	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, TransparencyValue);

	ObjectColorLocation = glGetUniformLocation(ImageShader, "objectColor");
	glUniform3f(ObjectColorLocation, Color.r, Color.g, Color.b);

	ModelLocation = glGetUniformLocation(ImageShader, "model");

	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix * ScaleMatrix));

	imageUtil.Render(LineTex);
}



void RectBrush::SetRenderType(RenderType Opt) {
	Type = Opt;
}

void RectBrush::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void RectBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (GLfloat)R;
	Color.g = (1.0f / 255.0f) * (GLfloat)G;
	Color.b = (1.0f / 255.0f) * (GLfloat)B;
}

void RectBrush::Draw(GLfloat X, GLfloat Y, GLfloat SizeX, GLfloat SizeY, GLfloat RotationValue, GLfloat Transparency) {
	TranslateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);
	TransparencyValue = Transparency;

	Transform::Move(TranslateMatrix, X, Y);
	Transform::Rotate(TranslateMatrix, RotationValue);
	Transform::Scale(ScaleMatrix, SizeX, SizeY);

	Render();
}

void RectBrush::Render() {
	camera.SetCamera(Type);

	glUseProgram(ImageShader);
	camera.PrepareRender(ShaderType::Image);

	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, TransparencyValue);

	ObjectColorLocation = glGetUniformLocation(ImageShader, "objectColor");
	glUniform3f(ObjectColorLocation, Color.r, Color.g, Color.b);

	ModelLocation = glGetUniformLocation(ImageShader, "model");

	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix * ScaleMatrix));

	imageUtil.Render(LineTex);
}
