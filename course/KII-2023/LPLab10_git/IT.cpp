#include "stdafx.h"

namespace IT
{
	IdTable Create(int size)
	{
		if (size > TI_MAXSIZE)
			throw ERROR_THROW(160);
		IdTable Table;
		Table.maxsize = size;
		Table.size = 0;
		Table.table = new Entry[size];
		return Table;
	}


	void Add(IdTable& idtable, Entry entry)
	{
		if (idtable.size > idtable.maxsize) 
			throw ERROR_THROW(161);
		idtable.table[idtable.size] = entry;
		idtable.size++;
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		return idtable.table[n];
	}
	 
	int IsId(IdTable& idtable, unsigned char id[ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (_mbscmp(idtable.table[i].id, id) == 0)
				return i;
		}

		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
	}

	void showTable(IdTable& idtable, Log::LOG log)
	{
		int i, numberOP = 0;
		*(log.stream) << std::setfill('-') << std::setw(87) << '-' << std::endl;
		*(log.stream) << "   �" << " | " << "�������������" << " | " << "��� ������" << " | " << "��� ��������������" << " | " << "������ � ��" << " | " << "��������    " << std::endl;
		*(log.stream) << std::setw(87) << '-' << std::endl;
		for (i = 0; i < idtable.size; i++)
		{
			*(log.stream) << std::setfill('0') << std::setw(4) << std::right << i << " | ";
			*(log.stream) << std::setfill(' ') << std::setw(13) << std::left << idtable.table[i].id << " | ";
			switch (idtable.table[i].iddatatype)
			{
			case UINT: *(log.stream) << std::setw(10) << std::left;
				if (idtable.table[i].idtype == OP) *(log.stream) << "-" << " | ";
				else *(log.stream) << "uint" << " | "; break;
			case CHR: *(log.stream) << std::setw(10) << std::left;
				if (idtable.table[i].idtype == OP) *(log.stream) << "-" << " | ";
				else *(log.stream) << "char" << " | "; break;
			case STRING: *(log.stream) << std::setw(10) << std::left;
				if (idtable.table[i].idtype == OP) *(log.stream) << "-" << " | ";
				else *(log.stream) << "string" << " | "; break;
			case BOOL: *(log.stream) << std::setw(10) << std::left;
				if (idtable.table[i].idtype == OP) *(log.stream) << "-" << " | ";
				else *(log.stream) << "bool" << " | "; break;
			default: *(log.stream) << std::setw(10) << std::left << "������" << " | "; break;
			}
			switch (idtable.table[i].idtype)
			{
			case V: *(log.stream) << std::setw(18) << std::left << "����������" << " | "; break;
			case F: *(log.stream) << std::setw(18) << std::left << "�������" << " | "; break;
			case P: *(log.stream) << std::setw(18) << std::left << "��������" << " | "; break;
			case L: *(log.stream) << std::setw(18) << std::left << "�������" << " | "; break;
			case OP: *(log.stream) << std::setw(18) << std::left << "��������" << " | ";
				numberOP++;
				break;
			default: *(log.stream) << std::setw(18) << std::left << "������" << " | "; break;
			}
			*(log.stream) << std::setw(11) << std::left << idtable.table[i].idxfirstLE << " | ";
			if (idtable.table[i].iddatatype == UINT && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				*(log.stream) << std::setw(18) << std::left << idtable.table[i].value.vint;
			else if (idtable.table[i].iddatatype == BOOL && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				*(log.stream) << std::setw(18) << std::left << idtable.table[i].value.vint;
			else if (idtable.table[i].iddatatype == CHR && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				*(log.stream) << "[" << idtable.table[i].value.vstr.len << "]\"" << idtable.table[i].value.vstr.str << "\"";
			else if (idtable.table[i].iddatatype == STRING && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				*(log.stream) << "[" << idtable.table[i].value.vstr.len << "]\"" << idtable.table[i].value.vstr.str << "\"";
			else
				*(log.stream) << "-";
			*(log.stream) << std::endl;
		}
		*(log.stream) << std::setfill('-') << std::setw(87) << '-' << std::endl;
		*(log.stream) << "���������� ���������������: " << i - numberOP << std::endl;
		*(log.stream) << std::setw(87) << '-' << std::endl;
	}
}