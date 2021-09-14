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

	float bgm;


	// 会話用
	SimpleMath::Vector3 pos_speach_girl_1p;
	SimpleMath::Vector3 pos_speach_boy_1p;

	SimpleMath::Vector3 pos_speach_girl_2p;
	SimpleMath::Vector3 pos_speach_boy_2p;

	SimpleMath::Vector3 pos_speach_synchro_1p;
	SimpleMath::Vector3 pos_speach_synchro_2p;

	LoadLines load;

	const float TALK_SPEED = 10.0f;

	const float pos_speach_normal_girl_limit_x_1p = 434.0f;
	const float pos_speach_normal_boy_limit_x_1p = 497.0f;
	const float pos_speach_normal_girl_limit_x_2p = 2406.0f;
	const float pos_speach_normal_boy_limit_x_2p = 2343.0f;

	//63 31.5
	const float pos_speach_synchro_x_1p = 465.0f;
	const float pos_speach_synchro_x_2p = 2437.0f;

	const float pos_speach_reply_y = 597.0f;

	float comment_reply;

	float time_stop;
	float alpha_text;

	int index;

	bool flag_talk_boy;
	bool flag_talk_girl;
	bool flag_talk_synchro;

	void LA_Load();
	bool Up_Fade(const float deltaTime);
	void Re_Sprite();
	void Re_Speak();
	void Re_DirectTwelve();

public:
	static int winner;  // 0:girl, 1:boy;
};