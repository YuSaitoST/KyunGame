#pragma once

#include"Base/pch.h"
#include"Base/dxtk.h"
#include"cppcoro/generator.h"
#include"Classes/LoadLines.h"

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
	void Re_Speak();
	void Re_Chara(int index);



	static float alpha_boy;
	static float alpha_girl;

private:

	DX9::SPRITEFONT font;


	//コルーチン
	cppcoro::generator<int> Action();
	cppcoro::generator<int> co_action;
	cppcoro::detail::generator_iterator<int> co_action_it;

	DX9::SPRITE boy_a[5];
	DX9::SPRITE boy_b[5];
	DX9::SPRITE girl_a[5];
	DX9::SPRITE girl_b[5];

	DX9::SPRITE speach_my_reply;
	DX9::SPRITE speach_my_attack;
	DX9::SPRITE speach_partner_reply;
	DX9::SPRITE speach_partner_attack;
	SimpleMath::Vector3 pos_speach_girl_1p;
	SimpleMath::Vector3 pos_speach_boy_1p;

	SimpleMath::Vector3 pos_speach_girl_2p;
	SimpleMath::Vector3 pos_speach_boy_2p;

	enum SPEACH {
		ATTACK_GIRL,
		ATTACK_BOY
	};
	int speach;

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


	int comment;
	bool speach_flag;


	LoadLines loadlines;

	const float COLOR_GRAY = 155.0f;
	const float COLOR_MAX = 255.0f;
	const float NUM_ALPHA_CHARA = 1100.0f;
	const float NUM_ALPHA_SPEACH = 400.0f;
	const float NUM_SPEED = 700.0f;
	float num_color[2];  // 画像の色
	float time_stop;


	// このクラスに移すために用意した変数
	SimpleMath::Vector2 POS_LEFT_GENE = SimpleMath::Vector2(0.0f, 240.0f);
	SimpleMath::Vector2 POS_LEFT_ATTACK = SimpleMath::Vector2(0.0f, 0.0f);
	SimpleMath::Vector2 POS_BOY_GE_A = SimpleMath::Vector2(1420.0f, 240.0f);
	SimpleMath::Vector2 POS_BOY_AT_A = SimpleMath::Vector2(620.0f, 0.0f);
	SimpleMath::Vector2 POS_GIRL_GE_B = SimpleMath::Vector2(1370.0f, 240.0f);
	SimpleMath::Vector2 POS_GIRL_AT_B = SimpleMath::Vector2(980.0f, 0.0f);

	float RC_GIRL_NOM_R = 550.0f;
	float RC_GIRL_ATT_R = 940.0f;
	float RC_BOY_NOM_R = 500.0f;
	float RC_BOY_ATT_R = 1300.0f;
};