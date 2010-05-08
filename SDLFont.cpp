// Copyright (C) 2003  Jeremy Stanley
// This is free software licensed under the GNU GPL; see COPYING for details

#include "SDLFont.h"
#include <string.h>

SDLFont::SDLFont(SDL_Surface *pSDL)
{
    pDisplay = pSDL;
    pFont = NULL;
    SetColor(255, 255, 255);

    dpitch = pSDL->pitch;
    Bpp = pSDL->format->BytesPerPixel;
}

SDLFont::~SDLFont()
{
    Clear();
}

bool SDLFont::LoadFont(const char *bitmap)
{
    Clear();

    if(!(pFont=SDL_LoadBMP(bitmap)))
        return false;

    if(pFont->format->BytesPerPixel != 1)
    {
        Clear();
        return false;
    }

	InitCommon();

    return true;
}

bool SDLFont::InitFont(void *pixels, int width, int height)
{
	Clear();

	if ( !(pFont = SDL_CreateRGBSurfaceFrom(pixels, width, height, 8, width,
			0, 0, 0, 0)) )
		return false;

	InitCommon();

	return true;
}

void SDLFont::InitCommon()
{    
	cw = pFont->w / 96;
    ch = pFont->h;
    bgcolor = *(Uint8 *)pFont->pixels;
    cwidth  = Bpp * cw;
    cpitch = pFont->pitch;
    dinc = dpitch - cwidth;
    cinc = cpitch - cw;
}

void SDLFont::Clear()
{
    if(pFont != NULL) {
        SDL_FreeSurface(pFont);
        pFont=NULL;
    }
}

void SDLFont::SetColor(int r, int g, int b)
{
    pixel = SDL_MapRGB(pDisplay->format, r, g, b);
}

void SDLFont::WriteText(int x, int y, const char *text)
{
    if(pFont==NULL)
        return;			// not initialized

    if(y + ch > pDisplay->h)
        return;			// off the bottom of the screen

    Uint8 *pixloc = (Uint8 *)pDisplay->pixels + dpitch * y + Bpp * x;
    Uint8 *pChar, *pScreen;
    char c;
    int i, j;

    for(const char *p = text; *p != '\0'; p++)
    {
        if(x + cw > pDisplay->w)
            return;			// off the edge of the screen

        c = *p - 32;  if(c < 0 || c > 95) c = 0;

        pScreen = pixloc;
        pChar = (Uint8 *)pFont->pixels + c * cw;

        switch(Bpp) {
        case 1:
            for(i = 0; i < ch; i++) {
                for(j = 0; j < cw; j++) {
                    if(*pChar != bgcolor)
                        *pScreen = (Uint8)pixel;
                    pChar++;
                    pScreen += Bpp;
                }
                pChar += cinc;
                pScreen += dinc;
            }
            break;
        case 2:
            for(i = 0; i < ch; i++) {
                for(j = 0; j < cw; j++) {
                    if(*pChar != bgcolor)
                        *(Uint16 *)pScreen = (Uint16)pixel;
                    pChar++;
                    pScreen += Bpp;
                }
                pChar += cinc;
                pScreen += dinc;
            }
            break;
        case 4:
            for(i = 0; i < ch; i++) {
                for(j = 0; j < cw; j++) {
                    if(*pChar != bgcolor)
                        *(Uint32 *)pScreen = pixel;
                    pChar++;
                    pScreen += Bpp;
                }
                pChar += cinc;
                pScreen += dinc;
            }
            break;
        case 3:
            for(i = 0; i < ch; i++) {
                for(j = 0; j < cw; j++) {
                    if(*pChar != bgcolor) {
                        *(pScreen+2) = pixel >> 16;
                        *(pScreen+1) = pixel >> 8;
                        *(pScreen) = pixel;
                    }
                    pScreen += 3;
                    pChar++;
                }
                pChar += cinc;
                pScreen += dinc;
            }
            break;
        }

        x += cw;
        pixloc += cwidth;
    }
}

void SDLFont::CenterText(int x0, int y, const char *text)
{
    int x = x0 - (strlen(text) * cw) / 2;
    if(x < 0) x = 0;
    WriteText(x, y, text);
}


