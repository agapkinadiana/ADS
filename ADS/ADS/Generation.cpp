#include "pch.h"
#include <stack>

namespace Generation
{
	void Generation(Log::LOG log, Lex::LEX lex)
	{
		*(log.stream) << ".586\n\t.model flat, stdcall\n\tincludelib libucrt.lib\n\tincludelib kernel32.lib\n\tincludelib Library.lib\n\tExitProcess PROTO :DWORD\n\n";
		*(log.stream) << "\tprinti PROTO: DWORD\n\tprints PROTO: DWORD\n\tlen PROTO: DWORD\n\tpows PROTO: DWORD, :DWORD\n";
		*(log.stream) << "\n.stack 4096\n";

		*(log.stream) << ".const\n";

		for (int i = 0; i < lex.idtable.size; i++)
		{
			if (lex.idtable.table[i].idType == IT::IDTYPE_LIT)
			{
				*(log.stream) << "\t" << lex.idtable.table[i].id;
				if (lex.idtable.table[i].idDataType == IT::IDDATATYPE_STR)
				{
					*(log.stream) << " BYTE " << lex.idtable.table[i].value.rowValue.str << ", 0\n";
				}
				if (lex.idtable.table[i].idDataType == IT::IDDATATYPE_INT)
				{
					*(log.stream) << " DWORD " << lex.idtable.table[i].value.numValue << endl;
				}
			}
		}

		*(log.stream) << ".data\n";
		for (int i = 0; i < lex.lextable.size; i++)
		{
			if (lex.lextable.table[i].lexema == LEX_NEW)
			{
				*(log.stream) << "\t" << lex.idtable.table[lex.lextable.table[i + 2].indexTI].id;
				*(log.stream) << " DWORD ?\n";
				i += 3;
			}
		}

		stack<string> stk; // стек для правильной последовательности передачи параметров в функцию ассемблера
		string func_name = "";					// имя локальной функции
		int num_of_points = 0,
			num_of_ret = 0,
			num_of_ends = 0;
		bool isInfuncion = false,				// внутри локальной функции
			isMain = false,    			// внутри главной функции
			isRet = false,
			isIf = false,
			isElse = false,
			isThen = false;



		*(log.stream) << "\n.code\n\n";
		for (int i = 0; i < lex.lextable.size; i++)
		{
			switch (lex.lextable.table[i].lexema)
			{
			case LEX_FUNC:
			{
				*(log.stream) << (func_name = lex.idtable.table[lex.lextable.table[++i].indexTI].id) << " PROC ";
				while (lex.lextable.table[i].lexema != LEX_RIGHTHESIS)
				{
					if (lex.idtable.table[lex.lextable.table[i].indexTI].idType == IT::IDTYPE_PAR)
					{
						*(log.stream) << lex.idtable.table[lex.lextable.table[i].indexTI].id << " : ";
						if (lex.idtable.table[lex.lextable.table[i].indexTI].idDataType == IT::IDDATATYPE_INT)
						{
							*(log.stream) << "SDWORD";
						}
						else
						{
							*(log.stream) << "DWORD";
						}
					}
					if (lex.lextable.table[i].lexema == LEX_COMMA)
					{
						*(log.stream) << ", ";
					}
					i++;
				}
				isInfuncion = true;
				*(log.stream) << endl;
				break;
			}
			case LEX_MAIN:
			{
				isMain = true;
				*(log.stream) << "main PROC\n";
				break;
			}
			case LEX_EQUAL:
			{
				int result_position = i - 1;
				while (lex.lextable.table[i].lexema != LEX_SEMICOLON)
				{
					switch (lex.lextable.table[i].lexema)
					{
					case LEX_ID:
					case LEX_INT:
					{
						*(log.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i].indexTI].id << endl;
						stk.push(lex.idtable.table[lex.lextable.table[i].indexTI].id);
						break;
					}
					case LEX_STR:
					{
						*(log.stream) << "\tpush offset " << lex.idtable.table[lex.lextable.table[i].indexTI].id << endl;
						stk.push("offset " + lex.idtable.table[lex.lextable.table[i].indexTI].id);
						break;
					}
					case LEX_SUBST:
					{
						for (int j = 1; j <= lex.lextable.table[i].priority; j++)
						{
							*(log.stream) << "\tpop edx\n";
						}
						for (int j = 1; j <= lex.lextable.table[i].priority; j++)
						{
							*(log.stream) << "\tpush " << stk.top() << endl;
							stk.pop();
						}
						*(log.stream) << "\t\tcall " << lex.idtable.table[lex.lextable.table[i].indexTI].id << "\n\tpush eax\n";
						break;
					}
					case LEX_ACTION:
					{
						switch (lex.idtable.table[lex.lextable.table[i].indexTI].id[0])
						{
						case LEX_MULTIPLY:
						{
							*(log.stream) << "\tpop eax\n\tpop ebx\n";
							*(log.stream) << "\tmul ebx\n\tpush eax\n";
							break;
						}
						case LEX_PLUS:
						{
							*(log.stream) << "\tpop eax\n\tpop ebx\n";
							*(log.stream) << "\tadd eax, ebx\n\tpush eax\n";
							break;
						}
						case LEX_MINUS:
						{
							*(log.stream) << "\tpop ebx\n\tpop eax\n";
							*(log.stream) << "\tsub eax, ebx\n\tpush eax\n";
							break;
						}
						}
						break;
					}
					}
					i++;
				}
				*(log.stream) << "\tpop " << lex.idtable.table[lex.lextable.table[result_position].indexTI].id << "\n";
				break;
			}
			case LEX_RET:
			{
				*(log.stream) << "\tpush ";
				i++;
				if (lex.idtable.table[lex.lextable.table[i].indexTI].idType == IT::IDTYPE_LIT)
				{
					*(log.stream) << lex.idtable.table[lex.lextable.table[i++].indexTI].value.numValue;
				}
				else
				{
					*(log.stream) << lex.idtable.table[lex.lextable.table[i++].indexTI].id;
				}
				if (isInfuncion)
				{
					*(log.stream) << "\n\t\tjmp local" << num_of_ret << endl;
					isRet = true;
				}
				if (isMain)
				{
					*(log.stream) << "\n\t\tjmp theend\n";
					isRet = true;
				}
				break;
			}
			case LEX_RIGHTBRACE:
			{
				if (isMain && !isThen && !isElse && !isInfuncion)
				{
					if (isRet)
					{
						*(log.stream) << "theend:\n";
						isRet = false;
					}
					*(log.stream) << "\tcall ExitProcess\nmain ENDP\nend main";
				}
				if (isInfuncion)
				{
					if (isRet)
					{
						*(log.stream) << "local" << num_of_ret++ << ":\n";
						*(log.stream) << "\tpop eax\n\tret\n";
						isRet = false;
					}
					*(log.stream) << func_name << " ENDP\n\n";
					isInfuncion = false;
				}
				if (isThen)
				{
					isThen = false;
					if (isElse)
					{
						*(log.stream) << "\tjmp e" << num_of_ends << endl;
						isElse = false;
					}
					*(log.stream) << "m" << num_of_points++ << ":\n";
				}
				if (isElse)
				{
					isElse = false;
					*(log.stream) << "e" << num_of_ends++ << ":\n";
				}
				break;
			}
			case LEX_IF:
			{
				isIf = true;
				break;
			}
			case LEX_LEFTHESIS:
			{
				if (isIf)
				{
					*(log.stream) << "\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 1].indexTI].id << endl; //первое число
					*(log.stream) << "\tcmp eax, " << lex.idtable.table[lex.lextable.table[i + 3].indexTI].id << endl;
					if (lex.lextable.table[i + 2].lexema == LEX_MORE)
					{
						*(log.stream) << "\t\tjg m" << num_of_points << endl;
						*(log.stream) << "\t\tjl m" << num_of_points + 1 << endl;
					}
					else
					{
						*(log.stream) << "\t\tjl m" << num_of_points << endl;
						*(log.stream) << "\t\tjg m" << num_of_points + 1 << endl;
					}
					*(log.stream) << "\t\tje m" << num_of_points + 1 << endl;
					int j = i;
					while (lex.lextable.table[j++].lexema != LEX_RIGHTBRACE)
					{
						if (lex.lextable.table[j + 1].lexema == LEX_ELSE)
						{
							isElse = true;
							break;
						}
					}
					isIf = false;
				}
				break;
			}
			case LEX_THEN:
			{
				isThen = true;
				*(log.stream) << "m" << num_of_points++ << ":\n";
				break;
			}
			case LEX_ELSE:
			{
				isElse = true;
				break;
			}
			case LEX_PRINT:
			{
				if (lex.idtable.table[lex.lextable.table[i + 2].indexTI].idDataType == IT::IDDATATYPE_INT)
				{
					*(log.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 2].indexTI].id << "\n\t\tcall printi\n";
				}
				else
				{
					*(log.stream) << "\tpush offset " << lex.idtable.table[lex.lextable.table[i + 2].indexTI].id << "\n\t\tcall prints\n";
				}
				break;
			}
			}
		}
	}
}