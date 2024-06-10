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
	// 1.移動入力
	Move();
	// 2.移動量を加味して衝突判定する
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.move = _velocity;
	MapCollision(collisionMapInfo);
	// 3.判定結果を反映して移動させる
	_worldTransform.translation_ += collisionMapInfo.move;
	// 4.天井に接触している場合の処理
	if (collisionMapInfo.ceiling) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		_velocity.y = 0;
	}
	// 5.壁に接触している場合の処理
	// 6.接地状態の切り替え
	// 7.旋回制御
	Rotating();
	// 8.行列計算
	_worldTransform.UpdateMatrix(); // 行列計算
}

void Player::Draw() { _model->Draw(_worldTransform, *_viewProjection); }

void Player::Move() {
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
}

void Player::Rotating() {
	// 旋回制御
	if (_turnNowFram >= 1 && _turnNowFram < _turnEndFrame) {
		_turnNowFram++;
		float easing = powf(float(_turnNowFram) / float(_turnEndFrame), 3);
		_worldTransform.rotation_.y = _turnUseRotationY * easing + _turnStartRotationY;
	}
}

void Player::MapCollision(CollisionMapInfo& info) { MapCollision_Up(info); }

void Player::MapCollision_Up(CollisionMapInfo& info) {
	// 移動後の４つ角の座標
	std ::array<Vector3, static_cast<int>(kNumCorner)> positionsNew{};
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(_worldTransform.translation_ + info.move, static_cast<Corner>(i));
	}
	// もし上に移動してないなら、判断の必要がない
	if (info.move.y <= 0)
		return;

	// 当たりかどうか？
	bool hit = false;
	MapChipField::IndexSet indexSet{};
	MapChipType mapChipType{};
	// 左上の角
	indexSet = _mapChipField->GetMapChipIndexByPosition(positionsNew[kLeftTop]);
	mapChipType = _mapChipField->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上の角
	indexSet = _mapChipField->GetMapChipIndexByPosition(positionsNew[kRightTop]);
	mapChipType = _mapChipField->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = _mapChipField->GetMapChipIndexByPosition(positionsNew[kLeftTop]);
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = _mapChipField->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		float moveY = rect.bottom - positionsNew[kLeftTop].y - _kHeight / 2 + _kBlank;
		info.move.y = max(0.0f, moveY);
		// 天井に当たったことを記録する
		info.ceiling = true;
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	float offest = 1; // KAMATAエンジンの凡ミスなので、ちょっとoffestを加えた方がいい
	Vector3 offsetTable[Player::kNumCorner] = {
	    {_kWidth / 2 + offest, -_kHeight / 2 + offest, 0}, //  kRightBottom
	    {-_kWidth / 2,         -_kHeight / 2,          0}, //  kLeftBottom
	    {_kWidth / 2 + offest, _kHeight / 2 + offest,  0}, //  kRightTop
	    {-_kWidth / 2,         _kHeight / 2,           0}  //  kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}
