#pragma once

#pragma once

#include <tchar.h>
#include <string>

#ifdef SVMCORE_EXPORTS
#define SVMCORE_API __declspec(dllexport)
#else
#define SVMCORE_API __declspec(dllimport) 
#endif

/*! \mainpage MachineLearningEye
*
* \section intro_sec �T�v
*
*MachineLearningEye�̎�ȋ@�\�́A1��EXE�t�@�C���A3��DLL�t�@�C������\������Ă���B
*Doxygen���3��DLL�̃C���^�[�t�F�[�X�ɂ��ăh�L�������g������B
*3��DLL��C++�ŋL�q����Ă���B
*�܂��AMachineLearningEye�A1��EXE�t�@�C���́AC\#�ŋL�q����Ă���B
*����I�ɂ�C\#����C++��DLL���Ăяo���Ďg���Ă��邱�ƂɂȂ�B
*
*/

extern "C" {
	/**
	* @fn void Training()
	* @brief �T�|�[�g�x�N�^�[�}�V�����g���Ċђʏ󋵂��w�K����֐�
	*/
	SVMCORE_API void Training();

	/**
	* @fn int Predict(float a,float b, int datasize)
	* @brief �T�|�[�g�x�N�^�[�}�V�����g���Ċђʏ󋵂�\������֐�
	* @param[in] a �d���l
	* @param[in] b ��]��
	* @param[in] datasize �����̐�
	* @return int �\�����ʂ�Ԃ��@�ђʂƔ��f�����ꍇ�ɂ�1��Ԃ��B����ȊO��0��Ԃ�
	*/
	SVMCORE_API int Predict(float a,float b, int datasize);

	/**
	* @fn void ClearDefectFlg()
	* @brief �T�|�[�g�x�N�^�[�}�V�������Ŏg�p����Ă��锻�f�t���O���N���A����
	*/
	SVMCORE_API void ClearDefectFlg();
}

