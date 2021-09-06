#pragma once

#include"Base/pch.h"
#include"Base/dxtk.h"
#include"cppcoro/generator.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class BarUI {
public:
	void Initialize();
	void LoadAsset();
	bool Up_Bar(const float deltaTime);
	void Render();
private:


};