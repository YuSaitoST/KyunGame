#include "Classes/Smoke.h"
#include <iostream>
#include <MainScene.h>

void Smoke::Initialize() {

	alpha_white		= 0;
	time_delta			= 0.0f;
	time_stop			= 2.0f;
	count_change	= 0;

	pos_ui_turn_my.x = 0.0f;
	pos_ui_turn_my.y = 405.0f;
	pos_ui_turn_my.z = 0.0f;

	time_stop = 0.0f;

	width_ui = 0.0f;
	width_pos = 0.0f;
}

void Smoke::LoadAssets() {
	white = DX9::Sprite::CreateFromFile(DXTK->Device9, L"Effect/white.png");

	ui_turn_my = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/turn_ui.png");
	ui_turn_partner = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/partner_turn_ui.png");
	ui_move = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/move_ui.png");
}

bool Smoke::Up_Change(const float deltaTime) {
	if (count_change == 0) {
		count_change += 1;
		co_change			= Change();             //�R���[�`���̐���
		co_change_it		= co_change.begin(); //�R���[�`���̎��s�J�n
	}
	time_delta = deltaTime;
	if (co_change_it == co_change.end()) return true;
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


	DX9::SpriteBatch->DrawSimple(
		ui_turn_my.Get(), pos_ui_turn_my,
		Rect(width_pos, 0.0f, 0.0f + (int)width_ui, 1080.0f),
		DX9::Colors::RGBA(255, 255, 255, 255)
	);
}

cppcoro::generator<int>Smoke::Change() {
	co_yield 0;

	while (width_ui < 1920)
	{
		alpha_white += num_alpha * time_delta;
		if (alpha_white > 120)
			alpha_white = 120;
		width_ui += num_speed * time_delta;
		co_yield 1;
	}
	//alpha_white = 120;
	width_ui = 1920;

	while (time_stop < 0.9f) {
		time_stop += time_delta;
		co_yield 2;
	}

	while (width_pos < 1920.0f)
	{
		width_pos += num_speed * time_delta;
		pos_ui_turn_my.x += num_speed * time_delta;

		alpha_white = std::max(alpha_white - num_alpha * time_delta, 0.0f);
		co_yield 3;
	}

	co_return;
}