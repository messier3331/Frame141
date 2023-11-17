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

	//���� ���丮 ��ġ �޾ƿͼ�
	GetCurrentDirectory(MAX_PATH, szCurDirectory);


	//���� ���丮 -> ��ü ��η� ���� ��� ��� ���ϱ�. szRelativePath ����(�� ����̺길 ����)
	PathRelativePathTo(szRelativePath,
		szCurDirectory,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(),
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathList)
{
	//����, ��� ���� ���� Ŭ����
	CFileFind	Find;

	//�ؽ��ĸ� �ҷ��� ������ ��ο� �����ϴ� ��� ���� ���
	wstring		wstrFilePath = wstrPath + L"/*.*";

	//������ ���� �� ���� ������ continue(��� ȣ�� �� �������� ����)
	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		//.������? pass
		if (Find.IsDots())
			continue;

		//�������? �� ��� �ܾ����
		//else if (Find.IsDirectory())
		//	DirInfoExtraction(wstring(Find.GetFilePath()), rPathList);

		if (Find.IsDirectory())
		{
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathList);
			continue;
		}

		//������ ������ ó��
		//else
		//{
			//���� ���� pass

		if (Find.IsHidden())
			continue;

		IMGPATH*	pImgPath = new IMGPATH;
		TCHAR		szPathBuff[MAX_PATH] = L"";

		//���� �ؽ��� ������ ���
		lstrcpy(szPathBuff, Find.GetFilePath().GetString());

		//�� �̸� ����
		PathRemoveFileSpec(szPathBuff);

		pImgPath->iCount = DirFileCount(szPathBuff);


		wstring wstrTextureName = Find.GetFileTitle().GetString();
		wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";

		TCHAR	szBuff[MAX_PATH] = L"";

		lstrcpy(szBuff, Find.GetFilePath().GetString());

		PathRemoveFileSpec(szBuff);
		PathCombine(szBuff, szBuff, wstrTextureName.c_str());
		pImgPath->wstrPath = ConvertRelativePath(szBuff);

		PathRemoveFileSpec(szBuff);
		pImgPath->wstrStateKey = PathFindFileName(szBuff);

		PathRemoveFileSpec(szBuff);
		pImgPath->wstrObjKey = PathFindFileName(szBuff);

		rPathList.push_back(pImgPath);
		bContinue = false;

		//}

	}
}

int CFileInfo::DirFileCount(const wstring& wstrPath)
{
	CFileFind	Find;
	wstring		wstrFilePath = wstrPath + L"/*.*";

	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	int iFileCnt = 0;

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots() || Find.IsSystem())
			continue;

		++iFileCnt;
	}

	return iFileCnt;
}
