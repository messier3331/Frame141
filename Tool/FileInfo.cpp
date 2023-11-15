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

	// PathRelativePathTo : szCurDirectory���� strFullPath�� ���� ��� ��θ� ���ؼ� szRelativePath�� ������ ��, ��, ���� ����̺� �������� ����

	PathRelativePathTo(szRelativePath, 
						szCurDirectory, 
						FILE_ATTRIBUTE_DIRECTORY, 
						strFullPath.GetString(), 
						FILE_ATTRIBUTE_DIRECTORY);
	
	return CString(szRelativePath);
}
