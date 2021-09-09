#include "Classes/Move.h"
#include <iostream>
#include <MainScene.h>

bool Move::flag_move;

void Move::Initialize() {
	flag_move = false;
	count_move = 0;
	time_delta = 0.0f;
	alpha_move = 0.0f;
}

void Move::LoadAseet() {
	ui_move = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/move_ui.png");
}

void Move::Up_Moveing(float deltaTime) {
	if (count_move == 0) {
		count_move += 1;
		co_fade = Fade();             //コルーチンの生成
		co_fade_it = co_fade.begin(); //コルーチンの実行開始
	}
	time_delta = deltaTime;
	if (flag_move) {
		Initialize();
		return;
	}

	if (co_fade_it != co_fade.end()) co_fade_it++;
}

void Move::Render(int index) {
	float pos_x = index ? 0.0f : 1920.0f;
	DX9::SpriteBatch->DrawSimple(
		ui_move.Get(), SimpleMath::Vector3(pos_x + 0.0f, 385.0f, 0.0f),
		Rect(0.0f, 0.0f, 1920.0f, 220.0f),
		DX9::Colors::RGBA(255, 255, 255, (int)alpha_move)
	);
}

cppcoro::generator<int> Move::Fade() {
	co_yield 0;

	// 初回の表示
	while (alpha_move < 255.0f) {
		alpha_move = std::min(alpha_move + num_al_mo * time_delta, 255.0f);
		co_yield 1;
	}
	while (!flag_move) {
		while (alpha_move > 100.0f) {
			alpha_move = std::max(alpha_move - num_al_mo * time_delta, 100.0f);
			co_yield 2;
		}
		while (alpha_move < 255.0f) {
			alpha_move = std::min(alpha_move + num_al_mo * time_delta, 255.0f);
			co_yield 3;
		}
	}


	co_return;
}