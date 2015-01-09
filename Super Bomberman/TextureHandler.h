#pragma once

//#include "include/GL/glew.h"
#include <vector>
#include <string>
#include "SOIL.h"
#include <freeglut.h>
#include <map>

using namespace std;

class TextureHandler
{
	static map<string, GLuint> loadedTextures;
	static map<GLuint, int> textureReferences;
	static map<GLuint, string> reverseTextureLookup;
	static map<string, int> textureWidth;
	static map<string, int> textureHeight;
public:
	static GLuint loadTexture(string texPath, float& width, float& height);
	static void refTexture(GLuint tex);
	static void unrefTexture(GLuint tex);
};

