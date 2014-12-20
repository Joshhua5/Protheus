#pragma once
namespace Pro {
	namespace Math {
		const float pi = 3.1415926535897932384626433832795028841971f;

		static inline float radToDeg(const float rad) {
			return (rad * 180) / pi; 
		}

		static inline float degToRad(const float deg) {
			return (deg * pi) / 180;
		}
	}
}