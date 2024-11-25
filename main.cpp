#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <Windows.h>

#include "App/TextureConverter.h"

// �R�}���h���C������
enum Argument {
	kApplicationPath,	// �A�v���P�[�V�����̃p�X
	kFilePath,			// �n���ꂽ�t�@�C���̃p�X

	NumArgument
};

int main(int argc,char* argv[]) {

	// �R�}���h���C�������Ȃ�
	if (argc < NumArgument) {
		// �g�p���@�̕\��
		TextureConverter::OutputUsage();
		return 0;
	}

	// COM ���C�u�����̏�����
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	// �e�N�X�`���R���o�[�^
	TextureConverter converter;

	// �e�N�X�`���ϊ�
	converter.ConverterTextureWICToDDS(argv[kFilePath]);

	// COM ���C�u�����̏I��
	CoUninitialize();

	return 0;
}