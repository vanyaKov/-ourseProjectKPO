#pragma once
#include "Log.h"
#define ID_MAXSIZE		10			// ������������ ���������� �������� � ��������������
#define TI_MAXSIZE		4096		// ������������ ���������� ����� � ������� ���������������
#define TI_INT_DEFAULT	0x00000000	// �������� �� ��������� ��� ���� uint/bool
#define TI_STR_DEFAULT	0x00		// �������� �� ��������� ��� ���� string/char
#define TI_NULLIDX		0xffffffff	// ��� �������� ������� ���������������
#define TI_STR_MAXSIZE	255			// 

namespace IT			// ������� ���������������
{
	enum IDDATATYPE { UINT = 1, CHR = 2, BOOL = 3, STRING = 4 };			// ���� ������ ���������������: integer, string
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, OP = 5 };	// ���� ���������������: ����������, �������, ��������, �������, ��������

	struct Entry	// ������ ������� ���������������
	{
		int			idxfirstLE;			// ������ ������ ������ � ������� ������
		unsigned char	id[ID_MAXSIZE];		// �������������� (������������� ��������� �� ID_MAXSIZE)
		IDDATATYPE	iddatatype;			// ��� ������
		IDTYPE		idtype;				// ��� ��������������
		short system;

		union
		{
			unsigned int vint;					// �������� integer/bool
			struct 
			{
				int len;						// ���������� �������� � string
				unsigned char str[TI_STR_MAXSIZE - 1];	// ������� string
			} vstr;				// �������� string
		} value;		// �������� ��������������
	};

	struct IdTable				// ��������� ������� ���������������
	{
		int maxsize;			// ������� ������� ��������������� < TI_MAXSIZE
		int size;				// ������� ������ ������� ��������������� < maxsize
		Entry* table;			// ������ ����� ������� ���������������
	};

	IdTable Create(				// ������� ������� ���������������
		int size				// ������� ������� ��������������� < TI_MAXSIZE
	);

	void Add(				// �������� ������ � ������� ���������������
		IdTable& idtable,	// ��������� ������� ���������������
		Entry entry			// ������ ������� ���������������
	);

	Entry GetEntry(			// �������� ������ ������� ���������������
		IdTable& idtable,	// ��������� ������� ���������������
		int n				// ����� ���������� ������
	);

	int IsId(				// �������: ����� ������ (���� ����), TI_NULLIDX (���� ���)
		IdTable& idtable,	// ��������� ������� ���������������
		unsigned char id[ID_MAXSIZE]	// �������������
	);

	void Delete(IdTable& idtable);	// ������� ������� ������ (���������� ������)
	void showTable(IdTable& idtable, Log::LOG log);
	void newShowTable(IdTable& idtable);
};