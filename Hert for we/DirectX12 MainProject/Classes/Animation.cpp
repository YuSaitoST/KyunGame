#include "Classes/Animation.h"
#include <iostream>
#include<Classes/Animation.h>
#include <MainScene.h>

void Animation::Initialize() {

	alpha_white = 0;
	time_delta = 0.0f;
	time_stop = 2.0f;

	co_change = Change();             //コルーチンの生成
	co_change_it = co_change.begin(); //コルーチンの実行開始
}

void Animation::LoadAssets() {
	white = DX9::Sprite::CreateFromFile(DXTK->Device9, L"white.png");
}

void Animation::Update(const float deltaTime) {
	time_delta = deltaTime;
	if (co_change_it != co_change.end()) {
		co_change_it++;
	}
}

void Animation::Render() {
	DX9::SpriteBatch->DrawSimple(
		white.Get(), SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
		Rect(0.0f, 0.0f, 1920.0f, 1080.0f),
		DX9::Colors::RGBA(255, 255, 255, (int)alpha_white)
	);
}

cppcoro::generator<int>Animation::Change() {
	co_yield 0;

	if (MainScene::phase == MainScene::Phase::START) {
		while (alpha_white < 120)
		{
			alpha_white = alpha_white + num_speed * time_delta;
			co_yield 1;
		}
		alpha_white = 120;

		while (time_stop < 0.0f) {
			time_stop = time_stop + num_speed * time_delta;
			co_yield 2;
		}

		while (alpha_white > 120)
		{
			alpha_white = alpha_white + num_speed * time_delta;
			co_yield 3;
		}
		alpha_white = 0;
	}

	co_return;
}
