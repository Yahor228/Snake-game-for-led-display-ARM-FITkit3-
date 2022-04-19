#pragma once
#include "System.h"
#include <array>

class Field
{
public:
	static constexpr auto rows = 8;
	static constexpr auto cols = 16;
public:
	void PutPixel(unsigned row, unsigned col)
	{
		field[col] |= 1 << row;
	}
	void Render()
	{
		for (unsigned i = 0; i < cols; i++)
		{
			unsigned char c = field[i];
			PortA p; 
			p |= column_select(i);
			p |= xrow_select(c);
			p.apply();
		}
	}
	void Reset()
	{
		__builtin_memset(field.data(), 0, field.size());
	}
private:
	/* Conversion of requested column number into the 4-to-16 decoder control.  */
	static unsigned column_select(unsigned col_num)
	{
		unsigned result = 0;

		for (unsigned i = 0; i < 4; i++, col_num = col_num / 2) 
		{
			switch (i) {

				// Selection signal A0
			case 0:
				result |= (col_num % 2) ? GPIO_PIN(8) : 0;
				break;

				// Selection signal A1
			case 1:
				result |= (col_num % 2) ? GPIO_PIN(10) : 0;
				break;

				// Selection signal A2
			case 2:
				result |= (col_num % 2) ? GPIO_PIN(6) : 0;
				break;

				// Selection signal A3
			case 3:
				result |= (col_num % 2) ? GPIO_PIN(11) : 0;
				break;
			default:
				break;
			}
		}
		return result;
	}
	static unsigned xrow_select(unsigned char row)
	{
		unsigned res = 0;
		if (row & 1)res |= GPIO_PIN(26);
		if (row & 2)res |= GPIO_PIN(24);
		if (row & 4)res |= GPIO_PIN(9);
		if (row & 8)res |= GPIO_PIN(25);
		if (row & 16)res |= GPIO_PIN(28);
		if (row & 32)res |= GPIO_PIN(7);
		if (row & 64)res |= GPIO_PIN(27);
		if (row & 128)res |= GPIO_PIN(29);
		return res;
	}
private:
	std::array<unsigned char, cols> field {}
	;
};
