#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "MapChipField.h"
#include "Model.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>

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

	// 自分の変数
	ViewProjection _viewProjection;
	Model* _model = nullptr;
	std::vector<std::vector<WorldTransform*>> _worldTransformBlocks;
	bool _isDebugCameraActrive = false;
	DebugCamera* _debugCamera = nullptr;
	Skydome* _skydomeObj = nullptr;
	// Player
	WorldTransform _worldTransform_player;
	Model* _model_player = nullptr;
	// MapChip
	MapChipField* _mapChipField;
	void GenerateBlocks(); // ブロック初期化と生成

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
