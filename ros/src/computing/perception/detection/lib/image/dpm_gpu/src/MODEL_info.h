/////////////////////////Car tracking project with laser_radar_data_fusion/////////////////////////////////////////
//////////////////////////////////////////////////////////////////////Copyright 2009-10 Akihiro Takeuchi///////////

///////////MODEL_info.h   Detector-Model information & definition header  /////////////////////////////////////////

//OpenCV library
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv2/legacy/legacy.hpp>

#include <cstdio>

#include "switch_float.h"
#include "switch_release.h"

#ifndef INCLUDED_Minfo_
#define INCLUDED_Minfo_

//file information///

//#define IM_NAME		"test5.jpg"				//Image name
//#define MNAME			"M1.avi"				//source movie name

//#define IN_S_NAME		"C:\\TESTIM_2010_2_3\\"			//Input-Image(successive)
//#define IN_S_NAME		"C:\\Users\\kawano\\Desktop\\re\\"
#define IN_S_NAME		"CAR_TRACKING/Test_Images/Daytime_Image_PNG/"			//Input-Image(successive)
#define OUTMNAME		"Out.avi"				//output movie name
#define OUT_NAME		"Out_Image/res"		//Result name
#define EX_NAME			".png"

#ifdef RELEASE

#define F_NAME_COM "/usr/local/geye_with_cam/bin/car_detecter/car_comp.csv" //file name (component)
#define F_NAME_ROOT	"/usr/local/geye_with_cam/bin/car_detecter/car_root.csv" //file name (root_filter)
#define F_NAME_PART	"/usr/local/geye_with_cam/bin/car_detecter/car_part.csv" //file name (part_filter)

#else

//#define F_NAME_COM		"car_comp.csv"			//file name (component)
#define F_NAME_COM "./CAR_TRACKING/car_comp.csv" //file name (component)
//#define F_NAME_ROOT		"car_root.csv"			//file name (root_filter)
#define F_NAME_ROOT "./CAR_TRACKING/car_root.csv"			//file name (root_filter)
//#define F_NAME_PART		"car_part.csv"			//file name (part_filter)
#define F_NAME_PART	"./CAR_TRACKING/car_part.csv"			//file name (part_filter)

#endif /* ifdef RELEASE */

//struct information///

#ifndef _MODEL_INFO
#define _MODEL_INFO
//struct for model component information
struct Model_info {

	//basic information
	//from xxxcomp.csv
	int numcomponent;	//number of component
	int sbin;			//cell size
	int interval;		//interval (for hierachical detection)
	int max_X;
	int max_Y;
	//from calculation
	int padx;			//pad information
	int pady;
	int max_scale;
	//image size information
	int IM_WIDTH;
	int IM_HEIGHT;

	//per root
	int *ridx;			//root index information
	int *oidx;			//offsetindex information
	FLOAT *offw;		//offset weight
	int *rsize;			//root size
	int *numpart;		//number of part filter per component

	//per part
	int **pidx;			//part index information
	int **didx;			//define index of part
	int **psize;

	//defs
	FLOAT *def;	//defs
	int *anchor;	//anchor

	//least_square info
	FLOAT **x1;
	FLOAT **y1;
	FLOAT **x2;
	FLOAT **y2;

	bool ini;	//flag for initialization
	FLOAT ratio;	//ratio of zooming image

};

//struct for root_filter_information
struct Rootfilters {
	int NoR;				//number of root filter
	int **root_size;		//size of root filter
	FLOAT **rootfilter;	//weight of root filter
	int *rootsym;			//symmetric information
};

//struct for part_filter_information
struct Partfilters {
	int NoP;				//number of part filter
	int **part_size;		//size of part filter
	FLOAT **partfilter;	//weight of root filter
	int *part_partner;		//symmetric-partner information
	int *part_sym;			//symmetric information of part filter
};

//model information
struct MODEL {
	Model_info *MI;
	Rootfilters *RF;
	Partfilters *PF;
};

#endif

//Particle filter informations
struct PINFO {
	int *partner;
	CvConDensation ** condens;
	int *se_num;
	int **L_P;
	FLOAT **L_VX;
	FLOAT **L_VY;
	FLOAT **ave_p;
};

//Result of Detection
struct RESULT {
	int num;
	int *point;
	int *OR_point;
	IplImage **IM;
	int *type;
	FLOAT *scale;
	FLOAT *score;
};

#endif
