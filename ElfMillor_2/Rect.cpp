#include "Rect.h"

Vec3 Rect::getCenter()
{
    return Vec3((left + right) / 2, (top + bottom) / 2);
}

bool Rect::isCol(const Rect& rect)
{
	if (top > rect.bottom)	return false;
	if (bottom < rect.top)	return false;
	if (left > rect.right)	return false;
	if (right < rect.left)	return false;

	return true;
}