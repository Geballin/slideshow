/*
    Slideshow
    2010-2018 - Géballin -  Based on Jeremy Stanley sources

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

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "file_list.h"
#include "slideshow.h"

#define MAX_PATH_LEN	1024

using namespace std;

struct arguments
{
	slideshow::options so;
	
	bool   recurse;
	bool   random;
	bool	repeat;
	file_list::sort_t sort;
	bool   sort_descending;
	std::string picture_path;
	bool   use_file_list;

	void print_usage()
	{
		cout << endl << "Slideshow\n 2010-2018 - Géballin" << endl;
		cout << "http://macniaque.free.fr" << endl;
		cout << "Version " << MN_SLIDESHOW_VERSION << "\n" << endl;
		cout << "Based on Perigee Slideshow\n(c) sources by Jeremy Stanley\n" << endl;
		cout << "Usage: slideshow [options] [picture_path]" << endl;
		cout << "       picture_path is either a directory or a text file containing a list\n"
				"       of image files, with one image file per line.  Specify - to read a \n"
				"       list of files on stdin.\n"
				"		If no picture_path are specified, the actual path is used.\n" << endl;
		cout << "Options:\n"
				"-h			: Draw this help screen\n"
				"-width X   : Set screen width to X pixels (default the actual screen resolution)\n"
				"-height Y  : Set screen height to Y pixels (default the actual screen resolution)\n"
				"-window    : Run in a window (dimensions specified by -width and -height)\n"
				"-name      : Display image filename\n"
				"-number    : Display image number indicator\n"
				"-advance X : Automatically advance images every X seconds\n"
				"-repeat    : Repeat slideshow\n"
				"-random    : Randomize picture order\n"
				"-ready     : Show indicator (small green square) when next image is ready\n"
				"-tran XXX  : Use a transition effect between slides.  XXX can be one of\n"
				"             fade, checkers, blinds, uncover, or random\n"
				"\nThe following options apply only when picture_path is a directory:\n"
				"-recurse   : Recurse into subdirectories\n"
				"-sort S    : Sort file list, where S is one of the following:\n"
				"             n+  : Name ascending (a-z)\n"
				"             n-  : Name descending (z-a)\n"
				"             cd+ : Create date ascending (oldest first)\n"
				"             cd- : Create date descending (newest first)\n"
				"             md+ : Mofify date ascending (oldest first)\n"
				"             md- : Mofify date descending (newest first)\n\n"
			"During execution:\n"
				"Arrow key right : next picture\n"
				"Arrow key left  : previous picture\n"
				"Home key        : first picture\n"
				"End key         : last picture\n"
				"t key or space  : start/stop timer\n"
				"d key or delete : delete picture\n"
				"n key           : start/stop displaying picture name\n"
				"c key           : start/stop displaying image number indicator\n"
				"Mouse wheel     : zoom picture\n"
				"Escape key      : quit\n\n";
	}

	arguments(int argc, char **argv) : 
		m_argc(argc),
		m_argv(argv),
		recurse(false),
		random(false),
		repeat(false),
		sort(file_list::sort_none),
		sort_descending(false),
		use_file_list(false)
	{
	}

	bool parse()
	{
		if ( m_argc < 1 )
			return false;

		for(int i = 1; i < m_argc; ++i )
		{
			std::string arg = m_argv[i];
			bool last_arg = (i == m_argc - 1);

			if (0 == arg.compare("-h"))
			{
				return false;
			}
			else if ( (0 == arg.compare("-width")) && !last_arg )
			{
				so.width = atoi(m_argv[++i]);
			}
			else if ( (0 == arg.compare("-height")) && !last_arg )
			{
				so.height = atoi(m_argv[++i]);
			}
			else if ( 0 == arg.compare("-window") )
			{
				so.windowed = true;
			}
			else if ( 0 == arg.compare("-repeat") )
			{
				so.repeat = true;
			}
			else if ( 0 == arg.compare("-name") )
			{
				so.display_name = true;
			}
			else if ( 0 == arg.compare("-number") )
			{
				so.display_number = true;
			}
			else if ( (0 == arg.compare("-advance")) && !last_arg )
			{
				so.advance_time = atof(m_argv[++i]);
			}
			else if ( (0 == arg.compare("-tran")) && !last_arg )
			{
				std::string tran = m_argv[++i];
				if ( 0 == tran.compare("random") )
				{
					so.transition = slideshow::tran_random;
				}
				else if ( 0 == tran.compare("checkers") )
				{
					so.transition = slideshow::tran_checkers;
				}
				else if ( 0 == tran.compare("fade") )
				{
					so.transition = slideshow::tran_fade;
				}
				else if ( 0 == tran.compare("blinds") )
				{
					so.transition = slideshow::tran_blinds;
				}
				else if ( 0 == tran.compare("uncover") )
				{
					so.transition = slideshow::tran_uncover;
				}
				else 
				{
					cerr << "Invalid transition effect: " << tran.c_str() << endl << endl;
					return false;
				}
			}
			else if ( 0 == arg.compare("-ready") )
			{
				so.show_ready_indicator = true;
			}
			else if ( 0 == arg.compare("-recurse") )
			{
				recurse = true;
			}
			else if ( 0 == arg.compare("-random") )
			{
				random = true;
			}
			else if ( (0 == arg.compare("-sort")) && !last_arg )
			{
				std::string sort_spec = m_argv[++i];
				if ( 0 == sort_spec.compare("cd+") )
				{
					sort = file_list::sort_date_create;
				}
				else if ( 0 == sort_spec.compare("cd-") )
				{
					sort = file_list::sort_date_create;
					sort_descending = true;
				}
				else if ( 0 == sort_spec.compare("md+") )
				{
					sort = file_list::sort_date_modify;
				}
				else if ( 0 == sort_spec.compare("md-") )
				{
					sort = file_list::sort_date_modify;
					sort_descending = true;
				}
				else if ( 0 == sort_spec.compare("n+") )
				{
					sort = file_list::sort_name;
				}
				else if ( 0 == sort_spec.compare("n-") )
				{
					sort = file_list::sort_name;
					sort_descending = true;
				}
				else 
				{
					cerr << "Invalid sort order specification: " << sort_spec.c_str() << "\n" << endl;
					return false;
				}
			}
			else
			{
				if ( picture_path.size() > 0 ){
					cerr << "Invalid command line: multiple picture_path\n" << endl;
					return false;
				}
				picture_path = arg;

				if ( 0 == picture_path.compare("-") )
				{
					use_file_list = true;
					continue;
				}

				struct stat statbuf;
				if (0 != stat(picture_path.c_str(), &statbuf))
				{
					cerr << "Picture path does not exist: " << picture_path.c_str() << "\n" << endl;
					return false;
				}

				if ( !(statbuf.st_mode & S_IFDIR) )
				{
					use_file_list = true;
				}
			}
		}
	
		if ( picture_path.size() < 1 )
		{
			/*** Use the current repertory	***/
			char myPath_temp[MAX_PATH_LEN];
			getcwd(myPath_temp, MAX_PATH_LEN);
			picture_path = myPath_temp;
		}
		return true;
	}

private:
	int m_argc;
	char **m_argv;
};

int main(int argc, char **argv)
{
	arguments args(argc, argv);
	if ( !args.parse() )
	{
		args.print_usage();
		return 1;
	}
	cout << endl << "Slideshow\n 2010-2018 - Géballin" << endl;
	cout << "Launch \"slideshow -h\" to see the help screen" << endl;
		

	file_list fl;
	if ( args.use_file_list )
	{
		if ( !fl.load_file_list( args.picture_path.c_str() ) )
		{
			cerr << "Failed to load file list " << args.picture_path.c_str() << endl;
			return 1;
		}
	}
	else
	{
		if ( !fl.load_directory( args.picture_path.c_str(), args.recurse, args.sort, args.sort_descending ) )
		{
			cerr << "Failed to find any image files in path " << args.picture_path.c_str() << endl;
			return 1;
		}
	}

	if ( args.random )
	{
		fl.randomize();
	}

	//fl.print();

	slideshow show(&args.so, &fl);
	if ( !show.run() )
		return 1;
	
	return 0;
}

