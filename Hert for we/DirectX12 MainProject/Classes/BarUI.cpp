#include "Classes/BarUI.h"
#include <iostream>
#include <MainScene.h>

void BarUI::Initialize() {

}

void BarUI::LoadAsset() {
	ui_turn_my				= DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/turn_ui.png");
	ui_turn_partner		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/partner_turn_ui.png");
	ui_move					= DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/move_ui.png");
}

void BarUI::Render() {

}