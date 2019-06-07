#include "pch.h"

namespace Lex
{
	LT::LexTable lexTable = LT::Create(LT_MAXSIZE);
	IT::IdTable idTable = IT::Create(TI_MAXSIZE);

	struct Function
	{
		int  indexTI;
		int size = 0;
		string name;
		IT::IDDATATYPE arrayOfDataType[FUNC_MAX_SIZE];	// ������ ����� ����������
	};
	struct FunctionName
	{
		Function arrayOfFunction[FUNC_MAX_SIZE]; //������ ���������� �������
		int size = -1;
	}Func_Han;

	FST::STATE_MACHINE SearchFST(string src)     //��������� �� ������� (����� ������� � � ���)
	{
		int size;
		FST::STATE_MACHINE result;
		FST::FST* arr_of_fst = new FST::FST;
		if (src.size() == 1)                      //��������� �������� ��� �����
		{
			arr_of_fst = FST::arr_of_fst_op();
			size = ARR_OF_FST_OP_SIZE;
		}
		else
		{
			arr_of_fst = FST::arr_of_fst_word();
			size = ARR_OF_FST_WORD_SIZE;
		}
		//�������� �� ������� ���������
		for (int i = 0; i < size; i++) {
			//��������� ���� -> �������
			arr_of_fst[i].str = src;
			//��������� �������, ����  i-�� ������� �������
			if (execute(arr_of_fst[i]))
			{
				result.lexema = arr_of_fst[i].lexema;
				result.lex_type = arr_of_fst[i].lex_type;
				return result;
			}
		}
		return { FST::SYMB_NOT_FOUND, 0 };
	}


	LT::Entry CreateLex(char lex, int line, int id_in_TI, int prior)
	{
		LT::Entry result;
		result.priority = prior;
		result.lexema = lex;
		result.lineNo = line;
		result.indexTI = id_in_TI;
		return result;
	}

	IT::Entry CreateId(int line, string id, IT::IDDATATYPE id_data_type, IT::IDTYPE id_type, int vint)  // ��� �����
	{
		IT::Entry result;
		result.id = id;
		result.idFirstInLT = line;
		result.idDataType = id_data_type;
		result.idType = id_type;
		result.value.numValue = vint;
		return result;
	}

	IT::Entry CreateId(int line, string id, IT::IDDATATYPE id_data_type, IT::IDTYPE id_type)
	{
		IT::Entry result;
		result.id = id;
		result.idFirstInLT = line;
		result.idDataType = id_data_type;
		result.idType = id_type;
		return result;
	}

	IT::Entry CreateId(int line, string id, IT::IDDATATYPE id_data_type, IT::IDTYPE id_type, string vstr)  // ��� �����
	{
		IT::Entry result;
		result.id = id;
		result.idFirstInLT = line;
		result.idDataType = id_data_type;
		result.idType = id_type;
		result.value.rowValue.str = vstr;
		result.value.rowValue.lenghtstr = vstr.length();
		if (vstr.length() > 0)
		{
			result.value.rowValue.lenghtstr = vstr.length()-2;
		}
		return result;
	}

	string MakeString(char symb, int num_of_lit)
	{
		string result = "1";
		result[0] = symb;
		return result + to_string(num_of_lit);
	}

	bool Check(IT::IDDATATYPE* arr_of_it_entry, string name)    //��������� ��� � ��� �������
	{
		for (int i = 0; i < Func_Han.size + 1; i++) {
			if (Func_Han.arrayOfFunction[i].name == name)
			{
				for (int j = 0; j < Func_Han.arrayOfFunction[i].size; j++) {
					if (Func_Han.arrayOfFunction[i].arrayOfDataType[j] != arr_of_it_entry[j])
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	LEX Lexer(In::IN in, Log::LOG log)
	{
		bool isInFunc = false,		//  ������ ��� ���������� �������
			isParmInFunc = false,	// ��� ����������� ���������� �������
			isNew = false,           // ����� �� ��������� ����������
			isAction = false,
			isRet = false,
			isMain = false,
			isLength = false,
			isPow = false,
			isFunction = false,
			isParametr = false,
			isEqual = false,
			isIf = false;

		LEX tables;
		string func_name = "", funcHesisName = ""; //��� ������� �������(��� ������� ���������) , ��� ������� (��� ���. ������� �� ����������)
		IT::IDDATATYPE arrayOfITEntry[FUNC_MAX_SIZE];// ������ ����� ���������� ���������� ��� �������(� ����)
		In::Word queue_word;
		LT::Entry elementLT; //������� , ������������� � ���������� ������ ��������
		FST::STATE_MACHINE lexFromFst;//������� �� ��������� ��������
		IT::IDDATATYPE data_type = IT::IDDATATYPE_OFF;// ��� ������
		int nLit = 0,                  //���-�� ���������
			nParam = 0,                //���-�� ����������
			size = in.queue_w.size();  //���-�� ������

		for (int i = 0; i < size; i++) {          //���������� ��� �����
			queue_word = in.queue_w.front();      //������ � ���� ��-��
			in.queue_w.pop();                     //������� ���� ��-�
			lexFromFst = SearchFST(queue_word.word);
			elementLT = CreateLex(lexFromFst.lexema, queue_word.line, TL_TI_NULLIDX, LT_NOPRIORITY);
			switch (lexFromFst.lex_type) {
			case FST::SYMB_NOT_FOUND:
			{
				throw ERROR_THROW_IN(120, elementLT.lineNo, -1);
				break;
			}
			case FST::SYMB_TABLE_ID://��������������
			{
				int index = 0;
				if (isFunction)    //��� �-���
				{
					isParmInFunc = true;
					func_name = queue_word.word;
					isFunction = false;
					if (IT::IsDublId(idTable, queue_word.word.substr(0, 7)) != TI_NULLIDX)   //��� ���� ����� �������������
					{
						throw ERROR_THROW_IN(300, elementLT.lineNo, -1);
					}
					else
					{
						if (data_type == IT::IDDATATYPE_INT || data_type == IT::IDDATATYPE_STR)
						{
							idTable.table[idTable.size] = CreateId(lexTable.size, queue_word.word.substr(0, 7), data_type, IT::IDTYPE_FUN);
							elementLT.indexTI = idTable.size++;
							Func_Han.size++;
						}
						else
						{
							throw ERROR_THROW_IN(301, elementLT.lineNo, -1)
						}
					}
					break;
				}
				if (isParmInFunc)
				{
					//���� ��� � ���� �����
					if ((index = IT::IsId(idTable, func_name + queue_word.word.substr(0, 7))) == TI_NULLIDX)
					{
						idTable.table[idTable.size] = CreateId(lexTable.size, func_name + queue_word.word.substr(0, 7), data_type, IT::IDTYPE_PAR);
						elementLT.indexTI = idTable.size++;
					}
					else
					{
						if (isMain)
						{
							//� ������ ���������� ������� ��������� ��� ������ �������������� (����� ��������) ( ���������� ��������� �� � ����������)
							if (nParam == 2)
								throw ERROR_THROW(203);
							arrayOfITEntry[nParam++] = idTable.table[index].idDataType;
						}
						elementLT.indexTI = index;
					}
				}
				if (isInFunc || isMain)
				{
					if ((index = IT::IsId(idTable, func_name + queue_word.word.substr(0, 7))) == TI_NULLIDX)  //main
					{
						if ((index = IT::IsId(idTable, queue_word.word.substr(0, 7))) != TI_NULLIDX)  
						{
							elementLT.indexTI = index;
							if (idTable.table[index].idType == IT::IDTYPE_FUN)  //�������� �-��� � ����
							{
								funcHesisName = idTable.table[index].id;
								isParmInFunc = true;
							}
						}

						if (isNew)               //���� ���������� � ����� ������� (onec)
						{
							if (data_type == IT::IDDATATYPE_STR)   //��� �����
							{
								idTable.table[idTable.size] = CreateId(lexTable.size, func_name + queue_word.word.substr(0, 7), data_type, IT::IDTYPE_TAB, "");
							}
							else                                  //��� ����������
							{
								idTable.table[idTable.size] = CreateId(lexTable.size, func_name + queue_word.word.substr(0, 7), data_type, IT::IDTYPE_TAB, 0);
							}
							elementLT.indexTI = idTable.size++;
						}
						//���������
						else if (index == TI_NULLIDX)
						{
							throw ERROR_THROW_IN(306, queue_word.line, -1);
						}
					}
					else       //���� ���� ������ ��������� � ���� ���������������
					{
						elementLT.indexTI = index;
						if (isNew)
						{
							throw ERROR_THROW_IN(300, queue_word.line, -1)
						}
						if (isEqual && !isParmInFunc)   //���������
						{
							if (idTable.table[index].idDataType != data_type)
							{
								throw ERROR_THROW_IN(307, queue_word.line, -1);   //�������������� �����
							}
						}
					}
				}
				if (isRet || isPow || isIf)         //������� ������ ����� � � ������� ����
				{
					if (idTable.table[elementLT.indexTI].idDataType != IT::IDDATATYPE_INT)
					{
						throw ERROR_THROW_IN(301, queue_word.line, -1);
					}
					isRet = false;
					isPow = false;

				}

				if (isLength)           //����� ������ ��� �����
				{
					if (idTable.table[elementLT.indexTI].idDataType != IT::IDDATATYPE_STR)
					{
						throw ERROR_THROW_IN(305, queue_word.line, -1);
					}
					isLength = false;
				}
				isAction = false;
				break;
			}

			case FST::SYMB_LITERAL:
			{
				switch (lexFromFst.lexema) {
				case LEX_INT:
				{
					int index = IT::CheckId(idTable, atoi(queue_word.word.c_str()));  //��-� � ������� ������������� ��������
					if (index == TI_NULLIDX)
					{
						idTable.table[idTable.size] = CreateId(lexTable.size, MakeString(LEX_L, nLit++), IT::IDDATATYPE_INT, IT::IDTYPE_LIT, atoi(queue_word.word.c_str()));
						elementLT.indexTI = idTable.size++;
					}
					else
					{
						elementLT.indexTI = index;
					}
					isAction = false;
					break;
				}
		
				case LEX_STR:
				{
					int index = IT::CheckId(idTable, queue_word.word);
					if (index == TI_NULLIDX)
					{
						idTable.table[idTable.size] = CreateId(lexTable.size, MakeString(LEX_L, nLit++), IT::IDDATATYPE_STR, IT::IDTYPE_LIT, queue_word.word);
						elementLT.indexTI = idTable.size++;
					}
					else
					{
						elementLT.indexTI = index;
					}
					if (isLength)
					{
						isLength = false;
					}
					break;
				}
				}
				if (isMain && isParmInFunc)
				{
					if (nParam == 2)
						throw ERROR_THROW(203);
					arrayOfITEntry[nParam++] = idTable.table[elementLT.indexTI].idDataType;


				}
				if (isEqual && !isParmInFunc)//semantic
				{

					if (idTable.table[elementLT.indexTI].idDataType != data_type)
					{
						throw ERROR_THROW_IN(307, queue_word.line, -1); //�������������� ����� � ��������� ������������
					}
				}
				break;
			}

			case FST::SYMB_NEED_PRIORITY:
			{
				switch (queue_word.word[0]) {
				case LEX_COMMA:
					elementLT.priority = 1;
					break;

				case LEX_LEFTHESIS:
					if (isParmInFunc && elementLT.lexema == LEX_LEFTHESIS)
					{
						elementLT.priority = 4;
						isParametr = true;          //�������, ��� ���� ������� ������

					}
					else
					{
						elementLT.priority = 0;
					}
					break;

				case LEX_RIGHTHESIS:
					if (isParmInFunc && isParametr)
					{
						if (isMain)
						{
							if (Check(arrayOfITEntry, funcHesisName))
							{
								throw ERROR_THROW_IN(302, elementLT.lineNo, -1);
							}
							if (nParam > 2)
								throw ERROR_THROW(203);
							funcHesisName = "";
							nParam = 0;
						}
						elementLT.priority = 4;
						isParametr = false;
						isParmInFunc = false;
					}
					else
					{
						elementLT.priority = 0;
					}
					break;

				}
				break;
			}

			case FST::SYMB_LEXEMS:
			{
				switch (lexFromFst.lexema) {
				case LEX_MAIN:
					if (isMain)
					{
						throw ERROR_THROW(308);

					}
					isMain = true;
					func_name = queue_word.word;
					break;

				case LEX_FUNC:
					isFunction = true;
					break;

				case LEX_IF:
					isIf = true;
					break;

				case LEX_THEN:
					isIf = false;
					break;

				case LEX_STR:
					if (isParmInFunc && !isMain)    //��������� ��������� �-���
					{
						Func_Han.arrayOfFunction[Func_Han.size].arrayOfDataType[Func_Han.arrayOfFunction[Func_Han.size].size++] = IT::IDDATATYPE_STR;
						Func_Han.arrayOfFunction[Func_Han.size].name = func_name;
					}
					data_type = IT::IDDATATYPE_STR;
					elementLT.lexema = LEX_T;
					break;

				case LEX_INT:
					if (isParmInFunc && !isMain)
					{
						//�� � ������� � ���������� ����� ��� ��� 
						Func_Han.arrayOfFunction[Func_Han.size].arrayOfDataType[Func_Han.arrayOfFunction[Func_Han.size].size++] = IT::IDDATATYPE_INT;
						Func_Han.arrayOfFunction[Func_Han.size].name = func_name;
					}
					data_type = IT::IDDATATYPE_INT;
					elementLT.lexema = LEX_T;
					break;

				case LEX_NEW:

					if (isInFunc)
					{
						isNew = true;
					}
					else
					{
						throw ERROR_THROW_IN(200, elementLT.lineNo - 1, -1)
					}
					break;

				case LEX_RET:
					isRet = true;
					break;

				case LEX_EQUAL:
					isEqual = true;
					data_type = idTable.table[lexTable.table[lexTable.size - 1].indexTI].idDataType;
					break;

				case LEX_SEMICOLON:
					if (isParmInFunc)
					{
						throw ERROR_THROW_IN(202, elementLT.lineNo, -1); //������ ����������: �������� ���������
					}
					isPow = false;
					isNew = false;
					isEqual = false;
					isLength = false;
					break;

				case LEX_LEFTBRACE:
					if (isParmInFunc)
					{
						throw ERROR_THROW_IN(202, elementLT.lineNo, -1);
					}
					isInFunc = true;  //���� ����� ����� �������
					break;

				case LEX_RIGHTBRACE:
					isInFunc = false;
					break;
				}
				break;
			}

			case FST::SYMB_ACTION:
			{
				switch (queue_word.word[0]) {
				case LEX_PLUS:
					elementLT.priority = 2;
					break;

				case LEX_MINUS:
					elementLT.priority = 2;
					break;

				case LEX_MULTIPLY:
					elementLT.priority = 3;
					break;
				}
				////SEMANTICA
				if (isAction)
				{
					throw ERROR_THROW_IN(124, elementLT.lineNo, -1);
				}
				isAction = true;
				elementLT.indexTI = idTable.size;
				idTable.table[idTable.size++] = CreateId(lexTable.size, queue_word.word, IT::IDDATATYPE_MATH, IT::IDTYPE_ACT);
				break;
			}

			case FST::SYMB_STATIC_LIB:
			{
				int index;
				if ((index = IT::IsId(idTable, queue_word.word)) == TI_NULLIDX)//
				{
					idTable.table[idTable.size] = CreateId(lexTable.size, queue_word.word, IT::IDDATATYPE_INT, IT::IDTYPE_FUN);
					elementLT.indexTI = idTable.size++;
				}
				else
				{
					elementLT.indexTI = index;
				}
				if (elementLT.lexema == LEX_POW)		// ��� ��������� ����� ����� ���������� ����� ���� ������ ���������� � �-��
				{
					isPow = true;
				}
				if (elementLT.lexema == LEX_LENGTH)		// ��� ���������
				{
					isLength = true;
				}
				isParmInFunc = true;
				break;
			}
			}
			lexTable.table[lexTable.size++] = elementLT;

			if (lexTable.size > LT_MAXSIZE)
			{
				throw ERROR_THROW(121);
			}

			if (idTable.size > TI_MAXSIZE)
			{
				throw ERROR_THROW(122);
			}

		}
		if (isMain == false)
		{
			throw ERROR_THROW(213);
		}

		IT::WriteTable(log, idTable);
		//IT::ShowTable(idTable);
		LT::Write(log, lexTable);
		//LT::ShowTable(lexTable);
		tables.idtable = idTable;
		tables.lextable = lexTable;
		return tables;
	}
}