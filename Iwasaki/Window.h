#pragma once
#include	"../Matsunaga/Vector/Vector.h"

class Window {
public:
	static void Init(Vector2 pos, int color_bit) { m_pos = pos; m_color_bit = color_bit; }

	static Vector2 GetPos() { return m_pos; }
	static int GetBit()  { return m_color_bit; }

private:
	static Vector2 m_pos;
	static int m_color_bit;
};