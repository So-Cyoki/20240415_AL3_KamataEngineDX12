#include "Player.h"

void Player::Initalize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {
	assert(model);
	_model = model;
	_textureHandle = textureHandle;
	_viewProjection = viewProjection;
	_worldTransform.Initialize();
}

void Player::Update() { _worldTransform.TransferMatrix(); }

void Player::Draw() { _model->Draw(_worldTransform, *_viewProjection, _textureHandle); }
