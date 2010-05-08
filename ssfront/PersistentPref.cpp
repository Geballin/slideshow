// PersistentPref.cpp: implementation of the PersistentPref class.
//
// (C) 2003 by Jeremy Stanley
//
// This program is free software licensed under the terms of the GNU GPL.
// See COPYING for details.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ssfront.h"
#include "PersistentPref.h"

#include <malloc.h>
#include <tchar.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PersistentPref::PersistentPref(HKEY key, LPCTSTR subkey) :
	m_key(key),
	m_subkey(subkey)
{
}

PersistentPref::~PersistentPref()
{
}

CString PersistentPref::get_string(LPCTSTR name, LPCTSTR default_value)
{
	HKEY subkey;
	if ( ERROR_SUCCESS != RegOpenKeyEx(m_key, m_subkey, 0, KEY_READ, &subkey) )
		return CString(default_value);

	// get string size
	DWORD size = 0, type = 0;
	if ( (ERROR_SUCCESS != RegQueryValueEx(subkey, name, NULL, &type, NULL, &size)) ||
		 (type != REG_SZ) )
	{
		RegCloseKey(subkey);
		return CString(default_value);
	}

	// get string
	TCHAR *str = (TCHAR *)alloca(size);
	if ( ERROR_SUCCESS != RegQueryValueEx(subkey, name, NULL, &type, (LPBYTE)str, &size) )
	{
		RegCloseKey(subkey);
		return CString(default_value);
	}

	RegCloseKey(subkey);
	return CString(str);
}

DWORD   PersistentPref::get_dword(LPCTSTR name, DWORD default_value)
{
	HKEY subkey;
	if ( ERROR_SUCCESS != RegOpenKeyEx(m_key, m_subkey, 0, KEY_READ, &subkey) )
		return default_value;

	// get data
	DWORD size = sizeof(DWORD), type = 0, data = 0;
	if ( (ERROR_SUCCESS != RegQueryValueEx(subkey, name, NULL, &type, (LPBYTE)&data, &size)) ||
		 (type != REG_DWORD) )
	{
		RegCloseKey(subkey);
		return default_value;
	}

	RegCloseKey(subkey);
	return data;
}

bool    PersistentPref::set_string(LPCTSTR name, LPCTSTR value)
{
	HKEY subkey;
	if ( ERROR_SUCCESS != RegCreateKey(m_key, m_subkey, &subkey) )
		return false;

	if ( ERROR_SUCCESS != RegSetValueEx(subkey, name, 0, REG_SZ, (CONST BYTE *)value, (_tcslen(value) + 1) * sizeof(TCHAR)) )
	{
		RegCloseKey(subkey);
		return false;
	}

	RegCloseKey(subkey);
	return true;
}

bool    PersistentPref::set_dword(LPCTSTR name, DWORD value)
{
	HKEY subkey;
	if ( ERROR_SUCCESS != RegCreateKey(m_key, m_subkey, &subkey) )
		return false;

	if ( ERROR_SUCCESS != RegSetValueEx(subkey, name, NULL, REG_DWORD, (CONST BYTE *)&value, sizeof(DWORD)) )
	{
		RegCloseKey(subkey);
		return false;
	}

	RegCloseKey(subkey);
	return true;
}

