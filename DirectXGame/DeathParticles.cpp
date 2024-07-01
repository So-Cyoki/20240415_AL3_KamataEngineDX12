#include "DeathParticles.h"

DeathParticles::~DeathParticles() { delete _model; }

void DeathParticles::Initalize(ViewProjection* viewProjection) {
	_model = Model::CreateFromOBJ("Ball", true);
	_viewProjection = viewProjection;
	int tempIndex = 0;
	for (auto& worldTransform : _worldTransforms) {
		worldTransform.Initialize();
		worldTransform.scale_ = {0.5f, 0.5f, 0.5f};
		// worldTransform.translation_ = position;
		tempIndex++;
	}

	_objectColor.Initialize();
	_color = {1, 1, 1, 1};
}

void DeathParticles::Update() {
	if (_isTimeFinished || !_isStart)
		return;
	// Move
	int tempIndex = 0;
	for (auto& worldTransform : _worldTransforms) {
		float angle = _kAngleUnit * tempIndex;        // 角度を計算
		Vector3 dir = {cosf(angle), sinf(angle), 0};  // 方向
		worldTransform.translation_ += dir * _kSpeed; // 移動
		worldTransform.UpdateMatrix();
		tempIndex++;
	}

	// Time
	if (_currentTime < _kDuration)
		_currentTime += 1 / 60.f;
	else
		_isTimeFinished = true;

	// Color
	_color.w = 1 - std::clamp(_currentTime / _kDuration, 0.f, 1.f);
	_objectColor.SetColor(_color);
	_objectColor.TransferMatrix();
}

void DeathParticles::Draw() {
	if (_isTimeFinished || !_isStart)
		return;
	for (auto& worldTransform : _worldTransforms) {
		_model->Draw(worldTransform, *_viewProjection, &_objectColor);
	}
}

void DeathParticles::SetStartPos(Vector3 pos) {
	for (auto& worldTransform : _worldTransforms) {
		worldTransform.translation_ = pos;
	}
}

const bool DeathParticles::GetParticlesOver() {
	if (_isTimeFinished)
		return true;
	return false;
}
