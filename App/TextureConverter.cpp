#include "TextureConverter.h"
#include <Windows.h>

void TextureConverter::ConverterTextureWICToDDS(const std::string& filepath)
{
	// �e�N�X�`���t�@�C����ǂݍ���
	LoadWICTextureFromFile(filepath);

	// DDS�`���ɕϊ����ď����o��
	aveDDSTextureToFile();

}

void TextureConverter::LoadWICTextureFromFile(const std::string& filepath)
{
	// �t�@�C���p�X�����C�h������ɕϊ�����
	std::wstring wfilepath = ConvertMultiByteStringToWideString(filepath);

	// WCI�e�N�X�`���̃��[�h
	HRESULT hr = DirectX::LoadFromWICFile(wfilepath.c_str(), DirectX::WIC_FLAGS_NONE, &mMetaData, mScratchImage);
	assert(SUCCEEDED(hr));

	// �t�H���_�p�X�ƃt�@�C�����𕪗�����
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

	// ��؂蕶�� '.' ���o�Ă����ԍŌ�̕���������
	pos1 = filepath.rfind('.');
	// �������q�b�g������
	if (pos1 != std::wstring::npos) {
		// ��؂蕶���̌����t�@�C���g���q�Ƃ��ĕۑ�
		mFileExt = filepath.substr(pos1 + 1, filepath.size() - pos1 - 1);
		// ��؂蕶���̑O�܂ł𔲂��o��
		exceptExt = filepath.substr(0, pos1);
	}
	else {
		mFileExt = L"";
		exceptExt = filepath;
	}
	
	// ��؂蕶�� '\\' ���łĂ����ԍŌ�̕���������
	pos1 = exceptExt.rfind('\\');
	if (pos1 != std::wstring::npos) {
		// ��؂蕶���̑O�܂ł��f�B���N�g���p�X�Ƃ��ĕۑ�
		mDirectryPath = exceptExt.substr(0, pos1 + 1);
		// ��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		mFileName = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	// ��؂蕶�� '/' ���łĂ����ԍŌ�̕���������
	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos) {
		// ��؂蕶���̑O�܂ł��f�B���N�g���p�X�Ƃ��ĕۑ�
		mDirectryPath = exceptExt.substr(0, pos1 + 1);
		// ��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		mFileName = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	// ��؂蕶�����Ȃ������ꍇ�A�t�@�C�����݂̂Ƃ��Ĉ���
	mDirectryPath = L"";
	mFileName = exceptExt;
}

void TextureConverter::aveDDSTextureToFile()
{
	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	mMetaData.format = DirectX::MakeSRGB(mMetaData.format);

	HRESULT hr;

	// �o�̓t�@�C������ݒ肷��
	std::wstring filepath = mDirectryPath + mFileName + L".dds";

	// DDS�t�@�C�������o��
	hr = DirectX::SaveToDDSFile(mScratchImage.GetImages(), mScratchImage.GetImageCount(), mMetaData,
		DirectX::DDS_FLAGS_NONE, filepath.c_str());
	assert(SUCCEEDED(hr));

}
