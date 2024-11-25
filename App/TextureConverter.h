#pragma once
#include <string>

#include "Externals/DirectXTex/d3dx12.h"
#include "Externals/DirectXTex/DirectXTex.h"

class TextureConverter
{
public: // -- 公開 メンバ関数 -- //

	/// <summary>
	/// テクスチャをWICからDDSに変換する
	/// </summary>
	/// <param name="filepath">ファイルパス</param>
	void ConverterTextureWICToDDS(const std::string& filepath);
	
	/// <summary>
	/// 使用法を表示する
	/// </summary>
	static void OutputUsage();


private: // -- 非公開 メンバ関数 -- //

	/// <summary>
	/// テクスチャファイル読み込み
	/// </summary>
	/// <param name="filepath"></param>
	void LoadWICTextureFromFile(const std::string& filepath);

	/// <summary>
	///	マルチバイト文字列をワイド文字列に変換
	/// </summary>
	/// <param name="mString">マルチバイト文字列</param>
	/// <returns>ワイド文字列</returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);

	/// <summary>
	/// フォルダパスとファイル名を分離する
	/// </summary>
	/// <param name="filepath">ファイルパス</param>
	void SeprateFilePath(const std::wstring& filepath);

	/// <summary>
	/// DDSテクスチャとしてファイル書き出し
	/// </summary>
	void aveDDSTextureToFile();

private: // -- 非公開 メンバ変数 -- //

	// 画像の情報
	DirectX::TexMetadata mMetaData;
	// 画像イメージのコンテナ
	DirectX::ScratchImage mScratchImage;

	// ディレクトリパス
	std::wstring mDirectryPath;
	// ファイル名
	std::wstring mFileName;
	// ファイル拡張子
	std::wstring mFileExt;
};

