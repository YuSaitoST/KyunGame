#include "Classes/BlackOut.h"
#include <iostream>
#include <MainScene.h>
#include <ResultScene.h>

void BlackOut::Initialize() {

	alpha_black = MainScene::phase == MainScene::Phase::SCENARIO ? 255.0f : 0.0f;
	alpha_text = 0;

	time_delta = 0.0f;
	time_stop = 2.0f;

	count_chnage = 0;
	time_stop = 0.0f;

}

void BlackOut::LoadAssets() {
	black	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Effect/black.png");
	text		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"prolog_text.png");
}

bool BlackOut::Up_Black(const float deltaTime) {
	if (count_chnage == 0) {
		count_chnage += 1;
		if (MainScene::phase != MainScene::Phase::SCENARIO) alpha_black = 0.0f;
		co_change = Change();             //コルーチンの生成
		co_change_it = co_change.begin(); //コルーチンの実行開始
	}
	time_delta = deltaTime;
	if (co_change_it == co_change.end()) {
		Initialize();
		return true;
	}

	if (co_change_it != co_change.end()) co_change_it++;
	return false;
}

void BlackOut::Render() {
	DX9::SpriteBatch->DrawSimple(
		black.Get(), SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
		Rect(0.0f, 0.0f, 3840.0f, 1080.0f),
		DX9::Colors::RGBA(255, 255, 255, (int)alpha_black)
	);

	DX9::SpriteBatch->DrawSimple(
		text.Get(), SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
		Rect(0.0f, 0.0f, 1920.0f, 990.0f),
		DX9::Colors::RGBA(255, 255, 255, (int)alpha_text)
	);
	DX9::SpriteBatch->DrawSimple(
		text.Get(), SimpleMath::Vector3(1920.0f, 0.0f, 0.0f),
		Rect(0.0f, 0.0f, 1920.0f, 990.0f),
		DX9::Colors::RGBA(255, 255, 255, (int)alpha_text)
	);
}

cppcoro::generator<int>BlackOut::Change() {
	co_yield 0;

	if (MainScene::phase == MainScene::Phase::SCENARIO) {
		// 待機
		while (time_stop < 2.5f) {
			time_stop += time_delta;
			co_yield 1;
		}
		// テキスト表示ループ
		while (alpha_text < 255.0f) {
			alpha_text = std::min(alpha_text + num_alpha * time_delta, 255.0f);
			co_yield 2;
		}
		// ブラックアウト
		while (alpha_black > 0.0f)
		{
			alpha_text		= std::max(alpha_text - num_alpha * time_delta, 0.0f);
			alpha_black	= std::max(alpha_black - num_alpha * time_delta, 0.0f);
			co_yield 3;
		}
	}

	if (MainScene::phase == MainScene::Phase::SUCCEED) {
		while (alpha_black < 255.0f) {
			alpha_black = std::min(alpha_black + num_alpha * time_delta, 255.0f);
			co_yield 4;
		}
	}

	co_return;
}