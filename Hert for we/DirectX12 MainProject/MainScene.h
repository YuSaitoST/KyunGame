//
// MainScene.h
//

#pragma once

#include "Scene.h"

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
	DX9::SPRITE area_attack;
	DX9::SPRITE heart_red;
	DX9::SPRITE heart_blue;
	DX9::SPRITE boy;
	DX9::SPRITE girl;
	DX9::SPRITE com_cursor;

	SimpleMath::Vector2 pos_cursor;
	SimpleMath::Vector2 pos_pointer;
	SimpleMath::Vector2 pos_heartR[2];
	SimpleMath::Vector2 pos_heartB[2];

	enum POSI_Z {
		TURN_TEXT,
		TURN_IMAGE,
		SMOKE,
		POINTER,
		HEART,
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

	const int pos_Bx = 1920;
	const float move_pointer = 179.5f;

	int num_turn;
	int num_player;  // �^�[���v���C���[��\��
	int num_color[2];

	bool seem_pointer;

	void LA_Load();
	void Up_Put();  // ��ɐԂƐǂ��炩��n���ē���������
	void Up_Select();
	void Up_Move_Pointer();
	void Re_Draw_PlayerA();
	void Re_Draw_PlayerB();
	void Re_DirectTwelve();
};