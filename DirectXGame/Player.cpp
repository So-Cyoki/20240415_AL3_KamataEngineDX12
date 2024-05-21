#include "Player.h"

Player::~Player() { delete _model; }

void Player::Initalize(ViewProjection* viewProjection, const Vector3& position) {
	_model = Model::CreateFromOBJ("Player", true);
	_viewProjection = viewProjection;
	_worldTransform.Initialize();
	_worldTransform.translation_ = position;
	//_worldTransform.rotation_.y = 90 * std::numbers::pi_v<float> / 180; // pi_v = pi(3.1415927f)
}

void Player::Update() {
	// 移動
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		Vector3 acceleration{};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			if (_lrDirection != LRDirection::kRight) {
				_lrDirection = LRDirection::kRight;
				_turnStartRotationY = _worldTransform.rotation_.y;
				_turnUseRotationY = 0 - _worldTransform.rotation_.y;
				_turnNowFram = 1;
			}
			if (_velocity.x < 0) {
				_velocity.x *= (1 - _kAttenuation);
			}
			acceleration.x += _kAcceleration;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			if (_lrDirection != LRDirection::kLeft) {
				_lrDirection = LRDirection::kLeft;
				_turnStartRotationY = _worldTransform.rotation_.y;
				_turnUseRotationY = acosf(-1) - _worldTransform.rotation_.y;
				_turnNowFram = 1;
			}
			if (_velocity.x > 0) {
				_velocity.x *= (1 - _kAttenuation);
			}
			acceleration.x -= _kAcceleration;
		}

		_velocity += acceleration;
		_velocity.x = std::clamp(_velocity.x, -_kLimitRunSpeed, _kLimitRunSpeed);
	} else {
		_velocity.x *= (1 - _kAcceleration);
	}
	// 跳ぶ
	if (!_isJump) {
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			_isJump = true;
			_isGround = false;
			_velocity += Vector3(0, _kJumpAcceleration, 0);
		}
	} else {
		_velocity += Vector3(0, -_kGravityAcceleration, 0);
		_velocity.y = max(_velocity.y, -_kLimitFallSpeed);
	}
	if (_velocity.y < 0) {
		if (_worldTransform.translation_.y <= 2) {
			_isJump = false;
			_isGround = true;
			_worldTransform.translation_.y = 2;
			_velocity.y = 0;
		}
	}

	_worldTransform.translation_ += _velocity;

	// 旋回制御
	if (_turnNowFram >= 1 && _turnNowFram < _turnEndFrame) {
		_turnNowFram++;
		float easing = powf(float(_turnNowFram) / float(_turnEndFrame), 3);
		_worldTransform.rotation_.y = _turnUseRotationY * easing + _turnStartRotationY;
	}

	_worldTransform.UpdateMatrix();
}

void Player::Draw() { _model->Draw(_worldTransform, *_viewProjection); }
