/////////////////////////Car tracking project with laser_radar_data_fusion/////////////////////////////////////////
//////////////////////////////////////////////////////////////////////Copyright 2009-10 Akihiro Takeuchi///////////

////////////////F_info.h   File input & output information & definition header ////////////////////////////////////

#ifndef INCLUDED_F_INFORMATION_
#define INCLUDED_F_INFORMATION_

#include "switch_release.h"

#ifdef RELEASE
#define L_DATA_PASS	"/usr/local/geye_with_cam/bin/car_detecter/"
#else
#define L_DATA_PASS	"./CAR_TRACKING/"
#endif  /* ifdef RELEASE */

#endif
