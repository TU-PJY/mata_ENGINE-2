#include "EngineHeader.h"
#include "Framework.h"
#include "CameraUtil.h"
#include "SoundUtil.h"
#include <thread>
#include <chrono>

// frametime values
float PrevTime, CurrentTime, DeltaTime;

GLvoid GLMain() {
	glClearColor(BackColor.r, BackColor.g, BackColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	framework.Routine();
	soundUtil.Update();

	CurrentTime = float(glutGet(GLUT_ELAPSED_TIME));
	
#if FRAME_LIMITS > 0
	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast <int>(TARGET_FPS)));
#endif

	DeltaTime = (CurrentTime - PrevTime) / 1000.0;
	framework.InputFrameTime(DeltaTime);

	PrevTime = CurrentTime;

	glutSwapBuffers();
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	SetupSystem(argc, argv);
	glutDisplayFunc(GLMain);
	glutReshapeFunc(DisplayReshape);
	glutMainLoop();
}