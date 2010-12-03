//Mathematical representation of Geometrical Point

#include "Vector.h"

template <typename T>
class Cartesian : public Vector3x1<T>
{
public:

	Cartesian(T x, T y, T z) : Vector3x1<T>(x,y,z)
	{
	}
};