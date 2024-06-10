#pragma once
#include "Player.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <algorithm>

namespace CameraTools {
// 矩形(Left,Right,Top,Bottom)
struct Rect {
	float left = 0;
	float right = 1;
	float top = 1;
	float bottom = 0;
};
} // namespace CameraTools

using namespace CameraTools;
class CameraController {
	// 基礎属性
	ViewProjection _viewProjection;        // 自身の位置(カメラだから、ViewProjectionにしないと)
	Vector3 _targetPos{};                  // 目標の位置
	Vector3 _targetOffset = {0, 0, -50};   // カメラとターゲットの距離の差
	Rect _movableArea;                     // カメラの移動範囲制限(L,R,T,B)
	const float kInterpolationRate = 0.2f; // 座標補間
	const float kVelocityRate = 10;        // 速度掛け率
	Rect _targetArea = {0, 100, 100, 0};   // ターゲットはカメラ内に映すって範囲制限

	Player* _target = nullptr;

public:
	void Initialize();
	void Update();
	void SetTarget(Player* target) { _target = target; };
	void Reset();
	void SetMovableArea(Rect area) { _movableArea = area; };

	const ViewProjection& GetViewProjection() { return _viewProjection; };
};
