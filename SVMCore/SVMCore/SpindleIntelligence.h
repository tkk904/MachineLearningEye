#pragma once

//============================================================================
//	The University of Tokyo
//	 Autonomous penetration detection system for bone cutting tool
//	
//----------------------------------------------------------------------------
//	Filename	: SpindleIntelligence.h
//	Abstract	: class for classifying the cutting state with SVM using openCV2.4.2 or later
//	Author		: osa@nml.t.u-tokyo.ac.jp
//----------------------------------------------------------------------------
//	2013 March	: Start developing
//============================================================================


#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "windows.h"

#include "LIB_RLS_CV.h"

using namespace cv;

class SpindleIntelligence
{
public:
	SpindleIntelligence();
	~SpindleIntelligence(void);

	//functions for SVM
	int		loadTrainDataSVM(char *prop_filename);
	int		trainSVM();
	void	setInputSVM(float x[]);
	int		predictSVM();

	//functions for RLS
	void	initRLS();
	double	updateEstimationRLS(double u[1], double d);

	//flags for SVM
	int		trainSVMDataLoadFlag;
	int		svmTrainFlag;

	//variables for RLS
	LIB_RLS_CV *rls;

private:

	//functions for SVM
	void	initSVMParam();
	int		prepareTrainDataCVMat(char *prop_filename, Mat &train_state, Mat &train_label);
	

	//variables for SVM
	CvSVM svmClassifier;

	Mat cv_SVMtrain_state, cv_SVMtrain_label;
	Mat cv_SVMcheck_state, cv_SVMresult;

	CvSVMParams params;
	CvMat	*class_W;
	//int		inputSVM_data_size;
	int		classNum_SVM;

	//variables for Recursive Least Square
	double w_ini[1];

#define SVMINPUT_SIZE 3

};

