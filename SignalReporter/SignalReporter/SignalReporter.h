#pragma once

#include <tchar.h>
#include <string>


#ifdef SIGNALREPORTER_EXPORTS
#define SIGNALREPORTER_API __declspec(dllexport)
#else
#define SIGNALREPORTER_API __declspec(dllimport) 
#endif

//using tstring = std::basic_string<TCHAR>;

extern "C" {
	/**
	* @fn bool Start(char* PortName)
	* @brief �O���ڑ��@��i�o�͑��j�̎w�肳�ꂽ�|�[�g���̐ڑ����m������B
	* @param[in] PortName �|�[�g��
	* @return bool �I�[�v�����ʂ�Ԃ��@�I�[�v���������ɂ�true�B����ȊO��false��Ԃ�
	*/
	SIGNALREPORTER_API bool Start(char* PortName);

	/**
	* @fn void StopNotify()
	* @brief  �O���ڑ��@��i�o�͑��j�N���[�Y�֐�
	* @return �Ȃ� 
	*/
	SIGNALREPORTER_API void StopNotify();

	/**
	* @fn void Notify(int time)
	* @brief  �O���ڑ��@��i�o�͑��j�B�ʒm���s���w�����鏈���B��̓I�ɂ́Atime�Ŏw�肳�ꂽ���ԁALED��_�����鏈��
	* @param[in] time �ʒm���s�̎w�����ԁB�w�肳�ꂽ���ԁA�ʒm���s���B
	* @return �Ȃ�
	*/
	SIGNALREPORTER_API void Notify(int time);
}

