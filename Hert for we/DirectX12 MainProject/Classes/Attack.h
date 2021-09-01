#pragma onse

#include"Base/pch.h"
#include"Base/dxtk.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class Attack {
public:
	void Initialize();
	void LoadAssets();
	void Update();
	void Render();
	bool Up_Select();

private:
};