#include "TextureConverter.h"
#include <Windows.h>
#include <iostream>

using namespace DirectX;

void TextureConverter::ConverterTextureWICToDDS(const std::string& filepath,
	int numOptions, char* options[])
{
	// テクスチャファイルを読み込む
	LoadWICTextureFromFile(filepath);

	// DDS形式に変換して書き出す
	SaveDDSTextureToFile(numOptions,options);

}

void TextureConverter::OutputUsage()
{
	std::cout << "画像ファイルをWIC方式からDDS形式に変換します" << std::endl;
	std::cout << "TextureConverter [ドライブ:][パス][ファイル名][-ml level]" << std::endl;
	std::cout << "[ドライブ:][パス][ファイル名]: 変換したいWIC形式の画像ファイルを指定します" << std::endl;
	std::cout << "[-ml level]: ミップレベルを指定します。0を指定すると1x1までのフルミップマップチェーンを生成します" << std::endl;

}

void TextureConverter::LoadWICTextureFromFile(const std::string& filepath)
{
	// ファイルパスをワイド文字列に変換する
	std::wstring wfilepath = ConvertMultiByteStringToWideString(filepath);

	// WCIテクスチャのロード
	HRESULT hr = LoadFromWICFile(wfilepath.c_str(), WIC_FLAGS_NONE, &mMetaData, mScratchImage);
	assert(SUCCEEDED(hr));

	// フォルダパスとファイル名を分離する
	SeprateFilePath(wfilepath);

}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString)
{
	if (mString.empty())
	{
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(
		CP_UTF8,
		0, 
		reinterpret_cast<const char*>(&mString[0]),
		static_cast<int>(mString.size()),
		NULL,
		0
	);

	if (sizeNeeded == 0)
	{
		return std::wstring();
	}

	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8,
		0, 
		reinterpret_cast<const char*>(&mString[0]),
		static_cast<int>(mString.size()),
		&result[0], 
		sizeNeeded
	);
	return result;

}

void TextureConverter::SeprateFilePath(const std::wstring& filepath)
{
	size_t pos1;
	std::wstring exceptExt;

	// 区切り文字 '.' が出てくる一番最後の部分を検索
	pos1 = filepath.rfind('.');
	// 検索がヒットしたら
	if (pos1 != std::wstring::npos) {
		// 区切り文字の後ろをファイル拡張子として保存
		mFileExt = filepath.substr(pos1 + 1, filepath.size() - pos1 - 1);
		// 区切り文字の前までを抜き出す
		exceptExt = filepath.substr(0, pos1);
	}
	else {
		mFileExt = L"";
		exceptExt = filepath;
	}
	
	// 区切り文字 '\\' がでてくる一番最後の部分を検索
	pos1 = exceptExt.rfind('\\');
	if (pos1 != std::wstring::npos) {
		// 区切り文字の前までをディレクトリパスとして保存
		mDirectryPath = exceptExt.substr(0, pos1 + 1);
		// 区切り文字の後ろをファイル名として保存
		mFileName = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	// 区切り文字 '/' がでてくる一番最後の部分を検索
	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos) {
		// 区切り文字の前までをディレクトリパスとして保存
		mDirectryPath = exceptExt.substr(0, pos1 + 1);
		// 区切り文字の後ろをファイル名として保存
		mFileName = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	// 区切り文字がなかった場合、ファイル名のみとして扱う
	mDirectryPath = L"";
	mFileName = exceptExt;
}

void TextureConverter::SaveDDSTextureToFile(int numOptions, char* options[])
{
	HRESULT hr;
	ScratchImage mipChain;
	size_t mipLevel = 0;

	// ミップマップレベル指定を検索
	for (int32_t i = 0; i < numOptions; i++) {
		if (std::string(options[i])== "-ml") {
			// ミップレベル指定
			mipLevel = std::atoi(options[i + 1]);
			break;
		}
	}

	// ミップマップ 生成
	hr = GenerateMipMaps(
		mScratchImage.GetImages(), mScratchImage.GetImageCount(), mScratchImage.GetMetadata(),
		TEX_FILTER_DEFAULT, mipLevel, mipChain);
	if (SUCCEEDED(hr)) {
		// イメージとメタデータをミップマップ版で置き換える
		mScratchImage = std::move(mipChain);
		mMetaData = mScratchImage.GetMetadata();
	}

	// 圧縮形式に変更
	ScratchImage converted;
	hr = Compress(
		mScratchImage.GetImages(), mScratchImage.GetImageCount(), mScratchImage.GetMetadata(),
		DXGI_FORMAT_BC7_UNORM_SRGB,TEX_COMPRESS_BC7_QUICK|TEX_COMPRESS_SRGB_OUT| TEX_COMPRESS_PARALLEL,
		1.0f,converted);
	if (SUCCEEDED(hr)) {
		mScratchImage = std::move(converted);
		mMetaData = mScratchImage.GetMetadata();
	}


	// 読み込んだディフューズテクスチャをSRGBとして扱う
	mMetaData.format = MakeSRGB(mMetaData.format);

	// 出力ファイル名を設定する
	std::wstring filepath = mDirectryPath + mFileName + L".dds";
	
	// DDSファイル書き出し
	hr = SaveToDDSFile(mScratchImage.GetImages(), mScratchImage.GetImageCount(), mMetaData,
		DDS_FLAGS_NONE, filepath.c_str());
	assert(SUCCEEDED(hr));

}
