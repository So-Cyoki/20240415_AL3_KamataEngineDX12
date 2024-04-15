#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// Sprite
	uint32_t _textureHandle = 0;
	Sprite* _sprite = nullptr;
	float _speedX = 1;
	float _speedY = 1;
	// Model
	uint32_t _textureHandle_model = 0;
	Model* _model = nullptr;
	WorldTransform _worldTransform;
	ViewProjection _viewProjection;
	// Sound
	uint32_t _soundDataHandle = 0;
	uint32_t _voiceHandle = 0;
	// ImGui
	float _scaleFloat[3] = {1, 1, 1};
	// DebugCamera
	DebugCamera* _debugCamera = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
