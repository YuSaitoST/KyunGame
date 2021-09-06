#pragma onse

#include"Base/pch.h"
#include"Base/dxtk.h"
#include"cppcoro/generator.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class Animation {
public:
	void Initialize();
	void LoadAssets();
	void Update(const float deltaTime);
	void Render();
	
	DX9::SPRITE white;
	SimpleMath::Vector3 pos_white;
	int alpha_white;
	float time_delta;
	float time_stop;

	//ÉRÉãÅ[É`Éì
	cppcoro::generator<int> Change();
	cppcoro::generator<int> co_change;
	cppcoro::detail::generator_iterator<int> co_cahnge_it;

private:
};