#pragma once
#include "Vector3.h"
#include <cmath>

// 軸平行境界箱(Vector3 min、Vector3 max)
struct AABB {
	Vector3 min;
	Vector3 max;
};

class My3dTools {
public:
	// 物理計算
	static bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
		if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z))
			return true;

		return false;
	};

	// ツール
	static AABB GetAABB(float width, float height, float depth, Vector3 worldPos) {
		AABB aabb{};
		aabb.min = {worldPos.x - width / 2, worldPos.y - height / 2, worldPos.z - depth / 2};
		aabb.max = {worldPos.x + width / 2, worldPos.y + height / 2, worldPos.z + depth / 2};
		return aabb;
	};
};