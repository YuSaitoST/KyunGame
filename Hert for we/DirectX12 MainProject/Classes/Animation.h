#pragma once

#include"Base/pch.h"
#include"Base/dxtk.h"
#include"cppcoro/generator.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class Animation {
public:
	void Initialize();
	void LoadAssets();
	void Update(const float deltaTime);
	void Render();
	
	DX9::SPRITE white;
	SimpleMath::Vector3 pos_white;
	float alpha_white;
	float time_delta;
	float time_stop;

	const float num_speed = 10.0f;

	//�R���[�`��
	cppcoro::generator<int> Change();
	cppcoro::generator<int> co_change;
	cppcoro::detail::generator_iterator<int> co_change_it;

private:
};