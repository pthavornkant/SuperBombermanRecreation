#include "TextureHandler.h"
#include <iostream>

map<string, GLuint> TextureHandler::loadedTextures = map<string, GLuint>();
map<GLuint, int> TextureHandler::textureReferences = map<GLuint, int>();
map<GLuint, string> TextureHandler::reverseTextureLookup = map<GLuint, string>();
map<string, int> TextureHandler::textureWidth = map<string, int>();
map<string, int> TextureHandler::textureHeight = map<string, int>();

GLuint TextureHandler::loadTexture(string texPath, float& width, float& height) {
	if(loadedTextures[texPath] == 0) {
		int lWid, lHei, lChan;
		unsigned char* data = SOIL_load_image(texPath.c_str(), &lWid, &lHei, &lChan, SOIL_LOAD_AUTO);
		SOIL_free_image_data(data);
		//GLuint texID = SOIL_create_OGL_texture(data, lWid, lHei, lChan, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO|SOIL_FLAG_MIPMAPS);
		GLuint texID = SOIL_load_OGL_texture(texPath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO|SOIL_FLAG_MIPMAPS);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		loadedTextures[texPath] = texID;
		width = textureWidth[texPath] = lWid;
		height = textureHeight[texPath] = lHei;
		reverseTextureLookup[texID] = texPath;
		return texID;
	} else {
		width = textureWidth[texPath];
		height = textureHeight[texPath];
		return loadedTextures[texPath];
	}
}

void TextureHandler::refTexture(GLuint texID) {
	textureReferences[texID]++;
}

void TextureHandler::unrefTexture(GLuint texID) {
	if(texID > 0) {
		if((--textureReferences[texID]) <= 0) {
			//glDeleteTextures(1, &texID);
			//loadedTextures.erase(reverseTextureLookup[texID]);
			//textureReferences[texID] = 0;
		}
	}
}
