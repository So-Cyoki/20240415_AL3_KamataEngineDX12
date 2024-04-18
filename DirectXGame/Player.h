#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "cassert"
class Player {
private:
	WorldTransform _worldTransform;
	Model* _model = nullptr;
	uint32_t _textureHandle = 0u;
	ViewProjection* _viewProjection = nullptr;

public:
	void Initalize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);
	void Update();
	void Draw();
};
