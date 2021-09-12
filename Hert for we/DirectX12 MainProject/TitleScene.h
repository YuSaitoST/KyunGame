//
// TitleScene.h
//

#pragma once

#include "Scene.h"
#include"cppcoro/generator.h"

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
    DX9::SPRITE comment;
    DX9::SPRITE ui_start;
    DX9::SPRITE white;

    DX9::MEDIARENDERER bgm_title;
    DX9::MEDIARENDERER se_start;
    DX9::MEDIARENDERER se_ope;
    DX9::MEDIARENDERER se_cansel;

    SimpleMath::Vector3 pos_comment_my;
    SimpleMath::Vector3 pos_comment_partner;

    int page_state;
    bool flag_comment;
    float alpha_white;
    const float num_alpha = 400.0f;
    const float num_speed = 3000.0f;

    float num_flash;
    bool flag_falsh;
    const float num_speed_flash_slow  = 2.0f;
    const float num_speed_flash_quick = 10.0f;

    float time_change;

    const float POS_START_COMMENT_Y = -990.0f;


    void LA_Load();
    NextScene Up_Scene_Change(const float deltaTime);
    void Up_comment(const float deltaTime);

    void Re_Draw_Title();
    void Re_DirectTwelve();
};