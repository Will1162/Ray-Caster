#ifndef WB_RT_VEC3_HPP
#define WB_RT_VEC3_HPP

namespace WB_RT
{
	class Vec3
	{
		public:
			float x;
			float y;
			float z;

			Vec3(float x, float y, float z);
			float Length();
			Vec3 Normalise();
			float Dot(Vec3 other);
			Vec3 Cross(Vec3 other);
			float AngleBetween(Vec3 other);

			friend Vec3 operator+(Vec3 a, Vec3 b);
			friend Vec3 operator-(Vec3 a, Vec3 b);
			friend Vec3 operator*(Vec3 a, float b);
	};
}

#endif