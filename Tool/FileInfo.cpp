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

	//현재 디렉토리 위치 받아와서
	GetCurrentDirectory(MAX_PATH, szCurDirectory);


	//현재 디렉토리 -> 전체 경로로 가는 상대 경로 구하기. szRelativePath 저장(같 드라이브만 가능)
	PathRelativePathTo(szRelativePath,
		szCurDirectory,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(),
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathList)
{
	//파일, 경로 제어 관련 클래스
	CFileFind	Find;

	//텍스쳐를 불러올 폴더의 경로에 존재하는 모든 파일 경로
	wstring		wstrFilePath = wstrPath + L"/*.*";

	//파일을 읽을 수 없을 때까지 continue(재귀 호출 때 빠져나올 조건)
	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		//.파일은? pass
		if (Find.IsDots())
			continue;

		//폴더라면? 밑 경로 긁어오기
		//else if (Find.IsDirectory())
		//	DirInfoExtraction(wstring(Find.GetFilePath()), rPathList);

		if (Find.IsDirectory())
		{
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathList);
			continue;
		}

		//나머지 파일의 처리
		//else
		//{
			//숨김 파일 pass

		if (Find.IsHidden())
			continue;

		IMGPATH*	pImgPath = new IMGPATH;
		TCHAR		szPathBuff[MAX_PATH] = L"";

		//읽을 텍스쳐 파일의 경로
		lstrcpy(szPathBuff, Find.GetFilePath().GetString());

		//끝 이름 제거
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
