#ifndef WB_RT_CAMERA_HPP
#define WB_RT_CAMERA_HPP

#include "vec3.hpp"

namespace WB_RT
{
	class Camera
	{
		public:
			Vec3 pos = Vec3(0.0f, 0.0f, 0.0f);
			Vec3 rot = Vec3(0.0f, 0.0f, 0.0f);
			float fov;
			float focalLength;

			Camera(Vec3 pos, Vec3 rot, float fov);
	};
}

#endif