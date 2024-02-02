#pragma once
#include "stdafx.h"
#include "Log.h"
#define LEXEMA_FIXSIZE	1			// фиксированный размер лексемы
#define LT_MAXSIZE		4096		// максимальное количество строк в таблице лексем
#define LT_TI_NULLIDX	0xfffffff	// нет элемента таблицы идентификаторов
#define LEX_UINT		't'	// лексема для unsigned integer
#define LEX_STRLEN		'o' // лексема для strlen
#define LEX_BOOL		'b'	// лексема для bool
#define LEX_CHAR		'a'	// лексема для char
#define LEX_TRANSLATE	'c'	// лексема для translate
#define LEX_CMP			'n'	// лексема для cmp
#define LEX_ENDL		'x'	// лексема для cmp
#define LEX_ID			'i'	// лексема для идентификатора
#define LEX_LITERAL		'l'	// лексема для литерала
#define LEX_FUNCTION	'f'	// лексема для function
#define LEX_LET		    'd'	// лексема для let 
#define LEX_RESULT		'r'	// лексема для result
#define LEX_SHOW		's'	// лексема для show
#define LEX_MAIN		'm'	// лексема для main
#define LEX_IF		    'q' // лексема для if
#define LEX_ELSE		'e' // лексема для else
#define LEX_SEMICOLON	';'	// лексема для ;
#define LEX_COMMA		','	// лексема для ,
#define LEX_LEFTWALL	'['	// лексема для [
#define LEX_RIGHTWALL	']'	// лексема для [
#define LEX_LEFTBRACE	'{'	// лексема для {
#define LEX_BRACELET	'}'	// лексема для }
#define LEX_LEFTTHESIS	'('	// лексема для (
#define LEX_RIGHTTHESIS	')'	// лексема для )
#define LEX_CONVERT	    'h'	// лексема для touint
#define LEX_AND		    '&'	// лексема для &
#define LEX_OR		    '%'	// лексема для %
#define LEX_NOT		    '~'	// лексема для ~
#define LEX_PLUS		'+'	// лексема для +
#define LEX_SUB			'-'	// лексема для -
#define LEX_XOR		    '^'	// лексема для ^
#define LEX_OPERATOR	'v'	// лексема для операторов
#define LEX_EQUAL		'=' // лексема для =

namespace LT		// таблица лексем
{
	struct Entry	// строка таблицы лексем
	{
		unsigned char lexema;	// лексема
		int sn;							// номер строки в исходном тексте
		int idxTI;						// индекс в таблице идентификаторов или LT_TI_NULLIDX
	};

	struct LexTable						// экземпляр таблицы лексем
	{
		int maxsize;					// емкость таблицы лексем < LT_MAXSIZE
		int size;						// текущий размер таблицы лексем < maxsize
		Entry* table;					// массив строк таблицы лексем
	};

	LexTable Create(		// создать таблицу лексем
		int size			// емкость таблицы лексем < LT_MAXSIZE
	);



	void Add(				// добавить строку в таблицу лексем
		LexTable& lextable,	// экземпляр таблицы лексем
		Entry entry			// строка таблицы лексем
	);

	Entry GetEntry(			// получить строку таблицы лексем
		LexTable& lextable,	// экземпляр таблицы лексем
		int n				// номер получаемой строки
	);

	void Delete(LexTable& lextable);	// удалить таблицу лексем (освободить память)

	Entry writeEntry(					// заполнить строку таблицы лексем
		Entry& entry,
		unsigned char lexema,
		int indx,
		int line
	);

	void showTable(LexTable lextable, Log::LOG& log);	// вывод таблицы лексем
};