//============================================================================
//	The University of Tokyo
//	 Autonomous penetration detection system for bone cutting tool
//	
//----------------------------------------------------------------------------
//	Filename	: SpindleIntelligence.cpp
//	Abstract	: class for classifying the cutting state with SVM using openCV2.4.2 or later
//	Author		: osa@nml.t.u-tokyo.ac.jp
//----------------------------------------------------------------------------
//	2013 March	: Start developing
//============================================================================

#include "StdAfx.h"
#include "SpindleIntelligence.h"
#include <fstream>

using namespace std;

SpindleIntelligence::SpindleIntelligence()
{
	this->classNum_SVM = 2; // for ball bar
	//this->classNum_SVM = 2; // for bone saw
	initRLS();
	initSVMParam();	
}


SpindleIntelligence::~SpindleIntelligence(void)
{
	cvReleaseMat(&class_W);
	delete rls;
}

void	SpindleIntelligence::initSVMParam(){
	//Prameters for SVM
	switch(this->classNum_SVM){
	case 3:
		class_W = cvCreateMat(3,1,CV_32F);
		cvmSet(class_W, 0, 0, 1);
		cvmSet(class_W, 1, 0, 1);
		cvmSet(class_W, 2, 0, 1);

		break;
	case 2:
		class_W = cvCreateMat(2,1,CV_32F);
		cvmSet(class_W, 0, 0, 1);
		cvmSet(class_W, 1, 0, 1.5);

		break;
	default:
		class_W = cvCreateMat(2,1,CV_32F);
		cvmSet(class_W, 0, 0, 1);
		cvmSet(class_W, 1, 0, 2);

		break;
	}
	
	params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 1e-9);
	params.class_weights = class_W;


	//params = CvSVMParams(CvSVM::C_SVC,   // Type of SVM; using N classes here
	//      CvSVM::LINEAR,  // Kernel type
	//      0.5,            // Param (degree) for poly kernel only
	//      1,            // Param (gamma) for poly/rbf kernel only
	//      1,            // Param (coef0) for poly/sigmoid kernel only
	//      0.0005,             // SVM optimization param C 0.005
	//      1,              // SVM optimization param nu (not used for N class SVM)
	//      0,              // SVM optimization param p (not used for N class SVM)
	//      class_W,           // class weights (or priors)
	//      /*
	//       * Optional weights, assigned to particular classes.
	//       * They are multiplied by C and thus affect the misclassification
	//       * penalty for different classes. The larger the weight, the larger
	//       * the penalty on misclassification of data from the corresponding
	//       * class.
	//       */
	//
	//      cvTermCriteria(CV_TERMCRIT_ITER, 1000, 0.01));

	//inputSVM_data_size		= 2;
	trainSVMDataLoadFlag	= FALSE;
	svmTrainFlag			= FALSE;
}


void	SpindleIntelligence::initRLS(){
	
	w_ini[0]= 0.15;
	this->rls = new LIB_RLS_CV(1,w_ini,0.8);

}


//-----------------------------------------
// Load the training data for SVM
//-----------------------------------------
int		SpindleIntelligence::loadTrainDataSVM(char *prop_filename){
	
	if(prepareTrainDataCVMat(prop_filename, this->cv_SVMtrain_state, this->cv_SVMtrain_label)){
		trainSVMDataLoadFlag = TRUE;
		printf("train data file for SVM was loaded.\n");
	}else{
		printf("Failed to load train data");
		return (FALSE);
	}

	return (TRUE);
}


//-----------------------------------------
// Train SVM using the loaded data
//-----------------------------------------
int		SpindleIntelligence::trainSVM(){
	if(this->svmTrainFlag == FALSE){
		ofstream fout;
		char filename[256] ="../output/loadeddata_SVM.txt";

		fout.open(filename);

		for(int i=0; i < cv_SVMtrain_state.rows; i++){
			for(int j =0; j< cv_SVMtrain_state.cols; j++) fout << cv_SVMtrain_state.at<float>(i,j) << "\t ";
			fout <<cv_SVMtrain_label.at<float>(i) <<endl; 
		}


		fout.close();
		
		this->svmClassifier.train(this->cv_SVMtrain_state, this->cv_SVMtrain_label, Mat(), Mat(), params);
		this->svmTrainFlag = TRUE;

		printf("SVM was successfully trained.\n");

	}else{
		   printf("SVM has been already trained.\n");
	}


	return (TRUE);
}


//-----------------------------------------
// Set input of SVM
//-----------------------------------------
void	SpindleIntelligence::setInputSVM(float x[]){
	Mat input_check_state;
	float state[SVMINPUT_SIZE];
	int i;

	for(i=0; i < SVMINPUT_SIZE; i++) state[i] = x[i];

	input_check_state	= Mat(1, SVMINPUT_SIZE,CV_32FC1,state);
	Mat( input_check_state ).copyTo( cv_SVMcheck_state );

}


//-----------------------------------------
// Predict the label of the input 
//-----------------------------------------
int		SpindleIntelligence::predictSVM(){

	float result = -1;
	ofstream fout;
	char filename[256] ="../output/check_stateSVM.txt";

	if(svmTrainFlag == TRUE){

		
		this->svmClassifier.predict(this->cv_SVMcheck_state, this->cv_SVMresult);
		result = this->cv_SVMresult.at<float>(0);

		//fout.open(filename);

		//for(int i=0; i < cv_SVMcheck_state.rows; i++){
		//	for(int j =0; j < SVMINPUT_SIZE; j++) fout << this->cv_SVMcheck_state.at<float>(i,j) << "\t " ;
		//	fout << "\t "<<result <<endl; 
		//}


		//fout.close();

	}else{
		printf("SVM has not been trained.\n");
		return (FALSE);
	}

	return (int) result;
}


//---------------------------------------------------
// Load the data file and copy the data to matrices
//---------------------------------------------------
int		SpindleIntelligence::prepareTrainDataCVMat(char *prop_filename, Mat &train_state, Mat &train_label){
	float trainSVM_sample[SVMINPUT_SIZE];
	float trainSVM_label;

	Mat input_traindata;
	Mat input_labeldata;
	vector<float> vec_trainLabel;
	
	ifstream fin;

	//reading data
	fin.open(prop_filename, ios::in);

	if (fin.fail()){
		printf("Failed to open the data file.\n");
		return (FALSE);
	}

	int i;

	while(!fin.eof()){
		for(i=0 ; i < SVMINPUT_SIZE; i++) fin >> trainSVM_sample[i];
		fin >>  trainSVM_label;

		input_traindata = Mat(1,SVMINPUT_SIZE, CV_32FC1, trainSVM_sample);
		vec_trainLabel.push_back(trainSVM_label);

		train_state.push_back(input_traindata);
	}

	

	fin.close();

	Mat( vec_trainLabel ).copyTo( input_labeldata );
	train_label.push_back(input_labeldata);

	input_traindata.release();
	input_labeldata.release();

	return (TRUE);
}


//-----------------------------------------
// Estimation using RLS
//-----------------------------------------
double	SpindleIntelligence::updateEstimationRLS(double u[1], double d){
	double k=0;
	Mat Wk;

	this->rls->updateRLS(u, d);
	this->rls->outputResult(Wk);

	k = Wk.at<double>(0,0);


	return k;
}