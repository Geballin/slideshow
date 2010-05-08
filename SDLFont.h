/*
    SDLFont
    Copyright (C) 2003  Jeremy Stanley

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License 
    as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library; if not, write to the Free Software Foundation,
    59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef SDLFONT_H
#define SDLFONT_H

// SDLFont is a simple raster-based text writer for SDL

// Use LoadFont() to load a font from a BMP file.
//  Font bitmaps must be 8-bit uncompressed BMP files,
//  consisting of characters 32 through 127 inclusive,
//  arranged in one horizontal row.  Characters must be monospace.
//  Character width will be computed as bitmap width / 96,
//  and character height is equal to bitmap height.
// Use InitFont() to initialize a font from pixel data in memory.
//  Same format as the BMP used by LoadFont().

// The value of the top-left pixel in the font bitmap will be
//  considered transparent.  Anything else is opaque.

#ifdef MACOSX
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

class SDLFont
{
    void InitCommon();
    SDL_Surface *pDisplay, *pFont;
    int cw, ch;
    Uint8 bgcolor;
    int Bpp, dpitch, cpitch, cwidth, dinc, cinc;
    Uint32 pixel;

public:
    inline int getcw() { return cw; }
    inline int getch() { return ch; }
    SDLFont(SDL_Surface *pSDL);
    void Clear();
    bool InitFont(void *pixels, int width, int height);
    bool LoadFont(const char *bitmap);
    void SetColor(int r, int g, int b);
    void WriteText(int x, int y, const char *text);
    void CenterText(int xc, int y, const char *text);
    virtual ~SDLFont();
};

#endif // SDLFONT_H
