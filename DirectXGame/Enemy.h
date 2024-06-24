#pragma once
#include "DebugText.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <cmath>
class Enemy {
private:
	WorldTransform _worldTransform;
	ViewProjection* _viewProjection = nullptr;
	Model* _model = nullptr;

	static inline const float kWalkSpeed = 0.1f; // speed
	Vector3 _velocity{};

	static inline const float kWalkMotionAngleStart = -30; // 最初の角度「度」
	static inline const float kWalkMotionAngleEnd = 30;    // 最後の角度「度」
	static inline const float kWalkMotionTime = 1;         // アニメーションの周期「秒」
	float _walkTime = 0;                                   // 経過時間
	bool _isAnimChange = false;

public:
	~Enemy();
	void Initalize(ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();
};
