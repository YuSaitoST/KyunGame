#include"Classes/Attack.h"
#include <iostream>
#include<MainScene.h>
#include <Classes/LoadLines.cpp>

void Attack::Initialize() {

	font = DX9::SpriteFont::CreateFromFontFile(DXTK->Device9, L"Font/HuiFont.ttf", L"ふい字", 20);

	count_chnage = 0;
	time_delta = 0.0f;
	alpha_boy = 255.0f;
	alpha_girl = 155.0f;
	time_stop = 0.0f;


	pos_speach_girl_1p.x = 0.0f;
	pos_speach_girl_1p.y = 0.0f;
	pos_speach_girl_1p.z = 0.0f;

	pos_speach_boy_1p.x = 920.0f;
	pos_speach_boy_1p.y = 0.0f;
	pos_speach_boy_1p.z = 0.0f;


	pos_speach_girl_2p.x = 2840.0f;
	pos_speach_girl_2p.y = 0.0f;
	pos_speach_girl_2p.z = 0.0f;

	pos_speach_boy_2p.x = 1920.0f;
	pos_speach_boy_2p.y = 0.0f;
	pos_speach_boy_2p.z = 0.0f;


	alpha_speach_my = 0.0f;
	alpha_speach_partner = 0.0f;

	num_color[0] = 255;
	num_color[1] = 255;
}

void Attack::LoadAssets() {
	boy[MainScene::EMOTION::GENERALLY]	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/boy_generally.png");
	boy[MainScene::EMOTION::PROPOSAL]	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/boy_attack.png");
	boy[MainScene::EMOTION::NERVOUS]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/boy_near.png");
	boy[MainScene::EMOTION::VICTORY]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/boy_victory.png");
	boy[MainScene::EMOTION::DEFEAT]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/boy_defeat.png");
	girl[MainScene::EMOTION::GENERALLY]	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/girl_generally.png");
	girl[MainScene::EMOTION::PROPOSAL]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/girl_attack.png");
	girl[MainScene::EMOTION::NERVOUS]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/girl_near.png");
	girl[MainScene::EMOTION::VICTORY]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/girl_victory.png");
	girl[MainScene::EMOTION::DEFEAT]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/girl_defeat.png");

	speach_my      = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/speech_balloon.png"  );
	speach_partner = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/speech_balloon_r.png");
}

bool Attack::Up_Attack(const float deltaTime) {
	if (count_chnage == 0) {
		count_chnage += 1;
		co_action = Action();             //コルーチンの生成
		co_action_it = co_action.begin(); //コルーチンの実行開始
	}
	time_delta = deltaTime;

	if (co_action_it == co_action.end()) {
		Initialize();
		return true;
	}
	if (co_action_it != co_action.end()) co_action_it++;
	return false;
}

// 反転用の変数を渡せるようにする（その変数はMainScene.hにて）
void Attack::Render(int index, SimpleMath::Vector2 pos_boy, SimpleMath::Vector2 pos_girl) {
	float pos_x_ = index == 1 ? 1920.0f : 0.0f;

	DX9::SpriteBatch->DrawSimple(
		boy[MainScene::emotion[index]].Get(),
		SimpleMath::Vector3(pos_x_ + pos_boy.x, pos_boy.y, MainScene::POSI_Z::PLAYER),
		Rect(0.0f, 0.0f, 402.0f, 590.0f),
		DX9::Colors::RGBA((int)num_color[0], (int)num_color[0], (int)num_color[0], (int)alpha_boy)
	);
	DX9::SpriteBatch->DrawSimple(
		girl[MainScene::emotion[index]].Get(),
		SimpleMath::Vector3(pos_x_ + pos_girl.x, pos_girl.y, MainScene::POSI_Z::PLAYER),
		Rect(0.0f, 0.0f, 438.0f, 476.0f),
		DX9::Colors::RGBA((int)num_color[1], (int)num_color[1], (int)num_color[1], (int)alpha_girl)
	);

	//1P
	DX9::SpriteBatch->DrawSimple(
		speach_my.Get(),
		pos_speach_girl_1p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, alpha_speach_my)
	);

	DX9::SpriteBatch->DrawSimple(
		speach_partner.Get(),
		pos_speach_boy_1p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, alpha_speach_partner)
	);

	//2P
	DX9::SpriteBatch->DrawSimple(
		speach_partner.Get(),
		pos_speach_girl_2p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, alpha_speach_my)
	);

	DX9::SpriteBatch->DrawSimple(
		speach_my.Get(),
		pos_speach_boy_2p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, alpha_speach_partner)
	);
}

cppcoro::generator<int>Attack::Action() {
	
	if (MainScene::num_player == 0) {
		co_yield 0;
		//女子の透明度を下げる
		while (alpha_girl > 0.0f) {
			alpha_girl -= NUM_ALPHA_CHARA * time_delta;
			co_yield 1;
		}
		alpha_girl = 0.0f;

		//女子の差分切り替え
		while (time_stop < 1.0f)
		{
			MainScene::emotion[MainScene::num_player] = MainScene::EMOTION::PROPOSAL;
			time_stop += time_delta;
			co_yield 2;
		}
		time_stop = 1.0f;

		//女子の透明度を上げる
		while (alpha_girl < 255.0f)
		{
			alpha_girl += NUM_ALPHA_CHARA * time_delta;
			co_yield 3;
		}
		alpha_girl = 255.0f;

		//吹き出しの移動
		while (alpha_speach_my < COLOR_MAX)
		{
			num_color[1] = COLOR_MAX;
			pos_speach_girl_1p.y = pos_speach_attack_y;
			pos_speach_girl_2p.y = pos_speach_attack_y;

			alpha_speach_my += NUM_ALPHA_SPEACH * time_delta;

			pos_speach_girl_1p.x += NUM_SPEED * time_delta;
			if (pos_speach_girl_1p.x > pos_speach_attack_girl_limit_x_1p)
				pos_speach_girl_1p.x = pos_speach_attack_girl_limit_x_1p;
			
			pos_speach_girl_2p.x -= NUM_SPEED * time_delta;
			if (pos_speach_girl_2p.x < pos_speach_attack_girl_limit_x_2p)
				pos_speach_girl_2p.x = pos_speach_attack_girl_limit_x_2p;

			co_yield 4;
		}

		//while ()
		//{
		RECT dest = RectWH(0, 0, 100, 100);
			LoadLines::lines1[0].length();
				DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[0].c_str(), 2, dest, DX9::Colors::White);
				//フォント,セリフの入った変数,1度に表示する文字数,文字を表示する場所,色
		//}

		co_return;
	}
}