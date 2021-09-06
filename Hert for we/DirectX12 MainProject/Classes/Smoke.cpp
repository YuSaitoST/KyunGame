#include "Classes/Smoke.h"
#include <iostream>
#include <MainScene.h>

void Smoke::Initialize() {

	alpha_white		= 0;
	time_delta			= 0.0f;
	time_stop			= 2.0f;
	count_change	= 0;
}

void Smoke::LoadAssets() {
	white = DX9::Sprite::CreateFromFile(DXTK->Device9, L"Effect/white.png");
}

bool Smoke::Up_Change(const float deltaTime) {
	if (count_change == 0) {
		count_change += 1;
		co_change			= Change();             //コルーチンの生成
		co_change_it		= co_change.begin(); //コルーチンの実行開始
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
}

cppcoro::generator<int>Smoke::Change() {
	co_yield 0;

	while (alpha_white < 120)
	{
		alpha_white = alpha_white + num_speed * time_delta;
		co_yield 1;
	}
	alpha_white = 120;

	while (time_stop < 10.0f) {
		time_stop = time_stop + time_delta;
		co_yield 2;
	}

	while (alpha_white > 0)
	{
		alpha_white = alpha_white - num_speed * time_delta;
		co_yield 3;
	}
	alpha_white = 0;

	co_return;
}
