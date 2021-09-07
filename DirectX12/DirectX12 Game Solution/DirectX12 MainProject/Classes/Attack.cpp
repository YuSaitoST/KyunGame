#include"Classes/Attack.h"
#include <iostream>
#include<MainScene.h>

int Attack::alpha_boy;
int Attack::alpha_girl;

void Attack::Initialize() {
	count_chnage = 0;
	time_delta = 0.0f;
	alpha_boy = 255;
	alpha_girl = 155;
	time_stop = 0.0f;
}

void Attack::LoadAssets() {

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

// 反転用の変数を渡せるようにする（その変数はMainScene.hにて）
void Attack::Render(int index, float pos_x) {

}

cppcoro::generator<int>Attack::Action() {
	co_yield 0;
	if (MainScene::num_player == 0) {
		while (alpha_girl > 0) {
			alpha_girl -= num_alpha * time_delta;
			co_yield 1;
		}
		alpha_girl = 0;

		while (time_stop < 1.0f)
		{
			time_stop += time_delta;
			co_yield 2;
		}
		time_stop = 1.0f;

		while (alpha_girl < 255)
		{
			MainScene::emotion[0] = MainScene::EMOTION::PROPOSAL;
			alpha_girl += num_alpha * time_delta;
			co_yield 3;
		}
		alpha_girl = 255;







	}
	co_return;
}