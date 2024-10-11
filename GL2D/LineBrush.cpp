#include "LineBrush.h"
#include "TransformUtil.h"
#include "MathUtil.h"
#include "ImageUtil.h"
#include "ShaderUtil.h"
#include "CameraUtil.h"
#include "SystemResources.h"

void LineBrush::SetRenderType(RenderType Opt) {
	Type = Opt;
}

void LineBrush::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void LineBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (GLfloat)R;
	Color.g = (1.0f / 255.0f) * (GLfloat)G;
	Color.b = (1.0f / 255.0f) * (GLfloat)B;
}

void LineBrush::Draw(GLfloat X1, GLfloat Y1, GLfloat X2, GLfloat Y2, GLfloat Width, GLfloat Transparency) {
	TranslateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);

	Length = Math::CalcDistance(X1, Y1, X2, Y2);
	Rotation = Math::CalcRadians(X1, Y1, X2, Y2);

	Transform::RotateRad(TranslateMatrix, Rotation);
	Transform::Move(TranslateMatrix, Length / 2.0, 0.0);
	Transform::Scale(ScaleMatrix, Length + Width, Width);

	camera.SetCamera(Type);

	glUseProgram(ImageShader);
	camera.PrepareRender(ShaderType::Image);

	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, Transparency);

	ObjectColorLocation = glGetUniformLocation(ImageShader, "objectColor");
	glUniform3f(ObjectColorLocation, Color.r, Color.g, Color.b);

	ModelLocation = glGetUniformLocation(ImageShader, "model");

	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix * ScaleMatrix));

	imageUtil.Render(LineTex);
}

void LineBrush::DrawStraightX(GLfloat X1, GLfloat X2, GLfloat Width, GLfloat Transparency) {
	TranslateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);
	
	Length = fabs(X1 - X2);

	Transform::Move(TranslateMatrix, Length / 2.0, 0.0);
	Transform::Scale(ScaleMatrix, Length + Width, Width);

	camera.SetCamera(Type);

	glUseProgram(ImageShader);
	camera.PrepareRender(ShaderType::Image);

	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, Transparency);

	ObjectColorLocation = glGetUniformLocation(ImageShader, "objectColor");
	glUniform3f(ObjectColorLocation, Color.r, Color.g, Color.b);

	ModelLocation = glGetUniformLocation(ImageShader, "model");

	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix * ScaleMatrix));

	imageUtil.Render(LineTex);
}

void LineBrush::DrawStraightY(GLfloat Y1, GLfloat Y2, GLfloat Width, GLfloat Transparency) {
	TranslateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);

	Length = fabs(Y1 - Y2);

	Transform::Move(TranslateMatrix, 0.0, Length / 2.0);
	Transform::Scale(ScaleMatrix, Width, Length + Width);

	camera.SetCamera(Type);

	glUseProgram(ImageShader);
	camera.PrepareRender(ShaderType::Image);

	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, Transparency);

	ObjectColorLocation = glGetUniformLocation(ImageShader, "objectColor");
	glUniform3f(ObjectColorLocation, Color.r, Color.g, Color.b);

	ModelLocation = glGetUniformLocation(ImageShader, "model");

	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix * ScaleMatrix));

	imageUtil.Render(LineTex);
}