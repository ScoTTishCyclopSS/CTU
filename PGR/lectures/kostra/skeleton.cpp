//----------------------------------------------------------------------------------------
/**
 * \file    skeleton.cpp : This file contains the 'main' function. Program execution begins and ends there.
 * \author  Jaroslav Sloup, Petr Felkel
 * \date    2022/03/03
 * \brief   Computer graphics course on FEE and FIT CTU CZ
 *
 * This is the skeleton of an OpenGL application written in FreeGLUT and PGR framework.
 * It serves as a starter boiler-plate code for a semester homework - a simple interactive application.
 * Write your code into the functions prepared. Add your data (models, textures, ...) and logic.
 *
 *
 * Comment your code using the doxygen documenting system style.
 * Create doxygen directory and prepare a configuration file with doxygen -f and edit the details.
 * Start documenting by the command doxygen.
 *
 * Start by renaming of this file to <your_name>.cpp and the project to <your_name>.vcxproj
 * In <your_name>.vcxproj
 *   - rename skeleton in <RootNamespace>skeleton</RootNamespace> to <your_name>
 *   - replace skeleton.cpp in <ClCompile Include="skeleton.cpp" /> to <your_name>.cpp
 */


 // TODO: tabulka klaves a jeji obsluha keyPressed/keyReleased a timer


#include <iostream>
#include "pgr.h"
#include "object.h"

constexpr int WINDOW_WIDTH = 500;
constexpr int WINDOW_HEIGHT = 500;
constexpr char WINDOW_TITLE[] = "PGR: Application Skeleton";

// -----------------------  Window callbacks ---------------------------------

/**
 * \brief Draw the window contents.
 */
void displayCb() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // always clear before draw new frame

	// draw the window contents (scene objects) 
	// draw();

	glutSwapBuffers();
}

/**
 * \brief Window was reshaped.
 * \param newWidth New window width
 * \param newHeight New window height
 */
void reshapeCb(int newWidth, int newHeight) {
	// TODO: Take new window size and update the application state,
	// window and projection.

	// glViewport(...);
};

// -----------------------  Keyboard ---------------------------------

/**
 * \brief Handle the key pressed event.
 * Called whenever a key on the keyboard was pressed. The key is given by the "keyPressed"
 * parameter, which is an ASCII character. It's often a good idea to have the escape key (ASCII value 27)
 * to call glutLeaveMainLoop() to exit the program.
 * \param keyPressed ASCII code of the key
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void keyboardCb(unsigned char keyPressed, int mouseX, int mouseY) {
}

// Called whenever a key on the keyboard was released. The key is given by
// the "keyReleased" parameter, which is in ASCII. 
/**
 * \brief Handle the key released event.
 * \param keyReleased ASCII code of the released key
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void keyboardUpCb(unsigned char keyReleased, int mouseX, int mouseY) {
}

//
/**
 * \brief Handle the non-ASCII key pressed event (such as arrows of F1).
 *  The special keyboard callback is triggered when keyboard function (Fx) or directional
 *  keys are pressed.
 * \param specKeyPressed int value of a predefined glut constant such as GLUT_KEY_UP
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void specialKeyboardCb(int specKeyPressed, int mouseX, int mouseY) {
}

void specialKeyboardUpCb(int specKeyReleased, int mouseX, int mouseY) {
} // key released

// -----------------------  Mouse ---------------------------------
// three events - mouse click, mouse drag, and mouse move with no button pressed

// 
/**
 * \brief React to mouse button press and release (mouse click).
 * When the user presses and releases mouse buttons in the window, each press
 * and each release generates a mouse callback (including release after dragging).
 *
 * \param buttonPressed button code (GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON)
 * \param buttonState GLUT_DOWN when pressed, GLUT_UP when released
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void mouseCb(int buttonPressed, int buttonState, int mouseX, int mouseY) {
}

/**
 * \brief Handle mouse dragging (mouse move with any button pressed).
 *        This event follows the glutMouseFunc(mouseCb) event.
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void mouseMotionCb(int mouseX, int mouseY) {
}

/**
 * \brief Handle mouse movement over the window (with no button pressed).
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void passiveMouseMotionCb(int mouseX, int mouseY) {

	// mouse hovering over window

	// create display event to redraw window contents
	glutPostRedisplay();
}

// -----------------------  Timer ---------------------------------

/**
 * \brief Callback responsible for the scene update.
 */
void timerCb(int)
{
	// update the application state

	// and plan a new event
	glutTimerFunc(33, timerCb, 0);

	// create display event
	glutPostRedisplay();
}



// -----------------------  Application ---------------------------------

/**
 * \brief Initialize application data and OpenGL stuff.
 */
void initApplication() {
	// init OpenGL
	// - all programs (shaders), buffers, textures, ...
	// init your Application
	// - setup the initial application state
}

/**
 * \brief Delete all OpenGL objects and application data.
 */
void finalizeApplication(void) {

	// cleanUpObjects();

	// delete buffers
	// cleanupModels();

	// delete shaders
	// cleanupShaderPrograms();
}

/**
 * \brief Entry point of the application.
 * \param argc number of command line arguments
 * \param argv array with argument strings
 * \return 0 if OK, <> elsewhere
 */
int main(int argc, char** argv) {

	// initialize the GLUT library (windowing system)
	glutInit(&argc, argv);

	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // glut_double -> double buffering

	// for each window
	{
		//   initial window size + title
		glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		glutCreateWindow(WINDOW_TITLE);

		// Callbacks - use only those you need
		glutDisplayFunc(displayCb); // redefine window, call always (exmp: if u change size -> the new part will not be printed)
		glutReshapeFunc(reshapeCb); // define always, or northing will appear
		// glutKeyboardFunc(keyboardCb);
		// glutKeyboardUpFunc(keyboardUpCb);
		// glutSpecialFunc(specialKeyboardCb);     // key pressed
		// glutSpecialUpFunc(specialKeyboardUpCb); // key released
		// glutMouseFunc(mouseCb);
		// glutMotionFunc(mouseMotionCb);
		// glutTimerFunc(33, timerCb, 0);
	}
	// end for each window 

	// initialize PGR framework (GL, DevIl, etc.)
	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("pgr init failed, required OpenGL not supported?");

	// init your stuff - shaders & program, buffers, locations, state of the application
	initApplication();

	// handle window close by user
	glutCloseFunc(finalizeApplication);

	// Infinite loop handling the events
	glutMainLoop();

	// code after glutLeaveMainLoop()
	// cleanup

	return 0;
}
