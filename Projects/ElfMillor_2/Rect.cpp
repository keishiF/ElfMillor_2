#include "Rect.h"

Vec3 Rect::GetCenter()
{
	return Vec3((left + right) * 0.5f, (top + bottom) * 0.5f);
}

bool Rect::IsCol(const Rect& rect)
{
	if (top > rect.bottom)	return false;
	if (bottom < rect.top)	return false;
	if (left > rect.right)	return false;
	if (right < rect.left)	return false;

	return true;
}