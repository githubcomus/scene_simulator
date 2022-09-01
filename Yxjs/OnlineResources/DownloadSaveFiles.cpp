#include "DownloadSaveFiles.h"

#include "./OnlineResourcesComponent.h"

//--------冲突的头文件--------//
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"


#include <windows.h>
#include <urlmon.h>
#include <wininet.h>

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
//--------冲突的头文件--------//


#pragma comment(lib, "urlmon.lib")

#pragma comment(lib, "Wininet.lib")


/*------------------------------------------------------------------*/

//
DownloadSaveFiles::DownloadSaveFiles(UOnlineResourcesComponent* onlineResourcesComponent_)
{
	onlineResourcesComponent = onlineResourcesComponent_;
	onlineResourcesComponent->ShowLog("[DownloadSaveFiles::DownloadSaveFiles] start");
}

//
DownloadSaveFiles::~DownloadSaveFiles()
{
	onlineResourcesComponent->ShowLog("[DownloadSaveFiles::~DownloadSaveFiles] end");

}

//
void DownloadSaveFiles::TestFunct1()
{
	const char* url = "https://ww4.sinaimg.cn/orj480/7014a546gw1ea47f0yyuij20go0goq4w.jpg";
	const char* filePath = "E://Code_Windows//C++//ConsoleApplication4//TestData//aa.jpg";
	if (DownloadFiles(url, filePath)) printf("OK!//n");
	else printf("Error!//n");
}

// 文件是否存在
bool DownloadSaveFiles::FileExistsStatus(const char* path)
{
	DWORD dwAttribute = GetFileAttributesA(path);
	//0XFFFFFFFF表示文件不存在
	if (dwAttribute == 0XFFFFFFFF)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// 删除缓存下载文件
bool DownloadSaveFiles::DownloadFiles(const char* url, const char* downloadPath)
{
	DeleteUrlCacheEntryA(url);

	if (URLDownloadToFileA(nullptr, url, downloadPath, 0, nullptr) == S_OK
		&& FileExistsStatus(downloadPath)
		)
	{
		return true;
	}
	return false;
}
