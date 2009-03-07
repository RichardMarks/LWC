
// CODESTYLE: v2.0

// BitmapFont.h
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: a direct SDL port of my ged101 BitmapFont class

/**
 * @file BitmapFont.h
 * @brief a direct SDL port of my ged101 BitmapFont class - Header
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __BITMAPFONT_H__
#define __BITMAPFONT_H__

struct SDL_Surface;

namespace LOFI
{
	/// the maximum length for a string to be printed is 1024 characters
	const unsigned int BITFNT_MAX_STRING_LENGTH = 0x400;
	
	/**
	 * @class BitmapFont
	 * @brief a direct SDL port of my ged101 BitmapFont class
	 */
	class BitmapFont
	{
	public:
		/**
		 * default constructor
		 * Initializes the font structure with the default font from an x86 BIOS for mode 0x13\n
		 * An 8x8 font containing all 256 characters 128 ASCII and 128 extended ASCII characters
		 */
		BitmapFont();
		
		/**
		 * default destructor - calls BitmapFont::Destroy() to de-allocate memory
		 */
		~BitmapFont();
		
		/**
		 * Initializes the font structure by loading the font image from a file.
		 * @param filePath is the path to the file that holds the font image data to be loaded
		 * @param letterWidth is the width of a single letter in pixels
		 * @param letterHeight is the height of a single letter in pixels
		 * @param spacing is the spacing between the letters in pixels
		 * @return true on success, false on failure
		 */
		bool Load(const char* filePath, int letterWidth = 8, int letterHeight = 8, int spacing = 2);
		
		/**
		 * Initializes the font structure using a pre-loaded SDL_Surface.
		 * @param source is a pointer to the SDL_Surface structure that holds the font image data.
		 * @param letterWidth is the width of a single letter in pixels
		 * @param letterHeight is the height of a single letter in pixels
		 * @param spacing is the spacing between the letters in pixels
		 */
		void Create(SDL_Surface* source, int letterWidth = 8, int letterHeight = 8, int spacing = 2);
		
		/**
		 * Prints @a text on the @a destination SDL_Surface at the position @a x, @a y.
		 * @param destination is the SDL_Surface to print the text on to.
		 * @param x is the X coordinate of the upper-left corner of the first letter of the string to be printed in pixels.
		 * @param y is the Y coordinate of the upper-left corner of the first letter of the string to be printed in pixels.
		 * @param text is a printf-style C-string holding the text to be printed.
		 */
		void Print(SDL_Surface* destination, int x, int y, const char* text, ...);
		
		/**
		 * @return the width of a single letter in pixels
		 */
		int GetLetterWidth() const;
		
		/**
		 * @return the height of a single letter in pixels
		 */
		int GetLetterHeight() const;
		
		/**
		 * @return the amount of pixels of spacing between letters
		 */
		int GetLetterSpacing() const;
		
		/**
		 * @return a pointer to the SDL_Surface structure that holds the font image data
		 */
		SDL_Surface* GetFontImage() const;
		
	private:
		/**
		 * De-allocates any allocated memory
		 */
		void Destroy();
		
		/**
		 * @var fontImage_
		 * @brief the SDL_Surface structure that holds the font image data
		 */
		SDL_Surface* fontImage_;
		
		/**
		 * @var letterWidth_
		 * @brief the width of a single letter in pixels
		 */
		int letterWidth_;
		
		/**
		 * @var letterHeight_
		 * @brief the height of a single letter in pixels
		 */
		int letterHeight_;
		
		/**
		 * @var spacing_
		 * @brief the amount of pixels of spacing between letters
		 */
		int spacing_;
	}; // end class

} // end namespace
#endif


