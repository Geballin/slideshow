// Copyright (C) 2003  Jeremy Stanley
// This is free software licensed under the GNU GPL; see COPYING for details

#if !defined(AFX_SLIDESHOW_H__A644DF2D_934C_44DE_B80C_E4DB28603405__INCLUDED_)
#define AFX_SLIDESHOW_H__A644DF2D_934C_44DE_B80C_E4DB28603405__INCLUDED_

#if _MSC_VER > 1000
#pragma warning ( disable : 4786 )
#pragma once
#endif // _MSC_VER > 1000

#ifdef MACOSX
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include "cache.h"

#define MN_SLIDESHOW_VERSION	0.8

class file_list;
class SDLFont;

class slideshow  
{
public:
	enum transition_t {
		tran_none,
		tran_random,
		tran_BEGIN, 
		tran_fade,
		tran_blinds,
		tran_checkers,
		tran_uncover,
		tran_END
	};

	struct options
	{
		enum {
			DEFAULT_WIDTH = 0,
			DEFAULT_HEIGHT = 0
		};

		int    width;
		int    height;
		bool   windowed;
		bool   repeat;
		bool   display_name;
		bool   display_number;
		bool   show_ready_indicator;
		double advance_time;
		transition_t transition;

		options() :
			width(DEFAULT_WIDTH), 
			height(DEFAULT_HEIGHT), 
			windowed(false),
			repeat(false),
			display_name(false), 
			display_number(false),
			show_ready_indicator(false),
			advance_time(0.0),
			transition(tran_none)
		{
		}
	};

	slideshow(options *opt, file_list *fl);
	virtual ~slideshow();

	bool run();

private:
	options   *m_options;
	file_list *m_file_list;

	// === Graphics utility ====================================

	SDL_Surface *m_sdl;
	SDLFont   *m_font;
	SDL_Surface *m_imageSurface;

	bool init_graphics();

	void init_true_color_format();
	SDL_PixelFormat m_true_color_format;
	bool is_true_color(SDL_Surface *in);
	SDL_Surface *convert_to_true_color(SDL_Surface *in);
	
	SDL_Surface *frame_image(SDL_Surface *in, int w, int h);
	
	SDL_Surface *scale_image(SDL_Surface *in, int w, int h);
	void bilinearpix(SDL_Surface *in, double u, double v, Uint8 *c);
	
	void reset_zoom(void);

	void draw_centered_dialog_box(SDLFont *font, const std::string &text, int x_arg, int y_arg);
	void draw_shadow_text(SDLFont &font, const std::string &text, int x, int y, int r, int g, int b);
	void center_shadow_text(SDLFont &font, const std::string &text, int x, int y, int r, int g, int b);

	void confirm_delete();
	void draw_picture_name();
	void draw_picture_number();

	void do_transition(SDL_Surface *from, SDL_Surface *to);

	// === Slideshow mechanics =================================

	SDL_TimerID m_timer_id;
	int         m_image_index;
	int         m_prev_image_index;
	Uint32      m_frame_color;
	int         m_zoom_percent;
	bool        m_zoom_mode;
	SDL_Surface *image_in_zoom;

	enum user_command {
		c_next_slide,
		c_prev_slide,
		c_first_slide,
		c_last_slide,
		c_toggle_timer,
		c_redraw,
		c_timer_advance,
		c_zoom,
		c_quit
	};

	user_command wait_for_command();

	void start_timer();
	void stop_timer();

	void show_image(SDL_Surface *image);

	// === Image cache =========================================

public:
	SDL_Surface *prep_image(int index);

private:
	SDL_Surface *create_placeholder_image( const std::string &filename, const char *error_str );

	typedef cache<int, SDL_Surface *> image_cache;
	class image_cache_callback : public image_cache::data_callback 
	{
	public:	
		image_cache_callback(slideshow &sw) : m_slideshow(sw) {	}
		SDL_Surface *load(int ix)
		{
			return m_slideshow.prep_image(ix);
		}
		void discard(SDL_Surface *data)
		{
			SDL_FreeSurface(data);
		}
	private:
		slideshow &m_slideshow;
	};

	image_cache_callback *m_cache_callback;
	image_cache          *m_image_cache;
};

#endif 
