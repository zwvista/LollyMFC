#include "StdAfx.h"
#include "UTF8.h"


CString EncodeToUTF8(LPCTSTR szSource)
{
	WORD ch;

	BYTE bt1, bt2, bt3, bt4, bt5, bt6;

	int n, nMax = _tcslen(szSource);

	CString sFinal, sTemp;

	for (n = 0; n < nMax; ++n)
	{
		ch = (WORD)szSource[n];

		if (ch == _T('='))
		{
			sTemp.Format(_T("%%%02X"), ch);

			sFinal += sTemp;
		}
		else if (ch < 128)
		{
			sFinal += szSource[n];
		}
		else if (ch <= 2047)
		{
			 bt1 = (BYTE)(192 + (ch / 64));
			 bt2 = (BYTE)(128 + (ch % 64));

			sTemp.Format(_T("%%%02X%%%02X"), bt1, bt2);
						
			sFinal += sTemp;
		}
		else if (ch <= 65535)
		{
			 bt1 = (BYTE)(224 + (ch / 4096));
			 bt2 = (BYTE)(128 + ((ch / 64) % 64));
			 bt3 = (BYTE)(128 + (ch % 64));

			sTemp.Format(_T("%%%02X%%%02X%%%02X"), bt1, bt2, bt3);
						
			sFinal += sTemp;
		}
		else if (ch <= 2097151)
		{
			 bt1 = (BYTE)(240 + (ch / 262144));
			 bt2 = (BYTE)(128 + ((ch / 4096) % 64));
			 bt3 = (BYTE)(128 + ((ch / 64) % 64));
			 bt4 = (BYTE)(128 + (ch % 64));

			sTemp.Format(_T("%%%02X%%%02X%%%02X%%%02X"), bt1, bt2, bt3, bt4);
			sFinal += sTemp;
		}
		else if (ch <=67108863)
		{
			bt1 = (BYTE)(248 + (ch / 16777216));
			bt2 = (BYTE)(128 + ((ch / 262144) % 64));
			bt3 = (BYTE)(128 + ((ch / 4096) % 64));
			bt4 = (BYTE)(128 + ((ch / 64) % 64));
			bt5 = (BYTE)(128 + (ch % 64));

			sTemp.Format(_T("%%%02X%%%02X%%%02X%%%02X%%%02X"), bt1, bt2, bt3, bt4, bt5);
			sFinal += sTemp;
		}
		else if (ch <=2147483647)
		{
			 bt1 = (BYTE)(252 + (ch / 1073741824));
			 bt2 = (BYTE)(128 + ((ch / 16777216) % 64));
			 bt3 = (BYTE)(128 + ((ch / 262144) % 64));
			 bt4 = (BYTE)(128 + ((ch / 4096) % 64));
			 bt5 = (BYTE)(128 + ((ch / 64) % 64));
			 bt6 = (BYTE)(128 + (ch % 64));

			sTemp.Format(_T("%%%02X%%%02X%%%02X%%%02X%%%02X%%%02X"), bt1, bt2, bt3, bt4, bt5, bt6);
			sFinal += sTemp;
		}

	}

	return sFinal;
}

BYTE MakeByte(TCHAR ch1, TCHAR ch2);


CString DecodeFromUTF8(LPCTSTR szSource)
{
	TRACE(_T("\n"));

	int n, nMax = _tcslen(szSource);
	WORD ch;

	CString sFinal, sTemp;

	BYTE z, y, x, w, v, u;
	
	for (n = 0; n < nMax; ++n)
	{
		ch = (WORD)szSource[n];

		if (ch != _T('='))
		{
			sFinal += (TCHAR)ch;
			continue;
		}

		if (n >= nMax - 2) break; // something is wrong
		z = MakeByte(szSource[n+1], szSource[n+2]);

		if (z < 127)
		{
			sFinal += (TCHAR)z;
			n = n + 2;
		}
		else if (z >= 192 && z <= 223)
		{
			// character is two bytes
			if (n >= nMax - 5) break; // something is wrong
			y = MakeByte(szSource[n+4], szSource[n+5]);
			sFinal += (TCHAR)( (z-192)*64 + (y-128) );
			n = n + 5;
		}
		else if (z >= 224 && z <= 239)
		{
			// character is three bytes
			if (n >= nMax - 8) break; // something is wrong
			y = MakeByte(szSource[n+4], szSource[n+5]);
			x = MakeByte(szSource[n+7], szSource[n+8]);
			sFinal += (TCHAR)( (z-224)*4096 + (y-128)*64 + (x-128) );
			n = n + 8;
		}
		else if (z >= 240 && z <= 247)
		{
			// character is four bytes
			if (n >= nMax - 11) break; // something is wrong
			y = MakeByte(szSource[n+4], szSource[n+5]);
			x = MakeByte(szSource[n+7], szSource[n+8]);
			w = MakeByte(szSource[n+10], szSource[n+11]);
			sFinal += (TCHAR)( (z-240)*262144 + (y-128)*4096 + (x-128)*64 + (w-128) );
			n = n + 11;
		}
		else if (z >= 248 && z <= 251)
		{
			// character is four bytes
			if (n >= nMax - 14) break; // something is wrong
			y = MakeByte(szSource[n+4], szSource[n+5]);
			x = MakeByte(szSource[n+7], szSource[n+8]);
			w = MakeByte(szSource[n+10], szSource[n+11]);
			v = MakeByte(szSource[n+13], szSource[n+14]);
			sFinal += (TCHAR)( (z-248)*16777216 + (y-128)*262144 + (x-128)*4096 + (w-128)*64 + (v-128) );
			n = n + 14;
		}
		else if (z >= 252 && z <= 253)
		{
			// character is four bytes
			if (n >= nMax - 17) break; // something is wrong
			y = MakeByte(szSource[n+4], szSource[n+5]);
			x = MakeByte(szSource[n+7], szSource[n+8]);
			w = MakeByte(szSource[n+10], szSource[n+11]);
			v = MakeByte(szSource[n+13], szSource[n+14]);
			u = MakeByte(szSource[n+16], szSource[n+17]);
			sFinal += (TCHAR)( (z-252)*1073741824 + (y-128)*16777216 + (x-128)*262144 + (w-128)*4096 + (v-128)*64 + (u-128) );
			n = n + 17;
		}
		
	}

	return sFinal;
}


// helper function for decoding
BYTE MakeByte(TCHAR ch1, TCHAR ch2)
{
	BYTE bt1 = 0, bt2 = 0;

	switch (ch2)
	{
		case _T('0'):
			bt2 = 0x00;
			break;
		case _T('1'):
			bt2 = 0x01;
			break;
		case _T('2'):
			bt2 = 0x02;
			break;
		case _T('3'):
			bt2 = 0x03;
			break;
		case _T('4'):
			bt2 = 0x04;
			break;
		case _T('5'):
			bt2 = 0x05;
			break;
		case _T('6'):
			bt2 = 0x06;
			break;
		case _T('7'):
			bt2 = 0x07;
			break;
		case _T('8'):
			bt2 = 0x08;
			break;
		case _T('9'):
			bt2 = 0x09;
			break;
		case _T('A'):
			bt2 = 0x0A;
			break;
		case _T('B'):
			bt2 = 0x0B;
			break;
		case _T('C'):
			bt2 = 0x0C;
			break;
		case _T('D'):
			bt2 = 0x0D;
			break;
		case _T('E'):
			bt2 = 0x0E;
			break;
		case _T('F'):
			bt2 = 0x0F;
			break;
	}

	switch (ch1)
	{
		case _T('0'):
			bt1 = 0x00;
			break;
		case _T('1'):
			bt1 = 0x10;
			break;
		case _T('2'):
			bt1 = 0x20;
			break;
		case _T('3'):
			bt1 = 0x30;
			break;
		case _T('4'):
			bt1 = 0x40;
			break;
		case _T('5'):
			bt1 = 0x50;
			break;
		case _T('6'):
			bt1 = 0x60;
			break;
		case _T('7'):
			bt1 = 0x70;
			break;
		case _T('8'):
			bt1 = 0x80;
			break;
		case _T('9'):
			bt1 = 0x90;
			break;
		case _T('A'):
			bt1 = 0xA0;
			break;
		case _T('B'):
			bt1 = 0xB0;
			break;
		case _T('C'):
			bt1 = 0xC0;
			break;
		case _T('D'):
			bt1 = 0xD0;
			break;
		case _T('E'):
			bt1 = 0xE0;
			break;
		case _T('F'):
			bt1 = 0xF0;
			break;
	}

	BYTE btFinal = bt2 | bt1;

	return  btFinal;	

}

