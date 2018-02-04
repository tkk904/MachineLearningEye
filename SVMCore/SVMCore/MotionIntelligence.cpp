//============================================================================
//	The University of Tokyo
//	  Autonomous penetration detection system for bone cutting tool
//	
//----------------------------------------------------------------------------
//	Filename	: MotionIntelligence.cpp
//	Abstract	: class for classifying the motion state with SVM using openCV2.4.2 or later
//	Author		: osa@nml.t.u-tokyo.ac.jp
//----------------------------------------------------------------------------
//	2013 Aug.	: Start developing
//============================================================================


#include "stdafx.h"

#include "MotionIntelligence.h"
#include <fstream>
#include <windows.h>

using namespace std;

MotionIntelligence::MotionIntelligence(void)
{
	initSVMParam();
}

MotionIntelligence::~MotionIntelligence(void)
{
	cvReleaseMat(&class_W);
}


//-----------------------------------------
// Initialize parameters for SVM
//-----------------------------------------
void	MotionIntelligence::initSVMParam(){

	class_W = cvCreateMat(3,1,CV_32F);
	cvmSet(class_W, 0, 0, 1.0);
	cvmSet(class_W, 1, 0, 2.5);
	cvmSet(class_W, 2, 0, 2.5);

	params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 1e-9);
	params.class_weights = class_W;

	//params = CvSVMParams(CvSVM::C_SVC,   // Type of SVM; using N classes here
	//  CvSVM::LINEAR,  // Kernel type
	//  0.5,            // Param (degree) for poly kernel only
	//  1,            // Param (gamma) for poly/rbf kernel only
	//  1,            // Param (coef0) for poly/sigmoid kernel only
	//  1e-15,             // SVM optimization param C 0.005
	//  1,              // SVM optimization param nu (not used for N class SVM)
	//  0,              // SVM optimization param p (not used for N class SVM)
	//  class_W,           // class weights (or priors)
	//  /*
	//   * Optional weights, assigned to particular classes.
	//   * They are multiplied by C and thus affect the misclassification
	//   * penalty for different classes. The larger the weight, the larger
	//   * the penalty on misclassification of data from the corresponding
	//   * class.
	//   */
	//
	//  cvTermCriteria(CV_TERMCRIT_ITER, 10000, 1e-10));

	trainSVMDataLoadFlag	= FALSE;
	svmTrainFlag			= FALSE;
}

//-------------------------------------------------
// Load the data file and copy the data to matrices
//-------------------------------------------------
int		MotionIntelligence::prepareTrainDataCVMat(char *filename, Mat &train_state, Mat &train_label){

	float trainSVM_sample[SVM_MOTION_INPUT_SIZE];
	float trainSVM_label;

	Mat input_traindata;
	Mat input_labeldata;
	vector<float> vec_trainLabel;
	
	ifstream fin;

	//Open data file
	fin.open(filename, ios::in);

	if (fin.fail()){
		printf("Failed to open the data file.\n");
		return (FALSE);
	}

	int i;

	//Read data
	while(!fin.eof()){
		for(i=0 ; i < SVM_MOTION_INPUT_SIZE; i++) fin >> trainSVM_sample[i];
		fin >>  trainSVM_label;

		input_traindata = Mat(1,SVM_MOTION_INPUT_SIZE, CV_32FC1, trainSVM_sample);
		vec_trainLabel.push_back(trainSVM_label);

		train_state.push_back(input_traindata);
	}

	

	fin.close();

	Mat( vec_trainLabel ).copyTo( input_labeldata );
	train_label.push_back(input_labeldata);

	input_traindata.release();
	input_labeldata.release();

}

//-----------------------------------------
// Load the training data for SVM
//-----------------------------------------
int		MotionIntelligence::loadTrainDataSVM(char *filename){

	if(prepareTrainDataCVMat(filename, this->cv_SVMtrain_state, this->cv_SVMtrain_label)){
		trainSVMDataLoadFlag = TRUE;
		printf("train data file for SVM in MotionIntel was loaded.\n");
	}else{
		printf("Failed to load train data for MotionIntel. \n");
		return (FALSE);
	}

	return TRUE;
}


//-----------------------------------------
// Train SVM
//-----------------------------------------
int		MotionIntelligence::trainSVM(){
	if(this->svmTrainFlag == FALSE){
		ofstream fout;
		char filename[256] ="loadeddata_SVM_motion.txt";

		fout.open(filename);

		for(int i=0; i < cv_SVMtrain_state.rows; i++){
			for(int j =0; j< cv_SVMtrain_state.cols; j++) fout << cv_SVMtrain_state.at<float>(i,j) << "\t ";
			fout <<cv_SVMtrain_label.at<float>(i) <<endl; 
		}


		fout.close();
		
		this->svmClassifier.train(this->cv_SVMtrain_state, this->cv_SVMtrain_label, Mat(), Mat(), params);
		this->svmTrainFlag = TRUE;

		printf("SVM in MotionIntel was successfully trained.\n");

	}else{
		printf("SVM in MotionIntel has been already trained.\n");
	}


	return TRUE;
}


//-----------------------------------------
// Set the input vector of SVM
//-----------------------------------------
void	MotionIntelligence::setInputSVM(float x[]){
	Mat input_check_state;
	float state[SVM_MOTION_INPUT_SIZE];
	int i;

	for(i=0; i < SVM_MOTION_INPUT_SIZE; i++) state[i] = x[i];

	input_check_state	= Mat(1, SVM_MOTION_INPUT_SIZE,CV_32FC1,state);

	Mat( input_check_state ).copyTo( cv_SVMcheck_state );
}

//-----------------------------------------
// Predict the label of the input vector
//-----------------------------------------
int		MotionIntelligence::predictSVM(){

	float result = -1;
	ofstream fout;
	char filename[256] ="check_stateSVM_motion.txt";

	if(svmTrainFlag == TRUE){

		
		this->svmClassifier.predict(this->cv_SVMcheck_state, this->cv_SVMresult);
		result = this->cv_SVMresult.at<float>(0);

		fout.open(filename);

		for(int i=0; i < cv_SVMcheck_state.rows; i++){
			for(int j =0; j < SVM_MOTION_INPUT_SIZE; j++) fout << this->cv_SVMcheck_state.at<float>(i,j) << "\t " ;
			fout << "\t "<<result <<endl; 
		}


		fout.close();

	}else{
		printf("SVM has not been trained.\n");
		return (FALSE);
	}

	return (int) result;
}