#include "Classes/Animation.h"

void Animation::Initialize() {

	alpha_white = 0;
	time_delta = 0.0f;
	time_stop = 2.0f;

	co_change = Change();             //コルーチンの生成
	co_cahnge_it = co_change.begin(); //コルーチンの実行開始
}

void Animation::LoadAssets() {
	white = DX9::Sprite::CreateFromFile(DXTK->Device9, L"white.png");
}

void Animation::Update(const float deltaTime) {
	time_delta = deltaTime;
}

void Animation::Render() {
	DX9::SpriteBatch->DrawSimple(
		white.Get(), SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
		Rect(0.0f, 0.0f, 0.0f, 0.0f),
		DX9::Colors::RGBA(255, 255, 255, alpha_white)
	);
}

cppcoro::generator<int>Animation::Change() {
	co_yield 0;
	while (alpha_white < 120)
	{
		alpha_white++* time_delta;
		co_yield 1;
	}
	alpha_white = 120;

	while (time_stop < 0.0f) {
		time_stop--* time_delta;
		co_yield 2;
	}

	while (alpha_white > 120)
	{
		alpha_white--* time_delta;
		co_yield 3;
	}
	alpha_white = 0;
}
