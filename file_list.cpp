// Copyright (C) 2003  Jeremy Stanley
// This is free software licensed under the GNU GPL; see COPYING for details

#include "file_list.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#ifdef WIN32
#include <io.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

file_list::file_list() :
	m_sort(sort_none)
{
	init_supported_types();
}

file_list::~file_list()
{
}

bool file_list::load_directory(const char *pathname, bool recurse, sort_t sort, bool descending)
{
	if ( !pathname || !pathname[0] )
		return false;
	
	std::string thepath = pathname;
	std::replace(thepath.begin(), thepath.end(), '\\', '/');	// canonize slashes
	if ( thepath[thepath.size() - 1] != '/' )
		thepath += '/';											// append slash if there isn't one

	m_sort = sort;

	do_path(thepath, recurse);

	switch ( m_sort )
	{
	case sort_name:
		if ( descending )
		{
			std::stable_sort( m_list.begin(), m_list.end(), sort_name_desc() );
		}
		else
		{
			std::stable_sort( m_list.begin(), m_list.end(), sort_name_asc() );
		}
		break;

	case sort_date_create:
	case sort_date_modify:
		if ( descending )
		{
			std::stable_sort( m_list.begin(), m_list.end(), sort_date_desc() );
		}
		else
		{
			std::stable_sort( m_list.begin(), m_list.end(), sort_date_asc() );
		}
		break;
	}

	return (m_list.size() > 0);
}

// path must have a trailing /
void file_list::do_path(const std::string &path, bool recurse)
{
#ifdef WIN32
	std::string searchpath = path + '*';

	_finddata_t fd;
	int fh = _findfirst(searchpath.c_str(), &fd);
	if ( fh != -1 )
	{
		do
		{
			if ( 0 != (fd.attrib & _A_SUBDIR) )
			{
				if ( recurse && (0 != strcmp(fd.name, ".")) && (0 != strcmp(fd.name, "..")))
				{
					do_path(path + fd.name + '/', true);
				}
			}
			else
			{
				do_file(path + fd.name, (m_sort == sort_date_create) ? fd.time_create : fd.time_write);
			}
		} while (0 == _findnext(fh, &fd));
	}
#else
	DIR *dir = opendir(path.c_str());
	if ( !dir ) 
		return;
		
	while ( dirent *ent = readdir(dir) )
	{
		if ( (0 == strcmp(ent->d_name, ".")) || (0 == strcmp(ent->d_name, "..")) || (strspn(ent->d_name, ".")))
			continue;

		std::string fullname = path + ent->d_name;

		struct stat statbuf;
		if ( 0 != stat(fullname.c_str(), &statbuf) )
			continue;

		if ( 0 != (statbuf.st_mode & S_IFDIR) )
		{
			if ( recurse )
			{
				do_path(fullname + '/', true);
			}
		}
		else
		{
			do_file(fullname, (m_sort == sort_date_create) ? statbuf.st_ctime : statbuf.st_mtime);
		}
	}

	closedir( dir );
#endif
}

void file_list::do_file(const std::string &path, time_t timestamp /* = 0 */)
{
	int extpos = path.rfind('.');
	if ( extpos == std::string::npos )
		return;

	int slashpos = path.rfind('/');
	if ( slashpos != std::string::npos )
	{
		if ( slashpos > extpos )
			return;
	}

	if ( is_supported_type(path.substr(extpos + 1)) )
	{
		m_list.push_back( file_list_entry(path, timestamp) );
	}
}

bool file_list::load_file_list(const char *filename)
{
	std::istream *fl = NULL;
	std::ifstream file;
	
	if ( 0 == strcmp(filename, "-" ) )
	{
		fl = &std::cin;	// read from stdin
	}
	else
	{
		file.open(filename);
		if ( !file )
			return false;
		fl = &file;
	}
	
	char linebuf[1024];
	while( !fl->eof() )
	{
		fl->getline(linebuf, sizeof(linebuf));
		if ( !linebuf[0] )
			continue;

		m_list.push_back( file_list_entry(linebuf, 0) );
	}

	if ( file.is_open() )
		file.close();
	
	return (m_list.size() > 0);
}

void file_list::randomize()
{
	srand( (unsigned)time(NULL) );
	std::random_shuffle( m_list.begin(), m_list.end() );
}

void file_list::delete_file(int file2delete){
	unlink(get(file2delete).c_str());
	m_list.erase(m_list.begin()+file2delete);
}

void file_list::init_supported_types()
{
	m_ext_list.insert("tga");
	m_ext_list.insert("bmp");
	m_ext_list.insert("pnm");
	m_ext_list.insert("pbm");
	m_ext_list.insert("pgm");
	m_ext_list.insert("ppm");
	m_ext_list.insert("xpm");
	m_ext_list.insert("xcf");
	m_ext_list.insert("pcx");
	m_ext_list.insert("gif");
	m_ext_list.insert("jpg");
	m_ext_list.insert("jpeg");
	m_ext_list.insert("tif");
	m_ext_list.insert("tiff");
	m_ext_list.insert("png");
	m_ext_list.insert("lbm");
	m_ext_list.insert("iff");
}

bool file_list::is_supported_type(const std::string &type)
{
	std::string nctype = type;
	for(int i = 0; i < nctype.size(); i++)
	{
		nctype[i] = tolower(nctype[i]);
	}

	return ( m_ext_list.end() != m_ext_list.find(nctype) );
}

void file_list::print()
{
	for(int i = 0; i < count(); i++)
	{
		std::cout << m_list[i].name.c_str() << " ";
		if ( m_list[i].timestamp != 0 )
			std::cout << ctime(&m_list[i].timestamp);
		else
			std::cout << std::endl;
	}
}
