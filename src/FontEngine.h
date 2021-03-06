/*
 * class FontEngine
 * Handles rendering a bitmap font
 *
 * @author Clint Bellanger
 * @license GPL
 *
 */

#ifndef FONT_ENGINE_H
#define FONT_ENGINE_H


#include <fstream>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Settings.h"
#include "Utils.h"
#include "UtilsParsing.h"

using namespace std;

const int JUSTIFY_LEFT = 0;
const int JUSTIFY_RIGHT = 1;
const int JUSTIFY_CENTER = 2;

const int FONT_WHITE = 0;
const int FONT_RED = 1;
const int FONT_GREEN = 2;
const int FONT_BLUE = 3;
const int FONT_GRAY = 4;
const int FONT_GREY = 4;
const int FONT_BLACK = 5;

class FontEngine {
private:
	SDL_Color colors[6];
	int font_pt;
	int font_height;
	int line_height;
	SDL_Rect src;
	SDL_Rect dest;
	SDL_Surface *ttf;
	TTF_Font *font;

public:
	FontEngine();
	~FontEngine();

	int getLineHeight() { return line_height; }
	int getFontHeight() { return font_height; }
	
	int calc_length(string text);
	Point calc_size(string text_with_newlines, int width);

	void render(string text, int x, int y, int justify, SDL_Surface *target, int color);
	void render(string text, int x, int y, int justify, SDL_Surface *target, int width, int color);
	
	int cursor_y;
};

#endif
