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
	void Render();
private:
	DX9::SPRITE ui_turn_my;
	DX9::SPRITE ui_turn_partner;
	DX9::SPRITE ui_move;
};