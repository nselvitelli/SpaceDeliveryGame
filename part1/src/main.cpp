// Support Code written by Michael D. Shah
// Last Updated: 6/11/21
// Please do not redistribute without asking permission.

// Functionality that we created
#include "render/SDLGraphicsProgram.hpp"

int main(int argc, char** argv){

	// Create an instance of an object for a SDLGraphicsProgram
	SDLGraphicsProgram mySDLGraphicsProgram(1280,720);
	// Run our program forever
	mySDLGraphicsProgram.Loop();
	// When our program ends, it will exit scope, the
	// destructor will then be called and clean up the program.
	return 0;
}
