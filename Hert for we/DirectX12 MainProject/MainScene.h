//
// MainScene.h
//

#pragma once

#include "Scene.h"
#include "Classes/Animation.h"
#include <random>

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class MainScene final : public Scene {
public:
	MainScene();
	virtual ~MainScene() { Terminate(); }

	MainScene(MainScene&&) = default;
	MainScene& operator= (MainScene&&) = default;

	MainScene(MainScene const&) = delete;
	MainScene& operator= (MainScene const&) = delete;

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
	DX12::SPRITEBATCH spriteBatch;
	DX12::HGPUDESCRIPTOR dx9GpuDescriptor;

	DX9::SPRITEFONT font;

	DX9::SPRITE bg;
	DX9::SPRITE map;
	DX9::SPRITE pointer;
	DX9::SPRITE area_attack;
	DX9::SPRITE area_move;
	DX9::SPRITE heart_red;
	DX9::SPRITE heart_blue;
	DX9::SPRITE boy;
	DX9::SPRITE girl;
	DX9::SPRITE com_cursor;

	SimpleMath::Vector2 pos_pointer;
	SimpleMath::Vector2 pos_cursor;
	SimpleMath::Vector2 pos_attack;
	SimpleMath::Vector2 pos_move[4];
	SimpleMath::Vector2 pos_cross_hR[4];  // ハート赤の十字座標
	SimpleMath::Vector2 pos_cross_hB[4];  // ハート青の十字座標
	SimpleMath::Vector2 pos_cross_pt[4];  // ポインターの十字座標
	SimpleMath::Vector2 pos_heartR[2];
	SimpleMath::Vector2 pos_heartB[2];

	std::mt19937 random_engine;
	std::uniform_int_distribution<> random_dist;

	std::basic_string<wchar_t> turn_player[2];
	std::basic_string<wchar_t> hit_heart;  // デバック用

	Animation anim;

	enum POSI_Z {
		TURN_TEXT,
		TURN_IMAGE,
		SMOKE,
		HEART,
		POINTER,
		MAP,
		COMMAND,
		PLAYER,
		BACK_GROUND
	};

//

//

	const SimpleMath::Vector2  pos_pointer_df	= SimpleMath::Vector2(360.0f + 520.0f, 415.0f);
	const float								pos_Bx				= 1920;
	const float								pos_outArea		= -200;
	const float								move_pointer	= 179.5f;

	int num_player;  // ターンプレイヤーを表す、パッドの判別にも使用
	int num_turn;  // ターン数
	int num_ready;  // 開始時のハート設置のカウント
	int num_ready_all;  // 互いの準備完了状態
	int num_color[2];  // 画像の色

	bool seem_pointer;
	bool fin_game;  // デバック用
	int win_player;  // デバック用

	void LA_Load();
	void Up_Put();  // 後に赤と青どちらかを渡して動かしたい
	void Up_Select();
	void Up_Attack();
	void Up_At_Check();
	void Up_Move();  // 未実装
	void Up_Move_Pointer();
	void Up_Mo_Check();
	void Up_Fine();
	void Re_Draw_PlayerA();
	void Re_Draw_PlayerB();
	void Re_DirectTwelve();

public:
	enum Phase {
		PUT_HEART,
		START,
		SELECT,
		ATTACK,
		MOVE,
		FINE
	};
	static Phase phase;
};