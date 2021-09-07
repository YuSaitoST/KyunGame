#pragma onse

#include"Base/pch.h"
#include"Base/dxtk.h"
#include"cppcoro/generator.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class Attack {
public:
	void Initialize();
	void LoadAssets();
	bool Up_Attack(const float deltaTime);
	void Render();


	static int alpha_boy;
	static int alpha_girl;

private:
	//ÉRÉãÅ[É`Éì
	cppcoro::generator<int> Action();
	cppcoro::generator<int> co_action;
	cppcoro::detail::generator_iterator<int> co_action_it;

	float time_delta;

	const int color_gray = 155;

};