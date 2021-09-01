//
// MainScene.h
//

#pragma once

#include "Scene.h"
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
	DX9::SPRITE heart_red;
	DX9::SPRITE heart_blue;
	DX9::SPRITE boy;
	DX9::SPRITE girl;
	DX9::SPRITE com_cursor;

	SimpleMath::Vector2 pos_pointer;
	SimpleMath::Vector2 pos_cursor;
	SimpleMath::Vector2 pos_attack;
	SimpleMath::Vector2 pos_heartR[2];
	SimpleMath::Vector2 pos_heartB[2];

	std::mt19937 random_engine;
	std::uniform_int_distribution<> random_dist;

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

	enum Phase {
		PUT_HEART,
		SELECT,
		ATTACK,
		MOVE,
		CHECK,
		FINE
	};

	Phase phase;

	const SimpleMath::Vector2  pos_pointer_df	= SimpleMath::Vector2(360.0f + 520.0f, 415.0f);
	const int									pos_Bx				= 1920;
	const int									pos_outArea		= -200;
	const float								move_pointer	= 179.5f;

	int num_player;  // �^�[���v���C���[��\���A�p�b�h�̔��ʂɂ��g�p
	int num_turn;  // �^�[����
	int num_ready;  // �J�n���̃n�[�g�ݒu�̃J�E���g
	int num_color[2];  // �摜�̐F

	bool seem_pointer;
	bool fin_game;  // �f�o�b�N�p
	int win_player;  // �f�o�b�N�p

	std::basic_string<wchar_t> turn_player[2];

	void LA_Load();
	void Up_Put();  // ��ɐԂƐǂ��炩��n���ē���������
	void Up_Select();
	void Up_Attack();
	void Up_Move();  // ������
	void Up_Move_Pointer();
	void Up_Check();
	void Up_Fine();
	void Re_Draw_PlayerA();
	void Re_Draw_PlayerB();
	void Re_DirectTwelve();
};