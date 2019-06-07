#include "pch.h"

namespace Error
{
	//	0   -  99  -  ���������
	//	100 - 109  -  ����������
	//	110 - 119  -  �������� � ������ ������
	//	120 - 199  -  ������ ������������ �������
	//  200 - 299  -  ������ ��������������� �������
	//  300 - 399  -  ������ �������������� �������

	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "������������ ��� ������"),
		ERROR_ENTRY(1, "��������� ����"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30),
		ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50), ERROR_ENTRY_NODEF10(60),
		ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),

		ERROR_ENTRY(100, "������ �������. �������� -in ������ ���� �����"),//+
		ERROR_ENTRY(101, "������ �������. ��������� ����� �������� ���������"),//+
		ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103), ERROR_ENTRY_NODEF(104), ERROR_ENTRY_NODEF(105),
		ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),

		ERROR_ENTRY(110, "������ �������� �������� �����. ������ ��� �������� ����� � �������� ����� (-in)"),//+
		ERROR_ENTRY(111, "������ �������� �������� �����. ������������ ������ � �������� ����� (-in)"),//+
		ERROR_ENTRY(112, "������ �������� �������� �����. ������ ��� �������� ����� ���������(-log)"),
		ERROR_ENTRY(113, "������ � ������ ���������� �������� - ������� �� ����� ������"),//+
		ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115), ERROR_ENTRY_NODEF(116),
		ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),

		ERROR_ENTRY(120, "[LA] ������� �������� �� ���������"),
		ERROR_ENTRY(121, "[LA] ������� ������ �����������"),
		ERROR_ENTRY(122, "[LA] ������� ��������������� �����������"),
		ERROR_ENTRY(123, "[LA] ������������ �������������� �������"),
		ERROR_ENTRY(124, "[LA] ������������ �������������� ��������"),
		ERROR_ENTRY_NODEF(125), ERROR_ENTRY_NODEF(126),
		ERROR_ENTRY_NODEF(127), ERROR_ENTRY_NODEF(128), ERROR_ENTRY_NODEF(129),
		ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),

		ERROR_ENTRY(200, "[SA] �������� ��������� ���������"),
		ERROR_ENTRY(201, "[SA] �������� �������������� �����������"),
		ERROR_ENTRY(202, "[SA] �������� ���������"),
		ERROR_ENTRY(203, "[SA] ������ � ���������� ������� ��� ��������� ����������"),
		ERROR_ENTRY(204, "[SA] ������ � ��������� ���������� ������� ����������� ����������"),
		ERROR_ENTRY(205, "[SA] ������ ��������������� ���������"),
		ERROR_ENTRY(206, "[SA] ������ � ��������� ��������� - ������� ������ �������"),
		ERROR_ENTRY(207, "[SA] ������ � �������� ���������"),
		ERROR_ENTRY_NODEF(208), ERROR_ENTRY_NODEF(209),
		ERROR_ENTRY_NODEF(210), ERROR_ENTRY_NODEF(211),
		ERROR_ENTRY(212, "[SA] ������� ��������� �� ��������� (���� �� ������)"),
		ERROR_ENTRY(213, "[SA] ����� ����� � ��������� �� ������"),
		ERROR_ENTRY_NODEF(214),
		ERROR_ENTRY(215, "[SA] �� ������ ����� �������"),
		ERROR_ENTRY_NODEF(216),
		ERROR_ENTRY_NODEF(217), ERROR_ENTRY_NODEF(218), ERROR_ENTRY_NODEF(219),
		ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230), ERROR_ENTRY_NODEF10(240),
		ERROR_ENTRY_NODEF10(250), ERROR_ENTRY_NODEF10(260), ERROR_ENTRY_NODEF10(270),
		ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),

		ERROR_ENTRY(300, "[SMA] ��������� ���������� ��������������"),
		ERROR_ENTRY(301, "[SMA] ������ � ���� ��������������"),
		ERROR_ENTRY(302, "[SMA] ������ � ������������ ��������� � �������"),
		ERROR_ENTRY_NODEF(303),
		ERROR_ENTRY(304, "[SMA] ��� ������ ���������� ��������� �� ������������� �������������� ��������������"),
		ERROR_ENTRY(305, "[SMA] ������ � ��������� ���������� ������� len ����������� ����������"),
		ERROR_ENTRY(306, "[SMA] ������������� �������������"),
		ERROR_ENTRY(307, "[SMA] �������������� ����� � ��������� ������������"),
		ERROR_ENTRY(308, "[SMA] ����� ����� � ��������� ������ ������"),
		ERROR_ENTRY_NODEF(309),

		ERROR_ENTRY_NODEF10(310), ERROR_ENTRY_NODEF10(320), ERROR_ENTRY_NODEF10(330),
		ERROR_ENTRY_NODEF10(340), ERROR_ENTRY_NODEF10(350), ERROR_ENTRY_NODEF10(360),
		ERROR_ENTRY_NODEF10(370), ERROR_ENTRY_NODEF10(380), ERROR_ENTRY_NODEF10(390),

		ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500), ERROR_ENTRY_NODEF100(600),
		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};

	ERROR GetError(int id)                                  //��� ������ ������� Error throw
	{
		if (id > ERROR_MAX_ENTRY || id < 0)
		{
			return errors[0];
		}
		return errors[id];
	}

	ERROR GetErrorIn(int id, int line = -1, int pos = -1)   //���� ����� ��� � �������
	{
		if (id > ERROR_MAX_ENTRY || id < 0)
		{
			return errors[0];
		}
		errors[id].intext.line = line;
		errors[id].intext.pos = pos;
		return errors[id];
	}
}