#include "Classes/LoadLines.h"
#include <iostream>

void LoadLines::Initialize() {
	std::random_device seed;
	random_engine = std::mt19937(seed());
	random_dist = std::uniform_int_distribution<>(0, 1);
	// ランダム値を入れる時は、  random_dist(random_engine)  を代入;
}

void LoadLines::In_File() {
	FILE* fp;
	assert(_wfopen_s(&fp, L"comment.csv", L"r") == 0);

	char dummy_line[256];
	fgets(dummy_line, 255, fp);

	char multibyte_string[256];
	sscanf_s(dummy_line, "%s", multibyte_string, 255);

	MultiByteToWideChar(CP_ACP, 0, multibyte_string, -1, flavor_text, 255);  // 変換

	fclose(fp);  // ファイルを閉じる
}