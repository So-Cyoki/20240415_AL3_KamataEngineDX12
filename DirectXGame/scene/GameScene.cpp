#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete _sprite;
	delete _model;
	delete _debugCamera;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// Sprite
	_textureHandle = TextureManager::Load("tex1.png");
	_sprite = Sprite::Create(_textureHandle, {0, 0});

	// Model
	_textureHandle_model = TextureManager::Load("uvChecker.png");
	_model = Model::Create();
	_worldTransform.Initialize();
	_viewProjection.Initialize();

	// Sound
	_soundDataHandle = audio_->LoadWave("fanfare.wav");
	audio_->PlayWave(_soundDataHandle);
	//_voiceHandle = audio_->PlayWave(_soundDataHandle, true);

	// DebugCamera
	_debugCamera = new DebugCamera(1280, 720);

	// 3DLineDraw
	PrimitiveDrawer::GetInstance()->SetViewProjection(&_debugCamera->GetViewProjection());

	// Axis
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&_debugCamera->GetViewProjection());
}

void GameScene::Update() {
	// Sprite Move
	Vector2 pos = _sprite->GetPosition();
	if (pos.x > 1280 - 128 || pos.x < 0) {
		_speedX *= -1;
	}
	if (pos.y > 720 - 128 || pos.y < 0) {
		_speedY *= -1;
	}
	pos.x += _speedX;
	pos.y += _speedY;
	_sprite->SetPosition(pos);

	// Sound
	if (input_->TriggerKey(DIK_SPACE)) {
		if (audio_->IsPlaying(_voiceHandle)) {
			audio_->StopWave(_voiceHandle);
		} else {
			_voiceHandle = audio_->PlayWave(_soundDataHandle);
		}
	}

// DebugText
#ifdef _DEBUG
	ImGui::Begin("Model Scale");
	ImGui::Text("XYZ = (%d/%d/%d)", int(_scaleFloat[0]), int(_scaleFloat[1]), int(_scaleFloat[2]));
	ImGui::InputFloat3("InputFloat", _scaleFloat);
	ImGui::SliderFloat3("SliderFloat", _scaleFloat, 0, 20);
	// ImGui::ShowDemoWindow();
	ImGui::End();
#endif

	// Model
	Matrix4x4 scaleMatrix = {_scaleFloat[0], 0, 0, 0, 0, _scaleFloat[1], 0, 0, 0, 0, _scaleFloat[2], 0, 0, 0, 0, 1};
	_worldTransform.matWorld_ = scaleMatrix;
	_worldTransform.TransferMatrix();

	// DebugCamera
	_debugCamera->Update();
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

	_model->Draw(_worldTransform, _debugCamera->GetViewProjection(), _textureHandle_model);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
	// 3DLineDraw
	for (int i = 0; i < 31; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d({-30, 0, 1.f * i}, {30, 0, 1.f * i}, {1, 0, 0, 1});
		PrimitiveDrawer::GetInstance()->DrawLine3d({-30, 0, -1.f * i}, {30, 0, -1.f * i}, {1, 0, 0, 1});
		PrimitiveDrawer::GetInstance()->DrawLine3d({1.f * i, 0, -30}, {1.f * i, 0, 30}, {0, 0, 1, 1});
		PrimitiveDrawer::GetInstance()->DrawLine3d({-1.f * i, 0, -30}, {-1.f * i, 0, 30}, {0, 0, 1, 1});
	}
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	_sprite->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
