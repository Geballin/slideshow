// PersistentPref.h: interface for the PersistentPref class.
//
// (C) 2003 by Jeremy Stanley
//
// This program is free software licensed under the terms of the GNU GPL.
// See COPYING for details.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERSISTENTPREF_H__91FCA392_8FF6_4EFC_9B15_97DD365CC851__INCLUDED_)
#define AFX_PERSISTENTPREF_H__91FCA392_8FF6_4EFC_9B15_97DD365CC851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

class PersistentPref
{
public:
	PersistentPref(HKEY key, LPCTSTR subkey);
	~PersistentPref();

	CString get_string(LPCTSTR name, LPCTSTR default_value);
	DWORD   get_dword(LPCTSTR name, DWORD default_value);

	bool    set_string(LPCTSTR name, LPCTSTR value);
	bool    set_dword(LPCTSTR name, DWORD value);

private:
	HKEY m_key;
	CString m_subkey;
	
};

#endif // !defined(AFX_PERSISTENTPREF_H__91FCA392_8FF6_4EFC_9B15_97DD365CC851__INCLUDED_)
