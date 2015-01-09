
// Ensure the header file is included only once in multi-file projects
#pragma once

#include "glut_window.h"
#include "World.h"

// The functionality of your application should be implemented inside GameWindow
class GameWindow : public GlutWindow
 { private :
    int _w, _h;

	//Game Vars
	static bool focus;

   public :
    GameWindow ( const char* label, int x, int y, int w, int h );
    void windowToScene ( float& x, float &y );

	//Game Vars
	static Player* player;
	static World* world;

   private : // functions derived from the base class
	virtual void idle();
    virtual void handle ( const Event& e );
    virtual void draw ();
    virtual void resize ( int w, int h );
 };