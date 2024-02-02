#pragma once
#include "stdafx.h"
using namespace std;
namespace PN {
	LT::LexTable PolishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable);
	void showPolishNotation(LT::LexTable lextable, Log::LOG& log);
} 