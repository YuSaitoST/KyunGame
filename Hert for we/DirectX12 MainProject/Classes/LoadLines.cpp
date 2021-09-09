#include "Classes/LoadLines.h"
#include <iostream>

std::basic_string<wchar_t> LoadLines::lines1[6];

void LoadLines::Initialize() {
	std::random_device seed;
	random_engine = std::mt19937(seed());
	random_dist = std::uniform_int_distribution<>(0, 1);
	// ランダム値を入れる時は、  random_dist(random_engine)  を代入;
}

void LoadLines::In_File() {
	FILE* fp;
	assert(_wfopen_s(&fp, L"comment.csv", L"r,ccs=UTF-8") == 0);

	wchar_t read_line[256];
	fgetws(read_line, 255, fp);

	//wchar_t flavor_text[256];
	//MultiByteToWideChar(CP_ACP, 0, read_line, -1, flavor_text, 255);  // 変換

	lines1[0] = read_line;

	fclose(fp);  // ファイルを閉じる
}