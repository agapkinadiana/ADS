#include "pch.h"

namespace Log
{
	LOG GetLog(wchar_t logfile[])
	{
		LOG a = { NULL, NULL };
		wcscpy_s(a.logfile, logfile);
		ofstream * q = new ofstream(logfile);
		if (!q->fail())
		{
			a.stream = q;
		}
		else
		{
			throw ERROR_THROW(112);
		}
		return a;
	}

	void WriteLine(LOG log, char* c, ...)
	{
		char **p = &c;
		while (strcmp(*p, "")) {
			*(log.stream) << *p;
			p++;
		}
	}

	void WriteLine(LOG log, wchar_t* c, ...)
	{
		wchar_t **p = &c;
		char buf[PARM_MAX_SIZE];
		while (wcscmp(*p, L"")) {
			wcstombs(buf, *p, PARM_MAX_SIZE);
			*(log.stream) << buf;
			p++;
		}
	}

	void WriteLog(LOG log)
	{
		struct tm newtime;
		__time64_t ltime;
		errno_t err;
		char s[100];
		_time64(&ltime);
		err = _localtime64_s(&newtime, &ltime);
		strftime(s, 100, "%c ", &newtime);
		*log.stream << "Дата протокола:" << "\t" << s << endl;
	}

	void WriteParm(LOG log, Parm::PARM parm)
	{
		char buf[PARM_MAX_SIZE];
		*(log.stream) << "\nПараметры:";
		wcstombs(buf, parm.in, PARM_MAX_SIZE);
		*(log.stream) << "\t-in:  " << buf << endl;
		wcstombs(buf, parm.log, PARM_MAX_SIZE);
		*(log.stream) << "\t\t-log: " << buf << endl;
		wcstombs(buf, parm.lex, PARM_MAX_SIZE);
		*(log.stream) << "\t\t-lex: " << buf << endl;
		wcstombs(buf, parm.syn, PARM_MAX_SIZE);
		*(log.stream) << "\t\t-syn: " << buf << endl;
		wcstombs(buf, parm.out, PARM_MAX_SIZE);
		*(log.stream) << "\t\t-out: " << buf << endl;
	}

	void WriteIn(LOG log, In::IN in)
	{
		*(log.stream) << "\nИсходные данные:";
		*(log.stream) << "\tКоличество символов: " << in.size << endl;
		*(log.stream) << "\t\tКоличество строк: " << in.lines << endl;
		*(log.stream) << "\t\tКоличество проигнорированных символов: " << in.ignor << endl;
		*(log.stream) << "--------------------------------------------------------------------------------" << endl;
	}

	void WriteError(LOG log, Error::ERROR error)
	{
		if (log.stream != 0) {
			*log.stream << "Ошибка " << error.id << ": " << error.message;
			if (error.intext.line != -1) {
				*log.stream << ", строка " << error.intext.line;
			}
			if (error.intext.pos != -1) {
				*log.stream << ", позиция " << error.intext.pos << endl << endl;
			}
		}

		cout << "Ошибка " << error.id << ": " << error.message;
		if (error.intext.line != -1) {
			cout << " строка " << error.intext.line;
		}
		if (error.intext.pos != -1) {
			cout << ", позиция " << error.intext.pos << endl;
		}
	}

	void Close(LOG log)
	{
		if (log.stream != 0) {
			log.stream->close();
		}
	}
}