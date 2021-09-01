//
// MainScene.cpp
//

#include "Base/pch.h"
#include "Base/dxtk.h"
#include "SceneFactory.h"

// Initialize member variables.
MainScene::MainScene()
{

}

// Initialize a variable and audio resources.
void MainScene::Initialize()
{
	// コントローラーの識別を入れる
	font						= DX9::SpriteFont::CreateFromFontName(DXTK->Device9, L"FixedSys 標準", 20);
	pos_cursor			= SimpleMath::Vector2(100.0f, 100.0f);
	pos_pointer		= SimpleMath::Vector2(360.0f + 520.0f, 415.0f);
	pos_attack			= SimpleMath::Vector2(-200.0f, -200.0f);
	pos_heartR[0]	= SimpleMath::Vector2(-200.0f, -200.0f);
	pos_heartR[1]	= SimpleMath::Vector2(-200.0f, -200.0f);
	pos_heartB[0]	= SimpleMath::Vector2(-200.0f, -200.0f);
	pos_heartB[1]	= SimpleMath::Vector2(-200.0f, -200.0f);
	phase					= Phase::PUT_HEART;

	std::random_device seed;
	random_engine = std::mt19937(seed());
	random_dist		= std::uniform_int_distribution<>(0, 1);

	num_player		= 0;// random_dist(random_engine);
	num_ready			= 0;
	num_color[0]		= 255;
	num_color[1]		= 120;
	seem_pointer		= false;
	fin_game			= false;
	win_player			= 0;


	// エラる
	//turn_player[0] = num_player % 2 == 0 ? "じぶんのターン" : "あいてのターン";
	//turn_player[1] = num_player % 2 == 1 ? "あいてのターン" : "じぶんのターン";


	num_turn = 0;
}

// Allocate all memory the Direct3D and Direct2D resources.
void MainScene::LoadAssets()
{
	descriptorHeap = DX12::CreateDescriptorHeap(DXTK->Device, 1);

	ResourceUploadBatch resourceUploadBatch(DXTK->Device);
	resourceUploadBatch.Begin();

	RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	SpriteBatchPipelineStateDescription pd(rtState, &CommonStates::NonPremultiplied);
	D3D12_VIEWPORT viewport = {
	0.0f, 0.0f, 1280.0f, 720.0f,
	D3D12_MIN_DEPTH, D3D12_MAX_DEPTH
	};

	spriteBatch = DX12::CreateSpriteBatch(DXTK->Device, resourceUploadBatch, pd, &viewport);

	dx9GpuDescriptor = DXTK->Direct3D9->CreateShaderResourceView(descriptorHeap.get(), 0);

	auto uploadResourcesFinished = resourceUploadBatch.End(DXTK->CommandQueue);
	uploadResourcesFinished.wait();

	LA_Load();
}

// Releasing resources required for termination.
void MainScene::Terminate()
{
	DXTK->ResetAudioEngine();
	DXTK->WaitForGpu();

	// TODO: Add your Termination logic here.

}

// Direct3D resource cleanup.
void MainScene::OnDeviceLost()
{

}

// Restart any looped sounds here
void MainScene::OnRestartSound()
{

}

// Updates the scene.
NextScene MainScene::Update(const float deltaTime)
{
	// If you use 'deltaTime', remove it.
	UNREFERENCED_PARAMETER(deltaTime);

	// TODO: Add your game logic here.

	switch (phase) {
		case Phase::PUT_HEART:	seem_pointer = true;	Up_Put();			break;
			// ここで先攻後攻決めるべきかな
		case Phase::SELECT:													Up_Select();	break;
		case Phase::ATTACK:			seem_pointer = true;	Up_Attack();	break;
		case Phase::MOVE:			break;
		case Phase::CHECK:													Up_Check();	break;
		case Phase::FINE:				break;
	}

	// パッドの使い方
	//const float squareX = DXTK->GamePadState[gamePad_index].thumbSticks.leftX;
	//const float squareY = DXTK->GamePadState[gamePad_index].thumbSticks.leftY;



	//Up_Move_Pointer();

	return NextScene::Continue;
}

// Draws the scene.
void MainScene::Render()
{
	// TODO: Add your rendering code here.


	DXTK->Direct3D9->Clear(DX9::Colors::RGBA(0, 0, 0, 255));  // 画面をクリア
	DXTK->Direct3D9->BeginScene();  // シーンの開始を宣言

	DX9::SpriteBatch->Begin();  // スプライトの描画を開始

	Re_Draw_PlayerA();
	Re_Draw_PlayerB();

	DX9::SpriteBatch->End();  // スプライトの描画を終了
	DXTK->Direct3D9->EndScene();  // シーンの終了を宣言

	Re_DirectTwelve();
}

void MainScene::LA_Load() {
	bg					= DX9::Sprite::CreateFromFile(DXTK->Device9, L"bg.png");
	map					= DX9::Sprite::CreateFromFile(DXTK->Device9, L"map.png");
	pointer			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"pointer.png");
	area_attack		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"attack_area.png");
	heart_red		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"love.png");
	heart_blue		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"trap.png");
	boy					= DX9::Sprite::CreateFromFile(DXTK->Device9, L"boy.png");
	girl					= DX9::Sprite::CreateFromFile(DXTK->Device9, L"girl.png");
	com_cursor	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"カーソル.png");
}

void MainScene::Up_Put() {
	Up_Move_Pointer();
	NEXT_PUT:
	if (DXTK->KeyEvent->pressed.Enter) {
		if (num_ready == 0) {
			pos_heartR[num_player] = pos_pointer;
			num_ready = 1;
			goto NEXT_PUT;
		}
		if (num_ready == 1) {
			if (pos_pointer == pos_heartR[num_player]) return;
			pos_heartB[num_player] = pos_pointer;
			num_ready = 0;
			seem_pointer = false;
			phase = Phase::SELECT;
		}
	}
}

void MainScene::Up_Select() {
	if (DXTK->KeyEvent->pressed.Up) {
		pos_cursor.y -= 100.0f;
		if (pos_cursor.y < 100.0f) pos_cursor.y = 200.0f;
	}
	if (DXTK->KeyEvent->pressed.Down) {
		pos_cursor.y += 100.0f;
		if (pos_cursor.y > 200.0f) pos_cursor.y = 100.0f;
	}

	if (DXTK->KeyEvent->pressed.Enter) {
		seem_pointer = false;
		if (pos_cursor.y == 100.0f) phase = Phase::ATTACK;
		if (pos_cursor.y == 200.0f) phase = Phase::MOVE;
	}
}

void MainScene::Up_Attack() {
	Up_Move_Pointer();
	if (DXTK->KeyEvent->pressed.Enter) {
		pos_attack			= pos_pointer;
		seem_pointer		= false;
		phase = Phase::CHECK;
	}
}

void MainScene::Up_Move() {

}

void MainScene::Up_Move_Pointer() {
	if (DXTK->KeyEvent->pressed.Up)			pos_pointer.y -= move_pointer;
	if (DXTK->KeyEvent->pressed.Down)		pos_pointer.y += move_pointer;
	if (DXTK->KeyEvent->pressed.Left)		pos_pointer.x -= move_pointer;
	if (DXTK->KeyEvent->pressed.Right)		pos_pointer.x += move_pointer;
	pos_pointer = SimpleMath::Vector2(
		std::clamp(pos_pointer.x, 510.0f, 510.0f + 718.0f),
		std::clamp(pos_pointer.y, 45.0f, 945.0f - 179.5f)
	);
}

void MainScene::Up_Check() {
	if (pos_heartR[num_player] == pos_pointer) {
		fin_game = true;
		phase = Phase::FINE;
	}
}

// 基本こちらで実装テストを行う。
void MainScene::Re_Draw_PlayerA() {
	DX9::SpriteBatch->DrawSimple(
		bg.Get(), SimpleMath::Vector3(0.0f, 0.0f, POSI_Z::BACK_GROUND)
	);
	DX9::SpriteBatch->DrawSimple(
		map.Get(), SimpleMath::Vector3(510.0f, 45.0f, POSI_Z::MAP)
	);
	DX9::SpriteBatch->DrawSimple(
		heart_red.Get(), SimpleMath::Vector3(pos_heartR[0].x, pos_heartR[0].y, POSI_Z::HEART)
	);
	DX9::SpriteBatch->DrawSimple(
		heart_blue.Get(), SimpleMath::Vector3(pos_heartB[0].x, pos_heartB[0].y, POSI_Z::HEART)
	);
	DX9::SpriteBatch->DrawSimple(
		boy.Get(), 
		SimpleMath::Vector3(pos_Bx - 402.0f, 990.0f - 590.0f, POSI_Z::PLAYER),
		Rect(0.0f, 0.0f, 402.0f, 590.0f), 
		DX9::Colors::RGBA(num_color[0], num_color[0], num_color[0], 255)
	);
	DX9::SpriteBatch->DrawSimple(
		girl.Get(), 
		SimpleMath::Vector3(0.0f, 990.0f - 476.0f, POSI_Z::PLAYER),
		Rect(0.0f, 0.0f, 438.0f, 476.0f), 
		DX9::Colors::RGBA(num_color[1], num_color[1], num_color[1], 255)
	);


	// エラる
	//DX9::SpriteBatch->DrawString(
	//	font.Get(), 
	//	SimpleMath::Vector2(200.0f, 100.0f),
	//	DX9::Colors::RGBA(255, 255, 255, 255),
	//	turn_player[0]
	//);
	//DX9::SpriteBatch->DrawString(
	//	font.Get(),
	//	SimpleMath::Vector2(200.0f, 100.0f),
	//	DX9::Colors::RGBA(255, 255, 255, 255),
	//	L"あいてのターン"
	//);

	if (phase == Phase::SELECT) {
		DX9::SpriteBatch->DrawSimple(
			com_cursor.Get(), SimpleMath::Vector3(pos_cursor.x, pos_cursor.y, POSI_Z::COMMAND)
		);
		DX9::SpriteBatch->DrawString(
			font.Get(),
			SimpleMath::Vector2(200.0f, 100.0f),
			DX9::Colors::RGBA(255, 255, 255, 255),
			L"ATTACK"
		);
		DX9::SpriteBatch->DrawString(
			font.Get(),
			SimpleMath::Vector2(200.0f, 200.0f),
			DX9::Colors::RGBA(255, 255, 255, 255),
			L"MOVE"
		);
	}
	if (phase == Phase::ATTACK) {
		DX9::SpriteBatch->DrawSimple(
			area_attack.Get(),
			SimpleMath::Vector3(pos_attack.x, pos_attack.y, POSI_Z::POINTER)
		);
	}
	if (seem_pointer) {
		DX9::SpriteBatch->DrawSimple(
			pointer.Get(),
			SimpleMath::Vector3(pos_pointer.x, pos_pointer.y, POSI_Z::POINTER)
		);
	}

	// 表示されない
	if (fin_game) {
		DX9::SpriteBatch->DrawString(
			font.Get(),
			SimpleMath::Vector2(200.0f, 100.0f),
			DX9::Colors::RGBA(255, 255, 255, 255),
			L"勝者 : プレイヤー%i" + win_player
		);
	}

	DX9::SpriteBatch->DrawString(
		font.Get(),
		SimpleMath::Vector2(200.0f, 500.0f),
		DX9::Colors::RGBA(255, 255, 255, 255),
		L"Phase : %i", (int)phase
	);
}

void MainScene::Re_Draw_PlayerB() {
	DX9::SpriteBatch->DrawSimple(
		bg.Get(), SimpleMath::Vector3(pos_Bx, 0.0f, POSI_Z::BACK_GROUND)
	);
	DX9::SpriteBatch->DrawSimple(
		map.Get(), SimpleMath::Vector3(pos_Bx + 510.0f, 45.0f, POSI_Z::MAP)
	);
	DX9::SpriteBatch->DrawSimple(
		boy.Get(), 
		SimpleMath::Vector3(pos_Bx, 990.0f - 590.0f, POSI_Z::PLAYER),
		Rect(0.0f, 0.0f, 402.0f, 590.0f),
		DX9::Colors::RGBA(num_color[0], num_color[0], num_color[0], 255)
	);
	DX9::SpriteBatch->DrawSimple(
		girl.Get(), 
		SimpleMath::Vector3(2 * pos_Bx - 438.0f, 990.0f - 476.0f, POSI_Z::PLAYER),
		Rect(0.0f, 0.0f, 438.0f, 476.0f),
		DX9::Colors::RGBA(num_color[1], num_color[1], num_color[1], 255)
	);

	// 仮のため、後に画像変数を変更
	DX9::SpriteBatch->DrawSimple(
		area_attack.Get(), SimpleMath::Vector3(pos_pointer.x + pos_Bx, pos_pointer.y, POSI_Z::POINTER)
	);
}

void MainScene::Re_DirectTwelve() {
	DXTK->Direct3D9->UpdateResource();

	DXTK->ResetCommand();
	DXTK->ClearRenderTarget(Colors::CornflowerBlue);


	const auto heapes = descriptorHeap->Heap();
	DXTK->CommandList->SetDescriptorHeaps(1, &heapes);

	spriteBatch->Begin(DXTK->CommandList);
	spriteBatch->Draw(
		dx9GpuDescriptor,
		XMUINT2(1280, 720),   // HD
		SimpleMath::Vector2(0.0f, 0.0f)
	);
	spriteBatch->End();

	DXTK->Direct3D9->WaitUpdate();
	DXTK->ExecuteCommandList();
}
