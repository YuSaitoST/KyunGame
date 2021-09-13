#include "Classes/LoadLines.h"
#include <iostream>

std::basic_string<wchar_t> LoadLines::lines1[57];

void LoadLines::Initialize() {
}

void LoadLines::In_File() {
	FILE* fp;
	//assert(_wfopen_s(&fp, L"comment.csv", L"r,ccs=UTF-8") == 0);

	if (fopen_s(&fp, "comment.csv", "r,ccs=UTF-8") != 0) {
		assert(!"comment.csvが開けません");
		throw std::exception();
	}

	wchar_t read_line[256];

	//wchar_t flavor_text[256];
	//MultiByteToWideChar(CP_ACP, 0, read_line, -1, flavor_text, 255);  // 変換

	for (int i = 0; i < 57; ++i) {
		fgetws(read_line, 255, fp);
		lines1[i] = read_line;

		// 改行を設定する
		for (int j = 0; j < lines1[i].length(); ++j) {
			if (lines1[i].at(j) == L'@')
				lines1[i].at(j) = L'\n';
		}
	}
	fclose(fp);  // ファイルを閉じる
}