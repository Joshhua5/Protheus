/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
-------------------------------------------------------------------------

Description:
A class which provides a wrapper around fstream

-------------------------------------------------------------------------
History:
- 27:05:2014: Waring J.
*************************************************************************/
#pragma once
 
#include <Buffer.h>

#include <fstream>
#include <string>

namespace Pro{
	namespace IO{
		using namespace std;
		using namespace Util;

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

			void Open(const std::string& filePath);

			/*!  Returns if there's more data to be read */
			bool IsEndOfFile();

			/*! Returns the size of the file in bytes*/
			unsigned int FileSize();

			/*!  Set's the offset inside the file to read */
			void SetReadPosition(unsigned int);

			/*!  Set's the offset inside the file to write */
			void SetWritePosition(unsigned int);

			/*!  Offset in which the file will read from */
			unsigned int GetReadPosition();

			/*!  Offset in which the file will write too */
			unsigned int GetWritePosition();

			/*!	Writes the buffer to the file */
			void Write(Buffer&); 

			/*!  Writes the string to the file */
			void Write(const std::string&);

			/*!  Writes a primative type to the file */
			template<typename T>
			T Write(T data){ 
				file.Write((char*)&data, sizeof(T))
			}

			/*!  Read's from the file and
			copies everything between the position
			and the size + position into the buffer */
			Buffer Read(unsigned size);

			//! Reads the whole file
			Buffer Read();

			//! Read a primative data type
			template<typename T>
			T Read(){
				T data;
				file.Read((char*)(&data), sizeof(T));
				return data;
			} 

			template<typename T>
			T* Read(unsigned size){
				T* data = new T[size];
				file.Read(data, sizeof(T) * size);
				return data;
			}


			/*!  Reads a upto the deliminator
			and returns between readPos and
			the found deliminator */
			string ReadToken(const char delim);

			/*!  Read's upto the first New line */
			string ReadLine();

			/*!	Read's upto the first Null Terminator */
			string ReadString();

			/*!	Closes the file and set's the flag to FILE_CLOSED */
			void Close();

			/*!	Returns wether or not the file has been closed */
			bool IsOpen();

			/*!	Returns the flag */
			EFile PeekError();

			/*!	Returns the flag and resets to NO_ERROR */
			EFile GetError();
		};
	}
}