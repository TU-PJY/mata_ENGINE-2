#include "GameObject.h"
#include "ImageUtil.h"
#include "CameraUtil.h"
#include <cmath>

// init functions
void GameObject::InitMatrix(int RenderType) {
	TranslateMatrix = glm::mat4(1.0f);
	RotateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);
	ImageAspectMatrix = glm::mat4(1.0f);
	FlipMatrix = glm::mat4(1.0f);

	TransparencyValue = 1.0f;
	BlurValue = 0.0;

	camera.SetCamera(RenderType);
}

void GameObject::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	ObjectColor.r = R;
	ObjectColor.g = G;
	ObjectColor.b = B;
}

void GameObject::SetColorRGB(int R, int G, int B) {
	ObjectColor.r = (1.0f / 255.0f) * (GLfloat)R;
	ObjectColor.g = (1.0f / 255.0f) * (GLfloat)G;
	ObjectColor.b = (1.0f / 255.0f) * (GLfloat)B;
}

GLfloat GameObject::ASP(GLfloat Value) {
	return Value * ASPECT;
}

void GameObject::UpdateViewportPosition(GLfloat& ValueX, GLfloat& ValueY, bool ApplyAspect) {
	if (ApplyAspect)
		ValueX = ASP(ViewportPosition().x);
	else
		ValueX = ViewportPosition().x;
	ValueY = ViewportPosition().y;
}

void GameObject::UpdateLocalPosition(GLfloat& ValueX, GLfloat& ValueY, bool ApplyAspect) {
	if(ApplyAspect)
		ValueX = ASP(LocalPosition().x);
	else
		ValueX = LocalPosition().x;
	ValueY = LocalPosition().y;
}

void GameObject::Move(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
	Matrix = translate(Matrix, glm::vec3(X, Y, 0.0));
}

void GameObject::Rotate(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(0.0, 0.0, 1.0));
}

void GameObject::RotateRad(glm::mat4& Matrix, GLfloat Radians) {
	Matrix = rotate(Matrix, Radians, glm::vec3(0.0, 0.0, 1.0));
}

void GameObject::RotateV(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(1.0, 0.0, 0.0));
}

void GameObject::RotateH(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(0.0, 1.0, 0.0));
}

void GameObject::Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
	Matrix = scale(Matrix, glm::vec3(X, Y, 1.0));
}

void GameObject::Flip(int FlipOpt) {
	switch (FlipOpt) {
	case FLIP_H:
		FlipMatrix = rotate(FlipMatrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		break;

	case FLIP_V:
		FlipMatrix = rotate(FlipMatrix, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
		break;

	case FLIP_HV:
		FlipMatrix = rotate(FlipMatrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		FlipMatrix = rotate(FlipMatrix, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
		break;
	}
}

void GameObject::Blur(int Strength) {
	BlurValue = (GLfloat)Strength;
}

void GameObject::Render(Image& Image, GLfloat Transparency, bool DisableAdjustAspect) {
	TransparencyValue = Transparency;

	if (!DisableAdjustAspect) {
		if (Image.Width > Image.Height)
			ImageAspectMatrix = glm::scale(ImageAspectMatrix, glm::vec3(1.0, (GLfloat)Image.Height / (GLfloat)Image.Width, 1.0));

		else if (Image.Width < Image.Height)
			ImageAspectMatrix = glm::scale(ImageAspectMatrix, glm::vec3((GLfloat)Image.Width / (GLfloat)Image.Height, 1.0, 1.0));
	}

	PrepareRender();
	imageUtil.Render(Image);
}

void GameObject::PlaySound(Sound Sound, SoundChannel& ChannelVar, unsigned int StartTime) {
	soundUtil.PlaySound(Sound, ChannelVar, StartTime);
}

void GameObject::PauseSound(SoundChannel& ChannelVar, bool Flag) {
	soundUtil.PauseSound(ChannelVar, Flag);
}

void GameObject::StopSound(SoundChannel& ChannelVar) {
	soundUtil.StopSound(ChannelVar);
}

void GameObject::SetPlaySpeed(SoundChannel& ChannelVar, float PlaySpeed) {
	soundUtil.SetPlaySpeed(ChannelVar, PlaySpeed);
}

void GameObject::ResetPlaySpeed(SoundChannel& ChannelVar) {
	soundUtil.ResetPlaySpeed(ChannelVar);
}

void GameObject::EnableFreqCutoff(SoundChannel& ChannelVar, float Frequency) {
	soundUtil.SetFreqCutOff(ChannelVar, Frequency);
}

void GameObject::EnableBeatDetect(SoundChannel& ChannelVar) {
	soundUtil.SetBeatDetect(ChannelVar);
}

void GameObject::DetectBeat(GLfloat& Value, float ThresHold, float SamplingRate) {
	Value =  soundUtil.DetectBeat(ThresHold, SamplingRate);
}

bool GameObject::IsBeat(float ThresHold, float SamplingRate) {
	return soundUtil.IsBeat(ThresHold, SamplingRate);
}

void GameObject::DisableFreqCutoff(SoundChannel& ChannelVar) {
	soundUtil.UnSetFreqCutOff(ChannelVar);
}

void GameObject::DisableBeatDetect(SoundChannel& ChannelVar) {
	soundUtil.UnSetBeatDetect(ChannelVar);
}

void GameObject::SetSoundDistance(SoundChannel& ChannelVar, float MinDist, float MaxDist) {
	soundUtil.SetDistance(ChannelVar, MinDist, MaxDist);
}

void GameObject::SetListnerPosition(float X, float Y) {
	soundUtil.SetListnerPosition(X, Y);
}

void GameObject::SetSoundPosition(SoundChannel& ChannelVar, float X, float Y, float Diff) {
	soundUtil.SetSoundPosition(ChannelVar, X, Y, Diff);
}

////////////////////////// private
void GameObject::PrepareRender() {
	glUseProgram(ImageShader);
	camera.PrepareRender(SHADER_TYPE_IMAGE);

	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, TransparencyValue);

	ObjectColorLocation = glGetUniformLocation(ImageShader, "objectColor");
	glUniform3f(ObjectColorLocation, ObjectColor.r, ObjectColor.g, ObjectColor.b);

	RadiusLocation = glGetUniformLocation(ImageShader, "Radius");
	glUniform1f(RadiusLocation, BlurValue);

	BoolBlurLocation = glGetUniformLocation(ImageShader, "UseBlur");
	if (BlurValue != 0.0)
		glUniform1i(BoolBlurLocation, 1);
	else
		glUniform1i(BoolBlurLocation, 0);

	TexelSizeLocation = glGetUniformLocation(ImageShader, "TexelSize");
	glUniform2f(TexelSizeLocation, ASP(1.0) / (GLfloat)WIDTH, 1.0 / (GLfloat)HEIGHT);

	ModelLocation = glGetUniformLocation(ImageShader, "model");

	if (ImageAspectMatrix != glm::mat4(1.0f))
		ResultMatrix = TranslateMatrix * RotateMatrix * ScaleMatrix * ImageAspectMatrix;
	else
		ResultMatrix = TranslateMatrix * RotateMatrix * ScaleMatrix;

	if (FlipMatrix != glm::mat4(1.0f))
		ResultMatrix *= FlipMatrix;

	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(ResultMatrix));
}

glm::vec4 GameObject::ViewportPosition() {
	return camera.Projection * camera.ViewMatrix * ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 GameObject::LocalPosition() {
	return ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}