#include"Classes/Attack.h"
#include <iostream>
#include<MainScene.h>

int Attack::alpha_boy;
int Attack::alpha_girl;

void Attack::Initialize() {
	count_chnage = 0;
	time_delta = 0.0f;
	alpha_boy = 255;
	alpha_girl = 155;
	time_stop = 0.0f;

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
		DX9::Colors::RGBA(num_color[0], num_color[0], num_color[0], 255)
	);
	DX9::SpriteBatch->DrawSimple(
		girl[MainScene::emotion[index]].Get(),
		SimpleMath::Vector3(pos_x_ + pos_girl.x, pos_girl.y, MainScene::POSI_Z::PLAYER),
		Rect(0.0f, 0.0f, 438.0f, 476.0f),
		DX9::Colors::RGBA(num_color[1], num_color[1], num_color[1], 255)
	);
}

cppcoro::generator<int>Attack::Action() {
	co_yield 0;
	if (MainScene::num_player == 0) {
		while (alpha_girl > 0) {
			alpha_girl -= NUM_ALPHA * time_delta;
			co_yield 1;
		}
		alpha_girl = 0;

		while (time_stop < 1.0f)
		{
			time_stop += time_delta;
			co_yield 2;
		}
		time_stop = 1.0f;
		MainScene::emotion[0] = MainScene::EMOTION::PROPOSAL;

		while (alpha_girl < 255)
		{
			alpha_girl += NUM_ALPHA * time_delta;
			co_yield 3;
		}
		alpha_girl = 255;







	}
	co_return;
}