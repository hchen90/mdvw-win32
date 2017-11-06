/*$ @trans.cpp : copyright (c) 2017 hsiang chen : under GPL*/
#include "stdafx.h"
#include "trans.h"

#include <regex>
#include <vector>

LONGLONG trans::skip(int ch, int ch_sec, unsigned char * ptr, LONGLONG ix, ULONGLONG len)
{
	for ( ; ix < (LONGLONG) len; ) {
		if (!ch_sec && ptr[ix] == ch) {
			ix++;
			continue;
		}
		if (ch_sec && ptr[ix] == ch && ix + 1 < (LONGLONG) len && ptr[ix + 1] == ch_sec) {
			ix += 2;
			continue;
		}
		break;
	}
	return ix;
}

LONGLONG trans::tostart(unsigned char* ptr, LONGLONG ix, ULONGLONG len)
{
	if (ix < (LONGLONG)len && ptr[ix] == ' ') {
		trans::skip(' ', 0, ptr, ix, len);
	}
	if (ix < (LONGLONG)len && ptr[ix] == '\t') {
		trans::skip('\t', 0, ptr, ix, len);
	}
	if (ptr[ix] == '\r' && ix + 1 < (LONGLONG)len && ptr[ix + 1] == '\n') {
		return trans::skip('\r', '\n', ptr, ix, len);
	}
	if (ptr[ix] == '\r') {
		return trans::skip('\r', 0, ptr, ix, len);
	}
	if (ptr[ix] == '\n') {
		return trans::skip('\n', 0, ptr, ix, len);
	}
	if (ix < (LONGLONG)len && (ptr[ix] == ' ' || ptr[ix] == '\t')) {
		return trans::tostart(ptr, ix, len);
	}
	return ix;
}

LONGLONG trans::toend(unsigned char* ptr, LONGLONG ix, ULONGLONG len)
{
	for ( ; ix < (LONGLONG) len; ix++) {
		if (ptr[ix] == '\n') return ix;
		if (ptr[ix] == '\r') return ix;
	}

	return len;
}

LONGLONG trans::count(int ch, unsigned char* ptr, LONGLONG ix, ULONGLONG len)
{
	LONGLONG en = trans::skip(ch, 0, ptr, ix, len);

	return en - ix;
}

LONGLONG trans::search(int ch, unsigned char* ptr, LONGLONG ix, ULONGLONG len)
{
	for (; ix < (LONGLONG) len; ix++) {
		if (ptr[ix] == ch) return ix;
	}

	return len;
}

///////////////////////////////////////////////////////////////////////////////

bool trans::isMD(const TCHAR* filenm)
{
	int n = lstrlen(filenm);
	if (n > 3 && \
		(filenm[n - 1] == 'd' || filenm[n - 1] == 'D') && \
		(filenm[n - 2] == 'm' || filenm[n - 2] == 'M') && \
		(filenm[n - 3] == '.')) {
		return true;
	}
	return false;
}

TCHAR* trans::pathtitle(const TCHAR* buf)
{
	UINT n = lstrlen(buf);
	TCHAR* p = (TCHAR*) buf + n;

	for (p--; p > buf; p--) {
		if (*p == '\\' && p + 1 < buf + n) return p + 1;
	}

	return NULL;
}

TCHAR* trans::tempfile(TCHAR* buf, const TCHAR* old)
{
	if (buf == NULL) return NULL;
	if (old == NULL) old = _T("translated_markdown_document");
	if (!GetTempPath(MAX_PATH, buf)) return NULL;
	lstrcat(buf, _T("\\"));
	lstrcat(buf, old);
	lstrcat(buf, _T("(UNSAVED).html"));
	return buf;
}

void trans::trans_start(const CString& from, const CString& to)
{
	CFile input(from, CFile::modeRead);
	CFile output(to, CFile::modeCreate | CFile::modeWrite);

	if (input.m_hFile == input.hFileNull || output.m_hFile == output.hFileNull) {
	___trans_end:
		if (input.m_hFile != input.hFileNull) input.Close();
		if (output.m_hFile != output.hFileNull) output.Close();
		return;
	}

	LONGLONG off = 0L;
	ULONGLONG len = input.GetLength();

	unsigned char* buf = (unsigned char*)GlobalAlloc(GPTR, 4 * (len / 4 + 1));

	if (buf == NULL) goto ___trans_end;

	if (input.Read(buf, (UINT) len) <= 0) {
		GlobalFree(buf);
		goto ___trans_end;
	}

	////


	output.Write("<!doctype html><html><head>", 27);

	if (buf[0] == 0xef && buf[1] == 0xbb && buf[2] == 0xbf) {
		off += 3; 
		output.Write("<meta charset=\"UTF-8\">", 22);
	}

	output.Write("</head><body>", 13);

	off = trans::tostart(buf, off, len);

	std::cmatch mr;
	std::regex rx0("^[#]+( +|\t+)?.*( +|\t+)?([#]+)?$");
	std::regex rx1("^[ \t]+.*$");
	std::regex rx2("^[+*-][^+*-]( +|\t+)?.*$");
	std::regex rx3("^[0-9]+( +|\t+)?.*$");
	std::regex rx4("^[+*-]+$");
	std::regex rxz("^.*$");

	std::vector<char*> tBlkCode;
	std::vector<char*> tBlkList;
	std::vector<char*> tBlkLstN;

	tBlkCode.clear();
	tBlkList.clear();
	tBlkLstN.clear();

	for (; off < (LONGLONG) len; ) {
		
		/*header*/
		if (std::regex_match((const char*)(buf + off), (const char*)(buf + trans::toend(buf, off, len)), mr, rx0)) {
			trans::chk_blkcode(output, tBlkCode);
			trans::chk_blklist(output, tBlkList);
			trans::chk_blkolst(output, tBlkLstN);

			char suf[2][1024];

			strncpy_s(suf[0], mr.str().c_str(), sizeof(suf[0]));
			
			size_t sl = strlen(suf[0]); unsigned int lv = 0;
			char* pt = suf[0] + (lv = (unsigned int)trans::skip('#', 0, (unsigned char*)suf[0], 0, sl));
			for (unsigned char* p = (unsigned char*)suf[0] + sl - 1; p >= (unsigned char*)suf[0]; p--) {
				if (*p == '#') {
					*p = '\0';
				}
				else break;
			}
			snprintf(suf[1], sizeof(suf[1]), "<h%d>%s</h%d>", lv, pt, lv);

			output.Write(suf[1], (UINT)strlen(suf[1]));

		}

		/*list*/
		else if (std::regex_match((const char*)(buf + off), (const char*)(buf + trans::toend(buf, off, len)), mr, rx2)) {
			trans::chk_blkcode(output, tBlkCode);
			trans::chk_blkolst(output, tBlkLstN);

			char suf[1024], ch,* s;

			strncpy_s(suf, mr.str().c_str(), sizeof(suf));

			for (ch = suf[0], s = suf; *s == ch; s++) continue;

			size_t n = strlen(s);
			char* p = new char[n + 1];

			if (p != NULL) {
				lstrcpynA(p, s, (int)(n + 1)); p[n] = '\0';
				tBlkList.push_back(p);
			}
		}

		/*ordering list*/
		else if (std::regex_match((const char*)(buf + off), (const char*)(buf + trans::toend(buf, off, len)), mr, rx3)) {
			trans::chk_blkcode(output, tBlkCode);
			trans::chk_blklist(output, tBlkList);

			char suf[1024], *s;

			strncpy_s(suf, mr.str().c_str(), sizeof(suf));

			for (s = suf; *s >= '0' && *s <= '9'; s++) continue;

			size_t n = strlen(s);
			char* p = new char[n + 1];

			if (p != NULL) {
				lstrcpynA(p, s, (int)(n + 1)); p[n] = '\0';
				tBlkLstN.push_back(p);
			}
		}

		/*honrizontal rule*/
		else if (std::regex_match((const char*)(buf + off), (const char*)(buf + trans::toend(buf, off, len)), mr, rx4)) {
			trans::chk_blkcode(output, tBlkCode);
			trans::chk_blklist(output, tBlkList);
			trans::chk_blkolst(output, tBlkLstN);

			output.Write("<hr/>", 6);
		}

		/*block code*/
		else if (std::regex_match((const char*) (buf + off), (const char*) (buf + trans::toend(buf, off, len)), mr, rx1)) {
			trans::chk_blklist(output, tBlkList);
			trans::chk_blkolst(output, tBlkLstN);
			
			char suf[1024],* s;

			strncpy_s(suf, mr.str().c_str(), sizeof(suf));

			for (s = suf; *s == ' '; s++);

			size_t n = strlen(s);
			char* p = new char[n + 1];

			if (p != NULL) {
				lstrcpynA(p, s, (int) (n + 1)); p[n] = '\0';
				tBlkCode.push_back(p);
			}

		}

		/*context*/
		else if (std::regex_match((const char*)(buf + off), (const char*)(buf + toend(buf, off, len)), mr, rxz)) {
			trans::chk_blkcode(output, tBlkCode);
			trans::chk_blklist(output, tBlkList);
			trans::chk_blkolst(output, tBlkLstN);

			char suf[1024 * 4];
			
			strncpy_s(suf, mr.str().c_str(), sizeof(suf));

			if (strlen(suf) > 0) {
				output.Write("<p>", 4);
				output.Write(suf, (UINT)strlen(suf));
				output.Write("</p>", 5);
			}

		}
		
		off = trans::toend(buf, off, len);
		off = trans::tostart(buf, off, len);
	}

	output.Write("</body></html>", 14);

	int i;

	for (i = 0; i < tBlkCode.size(); i++) {
		delete[] tBlkCode[i];
	}

	for (i = 0; i < tBlkList.size(); i++) {
		delete[] tBlkList[i];
	}

	for (i = 0; i < tBlkLstN.size(); i++) {
		delete[] tBlkLstN[i];
	}

	tBlkCode.clear();
	tBlkList.clear();
	tBlkLstN.clear();


	GlobalFree(buf);
	output.Close();
	input.Close();
}

////
void trans::chk_blkcode(CFile & output, std::vector<char*>& tBlkCode)
{
	if (tBlkCode.size() > 0) {

		output.Write("<pre><code><font color=\"grey\">", 31);
		for (int i = 0; i < tBlkCode.size(); i++) {
			output.Write(tBlkCode[i], (UINT)strlen(tBlkCode[i]));
			output.Write("\n", 1);
			delete[] tBlkCode[i];
		}
		output.Write("</font></code></pre>", 21);
		tBlkCode.clear();

	}
}

void trans::chk_blklist(CFile& output, std::vector<char*>& tBlkList)
{
	if (tBlkList.size() > 0) {

		output.Write("<ul>", 5);
		for (int i = 0; i < tBlkList.size(); i++) {
			output.Write("<li>", 5);
			output.Write(tBlkList[i], (UINT)strlen(tBlkList[i]));
			output.Write("</li>", 6);
		}
		output.Write("</ul>", 6);
		tBlkList.clear();

	}
}

void trans::chk_blkolst(CFile& output, std::vector<char*>& tBlkLstN)
{
	if (tBlkLstN.size() > 0) {

		output.Write("<ol>", 5);
		for (int i = 0; i < tBlkLstN.size(); i++) {
			output.Write("<li>", 5);
			output.Write(tBlkLstN[i], (UINT)strlen(tBlkLstN[i]));
			output.Write("</li>", 6);
		}
		output.Write("</ol>", 6);
		tBlkLstN.clear();

	}
}

// end