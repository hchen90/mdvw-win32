/*$ @trans.h : copyright (c) 2017 hsiang chen : under GPL*/
#pragma once

#include <vector>

namespace trans {
	LONGLONG skip(int ch, int ch_sec, unsigned char* ptr, LONGLONG ix, ULONGLONG len);
	LONGLONG tostart(unsigned char * ptr, LONGLONG ix, ULONGLONG len);
	LONGLONG toend(unsigned char* ptr, LONGLONG ix, ULONGLONG len);
	LONGLONG count(int ch, unsigned char * ptr, LONGLONG ix, ULONGLONG len);
	LONGLONG search(int ch, unsigned char * ptr, LONGLONG ix, ULONGLONG len);
	/////
	bool isMD(const TCHAR* filenm);
	TCHAR* pathtitle(const TCHAR* buf);
	TCHAR* tempfile(TCHAR* buf, const TCHAR* old);
	void trans_start(const CString& from, const CString& to);
	void truncate(const char * str, int ch, char buf[], size_t len);
	size_t url_get(const char * str, int ch1, int ch2, char buf[], size_t len);
	size_t url_label(const char* str, char buf[], size_t len);
	size_t url_text(const char* str, char buf[], size_t len);
	/////
	void sub_element(const char* str, char buf[], size_t len);
	void chk_blkcode(CFile& output, std::vector<char*>& tBlkCode);
	void chk_blklist(CFile & output, std::vector<char*>& tBlkList);
	void chk_blkolst(CFile & output, std::vector<char*>& tBlkLstN);
};

