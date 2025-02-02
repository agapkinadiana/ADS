#include "pch.h"
#define GRB_ERROR_SERIES 200

namespace GRB
{
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)

	Greibach greibach(
		NS('S'), TS('$'),                     // ��������� ������, ��� ����� NS-����������(������� �����),TS-��������
		8, 									  // ���������� ������
		Rule(
			NS('S'), GRB_ERROR_SERIES + 0,    // �������� ��������� ���������  
			2,                                // S->tfi(F){N}S | m{N};
			Rule::Chain(10, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}'))
		),
		Rule(
			NS('N'), GRB_ERROR_SERIES + 1,    // �������� �������������� �����������
			12,                               // N->nti;N | N->nti=E;N| rE; | i=E;N | o(i);N | o(s);N |o(x);N |o(s); |o(i);|o(x); |  ?Q1{N}2{N}N  |  ?Q1{N}N
			Rule::Chain(5, TS('n'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(7, TS('n'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(6, TS('o'), TS('('), TS('i'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(6, TS('o'), TS('('), TS('s'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(6, TS('o'), TS('('), TS('x'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(5, TS('o'), TS('('), TS('i'), TS(')'), TS(';')),
			Rule::Chain(5, TS('o'), TS('('), TS('s'), TS(')'), TS(';')),
			Rule::Chain(5, TS('o'), TS('('), TS('x'), TS(')'), TS(';')),
			Rule::Chain(11, TS('?'), NS('Q'), TS('1'), TS('{'), NS('N'), TS('}'), TS('2'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(7, TS('?'), NS('Q'), TS('1'), TS('{'), NS('N'), TS('}'), NS('N'))
		),
		Rule(
			NS('E'), GRB_ERROR_SERIES + 2,    // ������ � ���������
			16,                                // E->i | s | x | (E) | xM | i(W) | iM | nM | (E)M | i(W)M | p(i,x) | p(x,i)| p(x,x) |p(i,i) |c(s) |c(i) 
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('s')),
			Rule::Chain(1, TS('x')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(2, TS('x'), NS('M')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('n'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M')),
			Rule::Chain(6, TS('p'), TS('('), TS('i'), TS(','), TS('x'), TS(')')),
			Rule::Chain(6, TS('p'), TS('('), TS('x'), TS(','), TS('x'), TS(')')),
			Rule::Chain(6, TS('p'), TS('('), TS('i'), TS(','), TS('i'), TS(')')),
			Rule::Chain(6, TS('p'), TS('('), TS('i'), TS(','), TS('x'), TS(')')),
			Rule::Chain(4, TS('c'), TS('('), TS('i'), TS(')')),
			Rule::Chain(4, TS('c'), TS('('), TS('s'), TS(')'))
		),
		Rule(
			NS('F'), GRB_ERROR_SERIES + 3,    // ������ � ���������� �������
			2,                                // F -> ti | ti,F
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		Rule(
			NS('W'), GRB_ERROR_SERIES + 4,    // ������ � ���������� ���������� ������� 
			6,                                // W -> i | s | x | i,W | s,W | x,W 
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('s')),
			Rule::Chain(1, TS('x')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('s'), TS(','), NS('W')),
			Rule::Chain(3, TS('x'), TS(','), NS('W'))
		),
		Rule(
			NS('M'), GRB_ERROR_SERIES + 5,    // ��������
			3,								  // M -> a | aE | aEM 
			Rule::Chain(1, TS('a')),
			Rule::Chain(2, TS('a'), NS('E')),
			Rule::Chain(3, TS('a'), NS('E'), NS('M'))
		),
		Rule(

			NS('Q'), GRB_ERROR_SERIES + 6,
			2,									// Q->  (R<R)  |  (R>R)
			Rule::Chain(5, TS('('), NS('R'), TS('<'), NS('R'), TS(')')),
			Rule::Chain(5, TS('('), NS('R'), TS('>'), NS('R'), TS(')'))
		),

		Rule(
			NS('R'), GRB_ERROR_SERIES + 7,
			2,									// R->  i  |  x
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('d'))
		)
	);

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)  //����������� ������� - ������ ����� �������(���-�� �������� � �������, �������� ��� ����������...)
	{
		nt = new GRBALPHABET[chainLenght = psize];
		int* o = (int*)&s;
		for (short i = 0; i < psize; ++i) {
			nt[i] = (GRBALPHABET)o[i];
		}
	};

	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...) //����������� �������
	{															//(����������, ������������� ���������������� ���������, ���������� �������(������ ������ �������), ��������� ������� (������ ������ �������)
		nn = pnn;
		idDiagnosticError = piderror;
		chains = new Chain[nRightChain = psize];
		Chain* o = &c;
		for (int i = 0; i < nRightChain; i++) {
			chains[i] = o[i];
		}
	};
	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)//����������� ���������� ��������(��������� ������, ��� �����, ���������� ������, �������...)
	{
		startN = pstartN;
		stbottomT = pstbottom;
		rules = new Rule[nRule = psize];
		Rule*o = &r;
		for (int i = 0; i < nRule; i++) {
			rules[i] = o[i];
		}
	};

	Greibach getGreibach()
	{
		return greibach;
	};

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule) //�������� ������� �� �����������
	{
		short rc = -1;
		short k = 0;
		while (k < nRule && rules[k].nn != pnn)
			k++;   //���� � ������ ���������� ������ � ���� ����� ������ ������� �� ����� �������� �-���
		if (k < nRule)
			prule = rules[rc = k];    //������������ ������� ��������� ����� ������� � �������� �
		return rc; //������������ ����� ������� ��� -1
	};

	Rule Greibach::getRule(short n) //�������� ������� �� ������
	{
		Rule rc;
		if (n < nRule)
			rc = rules[n];
		return rc;
	};

	char* Rule::getCRule(char* b, short nchain) //�������� ������� � ���� N->������� (�����, ����� �������(������ �����) � �������)
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		chains[nchain].getRightChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);//��������� ������ (����, � ������ ��������, ������)
		return b;
	};

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j) //!!!�������� ��������� �� j ���������� �������, ������� � ����� ��� -1 
	{                                                                //(������ ������ �������, ������������ �������, ����� �������)
		short rc = -1;
		while (j < nRightChain && chains[j].nt[0] != t)
			++j;
		rc = (j < nRightChain ? j : -1);               //���������� ����� ������� ����������
		if (rc >= 0)pchain = chains[rc];
		return rc;
	};

	char*Rule::Chain::getRightChain(char*b)
	{
		for (int i = 0; i < chainLenght; i++)
			b[i] = Chain::alphabet_to_char(nt[i]);//���������� ������ �� �������� ����� �������
		b[chainLenght] = 0x00;
		return b; 
	};
};