#pragma once

#include"Base/pch.h"
#include"Base/dxtk.h"
#include"cppcoro/generator.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class Move {
public:
	void Initialize();
	void LoadAseet();
	void Up_Moveing(float deltaTime);
	void Render(int index);

	static bool flag_move;
private:
	cppcoro::generator<int> Fade();
	cppcoro::generator<int> co_fade;
	cppcoro::detail::generator_iterator<int> co_fade_it;

	DX9::SPRITE ui_move;

	int count_move;
	float time_delta;
	float alpha_move;

	const float num_alpha = 150.0f;
	const float num_al_mo = 120.0f;
};