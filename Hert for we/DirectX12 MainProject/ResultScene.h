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

	DX9::SPRITEFONT font;

	DX9::SPRITE black;
	DX9::SPRITE ope;

	DX9::MEDIARENDERER bgm_result;
	DX9::MEDIARENDERER se_flowers;

	//コルーチン
	cppcoro::generator<int> Operate();
	cppcoro::generator<int> co_operate;
	cppcoro::detail::generator_iterator<int> co_operate_it;

	int count_change;
	float time_delta;
	float num_alpha;
	bool flag_fade;

	void LA_Load();
	bool Up_Fade(const float deltaTime);
	void Re_Sprite();
	void Re_Speak();
	void Re_DirectTwelve();

public:
};