#pragma once

#include"Base/pch.h"
#include"Base/dxtk.h"
#include"cppcoro/generator.h"
#include <random>

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class LoadLines {
public:
	void Initialize();
	void In_File();
	static std::basic_string<wchar_t> lines1[57];  // �䎌������

private:
};