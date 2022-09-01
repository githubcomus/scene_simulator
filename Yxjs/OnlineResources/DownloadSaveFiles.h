#pragma once


#include <mutex>
#include <string>

//using namespace std;

class UOnlineResourcesComponent;


class DownloadSaveFiles
{


public:

	// owner
	UOnlineResourcesComponent* onlineResourcesComponent;

	//
	DownloadSaveFiles(UOnlineResourcesComponent* onlineResourcesComponent_);

	//
	~DownloadSaveFiles();

	void TestFunct1();
	bool FileExistsStatus(const char* path);
	bool DownloadFiles(const char* url, const char* downloadPath);
};

