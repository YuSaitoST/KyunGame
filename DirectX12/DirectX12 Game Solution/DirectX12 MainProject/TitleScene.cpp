//
// TitleScene.cpp
//

#include "Base/pch.h"
#include "Base/dxtk.h"
#include "SceneFactory.h"

// Initialize member variables.
TitleScene::TitleScene() : dx9GpuDescriptor{}
{

}

// Initialize a variable and audio resources.
void TitleScene::Initialize()
{
    page_state = TITLE;
}

// Allocate all memory the Direct3D and Direct2D resources.
void TitleScene::LoadAssets()
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
void TitleScene::Terminate()
{
    DXTK->ResetAudioEngine();
    DXTK->WaitForGpu();

    // TODO: Add your Termination logic here.

}

// Direct3D resource cleanup.
void TitleScene::OnDeviceLost()
{

}

// Restart any looped sounds here
void TitleScene::OnRestartSound()
{

}

// Updates the scene.
NextScene TitleScene::Update(const float deltaTime)
{
    // If you use 'deltaTime', remove it.
    UNREFERENCED_PARAMETER(deltaTime);

    // TODO: Add your game logic here.

    auto scene = Up_Scene_Change(deltaTime);
    if (scene != NextScene::Continue)
        return scene;


    return NextScene::Continue;
}

// Draws the scene.
void TitleScene::Render()
{
    // TODO: Add your rendering code here.
    DXTK->Direct3D9->Clear(DX9::Colors::RGBA(0, 0, 0, 255));

    DXTK->Direct3D9->BeginScene();
    DX9::SpriteBatch->Begin();


    Re_Draw_Title();


    DX9::SpriteBatch->End();
    DXTK->Direct3D9->EndScene();

    Re_DirectTwelve();
}

void TitleScene::LA_Load() {
    title    = DX9::Sprite::CreateFromFile(DXTK->Device9, L"Scene/title.png"  );
    comment  = DX9::Sprite::CreateFromFile(DXTK->Device9, L"Scene/comment.png");
    ui_start = DX9::Sprite::CreateFromFile(DXTK->Device9, L"Scene/start.png"  );
}

NextScene TitleScene::Up_Scene_Change(const float deltaTime) {
    if (DXTK->KeyEvent->pressed.Enter ||
        DXTK->GamePadEvent[0].b == GamePad::ButtonStateTracker::PRESSED ||
        DXTK->GamePadEvent[1].b == GamePad::ButtonStateTracker::PRESSED) {
        page_state++;
        if (page_state == CHANGE)
            return NextScene::MainScene;
    }

    return NextScene::Continue;
}

void TitleScene::Re_Draw_Title() {
    if (page_state == TITLE) {
        DX9::SpriteBatch->DrawSimple(
            title.Get(), SimpleMath::Vector3(0.0f, 0.0f, 0.0f)
        );
        DX9::SpriteBatch->DrawSimple(
            title.Get(), SimpleMath::Vector3(1920.0f, 0.0f, 0.0f)
        );
    }

    if (page_state == COMMENT) {
        DX9::SpriteBatch->DrawSimple(
            comment.Get(), SimpleMath::Vector3(0.0f, 0.0f, 0.0f)
        );
        DX9::SpriteBatch->DrawSimple(
            comment.Get(), SimpleMath::Vector3(1920.0f, 0.0f, 0.0f)
        );
    }


    DX9::SpriteBatch->DrawSimple(
        ui_start.Get(), SimpleMath::Vector3(0.0f, 0.0f, 0.0f)
    );

    DX9::SpriteBatch->DrawSimple(
        ui_start.Get(), SimpleMath::Vector3(1920.0f, 0.0f, 0.0f)
    );

}



void TitleScene::Re_DirectTwelve() {
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
