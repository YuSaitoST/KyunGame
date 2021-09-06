#include "Classes/BarUI.h"
#include <iostream>
#include <MainScene.h>

void BarUI::Initialize() {
	time_delta = 0.0f;

	pos_ui_turn_my.x = -1920.0f;
	pos_ui_turn_my.y = 405.0f;
	pos_ui_turn_my.z = 0.0f;

	time_stop = 0.0f;

	width_ui = 0;

	count_bar = 0;
	 
}

void BarUI::LoadAsset() {
	ui_turn_my				= DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/turn_ui.png");
	ui_turn_partner		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/partner_turn_ui.png");
	ui_move					= DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/move_ui.png");
}

bool BarUI::Up_Bar(const float deltaTime) {
	time_delta = deltaTime;

	if (count_bar == 0) {
		count_bar += 1;
		co_bar = Bar();             //コルーチンの生成
		co_bar_it = co_bar.begin(); //コルーチンの実行開始
	}
	if (co_bar_it == co_bar.end()) return true;
	if (co_bar_it != co_bar.end()) co_bar_it++;
	return false;
}

void BarUI::Render() {
	DX9::SpriteBatch->DrawSimple(
		ui_turn_my.Get(), pos_ui_turn_my,
		Rect(0.0f, 0.0f, 1920 + width_ui, 1080.0f),
		DX9::Colors::RGBA(255, 255, 255, 255)
	);
	DX9::SpriteBatch->DrawSimple(
		ui_turn_partner.Get(), pos_ui_turn_my,
		Rect(0.0f, 0.0f, 1920 + width_ui, 1080.0f),
		DX9::Colors::RGBA(255, 255, 255, 255)
	);
}

cppcoro::generator<int>BarUI::Bar() {
	co_yield 0;

	while (pos_ui_turn_my.x < 0.0f)
	{
		pos_ui_turn_my.x += num_speed * time_delta;
		co_yield 1;
	}
	pos_ui_turn_my.x = 0.0f;

	while (time_stop < 1.0f) {
		time_stop += time_delta;
		co_yield 2;
	}
	time_stop = 1.0f;

	while (width_ui < 1920)
	{
		width_ui += num_speed * time_delta;
		co_yield 3;
	}
	width_ui = 1920;

	while (width_ui > 0)
	{
		pos_ui_turn_my.x = -1920.0f;
		width_ui = 0;
		co_yield 4;
	}


	co_return;
}
