//
// TitleScene.cpp
//

#include "Base/pch.h"
#include "Base/dxtk.h"
#include "SceneFactory.h"

bool TitleScene::flag_fade;

// Initialize member variables.
TitleScene::TitleScene() : dx9GpuDescriptor{}
{

}

// Initialize a variable and audio resources.
void TitleScene::Initialize()
{
    flag_comment = false;
    alpha_white = 0.0f;

    pos_comment_my.x = 115.0f;
    pos_comment_my.y = POS_START_COMMENT_Y;
    pos_comment_my.z = 0.0f;

    pos_comment_partner.x = 1920.0f + 115.0f;
    pos_comment_partner.y = POS_START_COMMENT_Y;
    pos_comment_partner.z = 0.0f;

    num_flash = 0.0f;
    flag_falsh = false;
    time_change = 0.0f;

    alpha_black = 0.0f;
    time_delta = 0.0f;
    time_stop = 0.0f;



    if (flag_fade) {
        co_operate = Operate();             //コルーチンの生成
        co_operate_it = co_operate.begin(); //コルーチンの実行開

    }
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

    time_delta = deltaTime;

    if (flag_fade) {
        if (co_operate_it == co_operate.end()) {
            flag_fade = false;
        }
        if (co_operate_it != co_operate.end()) co_operate_it++;
    }

    if (flag_fade) return NextScene::Continue;

    bgm_title->Play();
    if (bgm_title->isComplete()) bgm_title->Replay();

    Up_comment(deltaTime);



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
    title              = DX9::Sprite::CreateFromFile(DXTK->Device9, L"Scene/title_bg.png"  );
    comment    = DX9::Sprite::CreateFromFile(DXTK->Device9, L"Scene/explanation_bg.png");
    ui_start        = DX9::Sprite::CreateFromFile(DXTK->Device9, L"Scene/start_ui.png"  );
    white           = DX9::Sprite::CreateFromFile(DXTK->Device9, L"Effect/white.png" );
    black           = DX9::Sprite::CreateFromFile(DXTK->Device9, L"Effect/black.png");

    bgm_title    = DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"BGM\\title_bgm.mp3");
    se_ope = DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\Title\\help.mp3");
    se_cansel = DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\Title\\help_cancel.mp3");
    se_start = DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\Title\\start.mp3");
}

void TitleScene::Up_comment(const float deltaTime) {
    if (flag_falsh == false) {
        if (flag_comment == false) {
            bool flag_padKey = 
                DXTK->GamePadEvent[0].a == GamePad::ButtonStateTracker::PRESSED ||
                DXTK->GamePadEvent[1].a == GamePad::ButtonStateTracker::PRESSED;
            if (DXTK->KeyEvent->pressed.Back || flag_padKey) {
                se_cansel->Replay();
                flag_comment = true;
            }
        }
        else
        {
            bool flag_padKey = 
                DXTK->GamePadEvent[0].a == GamePad::ButtonStateTracker::PRESSED ||
                DXTK->GamePadEvent[1].a == GamePad::ButtonStateTracker::PRESSED;
            if (DXTK->KeyEvent->pressed.Back || flag_padKey) {
                se_cansel->Replay();
                flag_comment = false;
            }
        }
    }

    if (flag_comment == true) {
        
        alpha_white += num_alpha * deltaTime;
        if (alpha_white > 155.0f)
            alpha_white = 155.0f;

        pos_comment_my.y += num_speed * deltaTime;
        pos_comment_partner.y += num_speed * deltaTime;

        if(pos_comment_my.y > 60.0f)
        {
            pos_comment_my.y = 60.0f;
            pos_comment_partner.y = 60.0f;
        }
    }
    else
    {
        alpha_white -= num_alpha * deltaTime;
        if (alpha_white < 0.0f) {
            alpha_white = 0.0f;
        }
        pos_comment_my.y -= num_speed * deltaTime;
        pos_comment_partner.y -= num_speed * deltaTime;

        if (pos_comment_my.y < POS_START_COMMENT_Y)
        {
            pos_comment_my.y = POS_START_COMMENT_Y;
            pos_comment_partner.y = POS_START_COMMENT_Y;
        }
    }
}

NextScene TitleScene::Up_Scene_Change(const float deltaTime) {
    if (flag_comment == false) {
        bool flag_padKey_ =
            DXTK->GamePadEvent[0].b == GamePad::ButtonStateTracker::PRESSED ||
            DXTK->GamePadEvent[1].b == GamePad::ButtonStateTracker::PRESSED;
        if (DXTK->KeyEvent->pressed.Enter || flag_padKey_) {
            flag_falsh = true;
            se_start->Replay();
        }
    }

    if (flag_falsh == true) {
        num_flash += num_speed_flash_quick * deltaTime;
        time_change += deltaTime;
        if (time_change > 2.0f)
            return NextScene::MainScene;
    }
    else {
        num_flash += num_speed_flash_slow * deltaTime;
    }
    return NextScene::Continue;
}

void TitleScene::Re_Draw_Title() {
    DX9::SpriteBatch->DrawSimple(
        black.Get(), SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
        Rect(0.0f, 0.0f, 3840.0f, 1080.0f),
        DX9::Colors::RGBA(255, 255, 255, (int)alpha_black)
    );

    DX9::SpriteBatch->DrawSimple(
        title.Get(), SimpleMath::Vector3(0.0f, 0.0f, 3.0f)
    );
    DX9::SpriteBatch->DrawSimple(
        title.Get(), SimpleMath::Vector3(1920.0f, 0.0f, 3.0f)
    );



    DX9::SpriteBatch->DrawSimple(
        white.Get(), SimpleMath::Vector3(0.0f, 0.0f, 1.0f),
        Rect(0.0f, 0.0f, 1920.0f, 1080.0f),
        DX9::Colors::RGBA(255, 255, 255, (int)alpha_white)
    );


    DX9::SpriteBatch->DrawSimple(
        white.Get(), SimpleMath::Vector3(1920.0f, 0.0f, 1.0f),
        Rect(0.0f, 0.0f, 1920.0f, 1080.0f),
        DX9::Colors::RGBA(255, 255, 255, (int)alpha_white)
    );



    DX9::SpriteBatch->DrawSimple(
        comment.Get(), pos_comment_my
    );
    DX9::SpriteBatch->DrawSimple(
        comment.Get(), pos_comment_partner
    );

    if ((int)num_flash % 2 == 0) {
        DX9::SpriteBatch->DrawSimple(
            ui_start.Get(), SimpleMath::Vector3(0.0f, 0.0f, 2.0f)
        );

        DX9::SpriteBatch->DrawSimple(
            ui_start.Get(), SimpleMath::Vector3(1920.0f, 0.0f, 2.0f)
        );
    }
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

cppcoro::generator<int> TitleScene::Operate() {
    co_yield 0;

    // 待機
    while (time_stop < 2.5f) {
        time_stop += time_delta;
        co_yield 1;
    }
    time_stop = 0.0f;

    while (alpha_black > 0.0f) {
        alpha_black = std::max(alpha_black - num_alpha * time_delta, 0.0f);
        co_yield 4;
    }

    co_return;
}