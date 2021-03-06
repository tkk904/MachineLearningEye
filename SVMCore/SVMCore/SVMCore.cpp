// SVMInteligence.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "SVMCore.h"
#include "SpindleIntelligence.h"
#include "MotionIntelligence.h"

int		MainLoopFlag;		//TRUE: thread for main loop is working
int		UILoopFlag;			//TRUE: thread to receive the user input is working
int		CheckLoopFlag;		//TRUE: Thread for check loop is working
int		SensorLoopFlag;
int		MotionCheckLoopFlag;
int		CheckFlag;			//TRUE: algorithm to check penetration is working

int		MonitorFlag;		//TRUE: log data is being recorded
int		DetectFlag = FALSE;	//TRUE: penetration is detected
int		ResetFlag;			//TRUE: restart the rotation of the cutting tool and check loop

int		StartFlag;
int		StopFlag;
int		DisplayFlag;

SpindleIntelligence spinIntel;
MotionIntelligence motionIntel;

int current_max = 0;
int		rotation_SettingSpeed = 0;
int		rotation_ActualSpeed = 0;
int		current = 0;
vector<int> current_vec;
vector<int>::iterator it;

double	k = 0;

float	accel[6] = { 0 };

float	motion_input[SVM_MOTION_INPUT_SIZE] = { 0 };
int		motion_state = -1;

float state[SVMINPUT_SIZE] = { 0 };
int		result = -1;

int		cut_cnt = 0;
int		stop_cnt = 0;

int		CuttingFlag;
int current_pre = 0;

float motion_pre[3] = { 0 };
float check_motion_state[3] = { 0 };


void Training()
{
	spinIntel.loadTrainDataSVM("cutstate.txt"); // for ball bar rotation speed 1000
	spinIntel.trainSVM();
}

void ClearDefectFlg() 
{
	DetectFlag = FALSE;
}

//int Predict(float* data, int datasize)
int Predict(float a, float b, int datasize)
{
	double u[1] = { 0 };
	double d = 0;
	/*
	current = data[0]/*Spindle_RequestRealCurrent()*/;
	//rotation_ActualSpeed = data[1]/*spindle.Spindle_RequestRealSpeed()*/;
	
	current = a/*Spindle_RequestRealCurrent()*/;
	rotation_ActualSpeed = b/*spindle.Spindle_RequestRealSpeed()*/;

	state[0] = current;
	state[1] = rotation_ActualSpeed;

	u[0] = rotation_ActualSpeed;
	d = current;

	if (u[0] > 20) {
		k = (double)spinIntel.updateEstimationRLS(u, d);
	}else {
		k = 0;
	}

	state[2] = (float)k;

	it = current_vec.begin();
	current_vec.insert(it, (int)state[0]);
	if (current_vec.size() > 4) {
		current_vec.pop_back();
	}

	if (current_max < state[0]) {
		current_max = (int)state[0];
	}

	//Classifying motion state
	for (int i = 0; i < SVM_MOTION_INPUT_SIZE; i++) {
		motion_input[0] = accel[1];
		check_motion_state[i] = motion_input[i] - motion_pre[i];
	}
	motionIntel.setInputSVM(check_motion_state);
	motion_state = motionIntel.predictSVM();

	for (int i = 0; i < SVM_MOTION_INPUT_SIZE; i++) {
		motion_pre[i] = motion_input[i];
	}

	spinIntel.setInputSVM(state);
	result = spinIntel.predictSVM();

	switch (result) {
	case 0:
		if (cut_cnt < 10) {
			printf("no cut\n");
			stop_cnt++;
			cut_cnt = 0;
			current_max = 0;
		} else {
			stop_cnt++;

			if (stop_cnt > 1) {
				printf("released \n");
				DetectFlag = TRUE;

				CheckFlag = FALSE;
				//MonitorFlag = FALSE;
				cut_cnt = 0;
				stop_cnt = 0;
			}
		}
		Sleep(50);
		break;

	case 1:
		printf("cutting \n");
		cut_cnt++;
		stop_cnt = 0;

		//minor change
		if (current_vec.size() == 4 && cut_cnt > 10) {
			if (current_vec[3] > 150 || current_vec[2] > 150 || current_vec[1] > 150) {
				if (current < 139) {
					printf("Resistance decreased! \n");
					DetectFlag = TRUE;
					cut_cnt = 0;
				}
			}
		}

		// FOR ROBOT
		//if(current_vec.size() == 4 && cut_cnt > 10){
		//	if(current_max * 0.88 > state[0]){
		//		printf("Resistance decreased! \n");
		//		DetectFlag = TRUE;
		//		CheckFlag = FALSE;
		//		cut_cnt = 0;

		//	}
		//}

		Sleep(50);
		break;
	case 2:
		printf("cutting. Press too much! \n");
		cut_cnt++;
		stop_cnt = 0;
		Beep(750, 50);
		break;
	default:
		printf("unknown state\n");
		Sleep(50);
		break;
	}

	switch (motion_state) {
	case 0:
		printf("Steady motion\n");
		break;
	case 1:
		printf("Retract \n");
		cut_cnt = 0;
		break;

	case 2:
		printf("Push! \n");
		/*if(cut_cnt > 3 && current < current_pre + 1){*/
		if (cut_cnt > 10) {
			printf("Deflect!");
			DetectFlag = TRUE;
			CheckFlag = FALSE;
			cut_cnt = 0;
		}
		break;
	}

	return DetectFlag ? 1 : 0;
} 
