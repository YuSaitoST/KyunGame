#pragma once

#include"Base/pch.h"
#include"Base/dxtk.h"
#include"cppcoro/generator.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class Attack {
public:
	void Initialize();
	void LoadAssets();
	bool Up_Attack(const float deltaTime);
	void Render(int index, SimpleMath::Vector2 pos_boy, SimpleMath::Vector2 pos_girl);


	static int alpha_boy;
	static int alpha_girl;

private:
	//コルーチン
	cppcoro::generator<int> Action();
	cppcoro::generator<int> co_action;
	cppcoro::detail::generator_iterator<int> co_action_it;

	DX9::SPRITE boy[5];
	DX9::SPRITE girl[5];

	int count_chnage;
	float time_delta;

	const int COLOR_GRAY = 155;
	const int NUM_ALPHA = 100;
	int num_color[2];  // 画像の色
	float time_stop;
};