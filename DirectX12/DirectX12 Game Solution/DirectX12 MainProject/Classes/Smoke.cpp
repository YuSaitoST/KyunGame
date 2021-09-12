#include "Classes/Smoke.h"
#include <iostream>
#include <MainScene.h>

void Smoke::Initialize() {

	alpha_white		= 0;
	time_delta			= 0.0f;
	time_stop			= 2.0f;
	count_chnage = 0;
	pos_ui_turn_my.x = 0.0f;
	pos_ui_turn_my.y = 385.0f;
	pos_ui_turn_my.z = 0.0f;

	pos_UI_turn_partner.x = 1920.0f;
	pos_UI_turn_partner.y = 385.0f;
	pos_UI_turn_partner.z = 0.0f;

	time_stop = 0.0f;

	width_ui = 0.0f;
	width_pos = 0.0f;
}

void Smoke::LoadAssets() {
	white						= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Effect/white.png");

	ui_turn_my				= DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/turn_ui.png");
	ui_turn_partner		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/partner_turn_ui.png");
	ui_move					= DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/move_ui.png");
}

bool Smoke::Up_Change(const float deltaTime) {
	if (count_chnage == 0) {
		count_chnage += 1;
		co_change			= Change();             //コルーチンの生成
		co_change_it		= co_change.begin(); //コルーチンの実行開始
	}
	time_delta = deltaTime;
	if (co_change_it == co_change.end()) {
		Initialize();
		return true;
	}

	if (co_change_it != co_change.end()) co_change_it++;
	return false;
}

void Smoke::Render() {
	DX9::SpriteBatch->DrawSimple(
		white.Get(), SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
		Rect(0.0f, 0.0f, 1920.0f, 1080.0f),
		DX9::Colors::RGBA(255, 255, 255, (int)alpha_white)
	);

	DX9::SpriteBatch->DrawSimple(
		white.Get(), SimpleMath::Vector3(1920.0f, 0.0f, 0.0f),
		Rect(0.0f, 0.0f, 1920.0f, 1080.0f),
		DX9::Colors::RGBA(255, 255, 255, (int)alpha_white)
	);


	if (MainScene::num_player == 0) {
		DX9::SpriteBatch->DrawSimple(
			ui_turn_my.Get(), pos_ui_turn_my,
			Rect(width_pos, 0.0f, 0.0f + (int)width_ui, 1080.0f),
			DX9::Colors::RGBA(255, 255, 255, 255)
		);

		DX9::SpriteBatch->DrawSimple(
			ui_turn_partner.Get(), pos_UI_turn_partner,
			Rect(width_pos, 0.0f, 0.0f + (int)width_ui, 1080.0f),
			DX9::Colors::RGBA(255, 255, 255, 255)
		);
	}
	else
	{
		DX9::SpriteBatch->DrawSimple(
			ui_turn_my.Get(), pos_UI_turn_partner,
			Rect(width_pos, 0.0f, 0.0f + (int)width_ui, 1080.0f),
			DX9::Colors::RGBA(255, 255, 255, 255)
		);

		DX9::SpriteBatch->DrawSimple(
			ui_turn_partner.Get(), pos_ui_turn_my,
			Rect(width_pos, 0.0f, 0.0f + (int)width_ui, 1080.0f),
			DX9::Colors::RGBA(255, 255, 255, 255)
		);
	}
}

cppcoro::generator<int>Smoke::Change() {
	co_yield 0;

	//UIの移動、スモーク
	while (width_ui < 1920)
	{
		alpha_white = std::min(alpha_white + num_alpha * time_delta, 120.0f);
		width_ui    = std::min(width_ui + num_speed * time_delta, 1920.0f);
		co_yield 1;
	}

	//間
	while (time_stop < 0.9f) {
		time_stop += time_delta;
		co_yield 2;
	}

	//UIの移動、スモークの解除
	while (width_pos < 1920.0f)
	{
		width_pos += num_speed * time_delta;
		pos_ui_turn_my.x += num_speed * time_delta;
		pos_UI_turn_partner.x += num_speed * time_delta;

		alpha_white = std::max(alpha_white - num_alpha * time_delta, 0.0f);
		co_yield 3;
	}
	co_return;
}