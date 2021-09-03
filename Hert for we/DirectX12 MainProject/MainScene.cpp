//
// MainScene.cpp
//

#include "Base/pch.h"
#include "Base/dxtk.h"
#include "SceneFactory.h"

MainScene::Phase MainScene::phase;

// Initialize member variables.
MainScene::MainScene()
{

}

// Initialize a variable and audio resources.
void MainScene::Initialize()
{
	font						= DX9::SpriteFont::CreateFromFontFile(DXTK->Device9, L"azuki.ttf", L"あずきフォント", 20);
	pos_pointer[0]	= pos_pointer_df;
	pos_pointer[1]	= pos_pointer_df + SimpleMath::Vector2(pos_Bx, 0.0f);
	pos_cursor			= SimpleMath::Vector2(100.0f, 100.0f);
	pos_attack			= SimpleMath::Vector2(pos_outArea, pos_outArea);
	pos_heartR[0]	= SimpleMath::Vector2(pos_outArea, pos_outArea);
	pos_heartR[1]	= SimpleMath::Vector2(pos_outArea, pos_outArea);
//	pos_heartB[0]	= SimpleMath::Vector2(pos_outArea, pos_outArea);
//	pos_heartB[1]	= SimpleMath::Vector2(pos_outArea, pos_outArea);
	std::fill(std::begin(pos_move),			std::end(pos_move),			SimpleMath::Vector2(pos_outArea, pos_outArea));
	std::fill(std::begin(pos_cross_hR),		std::end(pos_cross_hR),	SimpleMath::Vector2(pos_outArea, pos_outArea));
//	std::fill(std::begin(pos_cross_hB),		std::end(pos_cross_hB),		SimpleMath::Vector2(pos_outArea, pos_outArea));
	std::fill(std::begin(pos_cross_pt),		std::end(pos_cross_pt),		SimpleMath::Vector2(pos_outArea, pos_outArea));

	anim.Initialize();

	std::random_device seed;
	random_engine = std::mt19937(seed());
	random_dist		= std::uniform_int_distribution<>(0, 1);

	phase					= Phase::PUT_HEART;

	num_player		= 0;// random_dist(random_engine);
	num_ready			= 0;
	num_ready_all	= 0;
	num_ready_R[0] = 0;
	num_ready_R[1] = 0;
	num_turn			= 0;
	num_color[0]		= 255;
	num_color[1]		= 120;
	seem_pointer		= false;




	// デバック用
	fin_game			= false;
	win_player			= 0;
	hit_heart = 'none';

	//turn_player[0] = num_player % 2 == 0 ? L"じぶんのターン" : L"あいてのターン";
	//turn_player[1] = num_player % 2 == 1 ? L"あいてのターン" : L"じぶんのターン";
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
		case Phase::PUT_HEART:	seem_pointer = true;	Up_Put(0);		Up_Put(1);		break;
		case Phase::START:													Up_Start();								break;			// ここで先攻後攻決めるべきかな
		case Phase::SELECT:			seem_pointer = false;	Up_Select();							break;
		case Phase::ATTACK:			seem_pointer = true;	Up_Attack();							break;
		case Phase::MOVE:			seem_pointer = false;	Up_Move();								break;
		case Phase::FINE:														Up_Fine();								break;
	}

	anim.Update(deltaTime);

	return NextScene::Continue;
}

// Draws the scene.
void MainScene::Render()
{
	// TODO: Add your rendering code here.


	DXTK->Direct3D9->Clear(DX9::Colors::RGBA(0, 0, 0, 255));  // 画面をクリア
	DXTK->Direct3D9->BeginScene();  // シーンの開始を宣言

	DX9::SpriteBatch->Begin();  // スプライトの描画を開始

	anim.Render();
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
	area_move		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"move_area.png");
	heart_red		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"love.png");
//	heart_blue		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"trap.png");
	boy					= DX9::Sprite::CreateFromFile(DXTK->Device9, L"boy.png");
	girl					= DX9::Sprite::CreateFromFile(DXTK->Device9, L"girl.png");
	com_cursor	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"カーソル.png");

	anim.LoadAssets();
}

void MainScene::Up_Put(int index) {
	Up_Move_Pointer(index);
	//NEXT_PUT:

	const bool input_a = DXTK->GamePadEvent[index].a == GamePad::ButtonStateTracker::PRESSED;

	if (input_a) {
		//if (num_ready == 0) {
		if(num_ready_R[index] == 0)
			pos_heartR[index] = pos_pointer[index];
			num_ready_R[index] = 1;
			num_ready_all += 1;  // 青ハート採用時は消す
			//goto NEXT_PUT;
		//}
		//if (num_ready == 1) {
		//	if (pos_pointer[index] == pos_heartR[index]) return;
		//	pos_heartB[index] = pos_pointer[index];
		//	num_ready = 0;
		//	num_ready_all += 1;
		//}
	}
	if (num_ready_all != 2) return;
	phase = Phase::SELECT;
}

void MainScene::Up_Start() {
	phase = Phase::SELECT;
}

void MainScene::Up_Select() {
	const bool cross_up			= DXTK->GamePadEvent[num_player].dpadUp		== GamePad::ButtonStateTracker::PRESSED;
	const bool cross_down		= DXTK->GamePadEvent[num_player].dpadDown	== GamePad::ButtonStateTracker::PRESSED;
	const bool input_a				= DXTK->GamePadEvent[num_player].a					== GamePad::ButtonStateTracker::PRESSED;

	if (cross_up) {
		pos_cursor.y -= 100.0f;
		if (pos_cursor.y < 100.0f) pos_cursor.y = 200.0f;
	}
	if (cross_down) {
		pos_cursor.y += 100.0f;
		if (pos_cursor.y > 200.0f) pos_cursor.y = 100.0f;
	}

	if (input_a) {
		seem_pointer = false;
		if (pos_cursor.y == 100.0f) phase = Phase::ATTACK;
		if (pos_cursor.y == 200.0f) phase = Phase::MOVE;
	}
}

void MainScene::Up_Attack() {
	Up_Move_Pointer(0);
	const bool input_a = DXTK->GamePadEvent[num_player].a == GamePad::ButtonStateTracker::PRESSED;
	if (input_a) {
		pos_attack			= pos_pointer[num_player];
		seem_pointer		= false;
		Up_At_Check();
	}
}

// 未確認
void MainScene::Up_At_Check() {
	int partner = num_player == 0 ? 1 : 0;

	pos_cross_hR[0] = SimpleMath::Vector2(pos_heartR[partner].x, pos_heartR[partner].y - move_pointer);
	pos_cross_hR[1] = SimpleMath::Vector2(pos_heartR[partner].x + move_pointer, pos_heartR[partner].y);
	pos_cross_hR[2] = SimpleMath::Vector2(pos_heartR[partner].x, pos_heartR[partner].y + move_pointer);
	pos_cross_hR[3] = SimpleMath::Vector2(pos_heartR[partner].x - move_pointer, pos_heartR[partner].y);

	//pos_cross_hB[0] = SimpleMath::Vector2(pos_heartB[partner].x, pos_heartB[partner].y - move_pointer);
	//pos_cross_hB[1] = SimpleMath::Vector2(pos_heartB[partner].x + move_pointer, pos_heartB[partner].y);
	//pos_cross_hB[2] = SimpleMath::Vector2(pos_heartB[partner].x, pos_heartB[partner].y + move_pointer);
	//pos_cross_hB[3] = SimpleMath::Vector2(pos_heartB[partner].x - move_pointer, pos_heartB[partner].y);

	pos_cross_pt[0] = SimpleMath::Vector2(pos_attack.x, pos_attack.y - move_pointer);
	pos_cross_pt[1] = SimpleMath::Vector2(pos_attack.x + move_pointer, pos_attack.y);
	pos_cross_pt[2] = SimpleMath::Vector2(pos_attack.x, pos_attack.y + move_pointer);
	pos_cross_pt[3] = SimpleMath::Vector2(pos_attack.x - move_pointer, pos_attack.y);

	// エリア外をハートの座標と重ねるループ
	for (int i = 0; i < 4; i++) {
		pos_cross_hR[i]= SimpleMath::Vector2(
			std::clamp(pos_pointer[num_player].x, 510.0f, 510.0f + 718.0f),
			std::clamp(pos_pointer[num_player].y, 45.0f, 945.0f - 179.5f)
		);
		//pos_cross_hB[i] = SimpleMath::Vector2(
		//	std::clamp(pos_pointer[num_player].x, 510.0f, 510.0f + 718.0f),
		//	std::clamp(pos_pointer[num_player].y, 45.0f, 945.0f - 179.5f)
		//);
		pos_cross_pt[i] = SimpleMath::Vector2(
			std::clamp(pos_pointer[num_player].x, 510.0f, 510.0f + 718.0f),
			std::clamp(pos_pointer[num_player].y, 45.0f, 945.0f - 179.5f)
		);
	}

	// ポインターとハート、それぞれの十字が重なっているか
	for (int j = 0; j < 4; j++) {
		for (int r = 0; r < 4; r++) if (pos_cross_pt[j] == pos_cross_hR[r]) { hit_heart = 'red'; }  // 赤ハートの近く
		//for (int b = 0; b < 4; b++) if (pos_cross_pt[j] == pos_cross_hB[b]) { hit_heart = 'blue'; }  // 青ハートの近く
	}

	const bool win = pos_heartR[num_player] == pos_pointer[num_player];
	//const bool down = pos_heartB[num_player] == pos_pointer[num_player];

	if (win) {

	}

	//if (down) {

	//}

	phase = Phase::FINE;
}

// 未実装
void MainScene::Up_Move() {
	Up_Move_Pointer(num_player);
	Up_Mo_Check();
}

void MainScene::Up_Mo_Check() {
	// 赤と青の移動
	const bool input_a = DXTK->GamePadEvent[num_player].a == GamePad::ButtonStateTracker::PRESSED;

	if (!input_a && num_ready == 0) pos_heartR[num_player] = pos_pointer[num_player];
//	if (!input_a && num_ready == 1) pos_heartB[num_player] = pos_pointer[num_player];

	if (input_a) {
		pos_heartR[num_player] = pos_pointer[num_player];
		phase = Phase::FINE;
		//if (num_ready == 0) {
		//	pos_heartR[num_player] = pos_pointer[num_player];
		//	num_ready = 1;
		//	return;
		//}
		//if (num_ready == 1) {
		//	pos_heartB[num_player] = pos_pointer[num_player];
		//	num_ready = 2;
		//}
	}

	if (num_ready == 2) phase = Phase::FINE;
}

void MainScene::Up_Move_Pointer(int index) {
	if (phase == Phase::PUT_HEART) num_player = index;

	const bool cross_up			= DXTK->GamePadEvent[num_player].dpadUp		== GamePad::ButtonStateTracker::PRESSED;
	const bool cross_down		= DXTK->GamePadEvent[num_player].dpadDown	== GamePad::ButtonStateTracker::PRESSED;
	const bool cross_left			= DXTK->GamePadEvent[num_player].dpadLeft		== GamePad::ButtonStateTracker::PRESSED;
	const bool cross_right		= DXTK->GamePadEvent[num_player].dpadRight	== GamePad::ButtonStateTracker::PRESSED;

	if (cross_up)		pos_pointer[num_player].y	-= move_pointer * 0.5f;
	if (cross_down)	pos_pointer[num_player].y += move_pointer * 0.5f;
	if (cross_left)		pos_pointer[num_player].x -= move_pointer * 0.5f;
	if (cross_right)	pos_pointer[num_player].x += move_pointer * 0.5f;

	pos_pointer[num_player] = SimpleMath::Vector2(
		std::clamp(pos_pointer[num_player].x, 510.0f, 510.0f + 718.0f),
		std::clamp(pos_pointer[num_player].y, 45.0f, 945.0f - 179.5f)
	);
}

void MainScene::Up_Fine() {
	num_turn += 1;
	num_player = num_player == 0 ? 1 : 0;  // ターン切り替え
	phase = Phase::START;
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
	//DX9::SpriteBatch->DrawSimple(
	//	heart_blue.Get(), SimpleMath::Vector3(pos_heartB[0].x, pos_heartB[0].y, POSI_Z::HEART)
	//);
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


	DX9::SpriteBatch->DrawString(
		font.Get(), 
		SimpleMath::Vector2(200.0f, 20.0f),
		DX9::Colors::RGBA(255, 255, 255, 255),
		turn_player[0].c_str()  // c言語のstringに変換して渡す
	);


	DX9::SpriteBatch->DrawString(
		font.Get(),
		SimpleMath::Vector2(200.0f, 800.0f),
		DX9::Colors::RGBA(255, 255, 255, 255),
		hit_heart.c_str()
	);

//	if (num_player != 0) return;

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
	if (phase == Phase::PUT_HEART || phase == Phase::ATTACK) {
		DX9::SpriteBatch->DrawSimple(
			pointer.Get(),
			SimpleMath::Vector3(pos_pointer[0].x, pos_pointer[0].y, POSI_Z::POINTER)
		);
	}


	// デバック用
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
		heart_red.Get(), SimpleMath::Vector3(pos_Bx + pos_heartR[1].x, pos_heartR[1].y, POSI_Z::HEART)
	);
	//DX9::SpriteBatch->DrawSimple(
	//	heart_blue.Get(), SimpleMath::Vector3(pos_Bx + pos_heartB[1].x, pos_heartB[1].y, POSI_Z::HEART)
	//);
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


	DX9::SpriteBatch->DrawString(
		font.Get(),
		SimpleMath::Vector2(pos_Bx + 200.0f, 20.0f),
		DX9::Colors::RGBA(255, 255, 255, 255),
		turn_player[1].c_str()  // c言語のstringに変換して渡す
	);

	DX9::SpriteBatch->DrawString(
		font.Get(),
		SimpleMath::Vector2(200.0f, 800.0f),
		DX9::Colors::RGBA(255, 255, 255, 255),
		hit_heart.c_str()
	);

	//if (num_player != 1) return;

	if (phase == Phase::SELECT) {
		DX9::SpriteBatch->DrawSimple(
			com_cursor.Get(), SimpleMath::Vector3(pos_Bx + pos_cursor.x, pos_cursor.y, POSI_Z::COMMAND)
		);
		DX9::SpriteBatch->DrawString(
			font.Get(),
			SimpleMath::Vector2(pos_Bx + 200.0f, 100.0f),
			DX9::Colors::RGBA(255, 255, 255, 255),
			L"ATTACK"
		);
		DX9::SpriteBatch->DrawString(
			font.Get(),
			SimpleMath::Vector2(pos_Bx + 200.0f, 200.0f),
			DX9::Colors::RGBA(255, 255, 255, 255),
			L"MOVE"
		);
	}
	if (phase == Phase::ATTACK) {
		DX9::SpriteBatch->DrawSimple(
			area_attack.Get(),
			SimpleMath::Vector3(pos_Bx + pos_attack.x, pos_attack.y, POSI_Z::POINTER)
		);
	}
	if (phase == Phase::PUT_HEART || phase == Phase::ATTACK) {
		DX9::SpriteBatch->DrawSimple(
			pointer.Get(),
			SimpleMath::Vector3(pos_Bx + pos_pointer[1].x, pos_pointer[1].y, POSI_Z::POINTER)
		);
	}


	// 表示されない
	if (fin_game) {
		DX9::SpriteBatch->DrawString(
			font.Get(),
			SimpleMath::Vector2(pos_Bx + 200.0f, 100.0f),
			DX9::Colors::RGBA(255, 255, 255, 255),
			L"勝者 : プレイヤー %i" + win_player
		);
	}

	// デバック用
	DX9::SpriteBatch->DrawString(
		font.Get(),
		SimpleMath::Vector2(pos_Bx + 200.0f, 500.0f),
		DX9::Colors::RGBA(255, 255, 255, 255),
		L"Phase : %i", (int)phase
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
