#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <algorithm>
#include <array>
#include <cmath>
class DeathParticles {
private:
	static inline const uint32_t kNumParticles = 8;
	std::array<WorldTransform, kNumParticles> _worldTransforms;
	ViewProjection* _viewProjection = nullptr;
	Model* _model = nullptr;

	ObjectColor _objectColor; // 色変更オブジェクト
	Vector4 _color;           // 色

	static inline const float _kDuration = 1.5f;                           // 存続時間<秒>
	static inline const float _kSpeed = 0.4f;                              // スピード
	static inline const float _kAngleUnit = acosf(-1) * 2 / kNumParticles; // 分割下1個分の角度

	bool _isTimeFinished = false;
	float _currentTime = 0;
	bool _isStart = false;

public:
	~DeathParticles();
	void Initalize(ViewProjection* viewProjection);
	void Update();
	void Draw();

	void SetIsStart(bool flag) { _isStart = flag; };
	void SetStartPos(Vector3 pos);
	const bool GetParticlesOver();
};
