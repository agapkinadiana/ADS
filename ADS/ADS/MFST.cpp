#include "pch.h"

int FST_TRACE_n = -1;
char rbuf[205], sbuf[205], lbuf[1024];  //---------------------------------
#define NS(n)	GRB::Rule::Chain::N(n)
#define TS(n)	GRB::Rule::Chain::T(n)
#define ISNS(n)	GRB::Rule::Chain::isN(n)
#define MFST_TRACE1(log)	*(log.stream)<<setw(4)<<left<<++FST_TRACE_n << ": "\
						<<setw(20)<<left<<rule.getCRule(rbuf, rulechainNo)\
						<<setw(30)<<left<<getCLenta(lbuf, lenta_position)\
						<<setw(20)<<left<<getContainStack(sbuf)<<endl;

#define MFST_TRACE2(log)	*(log.stream)<<setw(4)<<left<< FST_TRACE_n << ": "\
						<<setw(20)<<left<<" "\
						<<setw(30)<<left<<getCLenta(lbuf, lenta_position)\
						<<setw(20)<<left<<getContainStack(sbuf)<<endl;

#define MFST_TRACE3(log)	*(log.stream)<<setw(4)<<left<< ++FST_TRACE_n << ": "\
						<<setw(20)<<left<<" "\
						<<setw(30)<<left<<getCLenta(lbuf, lenta_position)\
						<<setw(20)<<left<<getContainStack(sbuf)<<endl;

#define MFST_TRACE4(c, log)  *(log.stream)<<setw(4)<<left<< ++FST_TRACE_n<<": "<<setw(20)<<left<<c<<endl;
#define MFST_TRACE5(c, log)  *(log.stream)<<setw(4)<<left<< FST_TRACE_n<<": "<<setw(20)<<left<<c<<endl;
#define MFST_TRACE6(c,k, log) *(log.stream)<<setw(4)<<left<< FST_TRACE_n<<": "<<setw(20)<<left<<c<<k<<endl;
#define MFST_TRACE7(log)	  *(log.stream)<<setw(4)<<left<< state.lenta_position<<": " \
								<<setw(20)<<left<<rule.getCRule(rbuf,state.rulechainNo)<<endl;
										//-------------------------------------------------------------------------
namespace MFST
{
	MfstState::MfstState()
	{
		lenta_position = 0;
		rulechainNo = -1;
		ruleNo = -1;
	}
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain) //����������� (������� �� �����, ���� ��������, ����� ������� ������� � �������� �������)
	{
		lenta_position = pposition;
		stack = pst;
		rulechainNo = pnrulechain;
	}

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain) //����������� (������� �� �����, ���� ��������,����� �������� �������, ����� ������� �������)
	{
		lenta_position = pposition; //������� �� �����
		stack = pst;  //���� ��������
		ruleNo = pnrule;
		rulechainNo = pnrulechain; //����� �������� ������� � �������
	}
	Mfst::MfstDiagnosis::MfstDiagnosis()
	{
		lenta_position = -1;
		rc_step = SURPRISE;
		ruleNo = -1;
		rule_chainNo = -1;
	}
	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)//�����������
	{                                              //(������� �� �����, ��� ���������� ����, ����� �������, ����� ������� �������)
		lenta_position = plenta_position; //������� �� �����
		rc_step = prc_step; // ��� ���������� ����
		ruleNo = pnrule; //����� �������
		rule_chainNo = pnrule_chain; //����� ������� �������
	}
	Mfst::Mfst()
	{
		lenta = 0;
		lenta_position = 0;
		lenta_size = 0;
	}
	Mfst::Mfst(Lex::LEX plex, GRB::Greibach pgreibach) //(��������� ������ ������������ �����������, ���������� �������)
	{
		greibach = pgreibach;
		lex = plex;
		lenta = new short[lenta_size = lex.lextable.size];
		for (int k = 0; k < lenta_size; k++)					//(������� � ����� ���������)
		{
			lenta[k] = TS(lex.lextable.table[k].lexema);
		}
		lenta_position = 0;
		stack.push(greibach.stbottomT); //��������� ��� �����
		stack.push(greibach.startN); //��������� ��������� ������
		rulechainNo = -1; //���������� ������� ����� -1
	};
	Mfst::RC_STEP Mfst::step(Log::LOG log)//( bool need_trace) //��������� ��� ��������----------------------------------------------------?
	{
		RC_STEP rc = SURPRISE;
		if (lenta_position < lenta_size)
		{
			if (ISNS(stack.top())) //���� � ������� ����� ������ ������
			{
				GRB::Rule rule;
				if ((ruleNo = greibach.getRule(stack.top(), rule)) >= 0) //���� ������� �� �����������
				{
					GRB::Rule::Chain chain;
					if ((rulechainNo = rule.getNextChain(lenta[lenta_position], chain, rulechainNo + 1)) >= 0) //�������� ��������� ������� � ������� � �����, ���� ���������� -1
					{
						MFST_TRACE1(log) //�����
							saveState(log); //��������� ��������� ��������
						stack.pop(); //��������� �������� �����.
						push_chain(chain); //��������� ������� ������� � ����
						rc = NS_OK; //������� ������� � �������... ������� �������� � ����
						MFST_TRACE2(log) //�����
					}
					else
					{
						MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE", log)//�����
							saveDiagnosis(NS_NORULECHAIN);//��� ����������
						rc = restState(log) ? NS_NORULECHAIN : NS_NORULE;//������������ ��������� ��������
					};
				}
				else
				{
					rc = NS_ERROR;//���� �� ��������� ������� ������� ��� ����� �����������
				};
			}
			else if ((stack.top() == lenta[lenta_position]))//���� ������� ������ ����� ����� ������� �����
			{
				lenta_position++;//���������� �����
				stack.pop();//������� �����
				rulechainNo = -1;//����� �������� ������� ����� -1
				rc = TS_OK;
				MFST_TRACE3(log)
			}
			else //�������� ������������ ���������� ���� � ��������� ��� ���� ������ �������
			{
				MFST_TRACE4("TS_NOK/NS_NORULECHAIN", log)
					rc = restState(log) ? TS_NOK : NS_NORULECHAIN;
			};
		}
		else
		{
			if (stack.top() == greibach.stbottomT)//���� ������ � ����� � ������ ��� ����� �� ���������� ������� ���������
			{
				rc = LENTA_END;
				MFST_TRACE4("--------------LENTA_END", log)
			}
			else
			{
				throw ERROR_THROW(215);
			}
		};
		return rc;
	};
	bool Mfst::push_chain(GRB::Rule::Chain chain)//��������� ������� ������� � ���� � �������� �������!!! (������� �������)
	{

		for (int k = chain.chainLenght - 1; k >= 0; k--)//� = ������ �������-1. ������� ������� � ����
		{
			stack.push(chain.nt[k]);
		}
		return true;
	}
	bool Mfst::saveState(Log::LOG log) //��������� ��������� ��������
	{
		storestate.push(MfstState(lenta_position, stack, ruleNo, rulechainNo));//���� ��� ���������� ���������. ������� ����������� ���������
		MFST_TRACE6("SAVESTATE: ", storestate.size(), log)
			return true;
	}
	bool Mfst::restState(Log::LOG log)//������������ ��������� ��������
	{
		bool rc = false;
		MfstState state; //���� ���������
		if (rc = (storestate.size() > 0)) //���� ������ ����� ���� ������ 0
		{
			state = storestate.top();
			lenta_position = state.lenta_position;
			stack = state.stack;
			ruleNo = state.ruleNo;
			rulechainNo = state.rulechainNo;
			storestate.pop();
			MFST_TRACE5("RESTATE", log)
				MFST_TRACE2(log)
		}
		return rc;
	}
	bool Mfst::saveDiagnosis(RC_STEP pprc_step)
	{
		bool rc = false;
		short k = 0;
		while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)
		{
			k++;
		}
		if (rc = (k < MFST_DIAGN_NUMBER)) {
			diagnosis[k] = MfstDiagnosis(lenta_position, pprc_step, ruleNo, rulechainNo);
			for (short j = k + 1; j < MFST_DIAGN_NUMBER; j++)
			{
				diagnosis[j].lenta_position = -1;
			}
		}
		return rc;
	}
	bool Mfst::start(Log::LOG log) //������ ������ ��������
	{
		bool rc = false;
		RC_STEP rc_step = SURPRISE;
		char buf[MFST_DIAGN_MAXSIZE];
		rc_step = step(log);
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
		{
			rc_step = step(log);
		}
		switch (rc_step)
		{
		case LENTA_END:
		{
			MFST_TRACE4("----------------------------->LENTA_END", log)
				*(log.stream) << "---------------------------------------------------------------------------------------\n";
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: ����� ����� %d, �������������� ������ �������� ��� ������", 0, lenta_size);
			*(log.stream) << setw(4) << left << 0 << ": ����� ����� " << lenta_size << ", �������������� ������ �������� ��� ������\n";
			rc = true;
			break;
		}
		case NS_NORULE:
		{	*(log.stream) << "NS_NORULE";
		*(log.stream) << getDiagnosis(0, buf) << endl;
		*(log.stream) << getDiagnosis(1, buf) << endl;
		*(log.stream) << getDiagnosis(2, buf) << endl;
		break;
		}
		case NS_NORULECHAIN:	MFST_TRACE4("---------->NS_NORULECHAIN", log) break;
		case NS_ERROR:			MFST_TRACE4("---------->NS_ERROR", log) break;
		case SURPRISE:			MFST_TRACE4("---------->SURPRISE", log) break;
		}
		return rc;
	}
	char* Mfst::getContainStack(char* buf)//�������� ���������� �����
	{
		for (int k = (signed)stack.size() - 1; k >= 0; k--)
		{
			short p = stack._Get_container()[k];
			buf[stack.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p); //� ���������� ������ ���������� ���������� �����
		};
		buf[stack.size()] = 0x00;
		return buf;
	}
	char* Mfst::getCLenta(char* buf, short pos, short n) //�������� ���������� ������� �����
	{
		short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
		for (i = pos; i < k; i++)
		{
			buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]); //� ���������� ������ ���������� �������� �����
		}
		buf[i - pos] = 0x00;
		return buf;
	}
	char* Mfst::getDiagnosis(short n, char* buf)
	{
		char *rc = nullptr;
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = greibach.getRule(diagnosis[n].ruleNo).idDiagnosticError;              //������ �� �������
			Error::ERROR err = ERROR_THROW_IN(errid, lex.lextable.table[lpos].lineNo, -1);
			throw ERROR_THROW_IN(errid, lex.lextable.table[lpos].lineNo, -1);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: ������ %d, %s", err.id, lex.lextable.table[lpos].lineNo, err.message);
			rc = buf;
		}
		return rc;
	}
	void Mfst::printRules(Log::LOG &log)//������� ������ �������
	{
		MfstState state;
		GRB::Rule rule;

		*(log.stream) << endl << "������ ������� :" << endl;
		for (unsigned short k = 0; k < storestate.size(); k++)
		{
			state = storestate._Get_container()[k];
			rule = greibach.getRule(state.ruleNo);
			MFST_TRACE7(log)
		};
	}
}