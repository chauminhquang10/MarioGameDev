#include "NewMapCam.h"


CNewMapCam::CNewMapCam(int id, float start_cam_x, float end_cam_x, float y_limit, float y_start)
{
	this->id = id;
	this->start_cam_x = start_cam_x;
	this->end_cam_x = end_cam_x;
	this->y_limit = y_limit;
	this->y_start = y_start;
}