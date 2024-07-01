#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class TitleScene {
private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	ViewProjection _viewProjection;
	WorldTransform _worldTransform_title;
	WorldTransform _worldTransform_player;
	Model* _modelTitle = nullptr;
	Model* _modelPlayer = nullptr;

	float _titleSpeed = 0.03f;
	float _titleMoveTime = 1.5f;
	float _currentTime = 0;

public:
	~TitleScene();
	void Initalize();
	void Update();
	void Draw();
};
