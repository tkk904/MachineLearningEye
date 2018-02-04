//============================================================================
//	The University of Tokyo
//	  Laparoscopic Surgery Robot
//	
//----------------------------------------------------------------------------
//	Filename	: LIB_RLS_CV,cpp
//	Abstract	: class for recursive least square using openCV2.4.2
//	Author		: osa@nml.t.u-tokyo.ac.jp
//----------------------------------------------------------------------------
//	2012/11/30	: Start developing
//============================================================================

#include "stdafx.h"

#include "LIB_RLS_CV.h"
#include <iostream>


using namespace std;

LIB_RLS_CV::LIB_RLS_CV(int vecSize)
{
	initMat(vecSize);
}

LIB_RLS_CV::LIB_RLS_CV(int vecSize, double w_ini[])
{
	initMat(vecSize, w_ini);
}

LIB_RLS_CV::LIB_RLS_CV(int vecSize, double w_ini[], double lambda)
{
	initMat(vecSize, w_ini, lambda);
}

LIB_RLS_CV::~LIB_RLS_CV(void)
{
	releaseMat();
}

int LIB_RLS_CV::initMat(int vecSize ){
	this->vec_size = vecSize;

	cv_u = Mat(vecSize,1,CV_64F);
	cv_d = Mat(1,1,CV_64F);

	cv_Pn_p		= Mat::eye(vec_size,vec_size,CV_64F);
	cv_Pn		= Mat::eye(vec_size,vec_size,CV_64F);
	cv_P        = Mat::eye(vec_size,vec_size,CV_64F);
	cv_wn_p		= Mat::zeros(vec_size,1,CV_64F);
	cv_wn		= Mat::zeros(vec_size,1,CV_64F);
	cv_pi		= Mat::zeros(vec_size,1,CV_64F);
	cv_lambda	= Mat(1,1,CV_64F,1);
	cv_gamma	= Mat(1,1,CV_64F,1);
	cv_kn		= Mat(vec_size,1,CV_64F,1);
	cv_alpha	= Mat(1,1,CV_64F,1);

	cv_d_e		= Mat::zeros(1,1,CV_64F);
	
	return (TRUE);
}


//-----------------------------------------
// Initialize matrices
//-----------------------------------------
int LIB_RLS_CV::initMat(int vecSize, double w_ini[] ){
		this->vec_size = vecSize;

	cv_u = Mat(vecSize,1,CV_64F);
	cv_d = Mat(1,1,CV_64F);

	cv_Pn_p		= Mat::eye(vec_size,vec_size,CV_64F);
	cv_Pn		= Mat::eye(vec_size,vec_size,CV_64F);
	cv_P        = Mat::eye(vec_size,vec_size,CV_64F);
	cv_wn_p		= Mat(vec_size,1,CV_64F, w_ini);
	cv_wn		= Mat::zeros(vec_size,1,CV_64F);
	cv_pi		= Mat::zeros(vec_size,1,CV_64F);
	cv_lambda	= Mat(1,1,CV_64F,0.5);
	cv_gamma	= Mat(1,1,CV_64F,1);
	cv_kn		= Mat(vec_size,1,CV_64F,1);
	cv_alpha	= Mat(1,1,CV_64F,1);

	cv_d_e		= Mat::zeros(1,1,CV_64F);
	
	return (TRUE);

}

int LIB_RLS_CV::initMat(int vecSize, double w_ini[], double lambda ){
		this->vec_size = vecSize;

	cv_u = Mat(vecSize,1,CV_64F);
	cv_d = Mat(1,1,CV_64F);

	cv_Pn_p		= Mat::eye(vec_size,vec_size,CV_64F);
	cv_Pn		= Mat::eye(vec_size,vec_size,CV_64F);
	cv_P        = Mat::eye(vec_size,vec_size,CV_64F);
	cv_wn_p		= Mat(vec_size,1,CV_64F, w_ini);
	cv_wn		= Mat::zeros(vec_size,1,CV_64F);
	cv_pi		= Mat::zeros(vec_size,1,CV_64F);
	cv_lambda	= Mat(1,1,CV_64F,lambda);
	cv_gamma	= Mat(1,1,CV_64F,1);
	cv_kn		= Mat(vec_size,1,CV_64F,1);
	cv_alpha	= Mat(1,1,CV_64F,1);

	cv_d_e		= Mat::zeros(1,1,CV_64F);
	
	return (TRUE);

}
int LIB_RLS_CV::releaseMat(){
	
	
	cv_u.release();
	cv_d.release();

	cv_Pn_p.release();
	cv_Pn.release();	
	cv_P.release();    
	cv_wn_p.release();
	cv_wn.release();	
	cv_pi.release();	
	cv_lambda.release();
	cv_gamma.release();
	cv_kn.release();	
	cv_alpha.release();

	cv_d_e.release();

	return (TRUE);
}


//-----------------------------------------
// Recursive computation for RLS
//-----------------------------------------
int LIB_RLS_CV::updateRLS(double u[], double d){
	
		cv_u = Mat(vec_size,1,CV_64F, u);
		cv_d = Mat(1,1,CV_64F, d);	

		cv_pi	 = cv_u.t()*cv_Pn_p;
		cv_gamma = cv_lambda + cv_pi*cv_u;
			
		//cv_kn	 = cv_pi.t() / cv_gamma;
		cv_kn	 = cv_pi.t() / cv_gamma.at<double>(0,0);
		
		cv_d_e	 = cv_wn_p.t()*cv_u;
		cv_alpha = cv_d.at<double>(0,0) - cv_d_e;

		cv_wn	 = cv_wn_p + cv_kn * cv_alpha;
		cv_P	 = cv_kn * cv_pi;
		cv_Pn	 = ( cv_Pn_p - cv_P) / cv_lambda;

		cv_Pn_p = cv_Pn;
		cv_wn_p = cv_wn;


	return (TRUE);
}


//-----------------------------------------
// Display the result
//-----------------------------------------
void LIB_RLS_CV::dumpResult(){

	cout << "estimated output\n  " << cv_d_e << endl;
	cout << "estimated parameter\n  " << cv_wn << endl;
	cout << "estimation error\n  " << cv_alpha << endl;
}

void LIB_RLS_CV::outputResult(Mat &Wk){
	
	Mat( this->cv_wn ).copyTo( Wk );

}
