//
// TitleScene.h
//

#pragma once

#include "Scene.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class TitleScene final : public Scene {
public:
    TitleScene();
    virtual ~TitleScene() { Terminate(); }

    TitleScene(TitleScene&&) = default;
    TitleScene& operator= (TitleScene&&) = default;

    TitleScene(TitleScene const&) = delete;
    TitleScene& operator= (TitleScene const&) = delete;

    // These are the functions you will implement.
    void Initialize() override;
    void LoadAssets() override;

    void Terminate() override;

    void OnDeviceLost() override;
    void OnRestartSound() override;

    NextScene Update(const float deltaTime) override;
    void Render() override;

private:
    DX12::DESCRIPTORHEAP descriptorHeap;
    DX12::SPRITEBATCH    spriteBatch;
    DX12::HGPUDESCRIPTOR dx9GpuDescriptor;

    // ïœêîÇÃêÈåæ

    DX9::SPRITE title;
    SimpleMath::Vector3 pos_title;

    void LA_Load();
    NextScene Up_Scene_Change(const float deltaTime);

    void Re_Draw_Title();
    void Re_DirectTwelve();
};