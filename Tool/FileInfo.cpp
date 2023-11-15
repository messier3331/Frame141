#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	TCHAR	szRelativePath[MAX_PATH] = L"";
	TCHAR	szCurDirectory[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirectory);

	// PathRelativePathTo : szCurDirectory에서 strFullPath로 가는 상대 경로를 구해서 szRelativePath에 저장해 줌, 단, 같은 드라이브 내에서만 가능

	PathRelativePathTo(szRelativePath, 
						szCurDirectory, 
						FILE_ATTRIBUTE_DIRECTORY, 
						strFullPath.GetString(), 
						FILE_ATTRIBUTE_DIRECTORY);
	
	return CString(szRelativePath);
}
