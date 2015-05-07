/*
 * signals.cpp
 *
 *  Created on: Apr 9, 2015
 *      Author: sujiwo
 */


#include <iostream>
#include <ros/ros.h>
#include "Rate.h"
#include "vector_map.h"
#include <tf/transform_listener.h>
#include <sensor_msgs/CameraInfo.h>
#include <signal.h>
#include <cstdio>
#include "Math.h"
#include <Eigen/Eigen>
#include "traffic_light_detector/Signals.h"



static VectorMap vmap;

static Eigen::Vector3f position;
static Eigen::Quaternionf orientation;
static  float fx,
              fy,
              imageWidth,
              imageHeight,
              cx,
              cy;
static tf::StampedTransform trf;

#define SignalLampRadius 0.3


void cameraInfoCallback (const sensor_msgs::CameraInfo::ConstPtr camInfoMsg)
{
  fx = static_cast<float>(camInfoMsg->P[0]);
  fy = static_cast<float>(camInfoMsg->P[5]);
  imageWidth = camInfoMsg->width;
  imageHeight = camInfoMsg->height;
  cx = static_cast<float>(camInfoMsg->P[2]);
  cy = static_cast<float>(camInfoMsg->P[6]);
}


void getTransform (Eigen::Quaternionf &ori, Point3 &pos)
{
	static tf::TransformListener listener;

	// target_frame    source_frame
    ros::Time now = ros::Time();
	listener.waitForTransform ("camera", "map", now, ros::Duration(10.0));
	listener.lookupTransform ("camera", "map", now, trf);

	tf::Vector3 &p = trf.getOrigin();
	tf::Quaternion o = trf.getRotation();
	pos.x()=p.x(); pos.y()=p.y(); pos.z()=p.z();
	ori.w()=o.w(); ori.x()=o.x(); ori.y()=o.y(); ori.z()=o.z();
}


Point3 transform (const Point3 &psrc, tf::StampedTransform &tfsource)
{
	tf::Vector3 pt3 (psrc.x(), psrc.y(), psrc.z());
	tf::Vector3 pt3s = tfsource * pt3;
	return Point3 (pt3s.x(), pt3s.y(), pt3s.z());
}


/*
 * Project a point from world coordinate to image plane
 */
bool project2 (const Point3 &pt, int &u, int &v, bool useOpenGLCoord=false)
{
	float nearPlane = 1.0;
	float farPlane = 200.0;
    Point3 _pt = transform (pt, trf);
	float _u = _pt.x()*fx/_pt.z() + cx;
	float _v = _pt.y()*fy/_pt.z() + cy;

    u = static_cast<int>(_u);
    v = static_cast<int>(_v);
	if ( u < 0 || imageWidth < u || v < 0 || imageHeight < v || _pt.z() < nearPlane || farPlane < _pt.z() ) {
		u = -1, v = -1;
		return false;
	}

	if (useOpenGLCoord) {
		v = imageHeight - v;
	}

	return true;
}


void echoSignals2 (ros::Publisher &pub, bool useOpenGLCoord=false)
{
	int countPoint = 0;
    traffic_light_detector::Signals signalsInFrame;

	for (unsigned int i=1; i<=vmap.signals.size(); i++) {
		Signal signal = vmap.signals[i];
		int pid = vmap.vectors[signal.vid].pid;

		Point3 signalcenter = vmap.getPoint(pid);
		Point3 signalcenterx (signalcenter.x(), signalcenter.y(), signalcenter.z()+SignalLampRadius);

		int u, v;
		if (project2 (signalcenter, u, v, useOpenGLCoord) == true) {
			countPoint++;

			int radius;
			int ux, vx;
			project2 (signalcenterx, ux, vx, useOpenGLCoord);
			radius = (int)distance (ux, vx, u, v);

			traffic_light_detector::ExtractedPosition sign;
			sign.signalId = signal.id;
			sign.u = u, sign.v = v, sign.radius = radius;
			sign.x = signalcenter.x(), sign.y = signalcenter.y(), sign.z = signalcenter.z();
			sign.hang = vmap.vectors[signal.vid].hang;
			sign.type = signal.type, sign.linkId = signal.linkid;
			signalsInFrame.Signals.push_back (sign);
		}
	}

	signalsInFrame.header.stamp = ros::Time::now();
	pub.publish (signalsInFrame);

	printf ("There are %d out of %u signals in frame\n", countPoint, vmap.signals.size());
}


void interrupt (int s)
{
	exit(1);
}


int main (int argc, char *argv[])
{
  if (argc < 2)
    {
      std::cout << "Usage: traffic_light_extract <vector-map-dir>" << std::endl;
      return -1;
    }

	vmap.loadAll(argv[1]);
	ros::init(argc, argv, "traffic_light_extract");
	ros::NodeHandle rosnode;

	ros::Subscriber cameraInfoSubscriber = rosnode.subscribe ("/camera/camera_info", 100, cameraInfoCallback);
    ros::Publisher signalPublisher = rosnode.advertise <traffic_light_detector::Signals> ("traffic_light_pixel_xy", 100);
	signal (SIGINT, interrupt);

    Rate loop (25);
    while (true) {

      ros::spinOnce();

      try {
        getTransform (orientation, position);
      } catch (tf::TransformException &exc) {
      }

      echoSignals2 (signalPublisher, true);
      loop.sleep();
    }


}
