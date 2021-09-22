#include"StdAfx.h"
#include"MtghWindow.h"
#include<io.h>
#include <tlhelp32.h>  

std::vector<std::string> splitWithStl(const std::string &str, const std::string &pattern)
{
	std::vector<std::string> resVec;

	if ("" == str){
		return resVec;
	}
	//方便截取最后一段数据
	std::string strs = str + pattern;

	size_t pos = strs.find(pattern);
	size_t size = strs.size();

	while (pos != std::string::npos)
	{
		std::string x = strs.substr(0, pos);
		x.insert(0, ".");
		resVec.push_back(x);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(pattern);
	}

	return resVec;
}

bool IsFile(const char* buf)
{
	struct stat s;
	if (stat(buf, &s) == 0) {
		if (s.st_mode & S_IFREG) {
			return true;
		}
	}

	/*if (strcspn(buf, "*.*") > 0) {
		return true;
	}*/
	return false;
}
BOOL myDeleteDirectory(CString directory_path)   //删除一个文件夹下的所有内容
{
	BOOL ret = TRUE;
	CFileFind finder;
	CString path;
	path.Format(_T("%s/*.*"), directory_path);
	BOOL bWorking = finder.FindFile(path);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots())
		{//处理文件夹
			myDeleteDirectory(finder.GetFilePath()); //递归删除文件夹
			RemoveDirectory(finder.GetFilePath());//只能删除空的文件夹
		}
		else
		{//处理文件
			DeleteFile(finder.GetFilePath());
		}
	}
	ret = RemoveDirectory(directory_path);
	return ret;
}

BOOL myCopyDirectory(CString olddir,CString newdir, bool bCover)   //拷贝文件夹下的所有内容
{
	CreateDirectory(newdir, NULL);
	int ret = 0;
	CFileFind finder;
	CString path;
	path.Format(_T("%s/*.*"), olddir);
	BOOL bWorking = finder.FindFile(path);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots()){
			//处理文件夹
			CString newpath = finder.GetFilePath();
			newpath.Replace(olddir, newdir);
			myCopyDirectory(finder.GetFilePath(),newpath, bCover);
		}
		else{
			//处理文件
			CString newpath = finder.GetFilePath();
			if (IsFile(newpath.GetBuffer())) {
				newpath.Replace(olddir, newdir);
				bool res = MT_IsExist(newpath.GetBuffer(), false);
				if (!res || bCover)
					ret = CopyFile(finder.GetFilePath(), newpath, false);
			}
		}			
	}
	return ret;
}

bool MT_CmdOrder(EM_CmdType type,string params)
{	
	char cmd[296]="";
	switch(type)
	{
	case CT_KillExe:
		sprintf_s(cmd, 296, "cmd /c taskkill /f /t /im %s", params.c_str());
		break;
	case CT_Mkdir:
		sprintf_s(cmd, 296, "cmd /c mkdir %s", params.c_str());
		break;
	case CT_OPEN:
		sprintf_s(cmd, 296, "cmd /c start %s", params.c_str());
		break;
	default:
		break;
	}
	STARTUPINFO   si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION   pi;
	ZeroMemory(&pi, sizeof(pi));
	if (CreateProcess(NULL, cmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	return true;

	//UINT res = WinExec(cmd, SW_HIDE);
	//return 0 == system(cmd);
}

bool MT_IsDirHaveFolder(string Dir, string FolderName)
{
	string path = Dir + "\\" + FolderName;
	if (0 == _access(path.c_str(), 00)){
		return true;
	}
	return false;
}

void MT_LoadAllPath(const char* rootPath, std::vector<string>& vresult, bool bSub,const char* subPathName)
{
	string str = "";
	char TempPath[MAX_PATH];
	char path[MAX_PATH];
	sprintf_s(TempPath, "%s\\", rootPath);//mImagePath文件夹路径
	sprintf_s(path, "%s\\*.*", rootPath);
	
	WIN32_FIND_DATA FindFileData;

	HANDLE hFind = ::FindFirstFile(path, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)  return;

	while (true)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
			if (FindFileData.cFileName[0] != '.'/* && false*/){
				str = TempPath;
				str.append(FindFileData.cFileName);
				if (strlen(subPathName) == 0){
					vresult.emplace_back(str);
				}
				else if (MT_IsDirHaveFolder(str, subPathName))
					vresult.emplace_back(str);
				if(bSub)
					MT_LoadAllPath(str.c_str(), vresult, subPathName);
			}
		}
		if (!FindNextFile(hFind, &FindFileData))  break;
	}
	FindClose(hFind);
}
void MT_LoadAllFile(const char* rootPath, std::vector<string>& vresult, bool bSub, const char* suffix)
{
	string str = "";
	char TempPath[MAX_PATH];
	char path[MAX_PATH];
	sprintf_s(TempPath, "%s\\", rootPath);//mImagePath文件夹路径
	sprintf_s(path, "%s\\*.*", rootPath);

	vector<string> Vsuffix = splitWithStl(suffix, "|");

	WIN32_FIND_DATA FindFileData;

	HANDLE hFind = ::FindFirstFile(path, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)  return;

	while (true)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (FindFileData.cFileName[0] != '.'/* && false*/) {
				str = TempPath;
				str.append(FindFileData.cFileName);
				if (bSub)
					MT_LoadAllFile(str.c_str(), vresult, suffix);
			}
		}
		else
		{
			str = TempPath;
			str.append(FindFileData.cFileName);
			if (Vsuffix.size() == 0)
				vresult.emplace_back(str);
			else{
				for each (string var in Vsuffix)
				{
					if (str.find(var) != str.npos) {
						vresult.emplace_back(str);
						break;
					}
				}
			}
		}
		if (!FindNextFile(hFind, &FindFileData))  break;
	}
	FindClose(hFind);
}

bool MT_IsExist(const char* path, bool bCreate)
{
	if (0 == _access(path, 00)){
		return true;
	}
	else if(bCreate){
		return MT_CmdOrder(CT_Mkdir,path);
	}
	return false;
}

int MT_Rename(const char* oldFile, const char* newFile)
{
	if (strlen(oldFile) == 0 || strlen(newFile) == 0){
		return -1;
	}
	return rename(oldFile, newFile);
}
int MT_Delete(const char* File)
{
	if (strlen(File) == 0) {
		return -1;
	}	
	return IsFile(File) == true ? DeleteFile(File) : myDeleteDirectory(File);
}
int MT_CopyFile(const char* oldF, const char* newF, bool bCover)
{
	if (strlen(oldF) == 0 || strlen(newF) == 0) {
		return 0;
	}
	int res = 0;

	if (IsFile(oldF)) {
		string newpath = newF;
		int lt = newpath.rfind("\\");
		newpath = newpath.substr(0, lt);
		MT_IsExist(newpath.c_str(), true);
		bool res = MT_IsExist(newF, false);
		if(!res || bCover)
			res = CopyFile(oldF, newF, false);
	}
	else{
		MT_IsExist(newF,true);
		res = myCopyDirectory(oldF, newF, false);
	}
	return res;
}

bool MT_SelectFolder(string& path)
{
	TCHAR szFolderPath[MAX_PATH] = { 0 };
	CString strFolderPath = _T("");

	BROWSEINFO sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = CSIDL_DESKTOP;
	sInfo.ulFlags = BIF_NEWDIALOGSTYLE;//有新建文件夹按钮
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			path = szFolderPath;
		}
		::CoTaskMemFree(lpidlBrowse);
		return true;
	}
	return false;
}

bool MT_SelectFile(string& path, const char* lpszFilter)
{
	CFileDialog dlg(TRUE, NULL, "", OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,lpszFilter, NULL);

	dlg.m_ofn.lpstrTitle = "Select File";
	INT_PTR result = dlg.DoModal();
	if (result == IDOK)
	{
		path = dlg.GetPathName();//获取文件名-包含路径
		return true;
	}
	return false;
}

bool MT_SelectFile(vector<string>& all_sel_files, const char* lpszFilter)
{
	
	CFileDialog dlgFile(TRUE, "", NULL, OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, lpszFilter, NULL);
	dlgFile.m_ofn.lpstrTitle = "Select Files";;
	if (dlgFile.DoModal() == IDOK)
	{
		POSITION fileNamesPosition = dlgFile.GetStartPosition();
		while (fileNamesPosition != NULL)
		{
			string path = dlgFile.GetNextPathName(fileNamesPosition);
			all_sel_files.push_back(path);
		}
		return true;
	}
	return false;
}

DWORD IsExistProcess(const char*  szProcessName)
{
	PROCESSENTRY32 pe;
	DWORD id = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
		return 0;
	while (1)
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
			break;
		if (strcmp(pe.szExeFile, szProcessName) == 0)
		{
			id = pe.th32ProcessID;

			break;
		}
	}
	CloseHandle(hSnapshot);
	return id;
}
bool MT_KillExe(string exeName)
{
	return MT_CmdOrder(CT_KillExe, exeName);
}

bool MT_StartExe(string exePath, EM_StartFlag Flag,string cmdParam, string workDir)
{
	int lt = exePath.rfind('\\') + 1;
	int count = exePath.length() - lt;
	string mExeName = exePath.substr(lt,count);
	DWORD run = IsExistProcess(mExeName.c_str());
	if ((run == 0||Flag==SF_OPENMORE) || Flag!=SF_NO){
		if (Flag == SF_RESTART && run != 0) {
			MT_KillExe(mExeName);
		}
		SHELLEXECUTEINFO ShExecInfo = { 0 };

		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

		ShExecInfo.hwnd = NULL;

		ShExecInfo.lpVerb = NULL;

		ShExecInfo.lpFile = exePath.c_str();

		ShExecInfo.lpParameters = cmdParam.c_str();

		ShExecInfo.lpDirectory = workDir.c_str();

		ShExecInfo.nShow = SW_SHOW;

		ShExecInfo.hInstApp = NULL;

		return ShellExecuteEx(&ShExecInfo);
	}
	return false;
}