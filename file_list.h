// Copyright (C) 2003  Jeremy Stanley
// This is free software licensed under the GNU GPL; see COPYING for details

#if !defined(AFX_FILE_LIST_H__824CDB60_0B4B_4091_A275_611DE7C6FB2A__INCLUDED_)
#define AFX_FILE_LIST_H__824CDB60_0B4B_4091_A275_611DE7C6FB2A__INCLUDED_

#if _MSC_VER > 1000
#pragma warning ( disable : 4786 )	// get rid of bogus truncated debug info warnings
#pragma once
#endif // _MSC_VER > 1000

#include <time.h>
#include <string>
#include <vector>
#include <set>

class file_list  
{
public:
	file_list();
	virtual ~file_list();

	enum sort_t { sort_none, sort_name, sort_date_create, sort_date_modify };
	
	bool load_directory(const char *pathname, bool recurse, sort_t sort, bool descending);
	bool load_file_list(const char *filename);
	void randomize();
	
	void delete_file(int m_image_index);

	void print();

	int  count()                 { return m_list.size(); }
	std::string get(int index)   { return m_list.at(index).name;  }

private:
	void do_path(const std::string &path, bool recurse);
	void do_file(const std::string &path, time_t timestamp = 0);

	struct file_list_entry 
	{
		std::string name;
		time_t      timestamp;

		file_list_entry(const std::string &nm, time_t ts = 0) :
		name(nm), timestamp(ts) {}
	};

	class sort_name_asc
	{
	public:
		bool operator()(const file_list_entry &lhs, const file_list_entry &rhs)
		{ return (lhs.name < rhs.name); }
	};

	class sort_name_desc
	{
	public:
		bool operator()(const file_list_entry &lhs, const file_list_entry &rhs)
		{ return (lhs.name > rhs.name); }
	};

	class sort_date_asc
	{
	public:
		bool operator()(const file_list_entry &lhs, const file_list_entry &rhs)
		{ return (lhs.timestamp < rhs.timestamp); }
	};

	class sort_date_desc 
	{
	public:
		bool operator()(const file_list_entry &lhs, const file_list_entry &rhs)
		{ return (lhs.timestamp > rhs.timestamp); }
	};

	std::vector<file_list_entry> m_list;
	sort_t m_sort;

	void init_supported_types();
	bool is_supported_type(const std::string &type);
	typedef std::set<std::string> string_set;
	string_set m_ext_list;
};

#endif // !defined(AFX_FILE_LIST_H__824CDB60_0B4B_4091_A275_611DE7C6FB2A__INCLUDED_)
