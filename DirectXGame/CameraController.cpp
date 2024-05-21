#include "CameraController.h"

void CameraController::Initialize() { _viewProjection.Initialize(); }

void CameraController::Update() {
	// 追従対象の位置を参照する
	const WorldTransform& targetWorldTransform = _target->GetWorldTransform();
	_targetPos = targetWorldTransform.translation_ + _targetOffset + (_target->GetVeloctiy() * kVelocityRate);
	// 座標補完によりゆったり追従
	_viewProjection.translation_.x = std::lerp(_viewProjection.translation_.x, _targetPos.x, kInterpolationRate);
	_viewProjection.translation_.y = std::lerp(_viewProjection.translation_.y, _targetPos.y, kInterpolationRate);
	_viewProjection.translation_.z = std::lerp(_viewProjection.translation_.z, _targetPos.z, kInterpolationRate);
	// ターゲットはカメラ内に制限
	_viewProjection.translation_.x = std::clamp(_viewProjection.translation_.x, _targetPos.x + _targetArea.left, _targetPos.x + _targetArea.right);
	_viewProjection.translation_.y = std::clamp(_viewProjection.translation_.y, _targetPos.y + _targetArea.bottom, _targetPos.y + _targetArea.top);
	// 移動範囲制限
	_viewProjection.translation_.x = std::clamp(_viewProjection.translation_.x, _movableArea.left, _movableArea.right);
	_viewProjection.translation_.y = std::clamp(_viewProjection.translation_.y, _movableArea.bottom, _movableArea.top);
	// 更新する
	_viewProjection.UpdateMatrix();
}

void CameraController::Reset() {
	const WorldTransform& targetWorldTransform = _target->GetWorldTransform();
	_viewProjection.translation_ = targetWorldTransform.translation_ + _targetOffset;
}
