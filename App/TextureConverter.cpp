#include "TextureConverter.h"
#include <Windows.h>
#include <iostream>

using namespace DirectX;

void TextureConverter::ConverterTextureWICToDDS(const std::string& filepath,
	int numOptions, char* options[])
{
	// �e�N�X�`���t�@�C����ǂݍ���
	LoadWICTextureFromFile(filepath);

	// DDS�`���ɕϊ����ď����o��
	SaveDDSTextureToFile(numOptions,options);

}

void TextureConverter::OutputUsage()
{
	std::cout << "�摜�t�@�C����WIC��������DDS�`���ɕϊ����܂�" << std::endl;
	std::cout << "TextureConverter [�h���C�u:][�p�X][�t�@�C����][-ml level]" << std::endl;
	std::cout << "[�h���C�u:][�p�X][�t�@�C����]: �ϊ�������WIC�`���̉摜�t�@�C�����w�肵�܂�" << std::endl;
	std::cout << "[-ml level]: �~�b�v���x�����w�肵�܂��B0���w�肷���1x1�܂ł̃t���~�b�v�}�b�v�`�F�[���𐶐����܂�" << std::endl;

}

void TextureConverter::LoadWICTextureFromFile(const std::string& filepath)
{
	// �t�@�C���p�X�����C�h������ɕϊ�����
	std::wstring wfilepath = ConvertMultiByteStringToWideString(filepath);

	// WCI�e�N�X�`���̃��[�h
	HRESULT hr = LoadFromWICFile(wfilepath.c_str(), WIC_FLAGS_NONE, &mMetaData, mScratchImage);
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

void TextureConverter::SaveDDSTextureToFile(int numOptions, char* options[])
{
	HRESULT hr;
	ScratchImage mipChain;
	size_t mipLevel = 0;

	// �~�b�v�}�b�v���x���w�������
	for (int32_t i = 0; i < numOptions; i++) {
		if (std::string(options[i])== "-ml") {
			// �~�b�v���x���w��
			mipLevel = std::atoi(options[i + 1]);
			break;
		}
	}

	// �~�b�v�}�b�v ����
	hr = GenerateMipMaps(
		mScratchImage.GetImages(), mScratchImage.GetImageCount(), mScratchImage.GetMetadata(),
		TEX_FILTER_DEFAULT, mipLevel, mipChain);
	if (SUCCEEDED(hr)) {
		// �C���[�W�ƃ��^�f�[�^���~�b�v�}�b�v�łŒu��������
		mScratchImage = std::move(mipChain);
		mMetaData = mScratchImage.GetMetadata();
	}

	// ���k�`���ɕύX
	ScratchImage converted;
	hr = Compress(
		mScratchImage.GetImages(), mScratchImage.GetImageCount(), mScratchImage.GetMetadata(),
		DXGI_FORMAT_BC7_UNORM_SRGB,TEX_COMPRESS_BC7_QUICK|TEX_COMPRESS_SRGB_OUT| TEX_COMPRESS_PARALLEL,
		1.0f,converted);
	if (SUCCEEDED(hr)) {
		mScratchImage = std::move(converted);
		mMetaData = mScratchImage.GetMetadata();
	}


	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	mMetaData.format = MakeSRGB(mMetaData.format);

	// �o�̓t�@�C������ݒ肷��
	std::wstring filepath = mDirectryPath + mFileName + L".dds";
	
	// DDS�t�@�C�������o��
	hr = SaveToDDSFile(mScratchImage.GetImages(), mScratchImage.GetImageCount(), mMetaData,
		DDS_FLAGS_NONE, filepath.c_str());
	assert(SUCCEEDED(hr));

}
