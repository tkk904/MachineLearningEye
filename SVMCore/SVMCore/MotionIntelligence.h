#pragma once

//============================================================================
//	The University of Tokyo
//	  Autonomous penetration detection system for bone cutting tool
//	
//----------------------------------------------------------------------------
//	Filename	: MotionIntelligence.h
//	Abstract	: class for classifying the motion state with SVM using openCV2.4.2 or later
//	Author		: osa@nml.t.u-tokyo.ac.jp
//----------------------------------------------------------------------------
//	2013 Aug.	: Start developing
//============================================================================


#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/video/tracking.hpp"

using namespace cv;

class MotionIntelligence
{
public:
	MotionIntelligence(void);
	~MotionIntelligence(void);

	int		loadTrainDataSVM(char *filename);
	int		trainSVM();
	void	setInputSVM(float x[]);
	int		predictSVM();

	//flags for SVM
	int		trainSVMDataLoadFlag;
	int		svmTrainFlag;


private:

	//functions for SVM
	void	initSVMParam();
	int		prepareTrainDataCVMat(char *filename, Mat &train_state, Mat &train_label);
	

	//variables for SVM
	CvSVM svmClassifier;

	Mat cv_SVMtrain_state, cv_SVMtrain_label;
	Mat cv_SVMcheck_state, cv_SVMresult;

	CvSVMParams params;
	CvMat	*class_W;
	//int		inputSVM_data_size;
	int		classNum_SVM;


#define SVM_MOTION_INPUT_SIZE 1
};

