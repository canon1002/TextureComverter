#pragma once
#include <string>

#include "Externals/DirectXTex/d3dx12.h"
#include "Externals/DirectXTex/DirectXTex.h"

class TextureConverter
{
public: // -- ���J �����o�֐� -- //

	/// <summary>
	/// �e�N�X�`����WIC����DDS�ɕϊ�����
	/// </summary>
	/// <param name="filepath">�t�@�C���p�X</param>
	void ConverterTextureWICToDDS(const std::string& filepath);
	
	/// <summary>
	/// �g�p�@��\������
	/// </summary>
	static void OutputUsage();


private: // -- ����J �����o�֐� -- //

	/// <summary>
	/// �e�N�X�`���t�@�C���ǂݍ���
	/// </summary>
	/// <param name="filepath"></param>
	void LoadWICTextureFromFile(const std::string& filepath);

	/// <summary>
	///	�}���`�o�C�g����������C�h������ɕϊ�
	/// </summary>
	/// <param name="mString">�}���`�o�C�g������</param>
	/// <returns>���C�h������</returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);

	/// <summary>
	/// �t�H���_�p�X�ƃt�@�C�����𕪗�����
	/// </summary>
	/// <param name="filepath">�t�@�C���p�X</param>
	void SeprateFilePath(const std::wstring& filepath);

	/// <summary>
	/// DDS�e�N�X�`���Ƃ��ăt�@�C�������o��
	/// </summary>
	void aveDDSTextureToFile();

private: // -- ����J �����o�ϐ� -- //

	// �摜�̏��
	DirectX::TexMetadata mMetaData;
	// �摜�C���[�W�̃R���e�i
	DirectX::ScratchImage mScratchImage;

	// �f�B���N�g���p�X
	std::wstring mDirectryPath;
	// �t�@�C����
	std::wstring mFileName;
	// �t�@�C���g���q
	std::wstring mFileExt;
};

