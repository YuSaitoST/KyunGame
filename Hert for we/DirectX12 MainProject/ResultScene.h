//
// TemplateScene.h
//

#pragma once

#include "Scene.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class ResultScene final : public Scene {
public:
	ResultScene();
	virtual ~ResultScene() { Terminate(); }

	ResultScene(ResultScene&&) = default;
	ResultScene& operator= (ResultScene&&) = default;

	ResultScene(ResultScene const&) = delete;
	ResultScene& operator= (ResultScene const&) = delete;

	// These are the functions you will implement.
	void Initialize() override;
	void LoadAssets() override;

	void Terminate() override;

	void OnDeviceLost() override;
	void OnRestartSound() override;

	NextScene Update(const float deltaTime) override;
	void Render() override;

private:
	DX12::DESCRIPTORHEAP descriptorHeap;
	DX12::SPRITEBATCH    spriteBatch;
	DX12::HGPUDESCRIPTOR dx9GpuDescriptor;

	//コルーチン
	cppcoro::generator<int> Change();
	cppcoro::generator<int> co_change;
	cppcoro::detail::generator_iterator<int> co_change_it;

	void LA_Load();
	bool Up_Black(float deltaTime);
	void Re_DirectTwelve();

	// ブラックアウト用
	DX9::SPRITE black;
	SimpleMath::Vector3 pos_black;
	float alpha_black;
	float time_delta;
	float time_stop;
	const float num_alpha = 90.0f;
	const float num_speed = 2500.0f;
	int count_chnage; // コルーチンの呼び出し回数を制限するための変数

public:
};