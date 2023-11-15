#pragma once

#include "Incude.h"

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString	 ConvertRelativePath(CString strFullPath);
};

