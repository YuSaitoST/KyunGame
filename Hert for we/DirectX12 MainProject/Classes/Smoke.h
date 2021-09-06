#pragma once

#include"Base/pch.h"
#include"Base/dxtk.h"
#include"cppcoro/generator.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class Smoke {
public:
	void Initialize();
	void LoadAssets();
	bool Up_Change(const float deltaTime);
	void Render();
	
	// スモーク
	DX9::SPRITE white;
	SimpleMath::Vector3 pos_white;
	float alpha_white;
	float time_delta;
	float time_stop;
	const float num_alpha = 170.0f;
	const float num_speed = 2500.0f;

	DX9::SPRITE ui_turn_my;
	DX9::SPRITE ui_turn_partner;
	DX9::SPRITE ui_move;
	float width_ui;
	float width_pos;

	SimpleMath::Vector3 pos_ui_turn_my;


	//コルーチン
	cppcoro::generator<int> Change();
	cppcoro::generator<int> co_change;
	cppcoro::detail::generator_iterator<int> co_change_it;

private:
	int count_change;  // コルーチンの呼び出し回数を制限するための変数
};