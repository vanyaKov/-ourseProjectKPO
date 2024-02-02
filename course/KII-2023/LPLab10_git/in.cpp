#include "stdafx.h"

namespace In
{

	IN getin(wchar_t infile[], wchar_t outfile[])
	{
		int currentLine = 0, currentCol = 0;
		IN info{ 0, 0, 0, nullptr, IN_CODE_TABLE };					//uniform ������������� (������������ == ������ �������������)
		std::ifstream in(infile);									//��������� ���� ��� ������
		std::ofstream out(outfile);									//��������� ���� ��� ������
		if (!out.is_open())											//�������� �� �������� ����� ����� ������
			throw ERROR_THROW(110);
		if (!in.is_open())											//�������� �� �������� ����� ����� ������
			throw ERROR_THROW(110);
		std::string fulltext;
		std::string temp;

		while (!in.eof())											//���� ������ ����
		{
			info.lines++;											//������� ���-�� �����
			std::getline(in, temp);									//������ ������ � t�mp
			temp += "\n";											//���� ��������� ������
			fulltext += temp;
		}
		info.text = new unsigned char[fulltext.size()];			//�������� ������ ��� ������
		unsigned char * ptmp = info.text;							//��������� �� ������� ������ 
		for (int i = 0; i < fulltext.size(); ++i)
		{
			if (info.code[(unsigned char)fulltext[i]] == IN::F) //���� ������ �������� ����������� => ������
			{
				throw ERROR_THROW_IN(111, currentLine, currentCol)
			}
			else if (info.code[(unsigned char)fulltext[i]] == IN::I) // ���� ������ �������� ������������ 
				++info.ignor;
			else if (info.code[(unsigned char)fulltext[i]] == IN::T || info.code[(unsigned char)fulltext[i]] == IN::S)// ���� ����������� ������ 
			{
				*ptmp = fulltext[i];
				++ptmp;
			}
			else													//���� �� ������ ������ ,  ������ ������� ������������ ��������� �� 1
			{
				*ptmp = info.code[(unsigned char)fulltext[i]];
				++ptmp;
			}
			if (fulltext[i] == '\n')								//���� ����� ������, ������ ������� ������ �� 1 
			{
				++currentLine;
				currentCol = 0;
			}
			else
				++currentCol;
		}
		*ptmp = IN_CODE_ENDL;//������� ����� ������

		int size = 0;
		while (info.text[size] != '\0') {
			size++;
		}
		info.size = size;
		info.size = strlen((char*)info.text);			//���������� ������� = ���������� ���� ���� - \n
		out.close();
		in.close();
		return info;
	}
}