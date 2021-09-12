//
// MainScene.cpp
//

#include "Base/pch.h"
#include "Base/dxtk.h"
#include "SceneFactory.h"

// Initialize member variables.
ResultScene::ResultScene()
{

}

// Initialize a variable and audio resources.
void ResultScene::Initialize()
{
    font = DX9::SpriteFont::CreateFromFontFile(DXTK->Device9, L"Font/HuiFont.ttf", L"ふい字", 35);
    bgm_result = DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"BGM\\ending_bgm.mp3");
    se_flowers = DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\Result\\hanabi.mp3");

    count_change = 0;
    time_delta = 0.0f;
    num_alpha = 0.0f;
    flag_fade = false;

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
    time_delta += deltaTime;

    se_flowers->Play();

    if (co_operate_it == co_operate.end()) {
        const bool input_1_ = DXTK->GamePadEvent[0].b == GamePad::ButtonStateTracker::PRESSED;
        const bool input_2_ = DXTK->GamePadEvent[1].b == GamePad::ButtonStateTracker::PRESSED;
        if (input_1_ || input_2_) return NextScene::TitleScene;
    }
    if (co_operate_it != co_operate.end()) co_operate_it++;



    //bool flag_fade = Up_Fade(deltaTime);
    //if (!flag_fade) return NextScene::Continue;



	return NextScene::Continue;
}

// Draws the scene.
void ResultScene::Render()
{
    // TODO: Add your rendering code here.
    DXTK->Direct3D9->Clear(DX9::Colors::RGBA(0, 0, 0, 255));

    DXTK->Direct3D9->BeginScene();
    DX9::SpriteBatch->Begin();



    DX9::SpriteBatch->DrawSimple(
        black.Get(), SimpleMath::Vector3(0.0f, 0.0f, -3),
        Rect(0.0f, 0.0f, 3840.0f, 1080.0f),
        DX9::Colors::RGBA(255, 255, 255, 255)
    );

    DX9::SpriteBatch->DrawSimple(
        ope.Get(),
        SimpleMath::Vector3(0.0f, 0.0f, 0),
        Rect(0.0f, 0.0f, 1920.0f, 990.0f),
        DX9::Colors::RGBA(255, 255, 255, num_alpha)
    );
    DX9::SpriteBatch->DrawSimple(
        ope.Get(),
        SimpleMath::Vector3(1920.0f, 0.0f, 0),
        Rect(0.0f, 0.0f, 1920.0f, 990.0f),
        DX9::Colors::RGBA(255, 255, 255, num_alpha)
    );



    Re_Sprite();
    Re_Speak();

    DX9::SpriteBatch->End();
    DXTK->Direct3D9->EndScene();

    Re_DirectTwelve();
}

void ResultScene::LA_Load() {
    black = DX9::Sprite::CreateFromFile(DXTK->Device9, L"Effect/black.png");
    ope= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Scene/result_ui.png");

    se_flowers = DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\Result\\hanabi.mp3");
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
    //DX9::SpriteBatch->DrawSimple(
    //    black.Get(), SimpleMath::Vector3(0.0f, 0.0f, -3),
    //    Rect(0.0f, 0.0f, 3840.0f, 1080.0f),
    //    DX9::Colors::RGBA(255, 255, 255, 255)
    //);

    //DX9::SpriteBatch->DrawSimple(
    //    ope.Get(),
    //    SimpleMath::Vector3(0.0f, 0.0f, -2),
    //    Rect(0.0f, 0.0f, 1920.0f, 990.0f),
    //    DX9::Colors::RGBA(255, 255, 255, num_alpha)
    //);
}

// テキスト表示を行う
void ResultScene::Re_Speak() {

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

    while (num_alpha < 255.0f) {
        num_alpha = std::min(num_alpha + time_delta * 51.0f, 255.0f);
        co_yield 1;
    }
}