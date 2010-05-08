/*
    cache.h - A simple C++ template-based cache class
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


#ifndef PS_CACHE_H
#define PS_CACHE_H

#if _MSC_VER > 1000
#pragma warning ( disable : 4786 ) // get rid of bogus debug info warning
#pragma once
#endif

#include <map>
#include <list>
#include <cassert>

template <typename index_t, typename data_t>
class cache
{
public:
	class data_callback {
	public:
		virtual data_t load(index_t ix) = 0;
		virtual void discard(data_t data) = 0;
	};

	cache(data_callback *cb, int capacity) :
		m_callback(cb),
		m_capacity(capacity)
	{
	}

	~cache()
	{
		flush();
	}

	void flush()
	{
		for(typename data_list::iterator it = m_data.begin(); it != m_data.end(); ++it)
		{
			m_callback->discard(it->second);
		}
		m_data.clear();
		m_index.clear();
	}

	data_t lookup(index_t ix)
	{
		typename index_map::iterator it_i = m_index.find(ix);
		if (it_i != m_index.end())
		{
			// cache hit:
			// move to front of MRU list
			typename data_list::iterator it_d = it_i->second;
			data_list_entry ent = *it_d;
			m_data.erase(it_d);
			m_data.push_front(ent);
			m_index[ix] = m_data.begin();

			// return pointed-to data
			return ent.second;
		}

		// cache miss:
		// remove last element of list, if list size == capacity.
		if ( m_index.size() == m_capacity )
		{
			typename data_list::reverse_iterator last = m_data.rbegin();

			// remove index element
			typename index_map::iterator last_i = m_index.find(last->first);
			assert( last_i != m_index.end() );
			m_index.erase(last_i);

			// remove list element
			m_callback->discard(last->second);
			m_data.pop_back();
		}

		// insert new item at the beginning of the list, initialized with callback.
		data_t dat = m_callback->load(ix);
		m_data.push_front( data_list_entry(ix, dat) );
		m_index[ix] = m_data.begin();

		// return data
		return dat;
	}

private:
	data_callback *m_callback;
	int            m_capacity;

	typedef typename std::pair<index_t, data_t> data_list_entry;
	typedef std::list<data_list_entry> data_list;
	typedef std::map<index_t, typename data_list::iterator> index_map;
	
	data_list m_data;	// MRU sorted 
	index_map m_index;

/*
	void debug_print(const char *msg)
	{
		using namespace std;
		cout << msg << ": \n\tindex = ";
		for(index_map::iterator im = m_index.begin(); im != m_index.end(); ++im)
		{
			cout << "(" << im->first << ", (" << im->second->first << ", " << im->second->second << ")) ";
		}
		cout << "\n\tdata = ";
		for(data_list::iterator id = m_data.begin(); id != m_data.end(); ++id)
		{
			cout << "(" << id->first << ", " << id->second << ") ";
		}
		cout << endl;
	}
*/
};


#endif // PS_CACHE_H


