#pragma once

#ifdef _DLL_EXPORTS
#define DLL_API _declspec(dllexport)
#else
#define DLL_API _declspec(dllimport)
#endif

#include<vector>
#include<string>
using namespace std;

#pragma region file and folder operation

/*
*返回Dir路径下是否包含FolderName文件夹
*params
*Dir 根目录
*FolderName 指定文件夹名称
*/
DLL_API bool MT_IsDirHaveFolder(string Dir, string FolderName);

/*
*查找rootPath文件夹中 包含subPathName命名的文件夹路径,结果存放vresult中,当subPathName长度为0时，则查找的所有文件夹
*params
*rootPath 根目录
*vresult 返回结果
*bSub 是否包含子文件夹
*subPathName 筛选目录 区分大小写
*/
DLL_API void MT_LoadAllPath(const char* rootPath, std::vector<string>& vresult,bool bSub=true, const char* subPathName="");

/*
*查找rootPath文件夹中所有文件,结果存放vresult中
*params
*rootPath 根目录
*vresult 返回结果
*bSub 是否包含子文件夹
*suffix 文件过滤后缀 多个以'|'间隔 "jpg|png|bmp",查找文件时有效
*/
DLL_API void MT_LoadAllFile(const char* rootPath, std::vector<string>& vresult, bool bSub = true, const char* suffix = "");


/*
*判断文件、文件夹是否存在
*params
*path 路径名称
*bCreate 不存在时是否创建
*/
DLL_API bool MT_IsExist(const char* path, bool bCreate = false);

/*
*修改文件名
*params
*return 0 成功，-1 失败
*/
DLL_API int MT_Rename(const char* oldFile, const char* newFile);

/*
*删除文件 文件夹
*params
*return 0 失败，非零 成功
*/
DLL_API int MT_Delete(const char* File);

/*
*同步文件、文件夹
*params
*bCover true 存在则修改，false 存在不拷贝
*return 0 失败，非零 成功
*/
DLL_API int MT_CopyFile(const char* oldF, const char* newF,bool bCover=false);

/*
*选择文件夹
*params
*path 传出路径 
*/
DLL_API bool MT_SelectFolder(string& path);

/*
*选择文件
*params
*path 传出路径
*lpszFilter 文件过滤 BMP Files (*.bmp)|*.bmp|JPG Files (*.jpg)
*all_sel_files 传入vector时  可选择多个
*/
DLL_API bool MT_SelectFile(string& path, const char* lpszFilter = NULL);
DLL_API bool MT_SelectFile(vector<string>& all_sel_files, const char*  lpszFilter = NULL);

#pragma endregion

#pragma region exe operation

enum EM_StartFlag
{
	SF_NO=0,//无
	SF_RESTART,//重启
	SF_OPENMORE,//多开
};

enum EM_CmdType
{
	CT_KillExe = 0,	//结束进程
	CT_Mkdir,		//创建目录
	CT_OPEN,		//打开目录，文件		
};

/*
*启动exe
*params
*exePath exe路径
*Flag 启动标志
*cmdParam 命令行参数
*workDir 工作目录
*/
DLL_API bool MT_StartExe(string exePath, EM_StartFlag Flag = SF_NO, string cmdParam = "", string workDir = "");

/*
*关闭进程
*params
*exeName exe名称
*/
DLL_API bool MT_KillExe(string exeName);

/*
*执行cmd命令
*params
*type EM_CmdType
*params exe名称
*/
DLL_API bool MT_CmdOrder(EM_CmdType type, string params);

#pragma endregion

/*
*
*params
*/
//DLL_API