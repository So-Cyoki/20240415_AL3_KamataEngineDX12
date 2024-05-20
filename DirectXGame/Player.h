#pragma once
#include "Input.h"
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

	Vector3 _velocity{};
	const float _kAcceleration = 0.1f;
	const float _kAttenuation = 0.5f;   // 速度減衰
	const float _kLimitRunSpeed = 0.7f; // 最大速度

	enum class LRDirection { // 向く
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
};