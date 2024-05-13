#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete _model;
	delete _debugCamera;
	delete _skydomeObj;
	delete _model_player;
	// ブロックの容器の内容を一切クリアする
	for (std::vector<WorldTransform*>& line : _worldTransformBlocks) {
		for (WorldTransform* row : line) {
			delete row;
		}
	}
	_worldTransformBlocks.clear();
	delete _mapChipField;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// MapChip
	_mapChipField = new MapChipField();
	_mapChipField->LoadMapChipCsv("Resources/MapChip/map_01.csv");
	GenerateBlocks();

	// Obj
	_debugCamera = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight); // DebugCamera
	_model = Model::Create();                                                    // TempModel
	_viewProjection.Initialize();
	_skydomeObj = new Skydome(); // SkyDome
	_skydomeObj->Initialize(&_viewProjection);
	_worldTransform_player.Initialize(); // Player
	_worldTransform_player.translation_ = {2, 2, 0};
	_model_player = Model::CreateFromOBJ("Player", true);
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		_isDebugCameraActrive = !_isDebugCameraActrive;
	}
#endif // _DEBUG
	if (_isDebugCameraActrive) {
		_debugCamera->Update();
		_viewProjection.matView = _debugCamera->GetViewProjection().matView;
		_viewProjection.matProjection = _debugCamera->GetViewProjection().matProjection;
		_viewProjection.TransferMatrix();
	} else {
		_viewProjection.UpdateMatrix();
	}

	// Block
	for (std::vector<WorldTransform*>& line : _worldTransformBlocks) {
		for (WorldTransform* row : line) {
			if (!row)
				continue;
			row->UpdateMatrix();
		}
	}
	// Obj
	_skydomeObj->Update();
	_worldTransform_player.UpdateMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	for (std::vector<WorldTransform*>& line : _worldTransformBlocks) {
		for (WorldTransform* row : line) {
			if (!row)
				continue;
			_model->Draw(*row, _viewProjection);
		}
	}
	_skydomeObj->Draw();
	_model_player->Draw(_worldTransform_player, _viewProjection);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GenerateBlocks() {
	// ブロックを初期化
	const uint32_t kNumBlockHorizontal = MapChipField::kNumBlockHorizontal;
	const uint32_t kNumBlockVertical = MapChipField::kNumBlockVirtical;
	_worldTransformBlocks.resize(kNumBlockVertical); // 事前に要素数で容器のサイズを決める
	for (uint32_t i = 0; i < kNumBlockVertical; i++) {
		_worldTransformBlocks[i].resize(kNumBlockHorizontal);
	}
	// ブロック生成
	for (uint32_t i = 0; i < kNumBlockVertical; i++) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			switch (_mapChipField->GetMapChipTypeByIndex(j, i)) {
			case MapChipType::kBlock:
				_worldTransformBlocks[i][j] = new WorldTransform();
				_worldTransformBlocks[i][j]->Initialize();
				_worldTransformBlocks[i][j]->translation_ = _mapChipField->GetMapChipPositionByIndex(j, i);
				break;
			}
		}
	}
}
