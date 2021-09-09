#include"Classes/Attack.h"
#include <iostream>
#include<MainScene.h>
#include"LoadLines.h"
using namespace std;

float Attack::alpha_boy;
float Attack::alpha_girl;

void Attack::Initialize() {

	font = DX9::SpriteFont::CreateFromFontFile(DXTK->Device9, L"Font/HuiFont.ttf", L"ふい字", 20);

	count_chnage = 0;
	time_delta = 0.0f;
	alpha_boy = 255.0f;
	alpha_girl = 255.0f;
	time_stop = 0.0f;


	pos_speach_girl_1p.x = 0.0f;
	pos_speach_girl_1p.y = 0.0f;
	pos_speach_girl_1p.z = -10.0f;

	pos_speach_boy_1p.x = 920.0f;
	pos_speach_boy_1p.y = 0.0f;
	pos_speach_boy_1p.z = -10.0f;


	pos_speach_girl_2p.x = 2840.0f;
	pos_speach_girl_2p.y = 0.0f;
	pos_speach_girl_2p.z = -10.0f;

	pos_speach_boy_2p.x = 1920.0f;
	pos_speach_boy_2p.y = 0.0f;
	pos_speach_boy_2p.z = -10.0f;


	alpha_speach_my = 0.0f;
	alpha_speach_partner = 0.0f;

	num_color[0] = 255;
	num_color[1] = 255;

	speach_my_reply             = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/speech_balloon.png"   );
	speach_my_attack      = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/speech_balloon_ul.png");
	speach_partner_reply        = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/speech_balloon_r.png" );
	speach_partner_attack = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/speech_balloon_ur.png");

	speach = ATTACK_GIRL;

	comment = 0;
	speach_flag = false;


	index_boy = 0;
	index_girl = 1;


	loadlines.Initialize();
	loadlines.In_File();

}

void Attack::LoadAssets() {
	boy_a[MainScene::EMOTION::GENERALLY]	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/A_player/boy_generally.png");
	boy_a[MainScene::EMOTION::PROPOSAL]	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/A_player/boy_attack.png");
	boy_a[MainScene::EMOTION::NERVOUS]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/A_player/boy_near.png");
	boy_a[MainScene::EMOTION::VICTORY]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/A_player/boy_victory.png");
	boy_a[MainScene::EMOTION::DEFEAT]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/A_player/boy_defeat.png");
	girl_a[MainScene::EMOTION::GENERALLY]	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/A_player/girl_generally.png");
	girl_a[MainScene::EMOTION::PROPOSAL]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/A_player/girl_attack.png");
	girl_a[MainScene::EMOTION::NERVOUS]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/A_player/girl_near.png");
	girl_a[MainScene::EMOTION::VICTORY]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/A_player/girl_victory.png");
	girl_a[MainScene::EMOTION::DEFEAT]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/A_player/girl_defeat.png");

	boy_b[MainScene::EMOTION::GENERALLY]	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/boy_generally_l.png");
	boy_b[MainScene::EMOTION::PROPOSAL]	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/boy_attack_l.png");
	boy_b[MainScene::EMOTION::NERVOUS]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/boy_near_l.png");
	boy_b[MainScene::EMOTION::VICTORY]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/boy_victory_l.png");
	boy_b[MainScene::EMOTION::DEFEAT]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/boy_defeat_l.png");
	girl_b[MainScene::EMOTION::GENERALLY]	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/girl_generally_r.png");
	girl_b[MainScene::EMOTION::PROPOSAL]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/girl_attack_r.png");
	girl_b[MainScene::EMOTION::NERVOUS]		= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/girl_near_r.png");
	girl_b[MainScene::EMOTION::VICTORY]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/girl_victory_r.png");
	girl_b[MainScene::EMOTION::DEFEAT]			= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Character/B_player/girl_defeat_r.png");

}

bool Attack::Up_Attack(const float deltaTime) {
	if (count_chnage == 0) {
		count_chnage += 1;
		co_action = Action();             //コルーチンの生成
		co_action_it = co_action.begin(); //コルーチンの実行開始
	}
	time_delta = deltaTime;

	if (co_action_it == co_action.end()) {
		Initialize();
		return true;
	}
	if (co_action_it != co_action.end()) co_action_it++;
	return false;
}

// 反転用の変数を渡せるようにする（その変数はMainScene.hにて）この関数は使わない
void Attack::Render(int index, SimpleMath::Vector2 pos_boy, SimpleMath::Vector2 pos_girl) {
	float pos_x_ = index == 1 ? 1920.0f : 0.0f;

	DX9::SpriteBatch->DrawSimple(
		boy_a[MainScene::emotion[index]].Get(),
		SimpleMath::Vector3(pos_x_ + pos_boy.x, pos_boy.y, MainScene::POSI_Z::PLAYER),
		Rect(0.0f, 0.0f, 402.0f, 590.0f),
		DX9::Colors::RGBA((int)num_color[0], (int)num_color[0], (int)num_color[0], (int)alpha_boy)
	);
	DX9::SpriteBatch->DrawSimple(
		girl_a[MainScene::emotion[index]].Get(),
		SimpleMath::Vector3(pos_x_ + pos_girl.x, pos_girl.y, MainScene::POSI_Z::PLAYER),
		Rect(0.0f, 0.0f, 438.0f, 476.0f),
		DX9::Colors::RGBA((int)num_color[1], (int)num_color[1], (int)num_color[1], (int)alpha_girl)
	);

	//1P
	DX9::SpriteBatch->DrawSimple(
		speach_my_reply.Get(),
		pos_speach_girl_1p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, alpha_speach_my)
	);

	DX9::SpriteBatch->DrawSimple(
		speach_partner_reply.Get(),
		pos_speach_boy_1p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, alpha_speach_partner)
	);

	//2P
	DX9::SpriteBatch->DrawSimple(
		speach_partner_reply.Get(),
		pos_speach_girl_2p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, alpha_speach_my)
	);

	DX9::SpriteBatch->DrawSimple(
		speach_my_reply.Get(),
		pos_speach_boy_2p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, alpha_speach_partner)
	);
}

void Attack::Re_Speak() {
	//1P画面
	if (speach == ATTACK_GIRL) {
		DX9::SpriteBatch->DrawSimple(
			speach_my_attack.Get(),
			pos_speach_girl_1p,
			Rect(0.0f, 0.0f, 1000.0f, 260.0f),
			DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_my)
		);

		DX9::SpriteBatch->DrawSimple(
			speach_partner_reply.Get(),
			pos_speach_boy_1p,
			Rect(0.0f, 0.0f, 1000.0f, 260.0f),
			DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_partner)
		);
	}

	if (speach == ATTACK_BOY) {
		DX9::SpriteBatch->DrawSimple(
			speach_my_reply.Get(),
			pos_speach_girl_1p,
			Rect(0.0f, 0.0f, 1000.0f, 260.0f),
			DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_my)
		);

		DX9::SpriteBatch->DrawSimple(
			speach_partner_attack.Get(),
			pos_speach_boy_1p,
			Rect(0.0f, 0.0f, 1000.0f, 260.0f),
			DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_partner)
		);
	}


	//2P画面
	if (speach == ATTACK_GIRL) {
		DX9::SpriteBatch->DrawSimple(
			speach_partner_attack.Get(),
			pos_speach_girl_2p,
			Rect(0.0f, 0.0f, 1000.0f, 260.0f),
			DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_my)
		);

		DX9::SpriteBatch->DrawSimple(
			speach_my_reply.Get(),
			pos_speach_boy_2p,
			Rect(0.0f, 0.0f, 1000.0f, 260.0f),
			DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_partner)
		);
	}

	if (speach == ATTACK_BOY) {
		DX9::SpriteBatch->DrawSimple(
			speach_partner_reply.Get(),
			pos_speach_girl_2p,
			Rect(0.0f, 0.0f, 1000.0f, 260.0f),
			DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_my)
		);

		DX9::SpriteBatch->DrawSimple(
			speach_my_attack.Get(),
			pos_speach_boy_2p,
			Rect(0.0f, 0.0f, 1000.0f, 260.0f),
			DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_partner)
		);
	}


	if (speach_flag == true) {
		RECT dest = RectWH(pos_speach_girl_1p.x, pos_speach_girl_1p.y, 1000, 1000);
		DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[0].c_str(), comment, dest, DX9::Colors::Red);
		//フォント,セリフの入った変数,1度に表示する文字数,文字を表示する場所,色
	}

}


// 時間かかりそうだから、これを使わない方針で調整する
void Attack::Re_Chara(int index) {
	// プレイヤーの画面に合わせて使う画像を変える
	DX9::SPRITE boy = index == 0 ? boy_a[MainScene::emotion[MainScene::PLAYER::B]] : boy_b[MainScene::emotion[MainScene::PLAYER::B]];
	DX9::SPRITE girl = index == 0 ? girl_a[MainScene::emotion[MainScene::PLAYER::A]] : girl_b[MainScene::emotion[MainScene::PLAYER::A]];

	// 座標
	SimpleMath::Vector2 pos_boy_;
	SimpleMath::Vector2 pos_girl_;
	if (index) {
		pos_boy_ = MainScene::emotion[MainScene::PLAYER::B] == MainScene::EMOTION::PROPOSAL ? POS_BOY_AT_A : POS_BOY_GE_A;
		pos_girl_ = MainScene::emotion[MainScene::PLAYER::A] == MainScene::EMOTION::PROPOSAL ? POS_LEFT_ATTACK : POS_LEFT_GENE;
	}
	else {
		pos_boy_ = MainScene::emotion[MainScene::PLAYER::B] == MainScene::EMOTION::PROPOSAL ? POS_LEFT_ATTACK : POS_LEFT_GENE;
		pos_girl_ = MainScene::emotion[MainScene::PLAYER::A] == MainScene::EMOTION::PROPOSAL ? POS_GIRL_AT_B : POS_GIRL_GE_B;
	}

	// rectのx座標
	float rc_boy_x_ = MainScene::emotion[MainScene::PLAYER::B] == MainScene::EMOTION::PROPOSAL ? RC_BOY_ATT_R : RC_BOY_NOM_R;
	float rc_girl_x_ = MainScene::emotion[MainScene::PLAYER::A] == MainScene::EMOTION::PROPOSAL ? RC_GIRL_ATT_R : RC_GIRL_NOM_R;

	// rectのy座標
	float rc_y_g_ = MainScene::emotion[MainScene::PLAYER::A] == MainScene::EMOTION::PROPOSAL ? 990.f : 750.0f;
	float rc_y_b_ = MainScene::emotion[MainScene::PLAYER::B] == MainScene::EMOTION::PROPOSAL ? 990.f : 750.0f;

	DX9::SpriteBatch->DrawSimple(
		boy.Get(),
		SimpleMath::Vector3(pos_boy_.x, pos_boy_.y, MainScene::POSI_Z::PLAYER),
		Rect(0.0f, 0.0f, rc_boy_x_, rc_y_b_),
		DX9::Colors::RGBA(num_color[0], num_color[0], num_color[0], 255)
	);
	DX9::SpriteBatch->DrawSimple(
		girl_a[MainScene::emotion[MainScene::PLAYER::A]].Get(),
		SimpleMath::Vector3(POS_LEFT_GENE.x, POS_LEFT_GENE.y, MainScene::POSI_Z::PLAYER),
		Rect(0.0f, 0.0f, rc_girl_x_, rc_y_g_),
		DX9::Colors::RGBA(num_color[1], num_color[1], num_color[1], 255)
	);
}

cppcoro::generator<int>Attack::Action() {
	
	if (MainScene::num_player == 0) {
		co_yield 0;
		//女子の透明度を下げる
		//while (alpha_girl > 0.0f) {
		//	alpha_girl = max(alpha_girl - NUM_ALPHA_CHARA * time_delta, 0.0f);
		//	co_yield 1;
		//}
		//alpha_girl = 0.0f;
		//女子の差分切り替え
		//while (time_stop < 0.1f)
		//{
		//	//time_stop += time_delta;
		//	time_stop = min(time_stop + time_delta, 0.1f);
		//	co_yield 2;
		//}
		//女子の透明度を上げる
		//while (alpha_girl < 255.0f)
		//{
		//	alpha_girl = min(alpha_girl + NUM_ALPHA_CHARA * time_delta, 255.0f);
		//	co_yield 3;
		//}


		MainScene::emotion[MainScene::num_player] = MainScene::EMOTION::PROPOSAL;
		num_color[1] = COLOR_MAX;
		speach = ATTACK_GIRL;
		pos_speach_girl_1p.y = pos_speach_attack_y;
		pos_speach_girl_2p.y = pos_speach_attack_y;


		while (time_stop < 0.2f) {
			time_stop = min(time_stop + time_delta, 0.2f);
			co_yield 1;
		}

		//吹き出しの移動
		while (pos_speach_girl_2p.x > pos_speach_attack_girl_limit_x_2p)
		{

			//alpha_speach_my += NUM_ALPHA_SPEACH * time_delta;
			alpha_speach_my = min(alpha_speach_my + NUM_ALPHA_SPEACH * time_delta, COLOR_MAX);

			//pos_speach_girl_1p.x += NUM_SPEED * time_delta;
			//if (pos_speach_girl_1p.x > pos_speach_attack_girl_limit_x_1p)
			//	pos_speach_girl_1p.x = pos_speach_attack_girl_limit_x_1p;
			pos_speach_girl_1p.x = min(pos_speach_girl_1p.x + NUM_SPEED * time_delta, pos_speach_attack_girl_limit_x_1p);

			//pos_speach_girl_2p.x -= NUM_SPEED * time_delta;
			//if (pos_speach_girl_2p.x < pos_speach_attack_girl_limit_x_2p)
			//	pos_speach_girl_2p.x = pos_speach_attack_girl_limit_x_2p;
			pos_speach_girl_2p.x = max(pos_speach_girl_2p.x - NUM_SPEED * time_delta, pos_speach_attack_girl_limit_x_2p);

			co_yield 2;
		}
		//alpha_speach_my = COLOR_MAX;
		//pos_speach_girl_1p.x = pos_speach_attack_girl_limit_x_1p;
		//pos_speach_girl_2p.x = pos_speach_attack_girl_limit_x_2p;


		while (time_stop < 5.0f)
		{
			speach_flag = true;
			time_stop += time_delta;

			comment++;
			if (comment > LoadLines::lines1[0].length()) {
				comment = LoadLines::lines1[0].length();
			}

			co_yield 3;
		}


		//while (true)
		//{
		//}

		co_return;
	}
}