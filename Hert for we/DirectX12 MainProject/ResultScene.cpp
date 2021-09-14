//
// MainScene.cpp
//

#include "Base/pch.h"
#include "Base/dxtk.h"
#include "SceneFactory.h"
#include "Classes/LoadLines.h"

int ResultScene::winner;

// Initialize member variables.
ResultScene::ResultScene()
{

}

// Initialize a variable and audio resources.
void ResultScene::Initialize()
{
    font = DX9::SpriteFont::CreateFromFontFile(DXTK->Device9, L"Font/HuiFont29.ttf", L"ふい字", 35);
    bgm_result = DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"BGM\\ending_bgm.mp3");
    se_flowers = DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\Result\\hanabi.mp3");

    count_change = 0;
    time_delta = 0.0f;
    num_alpha = 0.0f;
    flag_fade = false;


	// 会話用
	pos_speach_girl_1p.x = pos_speach_normal_girl_limit_x_1p;
	pos_speach_girl_1p.y = pos_speach_reply_y;
	pos_speach_girl_1p.z = MainScene::POSI_Z::SPEAK;

	pos_speach_boy_1p.x = pos_speach_normal_boy_limit_x_1p;
	pos_speach_boy_1p.y = pos_speach_reply_y;
	pos_speach_boy_1p.z = MainScene::POSI_Z::SPEAK;


	pos_speach_girl_2p.x = pos_speach_normal_girl_limit_x_2p;
	pos_speach_girl_2p.y = pos_speach_reply_y;
	pos_speach_girl_2p.z = MainScene::POSI_Z::SPEAK;

	pos_speach_boy_2p.x = pos_speach_normal_boy_limit_x_2p;
	pos_speach_boy_2p.y = pos_speach_reply_y;
	pos_speach_boy_2p.z = MainScene::POSI_Z::SPEAK;

	pos_speach_synchro_1p.x = pos_speach_synchro_x_1p;
	pos_speach_synchro_1p.y = pos_speach_reply_y;
	pos_speach_synchro_1p.z = MainScene::POSI_Z::SPEAK;

	pos_speach_synchro_2p.x = pos_speach_synchro_x_2p;
	pos_speach_synchro_2p.y = pos_speach_reply_y;
	pos_speach_synchro_2p.z = MainScene::POSI_Z::SPEAK;


	flag_talk_boy = false;
	flag_talk_girl = false;
	flag_talk_synchro = false;

	comment_reply = 0.0f;

	time_stop = 0.0f;
	alpha_text = 0.0f;


	bgm = 0.0f;

	index = winner ? 45 : 50;
	//index = 45;

	load.In_File();

    co_operate = Operate();             //コルーチンの生成
    co_operate_it = co_operate.begin(); //コルーチンの実行開始
}

// Allocate all memory the Direct3D and Direct2D resources.
void ResultScene::LoadAssets()
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


    // グラフィックリソースの初期化処理
    LA_Load();
}

// Releasing resources required for termination.
void ResultScene::Terminate()
{
	DXTK->ResetAudioEngine();
	DXTK->WaitForGpu();

	// TODO: Add your Termination logic here.

}

// Direct3D resource cleanup.
void ResultScene::OnDeviceLost()
{

}

// Restart any looped sounds here
void ResultScene::OnRestartSound()
{

}

// Updates the scene.
NextScene ResultScene::Update(const float deltaTime)
{
	// If you use 'deltaTime', remove it.
	UNREFERENCED_PARAMETER(deltaTime);

	// TODO: Add your game logic here.


    // 台詞のコルーチンを、終わったら花火のse流す
    time_delta = deltaTime;

	bgm = std::max(bgm -  72.0f * time_delta, -5000.0f);
	bgm_result->SetVolume(bgm);

    if (co_operate_it == co_operate.end()) {
		TitleScene::flag_fade = true;
		return NextScene::TitleScene;
    }
    if (co_operate_it != co_operate.end()) co_operate_it++;



	return NextScene::Continue;
}

// Draws the scene.
void ResultScene::Render()
{
    // TODO: Add your rendering code here.
    DXTK->Direct3D9->Clear(DX9::Colors::RGBA(0, 0, 0, 255));

    DXTK->Direct3D9->BeginScene();
    DX9::SpriteBatch->Begin();



    Re_Sprite();
    Re_Speak();

    DX9::SpriteBatch->End();
    DXTK->Direct3D9->EndScene();

    Re_DirectTwelve();
}

void ResultScene::LA_Load() {
    black = DX9::Sprite::CreateFromFile(DXTK->Device9, L"Effect/black.png");
    ope= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Scene\\result_ui.png");

	bgm_result= DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"BGM\\ending_bgm.mp3");
    se_flowers = DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\Result\\hanabi.mp3");

	bgm_result->Play();
	bgm_result->SetVolume(500.0f);
}

bool ResultScene::Up_Fade(const float deltaTime) {
    if (count_change == 0) {
        count_change += 1;
        co_operate = Operate();             //コルーチンの生成
        co_operate_it = co_operate.begin(); //コルーチンの実行開始
    }
    time_delta = deltaTime;

    if (co_operate_it == co_operate.end()) { 
        return true;
    }
    if (co_operate_it != co_operate.end()) co_operate_it++;
    return false;

}

void ResultScene::Re_Sprite() {
	DX9::SpriteBatch->DrawSimple(
		black.Get(), SimpleMath::Vector3(0.0f, 0.0f, 3),
		Rect(0.0f, 0.0f, 3840.0f, 1080.0f),
		DX9::Colors::RGBA(255, 255, 255, 255)
	);
}

// テキスト表示を行う
void ResultScene::Re_Speak() {
	//男子セリフ表示
	if (flag_talk_boy == true) {
		//1P画面
		RECT dest = RectWH(pos_speach_boy_1p.x + 70, pos_speach_boy_1p.y + 70, 1000, 1000);
		DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[index].c_str(), (int)comment_reply, dest, DX9::Colors::White);
		//フォント,セリフの入った変数,1度に表示する文字数,文字を表示する場所,色

		//2P画面
		RECT means = RectWH(pos_speach_boy_2p.x + 115, pos_speach_boy_2p.y + 70, 1000, 1000);
		DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[index].c_str(), (int)comment_reply, means, DX9::Colors::White);
	}

	//女子セリフ表示
	if (flag_talk_girl == true) {
		//1P画面
		RECT place = RectWH(pos_speach_girl_1p.x + 125, pos_speach_girl_1p.y + 70, 1000, 1000);
		DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[index].c_str(), (int)comment_reply, place, DX9::Colors::White);
		//フォント,セリフの入った変数,1度に表示する文字数,文字を表示する場所,色

		//2P画面
		RECT spot = RectWH(pos_speach_girl_2p.x + 65, pos_speach_girl_2p.y + 70, 1000, 1000);
		DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[index].c_str(), (int)comment_reply, spot, DX9::Colors::White);
	}
}

void ResultScene::Re_DirectTwelve() {
    DXTK->Direct3D9->UpdateResource();

    DXTK->ResetCommand();
    DXTK->ClearRenderTarget(Colors::CornflowerBlue);

    const auto heapes = descriptorHeap->Heap();
    DXTK->CommandList->SetDescriptorHeaps(1, &heapes);

    spriteBatch->Begin(DXTK->CommandList);
    spriteBatch->Draw(
        dx9GpuDescriptor,
        XMUINT2(1280, 720),
        SimpleMath::Vector2(0.0f, 0.0f)
    );
    spriteBatch->End();

    DXTK->Direct3D9->WaitUpdate();
    DXTK->ExecuteCommandList();
}

cppcoro::generator<int> ResultScene::Operate() {
	co_yield 0;

	// 待機
	while (time_stop < 2.5f) {
		time_stop += time_delta;
		co_yield 1;
	}
	time_stop = 0.0f;


	// 46~50or51~55の台詞を表示

	// 男子勝ちver
	if (index == 45) {
		//セリフ(男子)の表示
		while (time_stop < 3.0f) {
			flag_talk_boy = true;
			time_stop += time_delta;

			comment_reply += TALK_SPEED * time_delta;
			if (comment_reply > LoadLines::lines1[index].length())
				comment_reply = LoadLines::lines1[index].length();

			co_yield 8;
		}
		time_stop = 0.0f;
		comment_reply = 0.0f;
		flag_talk_boy = false;
		index += 1;

		//セリフ(女子)の表示
		while (time_stop < 3.0f) {
			flag_talk_girl = true;
			time_stop += time_delta;

			comment_reply += TALK_SPEED * time_delta;
			if (comment_reply > LoadLines::lines1[index].length())
				comment_reply = LoadLines::lines1[index].length();

			co_yield 10;
		}
		time_stop = 0.0f;
		comment_reply = 0.0f;
		flag_talk_girl = false;
		index += 1;

		//セリフ(男子)の表示
		while (time_stop < 3.0f) {
			flag_talk_boy = true;
			time_stop += time_delta;

			comment_reply += TALK_SPEED * time_delta;
			if (comment_reply > LoadLines::lines1[index].length())
				comment_reply = LoadLines::lines1[index].length();

			co_yield 11;
		}
		time_stop = 0.0f;
		comment_reply = 0.0f;
		flag_talk_boy = false;
		index += 1;

		//セリフ(女子)の表示
		while (time_stop < 3.0f) {
			flag_talk_girl = true;
			time_stop += time_delta;

			comment_reply += TALK_SPEED * time_delta;
			if (comment_reply > LoadLines::lines1[index].length())
				comment_reply = LoadLines::lines1[index].length();

			co_yield 13;
		}
		time_stop = 0.0f;
		comment_reply = 0.0f;
		flag_talk_girl = false;
		index += 1;

		//セリフ(男子)の表示
		while (time_stop < 3.0f) {
			flag_talk_boy = true;
			time_stop += time_delta;

			comment_reply += TALK_SPEED * time_delta;
			if (comment_reply > LoadLines::lines1[index].length())
				comment_reply = LoadLines::lines1[index].length();

			co_yield 11;
		}
		time_stop = 0.0f;
		comment_reply = 0.0f;
		flag_talk_boy = false;
	}

	// 女子勝ちver
	if (index == 50) {
		//セリフ(女子)の表示
		while (time_stop < 3.0f) {
			flag_talk_girl = true;
			time_stop += time_delta;

			comment_reply += TALK_SPEED * time_delta;
			if (comment_reply > LoadLines::lines1[index].length())
				comment_reply = LoadLines::lines1[index].length();

			co_yield 10;
		}
		time_stop = 0.0f;
		comment_reply = 0.0f;
		flag_talk_girl = false;
		index += 1;

		//セリフ(男子)の表示
		while (time_stop < 3.0f) {
			flag_talk_boy = true;
			time_stop += time_delta;

			comment_reply += TALK_SPEED * time_delta;
			if (comment_reply > LoadLines::lines1[index].length())
				comment_reply = LoadLines::lines1[index].length();

			co_yield 11;
		}
		time_stop = 0.0f;
		comment_reply = 0.0f;
		flag_talk_boy = false;
		index += 1;

		//セリフ(女子)の表示
		while (time_stop < 3.0f) {
			flag_talk_girl = true;
			time_stop += time_delta;

			comment_reply += TALK_SPEED * time_delta;
			if (comment_reply > LoadLines::lines1[index].length())
				comment_reply = LoadLines::lines1[index].length();

			co_yield 10;
		}
		time_stop = 0.0f;
		comment_reply = 0.0f;
		flag_talk_girl = false;
		index += 1;

		//セリフ(男子)の表示
		while (time_stop < 3.0f) {
			flag_talk_boy = true;
			time_stop += time_delta;

			comment_reply += TALK_SPEED * time_delta;
			if (comment_reply > LoadLines::lines1[index].length())
				comment_reply = LoadLines::lines1[index].length();

			co_yield 11;
		}
		time_stop = 0.0f;
		comment_reply = 0.0f;
		flag_talk_boy = false;
		index += 1;

		//セリフ(女子)の表示
		while (time_stop < 3.0f) {
			flag_talk_girl = true;
			time_stop += time_delta;

			comment_reply += TALK_SPEED * time_delta;
			if (comment_reply > LoadLines::lines1[index].length())
				comment_reply = LoadLines::lines1[index].length();

			co_yield 10;
		}
		time_stop = 0.0f;
		comment_reply = 0.0f;
		flag_talk_girl = false;
	}

	se_flowers->Play();

	// 待機
	while (time_stop < 10.0f) {
		time_stop += time_delta;
		co_yield 1;
	}
	time_stop = 0.0f;



	co_return;
}