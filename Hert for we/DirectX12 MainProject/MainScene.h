//
// MainScene.h
//

#pragma once

#include "Scene.h"
//#include"Classes/LoadLines.h"
#include "Classes/Smoke.h"
#include"Classes/Attack.h"
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
	DX9::SPRITE boy_a[5];
	DX9::SPRITE girl_a[5];
	DX9::SPRITE boy_b[5];
	DX9::SPRITE girl_b[5];
	DX9::SPRITE com_cursor;
	DX9::SPRITE speech;

	SimpleMath::Vector2 pos_pointer;
	SimpleMath::Vector2 pos_pointer_ready[2];
	SimpleMath::Vector2 pos_cursor;
	SimpleMath::Vector2 pos_attack;
	SimpleMath::Vector2 pos_move[5];
	SimpleMath::Vector2 pos_cross_hR[5];  // �n�[�g�Ԃ̏\�����W(���S���܂߂�)
	SimpleMath::Vector2 pos_heart[2];
	SimpleMath::Vector2 pos_heart_old;

	std::mt19937 random_engine;
	std::uniform_int_distribution<> random_dist;

	std::basic_string<wchar_t> turn_player[2];

	Smoke smoke;
	Attack attack;

	const SimpleMath::Vector2	POS_FIELD				= SimpleMath::Vector2(505.0f,	40.0f);
	const SimpleMath::Vector2	POS_CENTER			= SimpleMath::Vector2(878.0f,	413.0f);
	const SimpleMath::Vector2	POS_END_UL			= SimpleMath::Vector2(520.0f,	55.0f);
	const SimpleMath::Vector2	POS_END_DR		= SimpleMath::Vector2(1236.0f,	771.0f);
	const float								POS_X2					= 1920.0f;
	const float								POS_OUTAREA		= -200.0f;
	const float								MOVE_POINTER	= 179.0f;

	int num_turn;  // �^�[����
	int num_ready;  // �݂��̏����������
	int num_ready_all[2];  // ���҂������������ǂ���
	int num_color[2];  // �摜�̐F

	bool flag_debug;  // �f�o�b�O���̂�true�ɂ���
	bool fin_game;  // �f�o�b�N�p
	int win_player;  // �f�o�b�N�p

	void LA_Load();
	void Up_Put(int index);  // ��ɐԂƐǂ��炩��n���ē���������
	void Up_Start(float deltaTime);
	void Up_Select();
	void Up_Attack(float deltaTime);
	void Up_At_Check(float deltaTime);
	void Up_Move();  // ������
	void Up_Mo_Check();
	void Up_Move_Pointer(int index);
	void Up_Fine();
	void Re_Draw_Standard(float pos_x, int index);
	void Re_Draw_PlayerA();
	void Re_Draw_PlayerB();
	void Re_DirectTwelve();

public:
	enum PLAYER {
		A,
		B
	};

	enum POSI_Z {
		TURN_TEXT,
		TURN_IMAGE,
		SMOKE,
		COMMAND,
		PLAYER,
		HEART,
		POINTER,
		MAP,
		BACK_GROUND
	};

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

	enum EMOTION {
		GENERALLY,  // �ʏ�
		PROPOSAL,  // �U��
		NERVOUS,  // �ł�
		VICTORY,
		DEFEAT  // �s�k
	};

	const SimpleMath::Vector2 POS_LEFT_GENE			= SimpleMath::Vector2(0.0f, 240.0f);
	const SimpleMath::Vector2 POS_LEFT_ATTACK		= SimpleMath::Vector2(0.0f, 0.0f);
	const SimpleMath::Vector2 POS_BOY_GE_A			= SimpleMath::Vector2(1420.0f, 240.0f);
	const SimpleMath::Vector2 POS_BOY_AT_A			= SimpleMath::Vector2(620.0f, 0.0f);
	const SimpleMath::Vector2 POS_GIRL_GE_B			= SimpleMath::Vector2(1370.0f, 240.0f);
	const SimpleMath::Vector2 POS_GIRL_AT_B			= SimpleMath::Vector2(980.0f, 0.0f);

	const float RC_GIRL_NOM_R = 550.0f;
	const float RC_GIRL_ATT_R = 940.0f;
	const float RC_BOY_NOM_R = 500.0f;
	const float RC_BOY_ATT_R = 1300.0f;

	static EMOTION emotion[2];  //�����p���ăL�����N�^�[�̏�Ԃ�\��

	static int num_player;  // �^�[���v���C���[��\���A�p�b�h�̔��ʂɂ��g�p
	static int flag_attack;  // �U���R���[�`���̃t���O
};