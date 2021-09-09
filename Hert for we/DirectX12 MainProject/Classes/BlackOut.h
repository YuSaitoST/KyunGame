#pragma once

#include"Base/pch.h"
#include"Base/dxtk.h"
#include"cppcoro/generator.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class BlackOut {
public:
	void Initialize();
	void LoadAssets();
	bool Up_Black(const float deltaTime);
	void Render();

	// �u���b�N�A�E�g�p
	DX9::SPRITE black;
	SimpleMath::Vector3 pos_black;
	float alpha_black;
	float time_delta;
	float time_stop;
	const float num_alpha = 90.0f;
	const float num_speed = 2500.0f;

	//�e�L�X�g�\���p
	DX9::SPRITE text;
	float alpha_text;


	//�R���[�`��
	cppcoro::generator<int> Change();
	cppcoro::generator<int> co_change;
	cppcoro::detail::generator_iterator<int> co_change_it;

private:
	int count_chnage; // �R���[�`���̌Ăяo���񐔂𐧌����邽�߂̕ϐ�
};