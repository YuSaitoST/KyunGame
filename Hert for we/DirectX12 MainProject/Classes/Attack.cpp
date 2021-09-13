#include"Classes/Attack.h"
#include <iostream>
#include<MainScene.h>
#include"LoadLines.h"
using namespace std;

float Attack::alpha_boy;
float Attack::alpha_girl;

void Attack::Initialize() {

	font = DX9::SpriteFont::CreateFromFontFile(DXTK->Device9, L"Font/HuiFont.ttf", L"ふい字", 35);

	count_chnage = 0;
	time_delta = 0.0f;
	alpha_boy = 255.0f;
	alpha_girl = 255.0f;
	time_stop = 0.0f;


	pos_speach_girl_1p.x = 400.0f;
	pos_speach_girl_1p.y = 0.0f;
	pos_speach_girl_1p.z = MainScene::POSI_Z::SPEAK;

	pos_speach_boy_1p.x = 520.0f;
	pos_speach_boy_1p.y = 0.0f;
	pos_speach_boy_1p.z = MainScene::POSI_Z::SPEAK;


	pos_speach_girl_2p.x = 2440.0f;
	pos_speach_girl_2p.y = 0.0f;
	pos_speach_girl_2p.z = MainScene::POSI_Z::SPEAK;

	pos_speach_boy_2p.x = 2320.0f;
	pos_speach_boy_2p.y = 0.0f;
	pos_speach_boy_2p.z = MainScene::POSI_Z::SPEAK;


	alpha_speach_girl = 0.0f;
	alpha_speach_boy = 0.0f;

	//num_color[0] = 255;
	//num_color[1] = 255;

	speach_my_reply             = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/speech_balloon.png"   );
	speach_my_attack      = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/speech_balloon_ul.png");
	speach_partner_reply        = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/speech_balloon_r.png" );
	speach_partner_attack = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/speech_balloon_ur.png");

	speach = ATTACK_GIRL;

	comment_attack = 0.0f;
	comment_reply = 0.0f;
	flag_speach_attack_girl = false;
	flag_speach_reply_boy   = false;

	flag_speach_attack_boy = false;
	flag_speach_reply_girl = false;

	flag_speach_hit_boy  = false;
	flag_speach_hit_girl = false;

	index_boy = 0;
	index_girl = 0;


	loadlines.Initialize();
	loadlines.In_File();

	count_random_boy = 0;
	count_random_girl = 0;


	//男子アタック時セリフ
	std::random_device seed;
	random_engine = std::mt19937(seed());
	random_attack_boy = std::uniform_int_distribution<>(0, 19);
	// ランダム値を入れる時は、  random_dist(random_engine)  を代入;

	//女子アタック時セリフ
	random_device flower;
	random_engine = mt19937(flower());
	random_attack_girl = uniform_int_distribution<>(20, 39);
	

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

	se_cv_attack = DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\Main\\attack_textbox.mp3");
	se_cv_graze = DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\Main\\kasuri_textbox.mp3");
	se_cv_nomal = DX9::MediaRenderer::CreateFromFile(DXTK->Device9, L"SE\\Main\\nomal_textbox.mp3");
}

bool Attack::Up_Attack(const float deltaTime) {
	if (count_random_boy == 0) {
	RE_SET:
		index_boy = random_attack_boy(random_engine);
		if (index_boy % 2 == 1) goto RE_SET;
		count_random_boy++;
	}

	if (count_random_girl == 0) {
	RE_START:
		index_girl = random_attack_girl(random_engine);
		if (index_girl % 2 == 1) goto RE_START;
		count_random_girl++;
	}

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

	//DX9::SpriteBatch->DrawSimple(
	//	boy_a[MainScene::emotion[index]].Get(),
	//	SimpleMath::Vector3(pos_x_ + pos_boy.x, pos_boy.y, MainScene::POSI_Z::PLAYER),
	//	Rect(0.0f, 0.0f, 402.0f, 590.0f),
	//	DX9::Colors::RGBA((int)num_color[0], (int)num_color[0], (int)num_color[0], (int)alpha_boy)
	//);
	//DX9::SpriteBatch->DrawSimple(
	//	girl_a[MainScene::emotion[index]].Get(),
	//	SimpleMath::Vector3(pos_x_ + pos_girl.x, pos_girl.y, MainScene::POSI_Z::PLAYER),
	//	Rect(0.0f, 0.0f, 438.0f, 476.0f),
	//	DX9::Colors::RGBA((int)num_color[1], (int)num_color[1], (int)num_color[1], (int)alpha_girl)
	//);

	//1P
	DX9::SpriteBatch->DrawSimple(
		speach_my_reply.Get(),
		pos_speach_girl_1p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, alpha_speach_girl)
	);

	DX9::SpriteBatch->DrawSimple(
		speach_partner_reply.Get(),
		pos_speach_boy_1p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, alpha_speach_boy)
	);

	//2P
	DX9::SpriteBatch->DrawSimple(
		speach_partner_reply.Get(),
		pos_speach_girl_2p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, alpha_speach_girl)
	);

	DX9::SpriteBatch->DrawSimple(
		speach_my_reply.Get(),
		pos_speach_boy_2p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, alpha_speach_boy)
	);
}

void Attack::Re_Speak() {
	//アタック(女子)
	if (speach == ATTACK_GIRL) {
		//1P画面
		DX9::SpriteBatch->DrawSimple(
			speach_my_attack.Get(),
			pos_speach_girl_1p,
			Rect(0.0f, 0.0f, 1000.0f, 260.0f),
			DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_girl)
		);

		DX9::SpriteBatch->DrawSimple(
			speach_partner_reply.Get(),
			pos_speach_boy_1p,
			Rect(0.0f, 0.0f, 1000.0f, 260.0f),
			DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_boy)
		);

		//2P画面
		DX9::SpriteBatch->DrawSimple(
			speach_partner_attack.Get(),
			pos_speach_girl_2p,
			Rect(0.0f, 0.0f, 1000.0f, 260.0f),
			DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_girl)
		);

		DX9::SpriteBatch->DrawSimple(
			speach_my_reply.Get(),
			pos_speach_boy_2p,
			Rect(0.0f, 0.0f, 1000.0f, 260.0f),
			DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_boy)
		);

		//女子アタックセリフ表示
		if (flag_speach_attack_girl == true) {
			//1P画面
			RECT dest = RectWH(pos_speach_girl_1p.x + 135, pos_speach_girl_1p.y + 70, 1000, 1000);
			DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[index_girl].c_str(), (int)comment_attack, dest, DX9::Colors::Black);
			//フォント,セリフの入った変数,1度に表示する文字数,文字を表示する場所,色

			//2P画面
			RECT means = RectWH(pos_speach_girl_2p.x + 60, pos_speach_girl_2p.y + 70, 1000, 1000);
			DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[index_girl].c_str(), (int)comment_attack, means, DX9::Colors::Black);
		}
		//男子返答セリフ表示
		if (flag_speach_reply_boy == true) {
			//1P画面
			RECT dest = RectWH(pos_speach_boy_1p.x + 70, pos_speach_boy_1p.y + 70, 1000, 1000);
			DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[index_girl+1].c_str(), (int)comment_reply, dest, DX9::Colors::Black);
			//フォント,セリフの入った変数,1度に表示する文字数,文字を表示する場所,色

			//2P画面
			RECT means = RectWH(pos_speach_boy_2p.x + 115, pos_speach_boy_2p.y + 70, 1000, 1000);
			DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[index_girl+1].c_str(), (int)comment_reply, means, DX9::Colors::Black);
		}

		//男子ハート直撃時返答セリフ表示
		if (flag_speach_hit_boy == true) {
			RECT dest = RectWH(pos_speach_boy_1p.x + 70, pos_speach_boy_1p.y + 70, 1000, 1000);
			DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[0].c_str(), (int)comment_reply, dest, DX9::Colors::Black);
			//フォント,セリフの入った変数,1度に表示する文字数,文字を表示する場所,色

			//2P画面
			RECT means = RectWH(pos_speach_boy_2p.x + 115, pos_speach_boy_2p.y + 70, 1000, 1000);
			DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[0].c_str(), (int)comment_reply, means, DX9::Colors::Black);
		}
	}

	//アタック(男子)
	if (speach == ATTACK_BOY) {
		//1P画面
		DX9::SpriteBatch->DrawSimple(
			speach_my_reply.Get(),
			pos_speach_girl_1p,
			Rect(0.0f, 0.0f, 1000.0f, 260.0f),
			DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_girl)
		);

		DX9::SpriteBatch->DrawSimple(
			speach_partner_attack.Get(),
			pos_speach_boy_1p,
			Rect(0.0f, 0.0f, 1000.0f, 260.0f),
			DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_boy)
		);

		//2P画面
		DX9::SpriteBatch->DrawSimple(
			speach_partner_reply.Get(),
			pos_speach_girl_2p,
			Rect(0.0f, 0.0f, 1000.0f, 260.0f),
			DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_girl)
		);

		DX9::SpriteBatch->DrawSimple(
			speach_my_attack.Get(),
			pos_speach_boy_2p,
			Rect(0.0f, 0.0f, 1000.0f, 260.0f),
			DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_boy)
		);

		//男子アタックセリフ表示
		if (flag_speach_attack_boy == true) {
			//1P画面
			RECT dest = RectWH(pos_speach_boy_1p.x + 75, pos_speach_boy_1p.y + 70, 1000, 1000);
			DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[index_boy].c_str(), (int)comment_attack, dest, DX9::Colors::Black);
			//フォント,セリフの入った変数,1度に表示する文字数,文字を表示する場所,色

			//2P画面
			RECT means = RectWH(pos_speach_boy_2p.x + 115, pos_speach_boy_2p.y + 70, 1000, 1000);
			DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[index_boy].c_str(), (int)comment_attack, means, DX9::Colors::Black);
		}
		//女子返答セリフ表示
		if (flag_speach_reply_girl == true) {
			//1P画面
			RECT dest = RectWH(pos_speach_girl_1p.x + 125, pos_speach_girl_1p.y + 70, 1000, 1000);
			DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[index_boy + 1].c_str(), (int)comment_reply, dest, DX9::Colors::Black);
			//フォント,セリフの入った変数,1度に表示する文字数,文字を表示する場所,色

			//2P画面
			RECT means = RectWH(pos_speach_girl_2p.x + 65, pos_speach_girl_2p.y + 70, 1000, 1000);
			DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[index_boy + 1].c_str(), (int)comment_reply, means, DX9::Colors::Black);
		}

		//女子ハート直撃時返答セリフ表示
		if (flag_speach_hit_girl == true) {
			RECT dest = RectWH(pos_speach_girl_1p.x + 125, pos_speach_girl_1p.y + 70, 1000, 1000);
			DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[1].c_str(), (int)comment_reply, dest, DX9::Colors::Black);
			//フォント,セリフの入った変数,1度に表示する文字数,文字を表示する場所,色

			//2P画面
			RECT means = RectWH(pos_speach_girl_2p.x + 65, pos_speach_girl_2p.y + 70, 1000, 1000);
			DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[1].c_str(), (int)comment_reply, means, DX9::Colors::Black);
		}
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

	//DX9::SpriteBatch->DrawSimple(
	//	boy.Get(),
	//	SimpleMath::Vector3(pos_boy_.x, pos_boy_.y, MainScene::POSI_Z::PLAYER),
	//	Rect(0.0f, 0.0f, rc_boy_x_, rc_y_b_),
	//	DX9::Colors::RGBA(num_color[0], num_color[0], num_color[0], 255)
	//);
	//DX9::SpriteBatch->DrawSimple(
	//	girl_a[MainScene::emotion[MainScene::PLAYER::A]].Get(),
	//	SimpleMath::Vector3(POS_LEFT_GENE.x, POS_LEFT_GENE.y, MainScene::POSI_Z::PLAYER),
	//	Rect(0.0f, 0.0f, rc_girl_x_, rc_y_g_),
	//	DX9::Colors::RGBA(num_color[1], num_color[1], num_color[1], 255)
	//);
}

cppcoro::generator<int>Attack::Action() {
	int partner_ = (MainScene::num_player == 0);  // 相手プレイヤーを指定

	//女子アタック時
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
		//alpha_speach_my += NUM_ALPHA_SPEACH * time_delta;
		//pos_speach_girl_1p.x += NUM_SPEED * time_delta;
		//if (pos_speach_girl_1p.x > pos_speach_attack_girl_limit_x_1p)
		//	pos_speach_girl_1p.x = pos_speach_attack_girl_limit_x_1p;
		//pos_speach_girl_2p.x -= NUM_SPEED * time_delta;
		//if (pos_speach_girl_2p.x < pos_speach_attack_girl_limit_x_2p)
		//	pos_speach_girl_2p.x = pos_speach_attack_girl_limit_x_2p;
		//alpha_speach_my = COLOR_MAX;
		//pos_speach_girl_1p.x = pos_speach_attack_girl_limit_x_1p;
		//pos_speach_girl_2p.x = pos_speach_attack_girl_limit_x_2p;

		se_cv_attack->Replay();
		MainScene::emotion[MainScene::num_player] = MainScene::EMOTION::PROPOSAL;
		MainScene::num_color[0] = COLOR_MAX;
		MainScene::num_color[1] = COLOR_MAX;
		speach = ATTACK_GIRL;
		pos_speach_girl_1p.y = pos_speach_attack_y;
		pos_speach_girl_2p.y = pos_speach_attack_y;
		pos_speach_boy_1p.y = pos_speach_reply_y;
		pos_speach_boy_2p.y = pos_speach_reply_y;

		//画像切り替え後の間
		while (time_stop < 0.2f) {
			time_stop = min(time_stop + time_delta, 0.2f);
			co_yield 1;
		}
		time_stop = 0.0f;

		//吹き出しの移動
		while (alpha_speach_girl < COLOR_MAX)
		{
			alpha_speach_girl = min(alpha_speach_girl + NUM_ALPHA_SPEACH * time_delta, COLOR_MAX);

			pos_speach_girl_1p.x = min(pos_speach_girl_1p.x + NUM_SPEED * time_delta, pos_speach_attack_girl_limit_x_1p);

			pos_speach_girl_2p.x = max(pos_speach_girl_2p.x - NUM_SPEED * time_delta, pos_speach_attack_girl_limit_x_2p);

			co_yield 2;
		}

		//セリフ(女子アタック)の表示
		while (time_stop < 3.0f)
		{
			flag_speach_attack_girl = true;
			time_stop += time_delta;

			comment_attack += TALK_SPEED * time_delta;
			if (comment_attack > LoadLines::lines1[index_girl].length()) {
				comment_attack = LoadLines::lines1[index_girl].length();
			}
			co_yield 3;
		}
		flag_speach_attack_girl = false;
		alpha_speach_girl = 0.0f;
		time_stop = 0.0f;
		comment_attack = 0.0f;
		MainScene::emotion[MainScene::num_player] = MainScene::EMOTION::GENERALLY;

		//間
		while (time_stop < 0.8f) {
			time_stop += time_delta;
			co_yield 4;
		}
		time_stop = 0.0f;

		//ハートがスカまたは直撃
		if (MainScene::flag_suka == true || MainScene::flag_hit == true) {
			MainScene::emotion[partner_] = MainScene::EMOTION::GENERALLY;

			pos_speach_boy_1p.x = pos_speach_normal_boy_limit_x_1p;
			pos_speach_boy_2p.x = pos_speach_normal_boy_limit_x_2p;
			//吹き出し(男子返答)の表示
			while (alpha_speach_boy < COLOR_MAX)
			{
				alpha_speach_boy = min(alpha_speach_boy + NUM_ALPHA_SPEACH * time_delta, COLOR_MAX);

				//pos_speach_boy_1p.x = max(pos_speach_boy_1p.x - NUM_SPEED * time_delta, pos_speach_normal_boy_limit_x_1p);
				//pos_speach_boy_2p.x = min(pos_speach_boy_2p.x + NUM_SPEED * time_delta, pos_speach_normal_boy_limit_x_2p);
				co_yield 5;
			}
			if (MainScene::flag_suka) se_cv_nomal->Replay();
			//セリフ(男子返答)の表示
			while (time_stop < 3.0f) {
				flag_speach_reply_boy = true;
				time_stop += time_delta;

				comment_reply += TALK_SPEED * time_delta;
				if (comment_reply > LoadLines::lines1[index_girl + 1].length()) {
					comment_reply = LoadLines::lines1[index_girl + 1].length();
				}
				co_yield 6;
			}
			comment_reply = 0.0f;
			time_stop = 0.0f;
			alpha_speach_boy = 0.0f;
			flag_speach_reply_boy = false;

			//直撃だった場合の演出
			if (MainScene::flag_hit == true) {
				flag_speach_hit_boy = true;

				//間
				while (time_stop < 0.8)
				{
					time_stop += time_delta;
					co_yield 7;
				}
				MainScene::emotion[partner_] = MainScene::EMOTION::DEFEAT;
				time_stop = 0.0f;

				//吹き出し(男子)の表示
				while (alpha_speach_boy < COLOR_MAX) {
					alpha_speach_boy = min(alpha_speach_boy + NUM_ALPHA_SPEACH * time_delta, COLOR_MAX);
					co_yield 8;
				}

				//直撃テンプレセリフ(男子)表示
				while (time_stop < 3.0f) 
				{
					time_stop += time_delta;

					comment_reply += TALK_SPEED * time_delta;
					if (comment_reply > LoadLines::lines1[0].length()) {
						comment_reply = LoadLines::lines1[0].length();
					}
					co_yield  9;
				}
				time_stop = 0.0f;

				//間
				while (time_stop < 0.8) {
					time_stop += time_delta;
					co_yield 10;
				}
				time_stop = 0.0f;
				MainScene::emotion[MainScene::num_player] = MainScene::EMOTION::VICTORY;

				//間
				while (time_stop < 1.0f) {
					time_stop += time_delta;
				}
				co_return;
			}

			MainScene::flag_suka = false;
			MainScene::flag_graze = false;
			MainScene::flag_hit = false;
			//MainScene::num_color[1] = COLOR_GRAY;
			co_return;
		}

		//ハートが近い
		else if (MainScene::flag_graze == true) {
			MainScene::emotion[partner_] = MainScene::EMOTION::NERVOUS;
			se_cv_graze->Replay();

			//男子のリアクション
			while (time_stop < 0.5f)
			{
				time_stop += time_delta;
				if (MainScene::POS_BOY_A > -1.0f)
				{
					MainScene::POS_BOY_A -= 100.0f * time_delta;
					MainScene::POS_BOY_B -= 100.0f * time_delta;
				}
				else if (MainScene::POS_BOY_A < -1.0f)
				{
					MainScene::POS_BOY_A += 300.0f * time_delta;
					MainScene::POS_BOY_B += 300.0f * time_delta;
				}
				co_yield 7;
			}
			MainScene::POS_BOY_A = 0.0f;
			MainScene::POS_BOY_B = 0.0f;
			time_stop = 0.0f;

			pos_speach_boy_1p.x = pos_speach_normal_boy_limit_x_1p;
			pos_speach_boy_2p.x = pos_speach_normal_boy_limit_x_2p;

			//吹き出し(男子返答)の表示
			while (alpha_speach_boy < COLOR_MAX) {
				alpha_speach_boy = min(alpha_speach_boy + NUM_ALPHA_SPEACH * time_delta, COLOR_MAX);

				//pos_speach_boy_1p.x = max(pos_speach_boy_1p.x - NUM_SPEED * time_delta, pos_speach_normal_boy_limit_x_1p);
				//pos_speach_boy_2p.x = min(pos_speach_boy_2p.x + NUM_SPEED * time_delta, pos_speach_normal_boy_limit_x_2p);
				co_yield 5;
			}

			//セリフ(男子返答)の表示
			while (time_stop < 3.0f) {
				flag_speach_reply_boy = true;
				time_stop += time_delta;

				comment_reply += TALK_SPEED * time_delta;
				if (comment_reply > LoadLines::lines1[index_girl + 1].length()) {
					comment_reply = LoadLines::lines1[index_girl + 1].length();
				}
				co_yield 8;
			}
			flag_speach_reply_boy = false;
			MainScene::flag_suka = false;
			MainScene::flag_graze = false;
			MainScene::flag_hit = false;
			//MainScene::num_color[1] = COLOR_GRAY;
			co_return;
		}
		co_return;
	}

	//男子アタック時
	if (MainScene::num_player == 1) {
		co_yield 0;
		se_cv_attack->Replay();
		MainScene::emotion[MainScene::num_player] = MainScene::EMOTION::PROPOSAL;
		MainScene::num_color[0] = COLOR_MAX;
		MainScene::num_color[1] = COLOR_MAX;
		speach = ATTACK_BOY;
		pos_speach_boy_1p.y = pos_speach_attack_y;
		pos_speach_boy_2p.y = pos_speach_attack_y;
		pos_speach_girl_1p.y = pos_speach_reply_y;
		pos_speach_girl_2p.y = pos_speach_reply_y;

		//画像切り替え後の間
		while (time_stop < 0.2f) {
			time_stop = min(time_stop + time_delta, 0.2f);
			co_yield 1;
		}
		time_stop = 0.0f;

		//吹き出しの移動
		while (alpha_speach_boy < COLOR_MAX)
		{
			alpha_speach_boy = min(alpha_speach_boy + NUM_ALPHA_SPEACH * time_delta, COLOR_MAX);

			pos_speach_boy_1p.x = max(pos_speach_boy_1p.x - NUM_SPEED * time_delta, pos_speach_attack_boy_limit_x_1p);

			pos_speach_boy_2p.x = min(pos_speach_boy_2p.x + NUM_SPEED * time_delta, pos_speach_attack_boy_limit_x_2p);

			co_yield 2;
		}

		se_cv_nomal->Replay();
		//セリフ(男子アタック)の表示
		while (time_stop < 3.0f)
		{
			flag_speach_attack_boy = true;
			time_stop += time_delta;

			comment_attack += TALK_SPEED * time_delta;
			if (comment_attack > LoadLines::lines1[index_boy].length()) {
				comment_attack = LoadLines::lines1[index_boy].length();
			}
			co_yield 3;
		}
		flag_speach_attack_boy = false;
		alpha_speach_boy = 0.0f;
		time_stop = 0.0f;
		comment_attack = 0;
		MainScene::emotion[MainScene::num_player] = MainScene::EMOTION::GENERALLY;

		//間
		while (time_stop < 0.8f) {
			time_stop += time_delta;
			co_yield 4;
		}
		time_stop = 0.0f;

		//ハートがスカまたは直撃
		if (MainScene::flag_suka == true || MainScene::flag_hit == true) {
			MainScene::emotion[partner_] = MainScene::EMOTION::GENERALLY;

			pos_speach_girl_1p.x = pos_speach_normal_girl_limit_x_1p;
			pos_speach_girl_2p.x = pos_speach_normal_girl_limit_x_2p;

			//吹き出し(女子返答)の表示
			while (alpha_speach_girl < COLOR_MAX)
			{
				alpha_speach_girl = min(alpha_speach_girl + NUM_ALPHA_SPEACH * time_delta, COLOR_MAX);

				//pos_speach_girl_1p.x = max(pos_speach_girl_1p.x - NUM_SPEED * time_delta, pos_speach_normal_girl_limit_x_1p);
				//pos_speach_girl_2p.x = min(pos_speach_girl_2p.x + NUM_SPEED * time_delta, pos_speach_normal_girl_limit_x_2p);
				co_yield 5;
			}

			//セリフ(女子返答)の表示
			while (time_stop < 3.0f) {
				flag_speach_reply_girl = true;
				time_stop += time_delta;

				comment_reply += TALK_SPEED * time_delta;
				if (comment_reply > LoadLines::lines1[index_boy + 1].length()) {
					comment_reply = LoadLines::lines1[index_boy + 1].length();
				}
				co_yield 6;
			}
			comment_reply = 0.0f;
			time_stop = 0.0f;
			alpha_speach_girl = 0.0f;
			flag_speach_reply_girl = false;

			//直撃だった場合の演出
			if (MainScene::flag_hit == true) {
				flag_speach_hit_girl = true;

				//間
				while (time_stop < 1.0f) {
					time_stop += time_delta;
					co_yield 7;
				}
				MainScene::emotion[partner_] = MainScene::EMOTION::DEFEAT;
				time_stop = 0.0f;

				//吹き出し(女子)の表示
				while (alpha_speach_girl < COLOR_MAX) {
					alpha_speach_girl = min(alpha_speach_girl + NUM_ALPHA_SPEACH * time_delta, COLOR_MAX);
					co_yield 8;
				}

				//直撃テンプレセリフ(女子)表示
				while (time_stop < 3.0f) {
					time_stop += time_delta;

					comment_reply += TALK_SPEED * time_delta;
					if (comment_reply > LoadLines::lines1[1].length()) {
						comment_reply = LoadLines::lines1[1].length();
						//呪文詠唱はこちら！↓↓↓
						//comment_reply > LoadLines::lines1[1].length();
					}
					co_yield 9;
				}
				time_stop = 0.0f;
				MainScene::emotion[MainScene::num_player] = MainScene::EMOTION::VICTORY;

				//間
				while (time_stop < 2.0f) {
					time_stop += time_delta;
					co_yield 10;
				}
				co_return;
			}

			MainScene::flag_suka = false;
			MainScene::flag_graze = false;
			MainScene::flag_hit = false;
			//MainScene::num_color[0] = COLOR_GRAY;
			co_return;
		}

		//ハートが近い
		else if (MainScene::flag_graze == true) {
			MainScene::emotion[partner_] = MainScene::EMOTION::NERVOUS;
			se_cv_graze->Replay();

			//女子のリアクション
			while (time_stop < 0.5f)
			{
				time_stop += time_delta;
				if (MainScene::POS_GIRL_A > -1.0f)
				{
					MainScene::POS_GIRL_A -= 100.0f * time_delta;
					MainScene::POS_GIRL_B -= 100.0f * time_delta;
				}
				else if (MainScene::POS_GIRL_A < -1.0f)
				{
					MainScene::POS_GIRL_A += 300.0f * time_delta;
					MainScene::POS_GIRL_B += 300.0f * time_delta;
				}
				co_yield 7;
			}
			MainScene::POS_GIRL_A = 0.0f;
			MainScene::POS_GIRL_B = 0.0f;
			time_stop = 0.0f;

			pos_speach_girl_1p.x = pos_speach_normal_girl_limit_x_1p;
			pos_speach_girl_2p.x = pos_speach_normal_girl_limit_x_2p;

			//吹き出し(女子返答)の表示
			while (alpha_speach_girl < COLOR_MAX) {
				alpha_speach_girl = min(alpha_speach_girl + NUM_ALPHA_SPEACH * time_delta, COLOR_MAX);

				//pos_speach_girl_1p.x = max(pos_speach_girl_1p.x - NUM_SPEED * time_delta, pos_speach_normal_girl_limit_x_1p);
				//pos_speach_girl_2p.x = min(pos_speach_girl_2p.x + NUM_SPEED * time_delta, pos_speach_normal_girl_limit_x_2p);
				co_yield 5;
			}

			//セリフ(女子返答)の表示
			while (time_stop < 3.0f) {
				flag_speach_reply_girl = true;
				time_stop += time_delta;

				comment_reply += TALK_SPEED * time_delta;
				if (comment_reply > LoadLines::lines1[index_boy + 1].length()) {
					comment_reply = LoadLines::lines1[index_boy + 1].length();
				}
				co_yield 8;
			}
			flag_speach_reply_girl = false;
			MainScene::flag_suka = false;
			MainScene::flag_graze = false;
			MainScene::flag_hit = false;
			//MainScene::num_color[0] = COLOR_GRAY;
			co_return;
		}
		co_return;
	}
}