#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <Windows.h>

#include "App/TextureConverter.h"

// コマンドライン引数
enum Argument {
	kApplicationPath,	// アプリケーションのパス
	kFilePath,			// 渡されたファイルのパス

	NumArgument
};

int main(int argc,char* argv[]) {

	// コマンドライン引数なし
	if (argc < NumArgument) {
		// 使用方法の表示
		TextureConverter::OutputUsage();
		return 0;
	}

	// COM ライブラリの初期化
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	// テクスチャコンバータ
	TextureConverter converter;

	// テクスチャ変換
	converter.ConverterTextureWICToDDS(argv[kFilePath]);

	// COM ライブラリの終了
	CoUninitialize();

	return 0;
}