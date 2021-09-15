#include "Classes/BlackOut.h"
#include <iostream>
#include <MainScene.h>
#include <ResultScene.h>

float BlackOut::alpha_black;

void BlackOut::Initialize() {

	font_skip = DX9::SpriteFont::CreateFromFontFile(DXTK->Device9, L"Font/HuiFont29.ttf", L"ふい字", 30);

	alpha_black = MainScene::phase == MainScene::Phase::SCENARIO ? 255.0f : 0.0f;
	alpha_text = 0;

	time_delta = 0.0f;
	time_stop  = 2.0f;

	count_chnage = 0;
	time_stop = 0.0f;


	font = DX9::SpriteFont::CreateFromFontFile(DXTK->Device9, L"Font/HuiFont.ttf", L"ふい字", 35);
	speach_my_reply		 = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/speech_balloon.png"	 );
	speach_partner_reply = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/speech_balloon_r.png" );
	speach_synchro		 = DX9::Sprite::CreateFromFile(DXTK->Device9, L"UI/speech_balloon_rl.png");

	pos_speach_girl_1p.x = pos_speach_normal_girl_limit_x_1p;
	pos_speach_girl_1p.y = pos_speach_reply_y;
	pos_speach_girl_1p.z = MainScene::POSI_Z::SPEAK;

	pos_speach_boy_1p.x = pos_speach_normal_boy_limit_x_1p;
	pos_speach_boy_1p.y = pos_speach_reply_y;
	pos_speach_boy_1p.z = MainScene::POSI_Z::SPEAK;


	pos_speach_girl_2p.x = pos_speach_normal_girl_limit_x_2p;
	pos_speach_girl_2p.y = pos_speach_reply_y;
	pos_speach_girl_2p.z = MainScene::POSI_Z::SPEAK;

	pos_speach_boy_2p.x = pos_speach_normal_boy_limit_x_2p;
	pos_speach_boy_2p.y = pos_speach_reply_y;
	pos_speach_boy_2p.z = MainScene::POSI_Z::SPEAK;

	pos_speach_synchro_1p.x = pos_speach_synchro_x_1p;
	pos_speach_synchro_1p.y = pos_speach_reply_y;
	pos_speach_synchro_1p.z = MainScene::POSI_Z::SPEAK;

	pos_speach_synchro_2p.x = pos_speach_synchro_x_2p;
	pos_speach_synchro_2p.y = pos_speach_reply_y;
	pos_speach_synchro_2p.z = MainScene::POSI_Z::SPEAK;

	alpha_speach_girl = 0.0f;
	alpha_speach_boy  = 0.0f;
	alpha_speach_synchro = 0.0f;

	comment_reply = 0.0f;
	comment_plus  = 0;

	flag_talk_boy  = false;
	flag_talk_girl = false;
	flag_talk_synchro = false;
}

void BlackOut::LoadAssets() {
	black	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"Effect/black.png");
	text	= DX9::Sprite::CreateFromFile(DXTK->Device9, L"prolog_text.png" );

}

bool BlackOut::Up_Black(const float deltaTime) {
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

void BlackOut::Render() {
	DX9::SpriteBatch->DrawSimple(
		black.Get(), SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
		Rect(0.0f, 0.0f, 3840.0f, 1080.0f),
		DX9::Colors::RGBA(255, 255, 255, (int)alpha_black)
	);

	DX9::SpriteBatch->DrawSimple(
		text.Get(), SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
		Rect(0.0f, 0.0f, 1920.0f, 990.0f),
		DX9::Colors::RGBA(255, 255, 255, (int)alpha_text)
	);
	DX9::SpriteBatch->DrawSimple(
		text.Get(), SimpleMath::Vector3(1920.0f, 0.0f, 0.0f),
		Rect(0.0f, 0.0f, 1920.0f, 990.0f),
		DX9::Colors::RGBA(255, 255, 255, (int)alpha_text)
	);

	if (MainScene::phase != MainScene::Phase::SCENARIO) return;

	DX9::SpriteBatch->DrawString(
		font_skip.Get(), SimpleMath::Vector2(0.0f, 0.0f),
		DX9::Colors::RGBA(255, 255, 255, 255), L"Aボタン長押しでスキップ"
	);
	DX9::SpriteBatch->DrawString(
		font_skip.Get(), SimpleMath::Vector2(1920.0f + 0.0f, 0.0f),
		DX9::Colors::RGBA(255, 255, 255, 255), L"Aボタン長押しでスキップ"
	);


	//ここに会話シーンの描画を入れる
	//男子吹き出し
	DX9::SpriteBatch->DrawSimple(
		speach_partner_reply.Get(),
		pos_speach_boy_1p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_boy)
	);

	DX9::SpriteBatch->DrawSimple(
		speach_my_reply.Get(),
		pos_speach_boy_2p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_boy)
	);

	//男子セリフ表示
	if (flag_talk_boy == true) {
		//1P画面
		RECT dest = RectWH(pos_speach_boy_1p.x + 70, pos_speach_boy_1p.y + 70, 1000, 1000);
		DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[TALK_BOY + comment_plus].c_str(), (int)comment_reply, dest, DX9::Colors::Black);
		//フォント,セリフの入った変数,1度に表示する文字数,文字を表示する場所,色

		//2P画面
		RECT means = RectWH(pos_speach_boy_2p.x + 115, pos_speach_boy_2p.y + 70, 1000, 1000);
		DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[TALK_BOY + comment_plus].c_str(), (int)comment_reply, means, DX9::Colors::Black);
	}


	//女子吹き出し
	DX9::SpriteBatch->DrawSimple(
		speach_my_reply.Get(),
		pos_speach_girl_1p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_girl)
	);

	DX9::SpriteBatch->DrawSimple(
		speach_partner_reply.Get(),
		pos_speach_girl_2p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_girl)
	);

	//女子セリフ表示
	if (flag_talk_girl == true) {
		//1P画面
		RECT place = RectWH(pos_speach_girl_1p.x + 125, pos_speach_girl_1p.y + 70, 1000, 1000);
		DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[TALK_GIRL + comment_plus].c_str(), (int)comment_reply, place, DX9::Colors::Black);
		//フォント,セリフの入った変数,1度に表示する文字数,文字を表示する場所,色

		//2P画面
		RECT spot = RectWH(pos_speach_girl_2p.x + 65, pos_speach_girl_2p.y + 70, 1000, 1000);
		DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[TALK_GIRL + comment_plus].c_str(), (int)comment_reply, spot, DX9::Colors::Black);
	}

	//吹き出し(両方)
	DX9::SpriteBatch->DrawSimple(
		speach_synchro.Get(),
		pos_speach_synchro_1p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_synchro)
	);

	DX9::SpriteBatch->DrawSimple(
		speach_synchro.Get(),
		pos_speach_synchro_2p,
		Rect(0.0f, 0.0f, 1000.0f, 260.0f),
		DX9::Colors::RGBA(255, 255, 255, (int)alpha_speach_synchro)
	);

	if (flag_talk_synchro == true) {
		//1P画面
		RECT place = RectWH(pos_speach_synchro_1p.x + 125, pos_speach_synchro_1p.y + 70, 1000, 1000);
		DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[TALK_SYNCHRO].c_str(), (int)comment_reply, place, DX9::Colors::Black);
		//フォント,セリフの入った変数,1度に表示する文字数,文字を表示する場所,色

		//2P画面
		RECT spot = RectWH(pos_speach_synchro_2p.x + 125, pos_speach_synchro_2p.y + 70, 1000, 1000);
		DX9::SpriteBatch->DrawText(font.Get(), LoadLines::lines1[TALK_SYNCHRO].c_str(), (int)comment_reply, spot, DX9::Colors::Black);
	}

}

cppcoro::generator<int>BlackOut::Change() {
	co_yield 0;

	if (MainScene::phase == MainScene::Phase::SCENARIO) {
		// 待機
		while (time_stop < 2.5f) {
			time_stop += time_delta;
			co_yield 1;
		}
		time_stop = 0.0f;
		// テキスト表示ループ
		while (alpha_text < 255.0f) {
			alpha_text = std::min(alpha_text + num_alpha * time_delta, 255.0f);
			co_yield 2;
		}
		// ブラックアウト
		while (alpha_black > 0.0f)
		{
			alpha_text  = std::max(alpha_text - num_alpha * time_delta, 0.0f);
			alpha_black	= std::max(alpha_black - num_alpha * time_delta, 0.0f);
			co_yield 3;
		}

		// ここに会話のコルーチンを入れる
		while (time_stop < 2.0f) {
			time_stop += time_delta;
			co_yield 4;
		}
		time_stop = 0.0f;
		
		//吹き出し(女子)の表示
		while (alpha_speach_girl < COLOR_MAX) {
			alpha_speach_girl = std::min(alpha_speach_girl + NUM_ALPHA_SPEACH * time_delta, COLOR_MAX);
			co_yield 5;
		}

		//セリフ(女子)の表示
		while (time_stop < 3.0f) {
			flag_talk_girl = true;
			time_stop += time_delta;

			comment_reply += TALK_SPEED * time_delta;
			if (comment_reply > LoadLines::lines1[TALK_GIRL].length())
				comment_reply = LoadLines::lines1[TALK_GIRL].length();

			co_yield 8;
		}
		time_stop = 0.0f;
		comment_reply = 0.0f;
		alpha_speach_girl = 0.0f;
		flag_talk_girl = false;

		//吹き出し(男子)の表示
		while (alpha_speach_boy < COLOR_MAX) {
			alpha_speach_boy = std::min(alpha_speach_boy + NUM_ALPHA_SPEACH * time_delta, COLOR_MAX);
			co_yield 9;
		}

		//セリフ(男子)の表示
		while (time_stop < 3.0f) {
			flag_talk_boy = true;
			time_stop += time_delta;

			comment_reply += TALK_SPEED * time_delta;
			if (comment_reply > LoadLines::lines1[TALK_BOY].length())
				comment_reply = LoadLines::lines1[TALK_BOY].length();

			co_yield 10;
		}
		time_stop = 0.0f;
		comment_reply = 0.0f;
		alpha_speach_boy = 0.0f;
		flag_talk_boy = false;
		comment_plus = 2;

		//吹き出し(女子)の表示
		while (alpha_speach_girl < COLOR_MAX) {
			alpha_speach_girl = std::min(alpha_speach_girl + NUM_ALPHA_SPEACH * time_delta, COLOR_MAX);
			co_yield 5;
		}

		//セリフ(女子)の表示
		while (time_stop < 3.0f) {
			flag_talk_girl = true;
			time_stop += time_delta;

			comment_reply += TALK_SPEED * time_delta;
			if (comment_reply > LoadLines::lines1[TALK_GIRL + comment_plus].length())
				comment_reply = LoadLines::lines1[TALK_GIRL + comment_plus].length();

			co_yield 11;
		}
		time_stop = 0.0f;
		comment_reply = 0.0f;
		alpha_speach_girl = 0.0f;
		flag_talk_girl = false;

		//吹き出し(男子)の表示
		while (alpha_speach_boy < COLOR_MAX) {
			alpha_speach_boy = std::min(alpha_speach_boy + NUM_ALPHA_SPEACH * time_delta, COLOR_MAX);
			co_yield 12;
		}

		//セリフ(男子)の表示
		while (time_stop < 3.0f) {
			flag_talk_boy = true;
			time_stop += time_delta;

			comment_reply += TALK_SPEED * time_delta;
			if (comment_reply > LoadLines::lines1[TALK_BOY + comment_plus].length())
				comment_reply = LoadLines::lines1[TALK_BOY + comment_plus].length();

			co_yield 13;
		}
		time_stop = 0.0f;
		comment_reply = 0.0f;
		alpha_speach_boy = 0.0f;
		flag_talk_boy = false;

		//男女のリアクション
		while (time_stop < 0.5f)
		{
			time_stop += time_delta;
			if (MainScene::POS_BOY_A > -1.0f)
			{
				MainScene::POS_BOY_A -= 100.0f * time_delta;
				MainScene::POS_BOY_B -= 100.0f * time_delta;
				MainScene::POS_GIRL_A -= 100.0f * time_delta;
				MainScene::POS_GIRL_B -= 100.0f * time_delta;

			}
			else if (MainScene::POS_BOY_A < -1.0f)
			{
				MainScene::POS_BOY_A += 300.0f * time_delta;
				MainScene::POS_BOY_B += 300.0f * time_delta;
				MainScene::POS_GIRL_A += 300.0f * time_delta;
				MainScene::POS_GIRL_B += 300.0f * time_delta;

			}
			co_yield 14;
		}
		MainScene::POS_BOY_A = 0.0f;
		MainScene::POS_BOY_B = 0.0f;
		MainScene::POS_GIRL_A = 0.0f;
		MainScene::POS_GIRL_B = 0.0f;
		time_stop = 0.0f;


		//吹き出し(両方)の表示
		while (alpha_speach_synchro < COLOR_MAX) {
			alpha_speach_synchro = std::min(alpha_speach_synchro += NUM_ALPHA_SPEACH * time_delta, COLOR_MAX);
			co_yield 15;
		}
		
		//セリフ(両方)の表示
		while (time_stop < 3.0f) {
			flag_talk_synchro = true;
			time_stop += time_delta;

			comment_reply += TALK_SPEED * time_delta;
			if (comment_reply > LoadLines::lines1[TALK_SYNCHRO].length())
				comment_reply = LoadLines::lines1[TALK_SYNCHRO].length();

			co_yield 16;
		}
	}

	if (MainScene::phase == MainScene::Phase::SUCCEED) {
		while (alpha_black < 255.0f) {
			alpha_black = std::min(alpha_black + num_alpha * time_delta, 255.0f);
			co_yield 4;
		}
	}

	co_return;
}