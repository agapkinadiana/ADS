#pragma once
#include "Log.h"
#define LT_NOPRIORITY 0xffffffff		//��� ����������
#define LEXEMA_FIXZISE 1				//������������� ������ �������
#define LT_MAXSIZE 4096			//������������ ���-�� ����� � ������� ������   
#define TL_TI_NULLIDX 0xffffffff		//��� �������� ������� ���������������
#define LEX_T			't'				//	
#define LEX_INT			'x'				//	int
#define LEX_STR			's'				//	string
#define LEX_NEW			'n'				//	new
#define LEX_FUNC		'f'				//	function
#define LEX_MAIN		'm'				//	main
#define LEX_RET			'r'				//	ret
#define LEX_LITERAL		'l'				//	�������
#define LEX_PRINT		'o'				//	print
#define LEX_POW			'p'				//	pow
#define LEX_LENGTH		'c'				//	strlen
#define LEX_ID			'i'				//	�������������
#define LEX_IF          '?'
#define LEX_ELSE        '2'
#define LEX_THEN        '1'
#define LEX_ACTION		'a'

#define LEX_SEMICOLON	';'				//	;
#define LEX_COLON		':'				//	:
#define LEX_COMMA		','				//	,
#define LEX_LEFTHESIS	'('				//	(
#define LEX_RIGHTHESIS	')'				//	)
#define LEX_EQUAL		'='				//  =
#define LEX_LEFTBRACE	'{'				//	{
#define LEX_RIGHTBRACE	'}'				//	}
#define LEX_LESS		'<'
#define LEX_MORE        '>'
#define LEX_LEFTSQBRACE		'['				//	[
#define LEX_RIGHTSQBRACE	']'				//	]
#define LEX_PLUS		'+'				//	+
#define LEX_MINUS		'-'				//	-
#define LEX_MULTIPLY	'*'				//	*
#define LEX_STRLIT		'"'				//  "
#define LEX_SUBST		'@'
#define LEX_L			'L'				//  L
#define LEX_SEP			'|'				// ������ ����� ������

namespace LT
{					// ������� ������
	struct Entry
	{
		char lexema;					// �������
		int lineNo;						// ����� ������ � �������� ����
		int indexTI;					// ������ � ������� ��������������� 
		short priority;					// ��������� ��� ��������
	};

	struct LexTable
	{
		int maxize;						// ������� ������� ������ 
		int size;						// ������� ������ ������� ������
		Entry* table;					// ������ ����� ������� ������	
	};

	LexTable Create(int size);
	Entry GetEntry(LexTable& lextable, int n);
	Entry Add(LexTable& lextable, Entry entry);
	void ShowTable(LexTable& lexTable);
	void Write(Log::LOG log, LexTable& lexTable);
}
