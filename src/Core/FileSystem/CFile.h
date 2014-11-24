/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
A class which provides a wrapper around fstream

-------------------------------------------------------------------------
History:
- 27:05:2014: Waring J.
*************************************************************************/
#pragma once
 
#include <CBuffer.h>

#include <fstream>
#include <string>

namespace Pro{
	namespace IO{
		using namespace std;

		enum struct EFile {
			FILE_BUFFER_OVERFLOW,
			FILE_NOT_FOUND,
			FILE_INVALID_DATA,
			FILE_CLOSED,
			FILE_FAILED,
			FILE_INVALID_OPERATION,
			END_OF_FILE,
			NO_FILE_ERROR
		};

		class CFile
		{
		private:
			fstream file;
			string m_file_path;
			EFile flag;
		public:
			CFile(const std::string& file); 
			~CFile();

			void CFile::open(const std::string& filePath);

			/*  Returns if there's more data to be read */
			bool isEndOfFile();

			/* Returns the size of the file in bytes*/
			unsigned int getSize();

			/*  Set's the offset inside the file to read */
			void setReadPosition(unsigned int);

			/*  Set's the offset inside the file to write */
			void setWritePosition(unsigned int);

			/*  Offset in which the file will read from */
			unsigned int getReadPosition();

			/*  Offset in which the file will write too */
			unsigned int getWritePosition();

			/*	Writes the buffer to the file */
			void write(CBuffer&); 

			/*  Writes the string to the file */
			void write(const std::string&);

			/*  Writes a primative type to the file */
			template<typename T>
			T write(T data){ 
				file.write((char*)&data, sizeof(T))
			}

			/*  Read's from the file and
			copies everything between the position
			and the size + position into the buffer */
			CBuffer read(unsigned int size);

			// Reads the whole file
			CBuffer CFile::read();

			// Read a primative data type
			template<typename T>
			T read(){
				T data;
				file.read((char*)(&data), sizeof(T));
				return data;
			} 

			template<typename T>
			T* read(unsigned size){
				T* data = new T[size];
				file.read(data, sizeof(T) * size);
				return data;
			}


			/*  Reads a upto the deliminator
			and returns between readPos and
			the found deliminator */
			string readToken(char delim);

			/*  Read's upto the first New line */
			string readLine();

			/*	Read's upto the first Null Terminator */
			string readString();

			/*	Closes the file and set's the flag to
			FILE_CLOSED */
			void close();

			/*	Returns wether or not the file has been closed */
			bool isOpen();

			/*	Returns the flag */
			EFile peekError();

			/*	Returns the flag and resets to NO_ERROR */
			EFile getError();
		};
	}
}