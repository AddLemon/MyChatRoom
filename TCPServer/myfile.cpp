#include "myfile.h"

MyFile::MyFile() {
	file = NULL;
}

MyFile::~MyFile()
{
	if (file != NULL) {
		fclose(file);
	}
}

bool MyFile::Open(const char* fileName, const char* mode) {
	file = fopen(fileName, mode);
	if (file == NULL) {
		return false;
	}
	return true;
}

size_t MyFile::Write(const void* buf, int size) {
	size_t byte = fwrite(buf, size, 1, file);
	return byte;
}

size_t MyFile::Read(void* buf, int size) {
	size_t byte = fread(buf, size, 1, file);
	return byte;
}

void MyFile::Seek(int offset, SEEK origin) {
	fseek(file, offset, origin);
}

void MyFile::Close() {
	fclose(file);
	file = NULL;
}

int MyFile::Feof() {
	return feof(file);
}