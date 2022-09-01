


class FileTools
{

public:

	FileTools();
	~FileTools();

	static bool MyIsExistFile(char* path);
	static bool MyReadFile(char* filePath, char* fileBuf_p, int& file_size);
	static bool MyWriteFile(char* fileBuffer_p, int fileSize, char* filePath);
};