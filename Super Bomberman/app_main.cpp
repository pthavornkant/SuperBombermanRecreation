
# include <iostream>
# include "GameWindow.h"


//==========================================================================
// Main routine
//==========================================================================
int main ( int argc, char** argv )
 {
   // Init freeglut library:
   glutInit ( &argc,argv );
   glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

   // Now create the window of your application:
   //AppWindow* w = new AppWindow ( "CSE165 Support Code", 300, 300, 640, 480 );
   GameWindow* w = new GameWindow( "Super Bomberman", 100, 100, 256, 256);

   // Finally start the main loop:
   w->run ();
}
