#include "Enemy.h"

Enemy::~Enemy() { delete _model; }

void Enemy::Initalize(ViewProjection* viewProjection, const Vector3& position) {
	_model = Model::CreateFromOBJ("Player", true);
	_viewProjection = viewProjection;
	_worldTransform.Initialize();
	_worldTransform.translation_ = position;
	_worldTransform.rotation_.y = 180 * acosf(-1) / 180;

	_velocity.x = -kWalkSpeed;

	_walkTime = 0;
	_isAnimChange = false;
	_worldTransform.rotation_.z = kWalkMotionAngleStart * acosf(-1) / 180;
}

void Enemy::Update() {
	// 回転アニメーション
	if (_walkTime < kWalkMotionTime) {
		_walkTime += 1 / 60.f;
		if (_walkTime >= kWalkMotionTime) {
			_walkTime = 0;
			_isAnimChange = !_isAnimChange;
		}
	}
	float startRadian = kWalkMotionAngleStart * acosf(-1) / 180;
	float endRadian = kWalkMotionAngleEnd * acosf(-1) / 180;
	if (!_isAnimChange)
		_worldTransform.rotation_.z = std::lerp(startRadian, endRadian, _walkTime / kWalkMotionTime);
	else
		_worldTransform.rotation_.z = std::lerp(endRadian, startRadian, _walkTime / kWalkMotionTime);
	//  移動
	_worldTransform.translation_ += _velocity;

	_worldTransform.UpdateMatrix(); // 行列計算

	_aabb = My3dTools::GetAABB(_kWidth, _kHeight, _kWidth, GetWorldPosition());
}

void Enemy::Draw() { _model->Draw(_worldTransform, *_viewProjection); }

const Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos{};
	worldPos.x = _worldTransform.matWorld_.m[3][0];
	worldPos.y = _worldTransform.matWorld_.m[3][1];
	worldPos.z = _worldTransform.matWorld_.m[3][2];
	return worldPos;
}
