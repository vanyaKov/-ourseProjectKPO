#pragma once
#include "stdafx.h"
#include "Log.h"
#define LEXEMA_FIXSIZE	1			// ������������� ������ �������
#define LT_MAXSIZE		4096		// ������������ ���������� ����� � ������� ������
#define LT_TI_NULLIDX	0xfffffff	// ��� �������� ������� ���������������
#define LEX_UINT		't'	// ������� ��� unsigned integer
#define LEX_STRLEN		'o' // ������� ��� strlen
#define LEX_BOOL		'b'	// ������� ��� bool
#define LEX_CHAR		'a'	// ������� ��� char
#define LEX_TRANSLATE	'c'	// ������� ��� translate
#define LEX_CMP			'n'	// ������� ��� cmp
#define LEX_ENDL		'x'	// ������� ��� cmp
#define LEX_ID			'i'	// ������� ��� ��������������
#define LEX_LITERAL		'l'	// ������� ��� ��������
#define LEX_FUNCTION	'f'	// ������� ��� function
#define LEX_LET		    'd'	// ������� ��� let 
#define LEX_RESULT		'r'	// ������� ��� result
#define LEX_SHOW		's'	// ������� ��� show
#define LEX_MAIN		'm'	// ������� ��� main
#define LEX_IF		    'q' // ������� ��� if
#define LEX_ELSE		'e' // ������� ��� else
#define LEX_SEMICOLON	';'	// ������� ��� ;
#define LEX_COMMA		','	// ������� ��� ,
#define LEX_LEFTWALL	'['	// ������� ��� [
#define LEX_RIGHTWALL	']'	// ������� ��� [
#define LEX_LEFTBRACE	'{'	// ������� ��� {
#define LEX_BRACELET	'}'	// ������� ��� }
#define LEX_LEFTTHESIS	'('	// ������� ��� (
#define LEX_RIGHTTHESIS	')'	// ������� ��� )
#define LEX_CONVERT	    'h'	// ������� ��� touint
#define LEX_AND		    '&'	// ������� ��� &
#define LEX_OR		    '%'	// ������� ��� %
#define LEX_NOT		    '~'	// ������� ��� ~
#define LEX_PLUS		'+'	// ������� ��� +
#define LEX_SUB			'-'	// ������� ��� -
#define LEX_XOR		    '^'	// ������� ��� ^
#define LEX_OPERATOR	'v'	// ������� ��� ����������
#define LEX_EQUAL		'=' // ������� ��� =

namespace LT		// ������� ������
{
	struct Entry	// ������ ������� ������
	{
		unsigned char lexema;	// �������
		int sn;							// ����� ������ � �������� ������
		int idxTI;						// ������ � ������� ��������������� ��� LT_TI_NULLIDX
	};

	struct LexTable						// ��������� ������� ������
	{
		int maxsize;					// ������� ������� ������ < LT_MAXSIZE
		int size;						// ������� ������ ������� ������ < maxsize
		Entry* table;					// ������ ����� ������� ������
	};

	LexTable Create(		// ������� ������� ������
		int size			// ������� ������� ������ < LT_MAXSIZE
	);



	void Add(				// �������� ������ � ������� ������
		LexTable& lextable,	// ��������� ������� ������
		Entry entry			// ������ ������� ������
	);

	Entry GetEntry(			// �������� ������ ������� ������
		LexTable& lextable,	// ��������� ������� ������
		int n				// ����� ���������� ������
	);

	void Delete(LexTable& lextable);	// ������� ������� ������ (���������� ������)

	Entry writeEntry(					// ��������� ������ ������� ������
		Entry& entry,
		unsigned char lexema,
		int indx,
		int line
	);

	void showTable(LexTable lextable, Log::LOG& log);	// ����� ������� ������
};