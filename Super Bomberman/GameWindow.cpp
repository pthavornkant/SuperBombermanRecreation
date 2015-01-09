
# include <iostream>
# include "GameWindow.h"
# include <time.h>
# include "EntityBomb.h"
# include "EntityFire.h"
# include "EntitydBlock.h"

using namespace std;

Player* GameWindow::player = NULL;
World* GameWindow::world = NULL;

GameWindow::GameWindow ( const char* label, int x, int y, int w, int h )
          :GlutWindow ( label, x, y, w, h )
{
	world = new World(15, 13);
	world->addPlayer(new Player(1,1,1));
	world->addPlayer(new PlayerNPC(13,1,2));
	world->addPlayer(new PlayerNPC(13,11,3));
	world->addPlayer(new PlayerNPC(1,11,4));
}

// mouse events are in window coordinates, but your scene is in [0,1]x[0,1],
// so make here the conversion when needed
void GameWindow::windowToScene ( float& x, float &y )
 {
   x = (2.0f*(x/float(_w))) - 1.0f;
   y = 1.0f - (2.0f*(y/float(_h)));
 }

void GameWindow::idle() {
	SYSTEMTIME st;
	GetSystemTime(&st);

	static WORD delay = (1000 / 60);
	static WORD ts = st.wSecond;
	static WORD tms = st.wMilliseconds;

	WORD dTms = st.wMilliseconds - tms + (1000 * (st.wSecond - ts));
	while(dTms < delay) {
		Sleep(delay - dTms);
		GetSystemTime(&st);
		dTms = st.wMilliseconds - tms + (1000 * (st.wSecond - ts));
	}

	ts = st.wSecond;
	tms = st.wMilliseconds;

	//Idle Functions (Update & Redraw Screen)
	world->update();
	redraw();
}

// Called every time there is a window event
void GameWindow::handle ( const Event& e )
 {
	 for(int i = 1; i <= world->MAXPLAYERS; i++) {
		 Player* player = world->getPlayer(i);
		 if(player != NULL) {
			world->getPlayer(i)->handleEvent(e);
		 }
	 }
}

void GameWindow::resize ( int w, int h )
 {
   // Define that OpenGL should use the whole window for rendering
	 //Remove 8 from width and height to adjust for border?
	w -= 8; h -= 8;
	//cout << w << ", " << h << endl;
	glViewport( 0, 0, w, h );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0, w, h, 0);
	//gluOrtho2D(0, 240, 240, 0);
	glOrtho(0, 240, 240, 0, 0, 300);
	//gluPerspective(45.0, (GLfloat)w/h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	_w=w; _h=h;
 }

// here we will redraw the scene according to the current state of the application.
void GameWindow::draw ()
 {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	world->draw();

	glutSwapBuffers();

	// SHOW FPS CODE
	/*
	static int fps = 0;
	SYSTEMTIME st;
	GetSystemTime(&st);
	static long timePrev = st.wSecond;
	if(timePrev == st.wSecond) {
		fps++;
	} else {
		cout << "FPS: " << fps << endl;
		fps = 0;
	}
	timePrev = st.wSecond;
	*/
}

