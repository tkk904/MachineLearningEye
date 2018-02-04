#pragma once

//============================================================================
//	The University of Tokyo
//	  Laparoscopic Surgery Robot
//	
//----------------------------------------------------------------------------
//	Filename	: LIB_RLS_CV.h
//	Abstract	: class for recursive least square using openCV2.4.2
//	Author		: osa@nml.t.u-tokyo.ac.jp
//----------------------------------------------------------------------------
//	2012/11/30	: Start developing
//============================================================================

#ifndef LIB_RLS_CV_H
#define LIB_RLS_CV_H


#include "opencv2/core/core.hpp"
#include "windows.h"

using namespace cv;

class LIB_RLS_CV
{
public:
	LIB_RLS_CV(int vecSize);
	LIB_RLS_CV(int vecSize, double w_ini[]);
	LIB_RLS_CV(int vecSize, double w_ini[], double lambda);
	~LIB_RLS_CV(void);
	int updateRLS(double u[], double d);
	void dumpResult();
	void outputResult(Mat& Wk);

private:
	int initMat(int vecSize);
	int initMat(int vecSize, double w_ini[]);
	int initMat(int vecSize, double w_ini[], double l);
	int releaseMat();
	

	Mat cv_u;
	Mat cv_d;

	Mat cv_Pn_p;		
	Mat cv_Pn;		
	Mat cv_P;        
	Mat cv_wn_p;		
	Mat cv_wn;		
	Mat cv_pi;		
	Mat cv_lambda;	
	Mat cv_gamma;	
	Mat cv_kn;		
	Mat cv_alpha;
	Mat cv_d_e;

	int vec_size;

};

#endif