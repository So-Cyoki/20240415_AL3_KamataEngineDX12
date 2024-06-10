#pragma once
#include "DebugText.h"
#include "Input.h"
#include "MapChipField.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <ImGuiManager.h>
#include <algorithm>
#include <cmath>
class Player {
private:
	WorldTransform _worldTransform;
	ViewProjection* _viewProjection = nullptr;
	Model* _model = nullptr;

	MapChipField* _mapChipField = nullptr; // どんなマップチップを使っている
	const float _kWidth = 2;               // Playerのサイズ
	const float _kHeight = 2;
	static inline const float _kBlank = 0.1f; // 当たったら微小な余白
	// マップと当たり情報
	struct CollisionMapInfo {
		bool ceiling = false; // isTop
		bool landing = false; // isGround
		bool isWall = false;  // isWall
		Vector3 move{};
	};
	// 角
	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上
		kNumCorner    // 要素数
	};

	Vector3 _velocity{};
	const float _kAcceleration = 0.1f;
	const float _kAttenuation = 0.5f;   // 速度減衰
	const float _kLimitRunSpeed = 0.7f; // 最大速度

	enum class LRDirection {
		// 向く
		kRight,
		kLeft,
	};
	LRDirection _lrDirection = LRDirection::kRight;
	float _turnStartRotationY = 0; // 旋回時の角度
	float _turnUseRotationY = 0;   // 旋回したら、角度はいくら
	int _turnNowFram = 0;
	const int _turnEndFrame = 30;

	bool _isJump = false;
	bool _isGround = true;
	const float _kGravityAcceleration = 0.05f;
	const float _kLimitFallSpeed = 0.5;
	const float _kJumpAcceleration = 1.f;

public:
	~Player();
	void Initalize(ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	const WorldTransform& GetWorldTransform() { return _worldTransform; };
	const Vector3& GetVeloctiy() { return _velocity; };
	void SetMapChipField(MapChipField* mapChipField) { _mapChipField = mapChipField; };

private:
	void Move();
	void Rotating();

	void MapCollision(CollisionMapInfo& info);
	void MapCollision_Up(CollisionMapInfo& info);
	Vector3 CornerPosition(const Vector3& center, Corner corner);
};