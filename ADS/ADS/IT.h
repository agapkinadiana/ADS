#pragma once
#include "pch.h"

#define ID_MAXSIZE		15			//максимальное кол-во символов в идентификаторе
#define TI_MAXSIZE		4096		//максимальное кол-во строк в ТИ
#define TI_INT_DEFAULT	0x00000000	//значение по умолчанию для типа int
#define TI_STRDEFAULT	0x00		//значение по умолчанию для типа str
#define TI_NULLIDX		0xffffffff	//нет элемента таблицы идентификаторов
#define TI_STR_MAXSIZE	255	

namespace IT
{
	enum IDDATATYPE { IDDATATYPE_OFF = 0, IDDATATYPE_INT = 1, IDDATATYPE_STR = 2, IDDATATYPE_MATH = 3};      //OFF - ничего, выбор (начало)
	enum IDTYPE { IDTYPE_N = 0, IDTYPE_TAB = 1, IDTYPE_FUN = 2, IDTYPE_PAR = 3, IDTYPE_LIT = 4, IDTYPE_ACT = 5 };   //переменная, функция, параметр функции, литерал, оператор
					            //таблица идентификаторов
	struct Entry
	{
		int	idFirstInLT;		//индекс первого вхождения в таблице лексем
		string id;				//идентификатор	(автоматические усекается до ID_MAXSIZE)
		IDDATATYPE idDataType;	//тип данных
		IDTYPE idType;			//тип идентификатора
		struct
		{
			int numValue;		//значение int
			struct
			{
				int lenghtstr;	//длина строки
				string str;		//символы string
			} rowValue;			//значение string
		} value;				//значение идентификатора
	};

	struct IdTable				//экземпляр таблицы идентификаторов
	{
		int maxsize;			//емкость таблицы идентификаторов < TI_MAXSIZE
		int size;				//текущий размер таблицы идентификаторов < maxsize
		Entry* table;			//массив строк таблицы идентификаторов
	};

	IdTable Create(int size);                      //создать таблицу идентификаторов
	void Add(IdTable& idtable, Entry entry);       //добавить строку в таблицу идентификаторов
	Entry GetEntry(IdTable& idtable, int n);       //получить строку таблицы идентификаторов
	int IsId(IdTable& idtable, string id);         //возврат: номер строки(если есть), TI_NULLIDX(если нет)
	int CheckId(IdTable& idtable, string value);
	int IsDublId(IdTable& idtable, string id);	   //проверка на дублирование ид
	int CheckId(IdTable& idtable, int value);
	void ShowTable(IdTable& idtable);
	void WriteTable(Log::LOG log, IdTable& idtable);
};
