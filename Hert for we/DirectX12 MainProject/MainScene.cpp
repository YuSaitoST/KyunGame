//
// MainScene.cpp
//

#include "Base/pch.h"
#include "Base/dxtk.h"
#include "SceneFactory.h"

MainScene::Phase MainScene::phase;
MainScene::EMOTION MainScene::emotion[2];
int MainScene::num_color[2];
int MainScene::num_player;
int MainScene::flag_attack;
bool MainScene::flag_suka;
bool MainScene::flag_graze;
bool MainScene::flag_hit;
bool MainScene::flag_debug;
SimpleMath::Vector2 MainScene::pos_girl_a;
SimpleMath::Vector2 MainScene::pos_girl_b;
SimpleMath::Vector2 MainScene::pos_boy_a;
SimpleMath::Vector2 MainScene::pos_boy_b;
float MainScene::POS_GIRL_A;
float MainScene::POS_GIRL_B;
float MainScene::POS_BOY_A;
float MainScene::POS_BOY_B;

// Initialize member variables.
MainScene::MainScene()
{

}

// Initialize a variable and audio resources.
void MainScene::Initialize()
{
	font										= DX9::SpriteFont::CreateFromFontFile(DXTK->Device9, L"Font/HuiFont.ttf", L"ふい字", 35);
	pos_pointer						= POS_CENTER;  // 通常時
	pos_pointer_ready[0]		= POS_CENTER;  // 以下2つは初回準備用
	pos_pointer_ready[1]		= POS_CENTER;
	pos_cursor							= SimpleMath::Vector2(100.0f, 100.0f);
	pos_attack							= SimpleMath::Vector2(POS_OUTAREA, POS_OUTAREA);
	pos_heart[0]						= SimpleMath::Vector2(POS_OUTAREA, POS_OUTAREA);
	pos_heart[1]						= SimpleMath::Vector2(POS_OUTAREA, POS_OUTAREA);
	pos_heart_old					= pos_heart[0];  // 一旦プレイヤーAの方を、ランダムで変わるならこっちも
	std::fill(std::begin(pos_move),			std::end(pos_move),			SimpleMath::Vector2(POS_OUTAREA, POS_OUTAREA));
	std::fill(std::begin(pos_cross_hR),		std::end(pos_cross_hR),	SimpleMath::Vector2(POS_OUTAREA, POS_OUTAREA));
	std::fill(std::begin(emotion),				std::end(emotion),				EMOTION::GENERALLY);

	black.Initialize();
	smoke.Initialize();
	attack.Initialize();
	move.Initialize();

	std::random_device seed;
	random_engine		= std::mt19937(seed());
	random_dist				= std::uniform_int_distribution<>(0, 1);

	phase						= Phase::SCENARIO;
	emotion[0]				= EMOTION::GENERALLY;
	emotion[1]				= EMOTION::GENERALLY;

	num_player			= 0;// random_dist(random_engine);
	num_ready				= 0;
	num_ready_all[0]	= 0;
	num_ready_all[1]	= 0;
	num_turn				= 0;
	num_color[0]			= 255;
	num_color[1]			= 255;

	POS_GIRL_A = 0.0f;
	POS_GIRL_B = 0.0f;
	POS_BOY_A = 0.0f;
	POS_GIRL_B = 0.0f;

	flag_attack = 0;
	flag_suka = false;
	flag_graze = false;
	flag_hit = false;

	/* ↓↓↓↓↓↓↓↓↓↓↓↓ */
	/*→*/flag_debug = true;  /*←*/   // リリーズ時falseにする
	/* ↑↑↑↑↑↑↑↑↑↑↑↑ */


	// デバック用
	fin_game			= false;
	win_player			= 0;
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

	// TODO: Add your game logic here

	bgm_main->Play();
	if (bgm_main->isComplete()) bgm_main->Replay();

	switch (phase) {
		case Phase::SCENARIO		:	Up_Scenario(deltaTime);				break;
		case Phase::PUT_HEART	:	Up_Put(0);		Up_Put(1);				break;
		case Phase::START				:	Up_Start(deltaTime);						break;		// ここで先攻後攻決めるべきかな
		case Phase::SELECT			:	Up_Select();									break;
		case Phase::ATTACK			:	Up_Attack(deltaTime);					break;
		case Phase::MOVE				:	Up_Move(deltaTime);					break;
		case Phase::FINE				:	Up_Fine();										break;
		case Phase::SUCCEED		:	return Up_Result(deltaTime);		break;
	}

	if (phase == Phase::MOVE) {
		move.Up_Moveing(deltaTime);
	}

	return NextScene::Continue;
}

// Draws the scene.
void MainScene::Render()
{
	// TODO: Add your rendering code here.


	DXTK->Direct3D9->Clear(DX9::Colors::RGBA(0, 0, 0, 255));  // 画面をクリア
	DXTK->Direct3D9->BeginScene();  // シーンの開始を宣言

	DX9::SpriteBatch->Begin();  // スプライトの描画を開始



	if (phase == Phase::START) smoke.Render();

	bool flag_black = phase == Phase::SCENARIO || phase == Phase::SUCCEED;
	if (flag_black) black.Render();
	if (phase == Phase::MOVE) move.Render(num_player);
	
	attack.Re_Speak();

	Re_Draw_PlayerA();
	Re_Draw_PlayerB();

	DX9::SpriteBatch->End();  // スプライトの描画を終了
	DXTK->Direct3D9->EndScene();  // シーンの終了を宣言

	Re_DirectTwelve();
}

void MainScene::LA_Load() {
	bg[PLAYER::A]								= DX9::Sprite::CreateFromFile(DXTK->Device9, L"BG/main_bg01.png");
	bg[PLAYER::B]								= DX9::Sprite::CreateFromFile(DXTK->Device9, L"BG/main_bg02.png");
	map[0]											= DX9::Sprite::CreateFromFile(DXTK->Device9, L"map_1.png");
	map[1]											= DX9::Sprite::CreateFromFile(DXTK->Device9, L"map_2.png");
	heart_red										= DX9::Sprite::CreateFromFile(DXTK->Device9, L"love.png");
	pointer											= DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/pointer.png");
	area_attack										= DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/attack_area.png");
	area_move										= DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/move_area.png");
	com_cursor									= DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/カーソル.png");
	speech											= DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/speech_balloon.png");
	select												= DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/choices.png");

	boy_a[EMOTION::GENERALLY]	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/boy_generally.png");
	boy_a[EMOTION::PROPOSAL]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/boy_attack.png");
	boy_a[EMOTION::NERVOUS]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/boy_near.png");
	boy_a[EMOTION::VICTORY]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/boy_victory.png");
	boy_a[EMOTION::DEFEAT]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/boy_defeat.png");
	girl_a[EMOTION::GENERALLY]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/A_player/girl_generally.png");
	girl_a[EMOTION::PROPOSAL]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/A_player/girl_attack.png");
	girl_a[EMOTION::NERVOUS]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/A_player/girl_near.png");
	girl_a[EMOTION::VICTORY]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/A_player/girl_victory.png");
	girl_a[EMOTION::DEFEAT]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/A_player/girl_defeat.png");

	boy_b[EMOTION::GENERALLY]	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/boy_generally_l.png");
	boy_b[EMOTION::PROPOSAL]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/boy_attack_l.png");
	boy_b[EMOTION::NERVOUS]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/boy_near_l.png");
	boy_b[EMOTION::VICTORY]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/boy_victory_l.png");
	boy_b[EMOTION::DEFEAT]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/boy_defeat_l.png");
	girl_b[EMOTION::GENERALLY]	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/girl_generally_r.png");
	girl_b[EMOTION::PROPOSAL]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/girl_attack_r.png");
	girl_b[EMOTION::NERVOUS]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/girl_near_r.png");
	girl_b[EMOTION::VICTORY]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/girl_victory_r.png");
	girl_b[EMOTION::DEFEAT]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/girl_defeat_r.png");

	bgm_main										= DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"BGM\\main_bgm.mp3");
	//se_decision= DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\title_bgm.mp3")
	//se_move= DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\title_bgm.mp3")
	//se_attack= DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\title_bgm.mp3")
	//se_nervous= DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\title_bgm.mp3")
	//se_speak= DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\title_bgm.mp3")
	//se_cancel= DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\title_bgm.mp3")
	//se_change= DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\title_bgm.mp3")


	black.LoadAssets();
	smoke.LoadAssets();
	attack.LoadAssets();
	move.LoadAseet();
}

void MainScene::Up_Scenario(float deltaTime) {
	bool flag_black = black.Up_Black(deltaTime);
	if (flag_black) 
		phase = Phase::PUT_HEART;
}

void MainScene::Up_Put(int index) {
	Up_Move_Pointer(index);

	const bool input_b_ = DXTK->GamePadEvent[index].b == GamePad::ButtonStateTracker::PRESSED;

	if (num_ready_all[index] == 0) {
		if (input_b_) {
			pos_heart[index] = pos_pointer_ready[index];
			num_ready_all[index] = 1;  // プレイヤーの操作完了を報告
			num_ready += 1;
		}
	}
	if (num_ready != 2) return;  // ゲームパッドが1台しかない場合、ここの値を1にすると1ターン分だけデバッグできる
	

	phase	= Phase::START;
}

void MainScene::Up_Start(float deltaTime) {
	std::fill(std::begin(num_ready_all), std::end(num_ready_all), 0);
	pos_heart_old = pos_heart[num_player];

	num_color[num_player] = 155;
	int partner_ = num_player ? 0 : 1;
	num_color[partner_] = 255;

	flag_suka		= false;
	flag_graze		= false;
	flag_hit			= false;

	//パチンコ演出したいなら↓
	//num_color[0] = num_color[0] == 120 ? 255 : 120;  // 色指定の式は先攻後攻処理完成後に再調整
	//num_color[1] = num_color[1] == 255 ? 120 : 255;
	bool fin_change_ = smoke.Up_Change(deltaTime);
	if(fin_change_) phase = Phase::SELECT;
}

void MainScene::Up_Select() {
	const bool cross_up_			= DXTK->GamePadEvent[num_player].dpadUp		== GamePad::ButtonStateTracker::PRESSED;
	const bool cross_down_		= DXTK->GamePadEvent[num_player].dpadDown	== GamePad::ButtonStateTracker::PRESSED;
	const bool input_b_				= DXTK->GamePadEvent[num_player].b					== GamePad::ButtonStateTracker::PRESSED;

	if (cross_up_)			pos_cursor.y = pos_cursor.y < 100.0f ? 200.0f : 100.0f;
	if (cross_down_)	pos_cursor.y = pos_cursor.y > 200.0f ? 100.0f : 200.0f;

	if (input_b_) {
		if (pos_cursor.y == 100.0f) phase = Phase::ATTACK;
		if (pos_cursor.y == 200.0f) phase = Phase::MOVE;
	}

	pos_pointer = POS_CENTER;
	pos_pointer_ready[num_player] = POS_CENTER;
	pos_heart[num_player] = pos_heart_old;


	// 十字方向の座標の取得
			/*以下のコードはハート設置のタイミングで代入するべき、常に値が変更される*/
	pos_cross_hR[0] = SimpleMath::Vector2(pos_heart[num_player].x, pos_heart[num_player].y - MOVE_POINTER);
	pos_cross_hR[1] = SimpleMath::Vector2(pos_heart[num_player].x + MOVE_POINTER, pos_heart[num_player].y);
	pos_cross_hR[2] = SimpleMath::Vector2(pos_heart[num_player].x, pos_heart[num_player].y + MOVE_POINTER);
	pos_cross_hR[3] = SimpleMath::Vector2(pos_heart[num_player].x - MOVE_POINTER, pos_heart[num_player].y);
	pos_cross_hR[4] = SimpleMath::Vector2(pos_heart[num_player].x, pos_heart[num_player].y);

	pos_move[0] = pos_cross_hR[0];
	pos_move[1] = pos_cross_hR[1];
	pos_move[2] = pos_cross_hR[2];
	pos_move[3] = pos_cross_hR[3];
	pos_move[4] = pos_cross_hR[4];


	if (phase != Phase::MOVE) return;

	// 場外にでた移動マスを消す
	for (int i = 0; i < 4; i++) {
		bool end_x_ = pos_move[i].x < POS_END_UL.x || pos_move[i].x > POS_END_DR.x;
		bool end_y_ = pos_move[i].y < POS_END_UL.y || pos_move[i].y > POS_END_DR.y;
		if (end_x_ || end_y_) {
			pos_move[i] = SimpleMath::Vector2(POS_OUTAREA, POS_OUTAREA);
		}
	}
	pos_pointer_ready[num_player] = pos_cross_hR[4];
}

void MainScene::Up_Attack(float deltaTime) {
	int partner_ = (num_player == 0);  // 相手プレイヤーを指定

	if (flag_attack) {
		// コルーチン用のフラグ
		// ポインターとハート、それぞれの十字が重なっているか
		float pos_x = 0;
		for (int j = 0; j < 4; j++) {
			if (pos_cross_hR[j] == SimpleMath::Vector2(pos_pointer.x + pos_x, pos_pointer.y)) flag_graze = true;
		}
		if (pos_heart[partner_] == pos_pointer_ready[num_player])		flag_hit		= true;
		else if (!flag_graze && !flag_hit)														flag_suka	= true;

		bool fin_attack = attack.Up_Attack(deltaTime);
		if (!fin_attack) return;

		
		if (flag_hit) {
			emotion[num_player] = EMOTION::VICTORY;
			emotion[partner_] = EMOTION::DEFEAT;
			phase = Phase::SUCCEED;
			return;
		}

		flag_attack = 0;
		emotion[num_player] = EMOTION::GENERALLY;
		phase = Phase::FINE;
		return;
	}


	Up_Move_Pointer(num_player);
	const bool input_a_		= DXTK->GamePadEvent[num_player].a == GamePad::ButtonStateTracker::PRESSED;
	const bool input_b_		= DXTK->GamePadEvent[num_player].b == GamePad::ButtonStateTracker::PRESSED;

	if (input_a_) {
		phase								= Phase::SELECT;
//		emotion[num_player]	= EMOTION::GENERALLY;
		return;
	}
	if (input_b_) {
		pos_attack = pos_pointer_ready[num_player];
		flag_attack += 1;
		Up_At_Check(deltaTime);
	}
}

void MainScene::Up_At_Check(float deltaTime) {
	int partner_ = (num_player == 0);  // 相手プレイヤーを指定

	pos_cross_hR[0] = SimpleMath::Vector2(pos_heart[partner_].x, pos_heart[partner_].y - 179.0f);
	pos_cross_hR[1] = SimpleMath::Vector2(pos_heart[partner_].x + 179.0f, pos_heart[partner_].y);
	pos_cross_hR[2] = SimpleMath::Vector2(pos_heart[partner_].x, pos_heart[partner_].y + 179.0f);
	pos_cross_hR[3] = SimpleMath::Vector2(pos_heart[partner_].x - 179.0f, pos_heart[partner_].y);
	pos_cross_hR[4] = SimpleMath::Vector2(pos_heart[partner_].x, pos_heart[partner_].y);
}

void MainScene::Up_Move(float deltaTime) {
	const bool cross_up_		= DXTK->GamePadEvent[num_player].dpadUp		== GamePad::ButtonStateTracker::PRESSED;
	const bool cross_down_	= DXTK->GamePadEvent[num_player].dpadDown	== GamePad::ButtonStateTracker::PRESSED;
	const bool cross_left_		= DXTK->GamePadEvent[num_player].dpadLeft		== GamePad::ButtonStateTracker::PRESSED;
	const bool cross_right_		= DXTK->GamePadEvent[num_player].dpadRight	== GamePad::ButtonStateTracker::PRESSED;

	if (cross_up_) {
		if (pos_pointer_ready[num_player] == pos_cross_hR[0]) pos_pointer_ready[num_player] = pos_cross_hR[0];
		else if (pos_pointer_ready[num_player] == pos_cross_hR[4]) pos_pointer_ready[num_player] = pos_cross_hR[0];
		else if (pos_pointer_ready[num_player] == pos_cross_hR[2]) pos_pointer_ready[num_player] = pos_cross_hR[4];
	}
	if (cross_down_) {
		if (pos_pointer_ready[num_player] == pos_cross_hR[0]) pos_pointer_ready[num_player] = pos_cross_hR[4];
		else if (pos_pointer_ready[num_player] == pos_cross_hR[4]) pos_pointer_ready[num_player] = pos_cross_hR[2];
		else if (pos_pointer_ready[num_player] == pos_cross_hR[2]) pos_pointer_ready[num_player] = pos_cross_hR[2];
	}
	if (cross_left_) {
		if (pos_pointer_ready[num_player] == pos_cross_hR[1]) pos_pointer_ready[num_player] = pos_cross_hR[4];
		else if (pos_pointer_ready[num_player] == pos_cross_hR[4]) pos_pointer_ready[num_player] = pos_cross_hR[3];
		else if (pos_pointer_ready[num_player] == pos_cross_hR[3]) pos_pointer_ready[num_player] = pos_cross_hR[3];
	}
	if (cross_right_) {
		if (pos_pointer_ready[num_player] == pos_cross_hR[1]) pos_pointer_ready[num_player] = pos_cross_hR[1];
		else if (pos_pointer_ready[num_player] == pos_cross_hR[4]) pos_pointer_ready[num_player] = pos_cross_hR[1];
		else if (pos_pointer_ready[num_player] == pos_cross_hR[3]) pos_pointer_ready[num_player] = pos_cross_hR[4];
	}


	float lumberjack_up_		= std::max(POS_END_UL.y, pos_cross_hR[0].y);
	float lumberjack_down_	= std::min(POS_END_DR.y, pos_cross_hR[2].y);
	float lumberjack_left_		= std::max(POS_END_UL.x, pos_cross_hR[3].x);
	float lumberjack_right_	= std::min(POS_END_DR.x, pos_cross_hR[1].x);

	float pos_bx_ = 0;

	pos_pointer_ready[num_player] = SimpleMath::Vector2(
		std::clamp(pos_pointer_ready[num_player].x, lumberjack_left_ + pos_bx_, lumberjack_right_ + pos_bx_),
		std::clamp(pos_pointer_ready[num_player].y, lumberjack_up_, lumberjack_down_)
	);

	// 十字外に出たときのランバージャック
	for (int i = 0; i < 5; i++) {
		if (pos_pointer_ready[num_player] == SimpleMath::Vector2(pos_cross_hR[0].x - 179.0f, pos_cross_hR[0].y))	pos_pointer_ready[num_player] = pos_cross_hR[4];
		if (pos_pointer_ready[num_player] == SimpleMath::Vector2(pos_cross_hR[0].x + 179.0f, pos_cross_hR[0].y))	pos_pointer_ready[num_player] = pos_cross_hR[4];
		if (pos_pointer_ready[num_player] == SimpleMath::Vector2(pos_cross_hR[2].x + 179.0f, pos_cross_hR[2].y))	pos_pointer_ready[num_player] = pos_cross_hR[4];
		if (pos_pointer_ready[num_player] == SimpleMath::Vector2(pos_cross_hR[2].x - 179.0f, pos_cross_hR[2].y))	pos_pointer_ready[num_player] = pos_cross_hR[4];
	}

	pos_heart[num_player] = pos_pointer_ready[num_player];

	Up_Mo_Check(deltaTime);
}

void MainScene::Up_Mo_Check(float deltaTime) {
	const bool input_a_ = DXTK->GamePadEvent[num_player].a == GamePad::ButtonStateTracker::PRESSED;
	const bool input_b_ = DXTK->GamePadEvent[num_player].b == GamePad::ButtonStateTracker::PRESSED;


	if (input_a_) {
		phase = Phase::SELECT;
		return;
	}

	if (!input_b_) return;  // 早期return

	phase = Phase::FINE;
}

void MainScene::Up_Move_Pointer(int index) {
	if (num_ready_all[index] == 1) return; // 初期設定時、操作を終えたプレイヤーはカーソルを操作できなくなる
//	if (phase == Phase::PUT_HEART) num_player = index;  // 初回時のみ両プレイヤーが操作可能に

	const bool cross_up_			= DXTK->GamePadEvent[index].dpadUp		== GamePad::ButtonStateTracker::PRESSED;
	const bool cross_down_		= DXTK->GamePadEvent[index].dpadDown	== GamePad::ButtonStateTracker::PRESSED;
	const bool cross_left_			= DXTK->GamePadEvent[index].dpadLeft		== GamePad::ButtonStateTracker::PRESSED;
	const bool cross_right_			= DXTK->GamePadEvent[index].dpadRight	== GamePad::ButtonStateTracker::PRESSED;

	if (cross_up_)			pos_pointer_ready[index].y	-=	MOVE_POINTER;
	if (cross_down_)	pos_pointer_ready[index].y	+=	MOVE_POINTER;
	if (cross_left_)			pos_pointer_ready[index].x	-=	MOVE_POINTER;
	if (cross_right_)		pos_pointer_ready[index].x	+=	MOVE_POINTER;


	float lumberjack_up_			= POS_END_UL.y;
	float lumberjack_down_		= POS_END_DR.y;
	float lumberjack_left_			= POS_END_UL.x;
	float lumberjack_right_		= POS_END_DR.x;


	float pos_bx_ = 0;

	pos_pointer_ready[index] = SimpleMath::Vector2(
		std::clamp(pos_pointer_ready[index].x, lumberjack_left_ + pos_bx_, lumberjack_right_ + pos_bx_),
		std::clamp(pos_pointer_ready[index].y, lumberjack_up_, lumberjack_down_)
	);

	pos_pointer = pos_pointer_ready[index];
}

void MainScene::Up_Fine() {
	num_turn		+= 1;
	num_player	= num_player == 0 ? 1 : 0;  // ターン切り替え
	phase				= Phase::START;
}

NextScene MainScene::Up_Result(float deltaTime) {
	bool flag_black = black.Up_Black(deltaTime);
	if (flag_black) return NextScene::ResultScene;
	return NextScene::Continue;
}

void MainScene::Re_Draw_Standard(float pos_x, int index) {
	DX9::SpriteBatch->DrawSimple(
		bg[index].Get(), SimpleMath::Vector3(pos_x + 0.0f, 0.0f, POSI_Z::BACK_GROUND)
	);

	if (flag_debug) {
		DX9::SpriteBatch->DrawString(
			font.Get(), SimpleMath::Vector2(pos_x + 1500.0f, 100.0f),
			DX9::Colors::RGBA(0, 0, 0, 255), L"ターン数 : %i", num_turn
		);
		DX9::SpriteBatch->DrawString(
			font.Get(), SimpleMath::Vector2(pos_x + 1500.0f, 150.0f),
			DX9::Colors::RGBA(0, 0, 0, 255), L"フェイズ : %i", (int)phase
		);
		if (emotion[index] == EMOTION::GENERALLY) {
			DX9::SpriteBatch->DrawString(
				font.Get(), SimpleMath::Vector2(pos_x + 1500.0f, 200.0f),
				DX9::Colors::RGBA(0, 0, 0, 255), L"ノーヒット"
			);
		}
		if (emotion[index] == EMOTION::NERVOUS) {
			DX9::SpriteBatch->DrawString(
				font.Get(), SimpleMath::Vector2(pos_x + 1500.0f, 200.0f),
				DX9::Colors::RGBA(0, 0, 0, 255), L"ハートの近くをあてられた"
			);
		}
		if (emotion[index] == EMOTION::DEFEAT) {
			DX9::SpriteBatch->DrawString(
				font.Get(), SimpleMath::Vector2(pos_x + 1500.0f, 200.0f),
				DX9::Colors::RGBA(0, 0, 0, 255), L"ハートを当てた"
			);
		}
	}
	if (phase == Phase::PUT_HEART) {
		if (num_ready_all[index] == 1) return;
		DX9::SpriteBatch->DrawSimple(
			pointer.Get(),
			SimpleMath::Vector3(pos_x + pos_pointer_ready[index].x, pos_pointer_ready[index].y, POSI_Z::POINTER)
		);
	}
	if (num_player == index) {
		if (phase != Phase::ATTACK) {
			// アタック蒔は自分のハートの位置関係ないので非表示にさせる
			DX9::SpriteBatch->DrawSimple(
				heart_red.Get(), SimpleMath::Vector3(pos_x + pos_heart[index].x, pos_heart[index].y, POSI_Z::HEART)
			);
			DX9::SpriteBatch->DrawSimple(
				map[num_player].Get(), SimpleMath::Vector3(pos_x + POS_FIELD.x, POS_FIELD.y, POSI_Z::MAP)
			);
		}

		// 中途半端、デバッグして確認する
		int partner_ = num_player ? 0 : 1;
		DX9::SpriteBatch->DrawSimple(
			bg[partner_].Get(), SimpleMath::Vector3(pos_x + 0.0f, 0.0f, POSI_Z::BACK_GROUND)
		);
	}
	if (phase == Phase::ATTACK) {   // !flag_attack
		DX9::SpriteBatch->DrawSimple(
			area_attack.Get(),
			SimpleMath::Vector3(pos_pointer.x, pos_pointer.y, POSI_Z::POINTER)
		);
		DX9::SpriteBatch->DrawSimple(
			area_attack.Get(),
			SimpleMath::Vector3(pos_pointer.x + 1920.0f, pos_pointer.y, POSI_Z::POINTER)
		);
	}

	if (num_player != index) return;
	if (phase == Phase::SELECT) {
		DX9::SpriteBatch->DrawSimple(
			select.Get(), SimpleMath::Vector3(pos_x + 22.0f, 16.0f, POSI_Z::PLAYER)
		);
		DX9::SpriteBatch->DrawSimple(
			com_cursor.Get(), SimpleMath::Vector3(pos_x + pos_cursor.x, pos_cursor.y, POSI_Z::COMMAND)
		);
		DX9::SpriteBatch->DrawString(
			font.Get(), SimpleMath::Vector2(pos_x + 200.0f, 100.0f),
			DX9::Colors::RGBA(0, 0, 0, 255), L"ATTACK"
		);
		DX9::SpriteBatch->DrawString(
			font.Get(), SimpleMath::Vector2(pos_x + 200.0f, 200.0f),
			DX9::Colors::RGBA(0, 0, 0, 255), L"MOVE"
		);
	}
	if (phase == Phase::MOVE) {
		DX9::SpriteBatch->DrawSimple(
			area_move.Get(), SimpleMath::Vector3(pos_x + pos_move[0].x, pos_move[0].y, POSI_Z::POINTER)
		);
		DX9::SpriteBatch->DrawSimple(
			area_move.Get(), SimpleMath::Vector3(pos_x + pos_move[1].x, pos_move[1].y, POSI_Z::POINTER)
		);
		DX9::SpriteBatch->DrawSimple(
			area_move.Get(), SimpleMath::Vector3(pos_x + pos_move[2].x, pos_move[2].y, POSI_Z::POINTER)
		);
		DX9::SpriteBatch->DrawSimple(
			area_move.Get(), SimpleMath::Vector3(pos_x + pos_move[3].x, pos_move[3].y, POSI_Z::POINTER)
		);
		DX9::SpriteBatch->DrawSimple(
			area_move.Get(), SimpleMath::Vector3(pos_x + pos_move[4].x, pos_move[4].y, POSI_Z::POINTER)
		);
	}
}

void MainScene::Re_Draw_PlayerA() {
	Re_Draw_Standard(0.0f, PLAYER::A);

	pos_boy_a	= emotion[PLAYER::B] == EMOTION::PROPOSAL ? POS_BOY_AT_A : POS_BOY_GE_A;
	pos_girl_a	= emotion[PLAYER::A] == EMOTION::PROPOSAL ? POS_LEFT_ATTACK : POS_LEFT_GENE;

	float rc_boy_x_ = emotion[PLAYER::B] == EMOTION::PROPOSAL ? RC_BOY_ATT_R : RC_BOY_NOM_R;
	float rc_girl_x_ = emotion[PLAYER::A] == EMOTION::PROPOSAL ? RC_GIRL_ATT_R : RC_GIRL_NOM_R;

	float rc_y_g_ = emotion[PLAYER::A] == EMOTION::PROPOSAL ? 990.f : 750.0f;
	float rc_y_b_ = emotion[PLAYER::B] == EMOTION::PROPOSAL ? 990.f : 750.0f;

	DX9::SpriteBatch->DrawSimple(
		boy_a[emotion[PLAYER::B]].Get(), 
		SimpleMath::Vector3(pos_boy_a.x, POS_BOY_A + pos_boy_a.y, POSI_Z::PLAYER),
		Rect(0.0f, 0.0f, rc_boy_x_, rc_y_b_), 
		DX9::Colors::RGBA(num_color[1], num_color[1], num_color[1], Attack::alpha_boy)
	);
	DX9::SpriteBatch->DrawSimple(
		girl_a[emotion[PLAYER::A]].Get(),
		SimpleMath::Vector3(pos_girl_a.x, POS_GIRL_A + pos_girl_a.y, POSI_Z::PLAYER),
		Rect(0.0f, 0.0f, rc_girl_x_, rc_y_g_), 
		DX9::Colors::RGBA(num_color[0], num_color[0], num_color[0], Attack::alpha_girl)
	);
}

void MainScene::Re_Draw_PlayerB() {
	Re_Draw_Standard(POS_X2, PLAYER::B);

	pos_boy_b = emotion[PLAYER::B] == EMOTION::PROPOSAL ? POS_LEFT_ATTACK : POS_LEFT_GENE;
	pos_girl_b = emotion[PLAYER::A] == EMOTION::PROPOSAL ? POS_GIRL_AT_B : POS_GIRL_GE_B;

	float rc_x_g_ = emotion[PLAYER::A] == EMOTION::PROPOSAL ? RC_GIRL_ATT_R : RC_GIRL_NOM_R;
	float rc_x_b_ = emotion[PLAYER::B] == EMOTION::PROPOSAL ? RC_BOY_ATT_R : RC_BOY_NOM_R;

	float rc_y_g_ = emotion[PLAYER::A] == EMOTION::PROPOSAL ? 990.f : 750.0f;
	float rc_y_b_ = emotion[PLAYER::B] == EMOTION::PROPOSAL ? 990.f : 750.0f;

	DX9::SpriteBatch->DrawSimple(
		boy_b[emotion[PLAYER::B]].Get(),
		SimpleMath::Vector3(POS_X2 + pos_boy_b.x, POS_BOY_B + pos_boy_b.y, POSI_Z::PLAYER),
		Rect(0.0f, 0.0f, rc_x_b_, rc_y_b_),
		DX9::Colors::RGBA(num_color[1], num_color[1], num_color[1], Attack::alpha_boy)
	);
	DX9::SpriteBatch->DrawSimple(
		girl_b[emotion[PLAYER::A]].Get(),
		SimpleMath::Vector3(POS_X2 + pos_girl_b.x, POS_GIRL_B + pos_girl_b.y, POSI_Z::PLAYER),
		Rect(0.0f, 0.0f, rc_x_g_, rc_y_g_),
		DX9::Colors::RGBA(num_color[0], num_color[0], num_color[0], Attack::alpha_girl)
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
