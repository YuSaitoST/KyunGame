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
    alpha_black = MainScene::phase == MainScene::Phase::SCENARIO ? 255.0f : 0.0f;

    time_delta = 0.0f;
    time_stop = 2.0f;

    count_chnage = 0;
    time_stop = 0.0f;
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


    bool flag_black_ = Up_Black(deltaTime);
    if (!flag_black_) return NextScene::Continue;

    const bool input_1_ = DXTK->GamePadEvent[0].b == GamePad::ButtonStateTracker::PRESSED;
    const bool input_2_ = DXTK->GamePadEvent[1].b == GamePad::ButtonStateTracker::PRESSED;
    if (input_1_ || input_2_) return NextScene::TitleScene;


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
        black.Get(), SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
        Rect(0.0f, 0.0f, 3840.0f, 1080.0f),
        DX9::Colors::RGBA(255, 255, 255, (int)alpha_black)
    );

    DX9::SpriteBatch->End();
    DXTK->Direct3D9->EndScene();

    Re_DirectTwelve();
}

void ResultScene::LA_Load() {

}

bool ResultScene::Up_Black(float deltaTime) {
    if (count_chnage == 0) {
        count_chnage += 1;
        if (MainScene::phase != MainScene::Phase::SCENARIO) alpha_black = 0.0f;
        co_change = Change();             //コルーチンの生成
        co_change_it = co_change.begin(); //コルーチンの実行開始
    }
    time_delta = deltaTime;
    if (co_change_it == co_change.end()) {
        Initialize();
        return true;
    }

    if (co_change_it != co_change.end()) co_change_it++;
    return false;
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

cppcoro::generator<int>ResultScene::Change() {
    co_yield 0;

    //// 待機
    //while (time_stop < 2.5f) {
    //    time_stop += time_delta;
    //    co_yield 1;
    //}

    while (alpha_black < 255.0f) {
        alpha_black = std::min(alpha_black + num_alpha * time_delta, 255.0f);
        co_yield 1;
    }
    
    // ブラックアウト
    while (alpha_black > 0.0f)
    {
        alpha_black = std::max(alpha_black - num_alpha * time_delta, 0.0f);
        co_yield 2;
    }
    co_return;
}