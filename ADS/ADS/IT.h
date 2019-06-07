#pragma once
#include "pch.h"

#define ID_MAXSIZE		15			//������������ ���-�� �������� � ��������������
#define TI_MAXSIZE		4096		//������������ ���-�� ����� � ��
#define TI_INT_DEFAULT	0x00000000	//�������� �� ��������� ��� ���� int
#define TI_STRDEFAULT	0x00		//�������� �� ��������� ��� ���� str
#define TI_NULLIDX		0xffffffff	//��� �������� ������� ���������������
#define TI_STR_MAXSIZE	255	

namespace IT
{
	enum IDDATATYPE { IDDATATYPE_OFF = 0, IDDATATYPE_INT = 1, IDDATATYPE_STR = 2, IDDATATYPE_MATH = 3};      //OFF - ������, ����� (������)
	enum IDTYPE { IDTYPE_N = 0, IDTYPE_TAB = 1, IDTYPE_FUN = 2, IDTYPE_PAR = 3, IDTYPE_LIT = 4, IDTYPE_ACT = 5 };   //����������, �������, �������� �������, �������, ��������
					            //������� ���������������
	struct Entry
	{
		int	idFirstInLT;		//������ ������� ��������� � ������� ������
		string id;				//�������������	(�������������� ��������� �� ID_MAXSIZE)
		IDDATATYPE idDataType;	//��� ������
		IDTYPE idType;			//��� ��������������
		struct
		{
			int numValue;		//�������� int
			struct
			{
				int lenghtstr;	//����� ������
				string str;		//������� string
			} rowValue;			//�������� string
		} value;				//�������� ��������������
	};

	struct IdTable				//��������� ������� ���������������
	{
		int maxsize;			//������� ������� ��������������� < TI_MAXSIZE
		int size;				//������� ������ ������� ��������������� < maxsize
		Entry* table;			//������ ����� ������� ���������������
	};

	IdTable Create(int size);                      //������� ������� ���������������
	void Add(IdTable& idtable, Entry entry);       //�������� ������ � ������� ���������������
	Entry GetEntry(IdTable& idtable, int n);       //�������� ������ ������� ���������������
	int IsId(IdTable& idtable, string id);         //�������: ����� ������(���� ����), TI_NULLIDX(���� ���)
	int CheckId(IdTable& idtable, string value);
	int IsDublId(IdTable& idtable, string id);	   //�������� �� ������������ ��
	int CheckId(IdTable& idtable, int value);
	void ShowTable(IdTable& idtable);
	void WriteTable(Log::LOG log, IdTable& idtable);
};
