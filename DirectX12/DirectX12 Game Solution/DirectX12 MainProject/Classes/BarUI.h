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
	DX9::SPRITE ui_turn_my;
	DX9::SPRITE ui_turn_partner;
	DX9::SPRITE ui_move;

	SimpleMath::Vector3 pos_ui_turn_my;

	float time_delta;
	const float num_speed = 50.0f;
	float time_stop;
	int width_ui;

	//コルーチン
	cppcoro::generator<int> Bar();
	cppcoro::generator<int> co_bar;
	cppcoro::detail::generator_iterator<int> co_bar_it;

	int count_bar;  // コルーチンの呼び出し回数を制限するための変数

};