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
	font = DX9::SpriteFont::CreateFromFontName(DXTK->Device9, L"FixedSys 標準", 20);
	pos_pointer = SimpleMath::Vector2(360.0f + 520.0f, 415.0f);
	//std::fill(pos_heartR[0], pos_heartR[1], SimpleMath::Vector2(0.0f, 0.0f));
	//std::fill(pos_heartB[0], pos_heartB[1], SimpleMath::Vector2(0.0f, 0.0f));
	//std::fill(field[0], field[5], 0);
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

	Up_Move_Pointer();

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
	area_attack		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"attack_area.png");
	heart_red		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"love.png");
	heart_blue		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"trap.png");
	boy					= DX9::Sprite::CreateFromFile(DXTK->Device9, L"boy.png");
	girl					= DX9::Sprite::CreateFromFile(DXTK->Device9, L"girl.png");
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

void MainScene::Re_Draw_PlayerA() {
	DX9::SpriteBatch->DrawSimple(
		bg.Get(), SimpleMath::Vector3(0.0f, 0.0f, 0.0f)
	);
	DX9::SpriteBatch->DrawSimple(
		map.Get(), SimpleMath::Vector3(510.0f, 45.0f, 0.0f)
	);
	DX9::SpriteBatch->DrawSimple(
		boy.Get(), SimpleMath::Vector3(pos_Bx - 402.0f, 990.0f - 590.0f, 0.0f)
	);
	DX9::SpriteBatch->DrawSimple(
		girl.Get(), SimpleMath::Vector3(0.0f, 990.0f - 476.0f, 0.0f)
	);
	DX9::SpriteBatch->DrawSimple(
		area_attack.Get(), SimpleMath::Vector3(pos_pointer.x, pos_pointer.y, 0.0f)
	);
}

void MainScene::Re_Draw_PlayerB() {
	DX9::SpriteBatch->DrawSimple(
		bg.Get(), SimpleMath::Vector3(pos_Bx, 0.0f, 0.0f)
	);
	DX9::SpriteBatch->DrawSimple(
		map.Get(), SimpleMath::Vector3(pos_Bx + 510.0f, 45.0f, 0.0f)
	);
	DX9::SpriteBatch->DrawSimple(
		boy.Get(), SimpleMath::Vector3(pos_Bx, 990.0f - 590.0f, 0.0f)
	);
	DX9::SpriteBatch->DrawSimple(
		girl.Get(), SimpleMath::Vector3(2 * pos_Bx - 438.0f, 990.0f - 476.0f, 0.0f)
	);
	DX9::SpriteBatch->DrawSimple(
		area_attack.Get(), SimpleMath::Vector3(pos_pointer.x + pos_Bx, pos_pointer.y, 0.0f)
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
