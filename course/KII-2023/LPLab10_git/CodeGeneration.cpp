#include "stdafx.h"
#include "CodeGeneration.h"
#include <stack>
#include < string.h > 

namespace CodeGeneration
{
	void CodeGeneration(Lex::LEX lex, Log::LOG log)
	{
		int k = 0, ifelse = 0; //счЄтчики
		bool eax = false;
		stack<LT::Entry> st;
		LT::LexTable str = LT::Create(LT_MAXSIZE);
		LT::LexTable tmp = LT::Create(LT_MAXSIZE);
		*(log.stream) << ".586\n.model flat, stdcall\n\nincludelib libucrt.lib\nincludelib kernel32.lib\nincludelib Lib.lib\n\nShowInt PROTO : DWORD, : DWORD, : DWORD\nShowChar PROTO : DWORD, : DWORD\nCharToInt PROTO : DWORD, : DWORD\nShowString PROTO : DWORD, : DWORD, : DWORD\nShowBool PROTO : DWORD, : DWORD\ninv PROTO : DWORD\nStringToInt PROTO : DWORD, : DWORD\ntranslate PROTO : DWORD, : DWORD, : DWORD\ncmpstr PROTO : DWORD, : DWORD\ncmpuint PROTO : DWORD, : DWORD\nExitProcess PROTO : DWORD\nstop PROTO\n"; //дефолт дл€ начала
		for (int i = 0; i < lex.idtable.size; i++) {
			if (lex.idtable.table[i].idtype == IT::F) { //создаю прототипы функций
				*(log.stream) << "func" << lex.idtable.table[i].id << " PROTO";
				k = ++i;
				while (lex.idtable.table[k].idtype == IT::P) { //провер€ю на параметр
					if (lex.idtable.table[k + 1].idtype == IT::P) {//если не последний параметр
						if (lex.idtable.table[i].iddatatype == IT::UINT)  //провер€ю тип параметра
							*(log.stream) << ":DWORD,";
						else if (lex.idtable.table[i].iddatatype == IT::BOOL)
							*(log.stream) << ":DWORD,";
						else if (lex.idtable.table[i].iddatatype == IT::CHR)
							*(log.stream) << ":DWORD,";
						else if (lex.idtable.table[i].iddatatype == IT::STRING)
							*(log.stream) << ":DWORD,";
					}
					else { //если параметр последний
						if (lex.idtable.table[i].iddatatype == IT::UINT) //провер€ю тип параметра
							*(log.stream) << " :DWORD ";
						else if (lex.idtable.table[i].iddatatype == IT::BOOL)
							*(log.stream) << " :DWORD ";
						else if (lex.idtable.table[i].iddatatype == IT::CHR)
							*(log.stream) << " :DWORD ";
						else if (lex.idtable.table[i].iddatatype == IT::STRING)
							*(log.stream) << " :DWORD ";
					}
					k++;
				}
				*(log.stream) << '\n';
			}
		}
		*(log.stream) << "\n.stack 4096\n";
		*(log.stream) << "\n.const\n";
		for (int i = 0; i < lex.idtable.size; i++) { //делаю литералы константами
			if ((lex.idtable.table[i].iddatatype == IT::UINT || lex.idtable.table[i].iddatatype == IT::BOOL) && lex.idtable.table[i].idtype == IT::L) { //литерал типа uint
				*(log.stream) << lex.idtable.table[i].id << " " << "DWORD" << " " << lex.idtable.table[i].value.vint << "\n"; //выдел€ю 4 байта
				*(log.stream) << "system" << lex.idtable.table[i].id << " DWORD " << lex.idtable.table[i].system << "\n";
			}
			else if (lex.idtable.table[i].iddatatype == IT::STRING && lex.idtable.table[i].idtype == IT::L) { //литерал типа char
				*(log.stream) << lex.idtable.table[i].id << " " << "DWORD "; // выдел€ю 4 байта
				if (lex.idtable.table[i].value.vstr.len != 1) {
					for (int j = 0; j < lex.idtable.table[i].value.vstr.len - 1; j++) {
						*(log.stream) << "'" << lex.idtable.table[i].value.vstr.str[j] << "',";
					}
					*(log.stream) << "'" << lex.idtable.table[i].value.vstr.str[lex.idtable.table[i].value.vstr.len - 1] << "'" << "\n";
				}
				else
					*(log.stream) << "'" << lex.idtable.table[i].value.vstr.str[lex.idtable.table[i].value.vstr.len - 1] << "'" << "\n";
			}
		}
		*(log.stream) << "\n.data\n"; //переменный и параметры вношу в .data
		for (int i = 0; i < lex.idtable.size; i++) {
			if (lex.idtable.table[i].idtype == IT::V) {//переменна€ типа uint
				*(log.stream) << lex.idtable.table[i].id << " DWORD 0\n"; //выдел€ю 4 байта
				if (lex.idtable.table[i].iddatatype == IT::UINT)
					*(log.stream) << "system" << lex.idtable.table[i].id << " " << "DWORD 10\n";
			}
			else if (lex.idtable.table[i].idtype == IT::P) {//переменна€ типа uint
				if (lex.idtable.table[i].iddatatype == IT::UINT)
					*(log.stream) << "system" << lex.idtable.table[i].id << " " << "DWORD 10\n";
			}
		}
		*(log.stream) << "\n.CODE\n";
		for (int i = 0; i < lex.idtable.size; i++) { //реализую функции
			if (lex.idtable.table[i].idtype == IT::F) { //если функци€
				IT::Entry buff = lex.idtable.table[i];
				*(log.stream) << "func" << lex.idtable.table[i].id << " PROC ";
				k = ++i;
				while (lex.idtable.table[k].idtype == IT::P) {
					if (lex.idtable.table[k + 1].idtype == IT::P)
						if (lex.idtable.table[i].iddatatype == IT::UINT)
							*(log.stream) << lex.idtable.table[k].id << ":DWORD,";
						else if (lex.idtable.table[i].iddatatype == IT::BOOL)
							*(log.stream) << lex.idtable.table[k].id << ":DWORD,";
						else
							*(log.stream) << lex.idtable.table[k].id << ":DWORD,";
					else {
						if (lex.idtable.table[i].iddatatype == IT::UINT)
							*(log.stream) << lex.idtable.table[k].id << " :DWORD ";
						else if (lex.idtable.table[i].iddatatype == IT::BOOL)
							*(log.stream) << lex.idtable.table[k].id << " :DWORD ";
						else
							*(log.stream) << lex.idtable.table[k].id << ":DWORD";
					}
					k++;
				}
				*(log.stream) << '\n';
				k = buff.idxfirstLE;
				int q;
				bool bodyif = false, bodyelse = false;
				bool in = true;
				while (lex.lextable.table[k].lexema != LEX_BRACELET) {
					if (lex.lextable.table[k].lexema == LEX_IF) {
						k += 2;
						bodyif = true;
						*(log.stream) << "mov eax ," << lex.idtable.table[lex.lextable.table[k].idxTI].id << "\n";
						*(log.stream) << "cmp eax,0\n";
						*(log.stream) << "je else" << ifelse << "\n";
					}
					else if (lex.lextable.table[k].lexema == LEX_TRANSLATE) {
						while (lex.lextable.table[k].lexema != LEX_SEMICOLON) {
							if (lex.lextable.table[k].lexema == LEX_ID || lex.lextable.table[k].lexema == LEX_LITERAL) {
								*(log.stream) << "push " << lex.idtable.table[lex.lextable.table[k].idxTI].id << "\n";
								if (in) {
									*(log.stream) << "push system" << lex.idtable.table[lex.lextable.table[k].idxTI].id << "\n";
									q = k;
									in = false;
								}
							}
							k++;
						}
						*(log.stream) << "call translate\n";
						*(log.stream) << "mov system" << lex.idtable.table[lex.lextable.table[q].idxTI].id << ",eax\n";
						in = true;
					}
					else if (lex.lextable.table[k].lexema == LEX_RIGHTWALL) {
						if (bodyif) {
							*(log.stream) << "call  go" << ifelse << "\n";
							bodyif = false;
						}
						else {
							*(log.stream) << "go" << ifelse << ":" << "\n";
							bodyelse = false;
							ifelse++;
						}
					}
					else if (lex.lextable.table[k].lexema == LEX_EQUAL) {
						str = PN::PolishNotation(k + 1, lex.lextable, lex.idtable);
						for (int i = 0; i < str.size; i++)
							st.push(str.table[i]);
						for (int i = 0; i < str.size; i++) {
							str.table[i] = st.top();
							st.pop();
						}
						if (lex.lextable.table[k + 1].lexema == LEX_CONVERT) {
							if (lex.idtable.table[lex.lextable.table[k + 2].idxTI].value.vstr.len == 1 || (lex.idtable.table[lex.lextable.table[k + 2].idxTI].iddatatype == IT::CHR)) {
								*(log.stream) << "push 1" << "\n";
								*(log.stream) << "push " << lex.idtable.table[lex.lextable.table[k + 2].idxTI].id << "\n";
								*(log.stream) << "call CharToInt" << "\n";
								*(log.stream) << "cmp eax, -1" << "\n";
								*(log.stream) << "je exit" << "\n";
							}
							else {
								*(log.stream) << "mov esi, offset " << lex.idtable.table[lex.lextable.table[k + 2].idxTI].id << "\n";
								*(log.stream) << "mov ecx, LENGTHOF " << lex.idtable.table[lex.lextable.table[k + 2].idxTI].id << "\n";
								*(log.stream) << "mov ebx,ecx" << "\n";
								*(log.stream) << "push ebx" << "\n";
								*(log.stream) << "push esi" << "\n";
								*(log.stream) << "call StringToInt" << "\n";
								*(log.stream) << "cmp eax, -1" << "\n";
								*(log.stream) << "je exit" << "\n";
							}
						}
						else if (lex.lextable.table[k + 1].lexema == LEX_CMP) {
							int o = k;
							while (lex.lextable.table[o].lexema != LEX_SEMICOLON) {
								if (lex.lextable.table[o].lexema == LEX_ID || lex.lextable.table[o].lexema == LEX_LITERAL) {
									if (lex.idtable.table[lex.lextable.table[o].idxTI].iddatatype == IT::UINT) {
										*(log.stream) << "push " << lex.idtable.table[lex.lextable.table[o].idxTI].id << "\n";
										*(log.stream) << "push " << lex.idtable.table[lex.lextable.table[o + 2].idxTI].id << "\n";
										*(log.stream) << "call cmpuint" << "\n";
										*(log.stream) << "push eax\n";
										break;
									}
									else {
										*(log.stream) << "mov esi, offset " << lex.idtable.table[lex.lextable.table[o].idxTI].id << "\n";
										*(log.stream) << "push esi" << "\n";
										*(log.stream) << "mov esi, offset " << lex.idtable.table[lex.lextable.table[o + 2].idxTI].id << "\n";
										*(log.stream) << "push esi" << "\n";
										*(log.stream) << "call cmpstr" << "\n";
										*(log.stream) << "push eax\n";
										break;
									}
								}
								o++;
							}
						}
						else
							for (int i = 0; i < str.size; i++) {
								if (str.table[i].lexema == LEX_ID) {
									if (lex.idtable.table[str.table[i].idxTI].idtype == IT::F) {
										st.push(str.table[i]);
										i++;
										while (str.table[i].lexema != '@') {
											*(log.stream) << "push " << lex.idtable.table[str.table[i].idxTI].id << "\n";
											i++;
										}
										i++;
										*(log.stream) << "call func" << lex.idtable.table[st.top().idxTI].id << "\n";
										*(log.stream) << "push eax\n";
										st.pop();
									}
									else {
										*(log.stream) << "push " << lex.idtable.table[str.table[i].idxTI].id << "\n";
									}
								}
								else if (str.table[i].lexema == LEX_LITERAL) {
									*(log.stream) << "push " << lex.idtable.table[str.table[i].idxTI].id << "\n";
								}
								else if (str.table[i].lexema == LEX_OPERATOR || str.table[i].lexema == LEX_NOT) {
									if (lex.idtable.table[str.table[i].idxTI].id[0] == LEX_OR) {
										*(log.stream) << "pop eax\n";
										*(log.stream) << "pop ebx\n";
										*(log.stream) << "or eax,ebx\n";
										*(log.stream) << "push eax\n";
									}
									else if (lex.idtable.table[str.table[i].idxTI].id[0] == LEX_AND) {
										*(log.stream) << "pop eax\n";
										*(log.stream) << "pop ebx\n";
										*(log.stream) << "and eax,ebx\n";
										*(log.stream) << "push eax\n";
									}
									else if (lex.idtable.table[str.table[i].idxTI].id[0] == LEX_XOR) {
										*(log.stream) << "pop eax\n";
										*(log.stream) << "pop ebx\n";
										*(log.stream) << "xor eax,ebx\n";
										*(log.stream) << "push eax\n";
									}
									else if (lex.idtable.table[str.table[i].idxTI].id[0] == LEX_PLUS) {
										*(log.stream) << "pop eax\n";
										*(log.stream) << "add eax,1\n";
										*(log.stream) << "push eax\n";
									}
									else if (lex.idtable.table[str.table[i].idxTI].id[0] == LEX_SUB) {
										*(log.stream) << "pop eax\n";
										*(log.stream) << "sub eax,1\n";
										*(log.stream) << "cmp eax, -1" << "\n";
										*(log.stream) << "je exit" << "\n";
										*(log.stream) << "push eax\n";
									}
									else if (lex.idtable.table[str.table[i].idxTI].id[0] == LEX_NOT) {
										*(log.stream) << "call inv\n";
										*(log.stream) << "push eax\n";
									}
								}
							}
						if (lex.lextable.table[k + 1].lexema != LEX_CONVERT)
							*(log.stream) << "pop eax\n";
						*(log.stream) << "mov " << lex.idtable.table[lex.lextable.table[k - 1].idxTI].id << ",eax\n";
					}
					else if (lex.lextable.table[k].lexema == LEX_ELSE) {
						*(log.stream) << "else" << ifelse << ":" << "\n";
						bodyelse = true;
					}
					else if (lex.lextable.table[k].lexema == LEX_SHOW) {
						int h = k + 1;
						bool endl = false;											//флаг дл€ endl
						while (lex.lextable.table[h].lexema != LEX_SEMICOLON) {
							if (lex.lextable.table[h].lexema == LEX_ENDL) {			//провер€ю , есть ли endl
								endl = true;
								break;
							}
							h++;
						}
						if (lex.idtable.table[lex.lextable.table[k + 1].idxTI].iddatatype == IT::UINT) {
							*(log.stream) << "push system" << lex.idtable.table[lex.lextable.table[k + 1].idxTI].id << "\n";
							if (endl)
								*(log.stream) << "push " << 1 << "\n";
							else
								*(log.stream) << "push " << 0 << "\n";
							*(log.stream) << "push " << lex.idtable.table[lex.lextable.table[k + 1].idxTI].id << "\n";
							*(log.stream) << "call" << " ShowInt" << "\n";
						}
						else if (lex.idtable.table[lex.lextable.table[k + 1].idxTI].iddatatype == IT::BOOL) {
							if (endl)
								*(log.stream) << "push " << 1 << "\n";
							else
								*(log.stream) << "push " << 0 << "\n";
							*(log.stream) << "push " << lex.idtable.table[lex.lextable.table[k + 1].idxTI].id << "\n";
							*(log.stream) << "call" << " ShowBool" << "\n";
						}
						else if (lex.idtable.table[lex.lextable.table[k + 1].idxTI].iddatatype == IT::CHR) {
							if (endl)
								*(log.stream) << "push " << 1 << "\n";
							else
								*(log.stream) << "push " << 0 << "\n";
							*(log.stream) << "push " << lex.idtable.table[lex.lextable.table[k + 1].idxTI].id << "\n";
							*(log.stream) << "call" << " ShowChar" << "\n";
						}
						else if (lex.idtable.table[lex.lextable.table[k + 1].idxTI].iddatatype == IT::STRING) {
							*(log.stream) << "mov esi, offset " << lex.idtable.table[lex.lextable.table[k + 1].idxTI].id << "\n";
							*(log.stream) << "mov ecx, LENGTHOF " << lex.idtable.table[lex.lextable.table[k + 1].idxTI].id << "\n";
							*(log.stream) << "mov ebx,ecx" << "\n";
							if (endl)
								*(log.stream) << "push " << 1 << "\n";
							else
								*(log.stream) << "push " << 0 << "\n";
							*(log.stream) << "push ebx" << "\n";
							*(log.stream) << "push esi" << "\n";
							*(log.stream) << "call" << " ShowString" << "\n";
						}
					}
					else if (lex.lextable.table[k].lexema == LEX_RESULT) {
						tmp = PN::PolishNotation(k + 1, lex.lextable, lex.idtable);
						*(log.stream) << "push " << lex.idtable.table[tmp.table[0].idxTI].id << "\n";
						*(log.stream) << "pop eax\n";
						*(log.stream) << "RET\n";
					}
					k++;
				}
				*(log.stream) << "func" << buff.id << " ENDP\n\n"; //конец функции
			}
		}
		for (int i = 0; i < lex.lextable.size; i++) {
			if (lex.lextable.table[i].lexema == LEX_MAIN) {
				*(log.stream) << "MAIN:\n";
				int k = i;
				bool bodyif = false, bodyelse = false;
				bool in = true;
				int q;
				while (lex.lextable.table[k].lexema != LEX_BRACELET) {
					if (lex.lextable.table[k].lexema == LEX_IF) {
						k += 2;
						bodyif = true;
						*(log.stream) << "mov eax ," << lex.idtable.table[lex.lextable.table[k].idxTI].id << "\n";
						*(log.stream) << "cmp eax,0\n";
						*(log.stream) << "je else" << ifelse << "\n";
					}
					else if (lex.lextable.table[k].lexema == LEX_RIGHTWALL) {
						if (bodyif) {
							*(log.stream) << "call" << " " << " go" << ifelse << "\n";
							bodyif = false;
						}
						else {
							*(log.stream) << "go" << ifelse << ":" << "\n";
							bodyelse = false;
							ifelse++;
						}
					}
					else if (lex.lextable.table[k].lexema == LEX_EQUAL) {
						str = PN::PolishNotation(k + 1, lex.lextable, lex.idtable);
						for (int i = 0; i < str.size; i++)
							st.push(str.table[i]);
						for (int i = 0; i < str.size; i++) {
							str.table[i] = st.top();
							st.pop();
						}
						if (lex.lextable.table[k + 1].lexema == LEX_CONVERT) {
							if (lex.idtable.table[lex.lextable.table[k + 2].idxTI].value.vstr.len == 1) {
								*(log.stream) << "push 1" << "\n";
								*(log.stream) << "push " << lex.idtable.table[lex.lextable.table[k + 2].idxTI].id << "\n";
								*(log.stream) << "call CharToInt" << "\n";
								*(log.stream) << "cmp eax, -1" << "\n";
								*(log.stream) << "je exit" << "\n";
							}
							else {
								*(log.stream) << "mov esi, offset " << lex.idtable.table[lex.lextable.table[k + 2].idxTI].id << "\n";
								*(log.stream) << "mov ecx, LENGTHOF " << lex.idtable.table[lex.lextable.table[k + 2].idxTI].id << "\n";
								*(log.stream) << "mov ebx,ecx" << "\n";
								*(log.stream) << "push ebx" << "\n";
								*(log.stream) << "push esi" << "\n";
								*(log.stream) << "call StringToInt" << "\n";
								*(log.stream) << "cmp eax, -1" << "\n";
								*(log.stream) << "je exit" << "\n";
							}
						}
						else if (lex.lextable.table[k + 1].lexema == LEX_CMP) {
							int o = k;
							while (lex.lextable.table[o].lexema != LEX_SEMICOLON) {
								if (lex.lextable.table[o].lexema == LEX_ID || lex.lextable.table[o].lexema == LEX_LITERAL) {
									if (lex.idtable.table[lex.lextable.table[o].idxTI].iddatatype == IT::UINT) {
										*(log.stream) << "push " << lex.idtable.table[lex.lextable.table[o].idxTI].id << "\n";
										*(log.stream) << "push " << lex.idtable.table[lex.lextable.table[o + 2].idxTI].id << "\n";
										*(log.stream) << "call cmpuint" << "\n";
										*(log.stream) << "push eax\n";
										break;
									}
									else {
										*(log.stream) << "mov esi, offset " << lex.idtable.table[lex.lextable.table[o].idxTI].id << "\n";
										*(log.stream) << "push esi" << "\n";
										*(log.stream) << "mov esi, offset " << lex.idtable.table[lex.lextable.table[o + 2].idxTI].id << "\n";
										*(log.stream) << "push esi" << "\n";
										*(log.stream) << "call cmpstr" << "\n";
										*(log.stream) << "push eax\n";
										break;
									}
								}
								o++;
							}
						}
						else
							for (int i = 0; i < str.size; i++) {
								if (str.table[i].lexema == LEX_ID) {
									if (lex.idtable.table[str.table[i].idxTI].idtype == IT::F) {
										st.push(str.table[i]);
										i++;
										while (str.table[i].lexema != '@') {
											*(log.stream) << "push " << lex.idtable.table[str.table[i].idxTI].id << "\n";
											i++;
										}
										*(log.stream) << "call func" << lex.idtable.table[st.top().idxTI].id << "\n";
										*(log.stream) << "push eax\n";
									}
									else {
										*(log.stream) << "push " << lex.idtable.table[str.table[i].idxTI].id << "\n";
									}
								}
								else if (str.table[i].lexema == LEX_LITERAL) {
									*(log.stream) << "push " << lex.idtable.table[str.table[i].idxTI].id << "\n";
								}
								else if (str.table[i].lexema == LEX_OPERATOR || str.table[i].lexema == LEX_NOT) {
									if (lex.idtable.table[str.table[i].idxTI].id[0] == LEX_OR) {
										*(log.stream) << "pop eax\n";
										*(log.stream) << "pop ebx\n";
										*(log.stream) << "or eax,ebx\n";
										*(log.stream) << "push eax\n";
									}
									else if (lex.idtable.table[str.table[i].idxTI].id[0] == LEX_AND) {
										*(log.stream) << "pop eax\n";
										*(log.stream) << "pop ebx\n";
										*(log.stream) << "and eax,ebx\n";
										*(log.stream) << "push eax\n";
									}
									else if (lex.idtable.table[str.table[i].idxTI].id[0] == LEX_XOR) {
										*(log.stream) << "pop eax\n";
										*(log.stream) << "pop ebx\n";
										*(log.stream) << "xor eax,ebx\n";
										*(log.stream) << "push eax\n";
									}
									else if (lex.idtable.table[str.table[i].idxTI].id[0] == LEX_PLUS) {
										*(log.stream) << "pop eax\n";
										*(log.stream) << "add eax,1\n";
										*(log.stream) << "push eax\n";
									}
									else if (lex.idtable.table[str.table[i].idxTI].id[0] == LEX_SUB) {
										*(log.stream) << "pop eax\n";
										*(log.stream) << "sub eax,1\n";
										*(log.stream) << "cmp eax, -1" << "\n";
										*(log.stream) << "je exit" << "\n";
										*(log.stream) << "push eax\n";
									}
									else if (lex.idtable.table[str.table[i].idxTI].id[0] == LEX_NOT) {
										*(log.stream) << "call inv\n";
										*(log.stream) << "push eax\n";
									}
								}
							}
						if (lex.lextable.table[k + 1].lexema != LEX_CONVERT)
							*(log.stream) << "pop eax\n";
						*(log.stream) << "mov " << lex.idtable.table[lex.lextable.table[k - 1].idxTI].id << ",eax\n";
					}
					else if (lex.lextable.table[k].lexema == LEX_ELSE) {
						*(log.stream) << "else" << ifelse << ":" << "\n";
						bodyelse = true;
					}
					else if (lex.lextable.table[k].lexema == LEX_SHOW) {
						int h = k + 1;
						bool endl = false;											//флаг дл€ endl
						while (lex.lextable.table[h].lexema != LEX_SEMICOLON) {
							if (lex.lextable.table[h].lexema == LEX_ENDL) {			//провер€ю , есть ли endl
								endl = true;
								break;
							}
							h++;
						}
						if (lex.idtable.table[lex.lextable.table[k + 1].idxTI].iddatatype == IT::UINT) {
							*(log.stream) << "push system" << lex.idtable.table[lex.lextable.table[k + 1].idxTI].id << "\n";
							if (endl)
								*(log.stream) << "push " << 1 << "\n";
							else
								*(log.stream) << "push " << 0 << "\n";
							*(log.stream) << "push " << lex.idtable.table[lex.lextable.table[k + 1].idxTI].id << "\n";
							*(log.stream) << "call" << " ShowInt" << "\n";
						}
						else if (lex.idtable.table[lex.lextable.table[k + 1].idxTI].iddatatype == IT::BOOL) {
							if (endl)
								*(log.stream) << "push " << 1 << "\n";
							else
								*(log.stream) << "push " << 0 << "\n";
							*(log.stream) << "push " << lex.idtable.table[lex.lextable.table[k + 1].idxTI].id << "\n";
							*(log.stream) << "call" << " ShowBool" << "\n";
						}
						else if (lex.idtable.table[lex.lextable.table[k + 1].idxTI].iddatatype == IT::CHR) {
							if (endl)
								*(log.stream) << "push " << 1 << "\n";
							else
								*(log.stream) << "push " << 0 << "\n";
							*(log.stream) << "push " << lex.idtable.table[lex.lextable.table[k + 1].idxTI].id << "\n";
							*(log.stream) << "call" << " ShowChar" << "\n";
						}
						else if (lex.idtable.table[lex.lextable.table[k + 1].idxTI].iddatatype == IT::STRING) {
							*(log.stream) << "mov esi, offset " << lex.idtable.table[lex.lextable.table[k + 1].idxTI].id << "\n";
							*(log.stream) << "mov ecx, LENGTHOF " << lex.idtable.table[lex.lextable.table[k + 1].idxTI].id << "\n";
							*(log.stream) << "mov ebx,ecx" << "\n";
							if (endl)
								*(log.stream) << "push " << 1 << "\n";
							else
								*(log.stream) << "push " << 0 << "\n";
							*(log.stream) << "push ebx" << "\n";
							*(log.stream) << "push esi" << "\n";
							*(log.stream) << "call" << " ShowString" << "\n";
						}
					}
					else if (lex.lextable.table[k].lexema == LEX_TRANSLATE) {
						while (lex.lextable.table[k].lexema != LEX_SEMICOLON) {
							if (lex.lextable.table[k].lexema == LEX_ID || lex.lextable.table[k].lexema == LEX_LITERAL) {
								*(log.stream) << "push " << lex.idtable.table[lex.lextable.table[k].idxTI].id << "\n";
								if (in) {
									*(log.stream) << "push system" << lex.idtable.table[lex.lextable.table[k].idxTI].id << "\n";
									q = k;
									in = false;
								}
							}
							k++;
						}
						*(log.stream) << "call translate\n";
						*(log.stream) << "mov system" << lex.idtable.table[lex.lextable.table[q].idxTI].id << ",eax\n";
						in = true;
					}
					k++;
				}
				*(log.stream) << "call stop\nexit:\npush 0\ncall ExitProcess\nEND MAIN";
			}
		}
	}
}