#include "EngineHeader.h"
#include "Framework.h"
#include "CameraUtil.h"
#include "ImageUtil.h"
#include "DataUtil.h"
#include "FontLoaderUtil.h"
#include "RenderModeUtil.h"
#include "ShaderUtil.h"
#include "SoundUtil.h"
#include "MouseUtil.h"
#include <iostream>
#include <windows.h>

// modes
#include "Mode1.h"


int WIDTH = WindowWidth;
int HEIGHT = WindowHeight;
int PREV_WIDTH, PREV_HEIGHT;

GLfloat ASPECT;
Rect rect;
glm::vec3 BackColor;

ShaderUtil shaderUtil;
Camera camera;
RenderModeUtil renderMode;
ImageUtil imageUtil;
SoundUtil soundUtil;
DataUtil dataUtil;
MouseUtil mouse;
FontLoaderUtil fontloaderUtil;
Framework fw;


void SetupSystem(int argc, char** argv) {
	HWND hWnd = GetConsoleWindow();

	if (!ShowConsole)
		ShowWindow(hWnd, SW_HIDE);
	else
		ShowWindow(hWnd, SW_SHOWNORMAL);

	glutInit(&argc, argv);
	SetupWindow();
	LoadShader();
	SetGlOption();
}


void SetupWindow() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GL_MULTISAMPLE);

	glutInitWindowPosition(GetSystemMetrics(SM_CXSCREEN) / 2 - WIDTH / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - HEIGHT / 2);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow(WindowName);

	if (StartWithFullScreen) {
		glutFullScreen();
		WIDTH = GetSystemMetrics(SM_CXSCREEN);
		HEIGHT = GetSystemMetrics(SM_CYSCREEN);
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Unable to initialize GLEW\n\n";
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n\n";
}

void LoadShader() {
	shaderUtil.LoadVertexShader("MATA_ENGINE_res//GLSL//Vertex.glsl");
	shaderUtil.LoadFragmentShader("MATA_ENGINE_res//GLSL//Fragment_Image.glsl");
	shaderUtil.CreateShader(ImageShader);

	shaderUtil.LoadFragmentShader("MATA_ENGINE_res//GLSL//Fragment_Text.glsl");
	shaderUtil.CreateShader(TextShader);
}

void SetGlOption() {
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void InitSystem() {
	SetBackColor(0.3, 0.3, 0.3);

	imageUtil.Init();
	soundUtil.Init();
	camera.Init();

	fw.Init(Mode1::GameMode1);
}