#pragma once
#include "Field.h"
#include <array>

class Snake
{
public:
	enum class Direction
	{
		Up,
		Down,
		Left,
		Right
	};
	static constexpr unsigned length = 5;
public:
	Snake()
	{
		Reset();
	}
public:
	void Reset()
	{
		for (auto& i : snek)
		{
			i = Direction::Right;
		}
		head.first = 2;
		head.second = 5;
		dir = Direction::Right;
		headi = 0;
		field.Reset();
	}
	void DrawSnake()
	{
		std::pair<unsigned, unsigned> xhead = head;
		
		for (unsigned i = 0, x = headi; i < length; i++, x = x==0?length-1:(x - 1))
		{
			field.PutPixel(xhead.first, xhead.second);
			switch (snek[x])
			{
			case Direction::Right: xhead.second = (xhead.second - 1) % Field::cols;break;
			case Direction::Left:  xhead.second = (xhead.second + 1) % Field::cols; break;
			case Direction::Up:	   xhead.first = (xhead.first + 1) % Field::rows; break;
			case Direction::Down:  xhead.first = (xhead.first - 1) % Field::rows; break;
			default:
				break;
			}
		}
	}
	bool CheckState()
	{
		uint8_t div = 0;
		auto x = dir;
		for(unsigned i = 0, r = headi; i < length; i++, r = r==0?length-1:(r - 1))
		{
			if (snek[r] == x)div=0;
			div++;
			if (div == 4) return false;
			x = snek[r];
		}
		return true;
	}
	void Render()
	{
		DrawSnake();
		field.Render();
	}
	void Step()
	{
		field.Reset();
		switch (dir)
		{
		case Direction::Right: head.second = (head.second + 1) % Field::cols; break;
		case Direction::Left:  head.second = (head.second - 1) % Field::cols; break;
		case Direction::Up:	   head.first = (head.first - 1) % Field::rows; break;
		case Direction::Down:  head.first = (head.first + 1) % Field::rows; break;
		default:
			break;
		}
		headi = (headi + 1) % length;
		snek[headi] = dir;
	}
	void ChangeDir(Direction xdir)
	{
		dir = xdir;
	}
private:
	Field field;
	std::pair<unsigned, unsigned> head {2, 5}
	;
	std::array<Direction, length> snek {}
	;
	Direction dir = Direction::Right;
	unsigned headi = 0;
};
