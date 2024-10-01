#include "TextureConverter.h"
#include <Windows.h>

void TextureConverter::ConverterTextureWICToDDS(const std::string& filepath)
{
	// テクスチャファイルを読み込む
	LoadWICTextureFromFile(filepath);

	// DDS形式に変換して書き出す
	aveDDSTextureToFile();

}

void TextureConverter::LoadWICTextureFromFile(const std::string& filepath)
{
	// ファイルパスをワイド文字列に変換する
	std::wstring wfilepath = ConvertMultiByteStringToWideString(filepath);

	// WCIテクスチャのロード
	HRESULT hr = DirectX::LoadFromWICFile(wfilepath.c_str(), DirectX::WIC_FLAGS_NONE, &mMetaData, mScratchImage);
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

void TextureConverter::aveDDSTextureToFile()
{
	// 読み込んだディフューズテクスチャをSRGBとして扱う
	mMetaData.format = DirectX::MakeSRGB(mMetaData.format);

	HRESULT hr;

	// 出力ファイル名を設定する
	std::wstring filepath = mDirectryPath + mFileName + L".dds";

	// DDSファイル書き出し
	hr = DirectX::SaveToDDSFile(mScratchImage.GetImages(), mScratchImage.GetImageCount(), mMetaData,
		DirectX::DDS_FLAGS_NONE, filepath.c_str());
	assert(SUCCEEDED(hr));

}
