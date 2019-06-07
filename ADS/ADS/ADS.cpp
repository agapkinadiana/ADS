// ADS.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"

int _tmain(int argc, _TCHAR* argv[])
{ 
	Log::LOG log = Log::INITLOG;
	try
	{
		setlocale(0, "");
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::GetLog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);

		//проверка файла с исходным кодом на допустимость символов
		In::IN in = In::GetIn(parm.in);
		Log::WriteIn(log, in);

		log = Log::GetLog(parm.lex);
		Lex::LEX lex = Lex::Lexer(in, log);

		log = Log::GetLog(parm.syn);
		MFST_TRACE_START(log)
		MFST::Mfst mfst(lex, GRB::getGreibach());
		mfst.start(log);
		mfst.printRules(log);

		CallPolishNotation(&lex.lextable, &lex.idtable);
		LT::Write(log, lex.lextable);

		log = Log::GetLog(parm.out);
		Generation::Generation(log, lex);
		Log::Close(log);
		cout << "Генерация выполненена успешно))" << endl;

	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
		cout << endl;
	}
	catch (...)
	{
		cout << "Ошибка: сбой системы" << endl;
	} 
	//system("notepad C:\\ADS\\syn.txt");
	//system("notepad C:\\ADS\\lex.txt");
	//system("notepad C:\\ADS\\log.txt");

	return 0;
}
