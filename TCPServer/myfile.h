#pragma once
#include "public.h"

enum  SEEK {
	BEG = SEEK_SET,
	CUR = SEEK_CUR,
	END = SEEK_END
};

class MyFile {
private:
	FILE* file; 
public:
	MyFile();
	~MyFile();
	
	/// <summary>
	/// open file
	/// </summary>
	/// <param name="fileName">file name</param>
	/// <param name="mode">the way to open the file</param>
	/// <returns>true for success, otherwise false</returns>
	bool Open(const char* fileName, const char* mode);

	/// <summary>
	/// write to the file
	/// </summary>
	/// <param name="buf">the context to write</param>
	/// <param name="size">the size of the buffer</param>
	/// <returns>the length of the context</returns>
	size_t Write(const void* buf, int size);

	/// <summary>
	/// read from the file
	/// </summary>
	/// <param name="buf">the buffer to store the context</param>
	/// <param name="size">the size of the buffer</param>
	/// <returns>the length of the context</returns>
	size_t Read(void* buf, int size);

	/// <summary>
	/// set the file pointer
	/// </summary>
	/// <param name="offset">the length to offset the original position</param>
	/// <param name="pos">the start position of the file pointer</param>
	void Seek(int offset, SEEK pos);

	/// <summary>
	/// close the file
	/// </summary>
	void Close();

	/// <summary>
	/// check if file pointer is at the end of file
	/// </summary>
	/// <returns></returns>
	int Feof();
};