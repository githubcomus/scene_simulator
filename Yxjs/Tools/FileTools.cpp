

#include "FileTools.h"

//--------冲突的头文件--------//
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"

#include "windows.h"
#include "fileapi.h"
#include "handleapi.h"

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
//--------冲突的头文件--------//

FileTools::FileTools()
{
}

FileTools::~FileTools()
{
}

// 文件是否存在
bool FileTools::MyIsExistFile(char* path)
{
	return false;
}

//
bool FileTools::MyReadFile(char* filePath, char* fileBuf_p, int& file_size)
{
	HANDLE hFileR = (HANDLE)CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFileR == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	file_size = (int)GetFileSize(hFileR, 0);
	unsigned long fileNumberXrf = 0;
	int filesucc = (int)ReadFile(hFileR, fileBuf_p, file_size, &fileNumberXrf, NULL);
	if (filesucc == 0)
	{
		return false;
	}
	CloseHandle(hFileR);
	return 1;
}

//
bool FileTools::MyWriteFile(char* fileBuffer_p, int fileSize, char* filePath)
{
	unsigned long fileNumberXwf = 0;
	HANDLE hFindW = (HANDLE)CreateFileA(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	bool wB = WriteFile(hFindW, fileBuffer_p, fileSize, &fileNumberXwf, NULL);
	CloseHandle(hFindW);
	return wB;
}