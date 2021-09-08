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


private:

	DX9::SPRITEFONT font;


	//コルーチン
	cppcoro::generator<int> Action();
	cppcoro::generator<int> co_action;
	cppcoro::detail::generator_iterator<int> co_action_it;

	DX9::SPRITE boy[5];
	DX9::SPRITE girl[5];

	DX9::SPRITE speach_my;
	DX9::SPRITE speach_partner;
	SimpleMath::Vector3 pos_speach_girl_1p;
	SimpleMath::Vector3 pos_speach_boy_1p;

	SimpleMath::Vector3 pos_speach_girl_2p;
	SimpleMath::Vector3 pos_speach_boy_2p;

	float alpha_speach_my;
	float alpha_speach_partner;


	const float pos_speach_normal_girl_limit_x_1p = 434.0f;
	const float pos_speach_attack_girl_limit_x_1p = 635.0f;
	const float pos_speach_normal_boy_limit_x_1p = 497.0f;
	const float pos_speach_attack_boy_limit_x_1p = 335.0f;

	const float pos_speach_normal_girl_limit_x_2p = 2406.0f;
	const float pos_speach_attack_girl_limit_x_2p = 2205.0f;
	const float pos_speach_normal_boy_limit_x_2p = 2343.0f;
	const float pos_speach_attack_boy_limit_x_2p = 2505.0f;


	const float pos_speach_normal_y = 597.0f;
	const float pos_speach_attack_y = 40.0f;


	int count_chnage;
	float time_delta;

	float alpha_boy;
	float alpha_girl;

	const float COLOR_GRAY = 155.0f;
	const float COLOR_MAX = 255.0f;
	const float NUM_ALPHA_CHARA = 500.0f;
	const float NUM_ALPHA_SPEACH = 100.0f;
	const float NUM_SPEED = 200.0f;
	float num_color[2];  // 画像の色
	float time_stop;
};