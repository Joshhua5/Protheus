/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.

*************************************************************************/
#pragma once
#include <utility>


namespace Pro {
	template<typename T>
	struct Vector3 {
		T x, y, z;

		Vector3() {}

		Vector3(const T& value) {
			x = y = z = value;
		}

		Vector3(T _x, T _y, T _z) {
			x = _x;
			y = _y;
			z = _z;
		}

		Vector3(std::initializer_list<T> list) {
			if (list.size() < 2) {
				x = y = z = 0;
				return;
			}
			x = *list.begin();
			y = *(list.begin() + 1);
			z = *(list.begin() + 2);
		}

		Vector3(Vector3&& rhs) {
			x = std::move(rhs.x);
			y = std::move(rhs.y);
			z = std::move(rhs.z);
		}

		Vector3(const Vector3& rhs) {
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
		}

		void operator=(const Vector3& rhs) {
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
		}

		void operator=(Vector3&& rhs) {
			x = std::move(rhs.x);
			y = std::move(rhs.y);
			z = std::move(rhs.z);
		}

		void operator+=(const Vector3& rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
		}
		void operator-=(const Vector3& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
		}
		void operator/=(const Vector3& rhs) {
			x /= rhs.x;
			y /= rhs.y;
			z /= rhs.z;
		}
		void operator*=(const Vector3& rhs) {
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
		}

		bool operator==(const Vector3& rhs) const {
			return x == rhs.x && y == rhs.y && z == rhs.z;
		}

		bool equals(T _x, T _y, T _z) const{
			return x == _x && y == _y && z == _z;
		}


		Vector3&& operator+(const Vector3& rhs) {
			Vector3<T> out;
			out.x = x + rhs.x;
			out.y = y + rhs.y;
			out.z = z + rhs.z;
			return std::move(out);
		}
		Vector3&& operator-(const Vector3& rhs) {
			Vector3<T> out;
			out.x = x - rhs.x;
			out.y = y - rhs.y;
			out.z = z - rhs.z;
			return std::move(out);
		}
		Vector3&& operator*(const Vector3& rhs) {
			Vector3<T> out;
			out.x = x * rhs.x;
			out.y = y * rhs.y;
			out.z = z * rhs.z;
			return std::move(out);
		}
		Vector3&& operator/(const Vector3& rhs) {
			Vector3<T> out;
			out.x = x / rhs.x;
			out.y = y / rhs.y;
			out.z = z / rhs.z;
			return std::move(out);
		}



	};
}
