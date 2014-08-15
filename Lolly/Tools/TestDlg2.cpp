#include "stdafx.h"
#include "Lolly.h"
#include "TestDlg.h"


//#define ssize_t size_t
//#include <eb/eb.h>
//#include <eb/error.h>
//#include <eb/text.h>
//#include <eb/appendix.h>
//
//#define MAXLEN_TEXT 1023
//
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif
//
//#pragma comment(lib, "eb")
//#pragma comment(lib, "intl")
//#pragma comment(lib, "zlib")
//
//#define EB_VERIFY(f) if(f != EB_SUCCESS) goto die;
//
//#define EUC_TO_ASCII_TABLE_START	0xa0
//#define EUC_TO_ASCII_TABLE_END		0xff
//
//static const unsigned char euc_a1_to_ascii_table[] = {
//    0x00, 0x20, 0x00, 0x00, 0x2c, 0x2e, 0x00, 0x3a,     /* 0xa0 */
//    0x3b, 0x3f, 0x21, 0x00, 0x00, 0x00, 0x60, 0x00,     /* 0xa8 */
//    0x5e, 0x7e, 0x5f, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0xb0 */
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x2f,     /* 0xb8 */
//    0x5c, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x27,     /* 0xc0 */
//    0x00, 0x22, 0x28, 0x29, 0x00, 0x00, 0x5b, 0x5d,     /* 0xc8 */
//    0x7b, 0x7d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0xd0 */
//    0x00, 0x00, 0x00, 0x00, 0x2b, 0x2d, 0x00, 0x00,     /* 0xd8 */
//    0x00, 0x3d, 0x00, 0x3c, 0x3e, 0x00, 0x00, 0x00,     /* 0xe0 */
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5c,     /* 0xe8 */
//    0x24, 0x00, 0x00, 0x25, 0x23, 0x26, 0x2a, 0x40,     /* 0xf0 */
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0xf8 */
//};
//
//static const unsigned char euc_a3_to_ascii_table[] = {
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0xa0 */
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0xa8 */
//    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,     /* 0xb0 */
//    0x38, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0xb8 */
//    0x00, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,     /* 0xc0 */
//    0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,     /* 0xc8 */
//    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,     /* 0xd0 */
//    0x58, 0x59, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0xd8 */
//    0x00, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,     /* 0xe0 */
//    0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,     /* 0xe8 */
//    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,     /* 0xf0 */
//    0x78, 0x79, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0xf8 */
//};
//
//CString strText;
//
///*
// * Hook which converts a character from EUC-JP to ASCII.
// */
//EB_Error_Code
//eb_hook_narrow_jisx0208(EB_Book *book, EB_Appendix *appendix, void *container,
//    EB_Hook_Code hook_code, int argc, const unsigned int *argv)
//{
//    int in_code1, in_code2;
//    int out_code = 0;
//	char str[3];
//
//    in_code1 = argv[0] >> 8;
//    in_code2 = argv[0] & 0xff;
//
//    if (in_code2 < EUC_TO_ASCII_TABLE_START
//	|| EUC_TO_ASCII_TABLE_END < in_code2) {
//	out_code = 0;
//    } else if (in_code1 == 0xa1) {
//	out_code = euc_a1_to_ascii_table[in_code2 - EUC_TO_ASCII_TABLE_START];
//    } else if (in_code1 == 0xa3) {
//	out_code = euc_a3_to_ascii_table[in_code2 - EUC_TO_ASCII_TABLE_START];
//    }
//
//    if (out_code == 0)
//	{
//		str[0] = in_code1;
//		str[1] = in_code2;
//		str[2] = '\0';
//	}
//    else
//	{
//		str[0] = out_code;
//		str[1] = '\0';
//	}
//	WCHAR wstr[MAXLEN_TEXT + 1];
//	::MultiByteToWideChar(20932, 0, str, -1, wstr, MAXLEN_TEXT);
//	strText += wstr;
//
//    return EB_SUCCESS;
//}
//
//EB_Error_Code
//eb_hook_wide_char(EB_Book *book, EB_Appendix *appendix, void *container,
//    EB_Hook_Code hook_code, int argc, const unsigned int *argv)
//{
//	char str[3];
//
//	str[0] = argv[0] >> 8;
//	str[1] = argv[0] & 0xff;
//	str[2] = '\0';
//	WCHAR wstr[MAXLEN_TEXT + 1];
//	::MultiByteToWideChar(hook_code == EB_HOOK_GB2312 ? 20936 : 20932, 0, str, -1, wstr, MAXLEN_TEXT);
//	strText += wstr;
//
//	return EB_SUCCESS;
//}
//
//EB_Error_Code
//eb_hook_other(EB_Book *book, EB_Appendix *appendix, void *container,
//			   EB_Hook_Code hook_code, int argc, const unsigned int *argv)
//{
//	CString str;
//	switch(hook_code){
//	case EB_HOOK_NEWLINE:
//		str = _T("\n");
//		break;
//	case EB_HOOK_BEGIN_KEYWORD:
//		str = _T("<HEAD>");
//		break;
//	case EB_HOOK_END_KEYWORD:
//		str = _T("</HEAD>");
//		break;
//	case EB_HOOK_SET_INDENT:
//		str.Format(_T("<INDENT=%02d>"), argv[1]);
//		break;
//	case EB_HOOK_BEGIN_REFERENCE:
//		str = _T("<LINK>");
//		break;
//	case EB_HOOK_END_REFERENCE:
//		str.Format(_T("</LINK[%04x:%04x]>"), argv[1], argv[2]);
//		break;
//	case EB_HOOK_BEGIN_EMPHASIS:
//		str = _T("<I>");
//		break;
//	case EB_HOOK_END_EMPHASIS:
//		str = _T("</I>");
//		break;
//	case EB_HOOK_BEGIN_DECORATION:
//		str = _T("<B>");
//		break;
//	case EB_HOOK_END_DECORATION:
//		str = _T("</B>");
//		break;
//	case EB_HOOK_BEGIN_WAVE:
//		str.Format(_T("<WAVE[%04x:%04x-%04x:%04x]>"), argv[2], argv[3], argv[4], argv[5]);
//		break;
//	case EB_HOOK_END_WAVE:
//		str = _T("</WAVE>");
//		break;
//	case EB_HOOK_BEGIN_NARROW:
//		str = _T("<N>");
//		break;
//	case EB_HOOK_END_NARROW:
//		str = _T("</N>");
//		break;
//	}
//	strText += str;
//	return EB_SUCCESS;
//}
//extern "C" {
//EB_Error_Code
//eb_narrow_alt_character_text(EB_Appendix *appendix, int character_number,
//							 char *text);
//EB_Error_Code
//eb_wide_alt_character_text(EB_Appendix *appendix, int character_number,
//							 char *text);
//}
//
//EB_Error_Code
//eb_hook_narrow_font(EB_Book *book, EB_Appendix *appendix,
//    void *container, EB_Hook_Code hook_code, int argc,
//    const unsigned int *argv)
//{
//    char alt_text[EB_MAX_ALTERNATION_TEXT_LENGTH + 1];
//
//    if (appendix == NULL
//	|| eb_narrow_alt_character_text(appendix, (int)argv[0], alt_text)
//	!= EB_SUCCESS) {
//	strText += _T("<?>");
//    } else {
//	strText += alt_text;
//    }
//
//    return EB_SUCCESS;
//}
//
//EB_Error_Code
//eb_hook_wide_font(EB_Book *book, EB_Appendix *appendix,
//    void *container, EB_Hook_Code hook_code, int argc,
//    const unsigned int *argv)
//{
//    char alt_text[EB_MAX_ALTERNATION_TEXT_LENGTH + 1];
//
//    if (appendix == NULL
//	|| eb_wide_alt_character_text(appendix, (int)argv[0], alt_text)
//	!= EB_SUCCESS) {
//	strText += _T("<?>");
//    } else {
//	strText += alt_text;
//    }
//
//    return EB_SUCCESS;
//}

void CTestDlg::OnBnClickedEbtest()
{
//	EB_Appendix app;
//	EB_Book book;
//	EB_Subbook_Code subbook_list[EB_MAX_SUBBOOKS];
//	int subbook_count;
//	int subbook_index;
//	EB_Position text_position;
//	char text[MAXLEN_TEXT + 1];
//	ssize_t text_length;
//	int text_count;
//	int i;
//	EB_Hookset hookset;
//	int alt_start;
//
//	text_count = 1;
//
//	/* EB ライブラリと `book' を初期化。*/
//	eb_initialize_library();
//	eb_initialize_appendix(&app);
//	eb_initialize_book(&book);
//	eb_initialize_hookset(&hookset);
//
//	static const EB_Hook hooks[] = {
//		{EB_HOOK_NARROW_FONT, eb_hook_narrow_font},
//		{EB_HOOK_WIDE_FONT,  eb_hook_wide_font},
//		{EB_HOOK_NARROW_JISX0208,eb_hook_narrow_jisx0208},
//		{EB_HOOK_WIDE_JISX0208,eb_hook_wide_char},
//		{EB_HOOK_GB2312,eb_hook_wide_char},
//		{EB_HOOK_NEWLINE,  eb_hook_other},
//		{EB_HOOK_BEGIN_KEYWORD,  eb_hook_other},
//		{EB_HOOK_END_KEYWORD,  eb_hook_other},
//		{EB_HOOK_SET_INDENT,  eb_hook_other},
//		{EB_HOOK_BEGIN_REFERENCE,  eb_hook_other},
//		{EB_HOOK_END_REFERENCE,  eb_hook_other},
//		{EB_HOOK_BEGIN_EMPHASIS,  eb_hook_other},
//		{EB_HOOK_END_EMPHASIS,  eb_hook_other},
//		{EB_HOOK_BEGIN_DECORATION,  eb_hook_other},
//		{EB_HOOK_END_DECORATION,  eb_hook_other},
//		{EB_HOOK_BEGIN_WAVE,  eb_hook_other},
//		{EB_HOOK_END_WAVE,  eb_hook_other},
//		{EB_HOOK_BEGIN_NARROW,  eb_hook_other},
//		{EB_HOOK_END_NARROW,  eb_hook_other},
//		{EB_HOOK_NULL,           NULL}
//	};
//	eb_set_hooks(&hookset, hooks);
//
//	/* 書籍を `book' に結び付ける。*/
//	EB_VERIFY(eb_bind(&book, "D:\\Software\\Learning\\ebwin\\MYDIC\\新辞林"));
//	EB_VERIFY(eb_bind_appendix(&app, "D:\\Software\\Learning\\ebwin\\MYDIC\\新辞林"));
//
//	/* 副本の一覧を取得。*/
//	EB_VERIFY(eb_subbook_list(&book, subbook_list, &subbook_count));
//	EB_VERIFY(eb_appendix_subbook_list(&app, subbook_list,&subbook_count));
//
//	/* 副本のインデックスを取得。*/
//	subbook_index = 0;
//
//	/*「現在の副本 (current subbook)」を設定。*/
//	EB_VERIFY(eb_set_subbook(&book, subbook_list[subbook_index]));
//	if(eb_set_appendix_subbook(&app, subbook_list[subbook_index]) < 0)
//		goto die;
//
//	/* テキストの開始位置を取得。*/
//	EB_VERIFY(eb_text(&book, &text_position));
//
//	/* テキストをシーク。*/
//	EB_VERIFY(eb_seek_text(&book, &text_position));
//
//	for (i = 0; i < text_count; i++) {
//		strText = _T("");
//
//		/* テキストを取得。*/
//		EB_VERIFY(eb_read_text(&book, &app, &hookset, NULL, MAXLEN_TEXT,
//			text, &text_length));
//
//		AfxMessageBox(strText);
//
//		if (!eb_is_text_stopped(&book))
//			continue;
//
//		/* 次の単語の説明へ移動。*/
//		EB_VERIFY(eb_forward_text(&book, NULL));
//		//if (error_code == EB_ERR_END_OF_CONTENT)
//		//	fputs("\n[END]\n", stdout);
//	}
//
//	/* 外字の開始位置を取得。*/
//	EB_VERIFY(eb_narrow_alt_start(&app, &alt_start));
//
//	i = alt_start;
//	for (;;) {
//		/* 外字の代替文字列を取得。*/
//		EB_VERIFY(eb_narrow_alt_character_text(&app, i, text));
//
//		/* 取得した代替文字列を出力。*/
//		strText.Format(_T("%04x: %s\n"), i, text);
//		AfxMessageBox(strText);
//
//		/* 外字の文字番号を一つ進める。*/
//		if (eb_forward_narrow_alt_character(&app, 1, &i) != EB_SUCCESS)
//			break;
//	}
//
//
//	/* 書籍と EB ライブラリの利用を終了。*/
//	/* エラー発生で終了するときの処理。*/
//die:
//	eb_finalize_hookset(&hookset);
//	eb_finalize_book(&book);
//	eb_finalize_appendix(&app);
//	eb_finalize_library();
}
