#include "stdafx.h"
#include <iostream>
#include <locale>
#include <cwchar>

#include "Error.h"		//��������� ������
#include "Parm.h"		//��������� ����������
#include "Log.h"		//������ � ����������
#include "In.h"			//���� ��������� �����
/*
void _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	std::cout << " ----------���� In::getin    ---" << std::endl << std::endl;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		In::IN in = In::getin(parm.in, parm.out);
		std::cout << in.text << std::endl;
		std::cout << "����� ��������" << in.size <<std::endl;
		std::cout << "����� �����" << in.lines<<std::endl;
		std::cout << "����� ���������" << in.ignor << std::endl;
	}
	catch (Error::ERROR e)
	{
		std::cout << "������ " << e.id << ":" << e.message << std::endl << std::endl;
	}; 

	system("pause");
}*/