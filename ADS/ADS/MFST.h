#pragma once
#include "pch.h"
#include <stack>

#define MFST_DIAGN_MAXSIZE	2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3
#define MFST_TRACE_START(log) *(log.stream)<<endl<< setfill(' ') << setw( 4)<<left<<"���"<<":"<<\
									setw(20)<<left<<"�������"<<\
									setw(30)<<left<<"������� �����"<<\
									setw(20)<<left<<"����"<<endl;


typedef stack<short> MFSTSTSTACK;	// ���� ��������

namespace MFST
{                  //���������� �������
	struct MfstState			// ��������� �������� (��� ����������)
	{
		short lenta_position;	// ������� �� �����
		short ruleNo;			// ����� �������� �������
		short rulechainNo;		// ����� ������� ������� �������� �������
		MFSTSTSTACK stack;		// ���� ��������
		MfstState();
		MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain);
		MfstState(
			short pposition,       // ������� �� �����
			MFSTSTSTACK pst,       // ���� ��������
			short pnrule,          // ����� �������� �������
			short pnrulechain      // ����� ������� �������
		);
	};
	struct Mfst						// ���������� �������
	{
		enum RC_STEP {				// ��� �������� ������� step
			NS_OK,					// ������� ������� � �������, ������� �������� � ����
			NS_NORULE,				// �� ������� ������� ���������� (������ � ����������)
			NS_NORULECHAIN,			// �� ������� ���������� ������� ������� (������ � �������� ����)
			NS_ERROR,				// ����������� �������������� ������ ����������
			TS_OK,					// ���. ������ ����� == ������� �����, ������������ �����, pop �����
			TS_NOK,					// ���. ������ ����� != ������� �����, ������������� ��������� 
			LENTA_END,				// ������� ������� ����� >= lenta_size
			SURPRISE				// ����������� ��� �������� (������ � step)
		};

		struct MfstDiagnosis		 // �����������
		{
			short lenta_position;			// ������� �� �����
			RC_STEP rc_step;				// ��� ���������� ����
			short ruleNo;					// ����� �������
			short rule_chainNo;				// ����� ������� �������
			MfstDiagnosis();
			MfstDiagnosis(			 // �����������
				short plenta_position,	// ������� �� �����
				RC_STEP prc_step,		// ��� ���������� ����
				short pnrule,			// ����� ������� 
				short pnrule_chain		// ����� ������� �������
			);
		} diagnosis[MFST_DIAGN_NUMBER];			// ��������� ����� �������� ���������
		GRBALPHABET* lenta;					// ���������������� (TS/NS) ����� (�� LEX)
		short lenta_position;				// ������� ������� �� �����
		short ruleNo;						// ����� �������� �������
		short rulechainNo;					// ����� ������� ������� �������� �������
		short lenta_size;					// ������ �����
		GRB::Greibach greibach;				// ���������� �������
		Lex::LEX lex;						// ��������� ������ ������������ �����������
		MFSTSTSTACK stack;							// ���� ��������
		std::stack<MfstState> storestate;	// ���� ��� ���������� ���������
		Mfst();
		Mfst(
			Lex::LEX plex,			// ��������� ������ ������������ �����������
			GRB::Greibach pgrebach			// ���������� �������
		);
		char* getContainStack(char* buf);			// �������� ���������� �����
		char* getCLenta(char* buf, short pos, short n = 25);	// �����: n �������� � pos
		char* getDiagnosis(short n, char* buf);		// �������� n-�� ������ ����������� ��� 0�00
		bool saveState(Log::LOG log);					// ��������� ��������� ��������
		bool restState(Log::LOG log);					// ������������ ��������� ��������
		bool push_chain(						// ��������� ������� ������� � ����
			GRB::Rule::Chain chain		// ������� ������
		);
		Mfst::RC_STEP step(Log::LOG log);// ��������� ��� ��������
		bool start(Log::LOG log);						// ��������� �������
		bool saveDiagnosis(
			RC_STEP pprc_step	// ��� ���������� ����
		);
		void printRules(Log::LOG &log);	// ������� ������������������ ������ (������ �������)


	};
}
