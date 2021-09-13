#pragma once

#include"Base/pch.h"
#include"Base/dxtk.h"
#include"cppcoro/generator.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class BlackOut {
public:
	void Initialize();
	void LoadAssets();
	bool Up_Black(const float deltaTime);
	void Render();

	// ブラックアウト用
	DX9::SPRITE black;
	SimpleMath::Vector3 pos_black;
	float alpha_black;
	float time_delta;
	float time_stop;
	const float num_alpha = 90.0f;
	const float num_speed = 2500.0f;

	//テキスト表示用
	DX9::SPRITE text;
	float alpha_text;

	//導入会話用
	DX9::SPRITE speach_my_reply;
	DX9::SPRITE speach_partner_reply;
	DX9::SPRITEFONT font;
	DX9::SPRITE speach_synchro;

	SimpleMath::Vector3 pos_speach_girl_1p;
	SimpleMath::Vector3 pos_speach_boy_1p;

	SimpleMath::Vector3 pos_speach_girl_2p;
	SimpleMath::Vector3 pos_speach_boy_2p;

	SimpleMath::Vector3 pos_speach_synchro_1p;
	SimpleMath::Vector3 pos_speach_synchro_2p;


	float alpha_speach_girl;
	float alpha_speach_boy;
	float alpha_speach_synchro;
	float comment_reply;

	bool flag_talk_boy;
	bool flag_talk_girl;
	bool flag_talk_synchro;

	const float pos_speach_normal_girl_limit_x_1p = 434.0f;
	const float pos_speach_normal_boy_limit_x_1p = 497.0f;
	const float pos_speach_normal_girl_limit_x_2p = 2406.0f;
	const float pos_speach_normal_boy_limit_x_2p = 2343.0f;

	//63 31.5
	const float pos_speach_synchro_x_1p = 465.0f;
	const float pos_speach_synchro_x_2p = 2437.0f;


	const float pos_speach_reply_y = 597.0f;
	const float TALK_SPEED = 20.0f;
	const float COLOR_MAX = 255.0f;
	const float NUM_ALPHA_SPEACH = 400.0f;
	//コルーチン
	cppcoro::generator<int> Change();
	cppcoro::generator<int> co_change;
	cppcoro::detail::generator_iterator<int> co_change_it;

private:
	int count_chnage; // コルーチンの呼び出し回数を制限するための変数
};