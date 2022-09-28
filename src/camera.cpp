#include <math.h>

#include "camera.hpp"

namespace WB_RT
{
	Camera::Camera(Vec3 pos, Vec3 rot, float fov)
	{
		this->pos = pos;
		this->rot = rot;
		this->fov = fov;
		focalLength = 1.0f / tan(fov * atan(1) / 90.0f);
	}
}