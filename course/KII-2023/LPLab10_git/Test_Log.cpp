#pragma comment(lib, "...\\Debug\\Lib.lib")
#include "stdafx.h" 
#include "CodeGeneration.h"
#include "semantics.h"
#include "../Lib/Lib.cpp"
int _tmain(int argc, _TCHAR ** argv)
{
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;
	Log::LOG new_lg = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		In::IN in = In::getin(parm.in, parm.out);
		Lex::LEX lextable = Lex::lexAnaliz(log, in);
		std::cout << "����������� ������ �������� ��� ������" << std::endl;
		LT::showTable(lextable.lextable, log);
		IT::showTable(lextable.idtable, log);
		MFST::Mfst mfst(lextable, GRB::getGreibach());
		if (mfst.start(log) && mfst.savededucation()) {
			mfst.printrules(log);
			std::cout << "C������������� ������ �������� ��� ������" << std::endl;
			semantics::semantics(lextable);
			std::cout << "������������� ������ �������� ��� ������" << std::endl;
			LT::LexTable str = LT::Create(LT_MAXSIZE);
			str = PN::PolishNotation(0, lextable.lextable, lextable.idtable);

			new_lg = Log::getlog(parm.out);
			CodeGeneration::CodeGeneration(lextable, new_lg);
			std::cout << "��������� ���� ��������� ��� ������" << std::endl;
			PN::showPolishNotation(str, log);
		}
		else
			std::cout << "�������������� ������ �� ��������" << std::endl;
		LT::showTable(lextable.lextable, log);
		Log::WriteIn(log, in);
		Log::Close(log);
	}
	catch (Error::ERROR e)
	{
		cout << e.id << ": ������ " << e.index.line << ", " << e.message << endl;
		Log::WriteError(log, e);
	}
	return 0;
}