#include "TitleScene.h"

TitleScene::~TitleScene() {
	delete _modelTitle;
	delete _modelPlayer;
}

void TitleScene::Initalize() {
	dxCommon_ = DirectXCommon::GetInstance();
	_viewProjection.Initialize();
	_worldTransform_title.Initialize();
	_worldTransform_player.Initialize();
	_modelTitle = Model::CreateFromOBJ("Title", true);
	_modelPlayer = Model::CreateFromOBJ("Player", true);

	_worldTransform_title.translation_ = {-20, 5, 0};
	_worldTransform_title.scale_ = {10, 10, 10};
	_worldTransform_player.translation_ = {0, -10, 0};
	_worldTransform_player.scale_ = {3, 3, 3};
	_worldTransform_player.rotation_ = {0, 1, 0.5f};
}

void TitleScene::Update() {
	if (_currentTime < _titleMoveTime)
		_currentTime += 1 / 60.f;
	else {
		_currentTime = 0;
		_titleSpeed *= -1;
	}
	_worldTransform_title.translation_ += {0, _titleSpeed, 0};

	_worldTransform_player.rotation_ += {0, 0.01f, 0.005f};

	_worldTransform_title.UpdateMatrix();
	_worldTransform_player.UpdateMatrix();
}

void TitleScene::Draw() {

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
	_modelTitle->Draw(_worldTransform_title, _viewProjection);
	_modelPlayer->Draw(_worldTransform_player, _viewProjection);

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
