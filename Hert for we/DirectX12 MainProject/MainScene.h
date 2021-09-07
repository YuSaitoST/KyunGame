//
// MainScene.h
//

#pragma once

#include "Scene.h"
#include "Classes/Smoke.h"
#include <random>
#include <Classes/BarUI.h>

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

	DX9::SPRITE bg[2];
	DX9::SPRITE map;
	DX9::SPRITE pointer;
	DX9::SPRITE area_attack;
	DX9::SPRITE area_move;
	DX9::SPRITE heart_red;
	DX9::SPRITE boy[5];
	DX9::SPRITE girl[5];
	DX9::SPRITE com_cursor;
	DX9::SPRITE speech;

	SimpleMath::Vector2 pos_pointer;
	SimpleMath::Vector2 pos_pointer_ready[2];
	SimpleMath::Vector2 pos_cursor;
	SimpleMath::Vector2 pos_attack;
	SimpleMath::Vector2 pos_move[4];
	SimpleMath::Vector2 pos_cross_hR[5];  // ハート赤の十字座標(中心も含める)
	SimpleMath::Vector2 pos_cross_pt[4];  // ポインターの十字座標
	SimpleMath::Vector2 pos_heart[2];

	std::mt19937 random_engine;
	std::uniform_int_distribution<> random_dist;

	std::basic_string<wchar_t> turn_player[2];

	Smoke smoke;
	BarUI baner;

	enum PLAYER {
		A,
		B
	};

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

	enum EMOTION {
		GENERALLY,  // 通常
		PROPOSAL,  // 攻撃
		NERVOUS,  // 焦り
		VICTORY,
		DEFEAT  // 敗北
	};

	EMOTION emotion[2];  //これを用いてキャラクターの状態を表す

	const SimpleMath::Vector2	POS_FIELD				= SimpleMath::Vector2(505.0f,	40.0f);
	const SimpleMath::Vector2	POS_CENTER			= SimpleMath::Vector2(878.0f,	413.0f);
	const SimpleMath::Vector2	POS_END_UL			= SimpleMath::Vector2(520.0f,	55.0f);
	const SimpleMath::Vector2	POS_END_DR		= SimpleMath::Vector2(1236.0f,	771.0f);
	const float								POS_X2					= 1920.0f;
	const float								POS_OUTAREA		= -200.0f;
	const float								MOVE_POINTER	= 179.0f;

	int num_turn;  // ターン数
	int num_ready;  // 互いの準備完了状態
	int num_ready_all[2];  // 両者が準備完了かどうか
	int num_color[2];  // 画像の色

	bool flag_debug;  // デバッグ時のみtrueにする
	bool fin_game;  // デバック用
	int win_player;  // デバック用

	void LA_Load();
	void Up_Put(int index);  // 後に赤と青どちらかを渡して動かしたい
	void Up_Start(float deltaTime);
	void Up_Select();
	void Up_Attack();
	void Up_At_Check();
	void Up_Move();  // 未実装
	void Up_Mo_Check();
	void Up_Move_Pointer(int index);
	void Up_Fine();
	void Re_Draw_Standard(float pos_x, int index);
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
		FINE,
		SUCCEED
	};
	static Phase phase;
	static int num_player;  // ターンプレイヤーを表す、パッドの判別にも使用
};