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
    font = DX9::SpriteFont::CreateFromFontFile(DXTK->Device9, L"Font/HuiFont.ttf", L"ふい字", 20);
    bgm_result = DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"BGM\\ending_bgm.mp3");
    /*se_flowers= DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\title_bgm.mp3")*/
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
        black.Get(), SimpleMath::Vector3(0.0f, 0.0f, -3),
        Rect(0.0f, 0.0f, 3840.0f, 1080.0f),
        DX9::Colors::RGBA(255, 255, 255, 255)
    );

    if (MainScene::flag_debug) {
        DX9::SpriteBatch->DrawString(
            font.Get(), SimpleMath::Vector2(0.0f, 0.0f),
            DX9::Colors::RGBA(255, 255, 255, 255), L"俺この開発が終わったら実家に帰るんだ"
        );
    }

    DX9::SpriteBatch->End();
    DXTK->Direct3D9->EndScene();

    Re_DirectTwelve();
}

void ResultScene::LA_Load() {
    black = DX9::Sprite::CreateFromFile(DXTK->Device9, L"Effect/black.png");
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