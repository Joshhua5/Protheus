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

#include "EFile.h"
#include "..\util\CBuffer.h"

#include <fstream>
#include <string>

namespace Pro{
	namespace Util{
		using namespace std;
		class CFile
		{
		private:
			fstream file;
			string m_file_path;
			EFile flag;
		public:
			CFile(const std::string& file);
			CFile();
			~CFile();

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
			/*  Writes the float to the file */
			void write(float);
			/*  Writes the string to the file */
			void write(const std::string&);
			/*  Writes the integer to the file */
			void write(int);

			/*  Read's from the file and
			copies everything between the position
			and the size + position into the buffer */
			CBuffer read(unsigned int size);

			/*  Reads an Byte at the position
			if no Byte is found then
			FILE_INVALID_DATA is set */
			char readByte();

			/*  Reads an float at the position
			if no float is found then
			FILE_INVALID_DATA is set */
			float readFloat();

			/*  Reads an int at the position
			if no int is found then
			FILE_INVALID_DATA is set */
			int readInt();

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