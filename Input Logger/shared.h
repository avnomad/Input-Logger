#ifndef SHARED_H
#define SHARED_H

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <utility>
using std::pair;

#include <Space-Time/Vector2D.h>

struct Point
{
	Vector2D<> position;
	double pressure;
};

extern vector<pair<unsigned int,vector<Point>>> completeStrokes;
extern map<unsigned int, vector<Point>> currentStrokes;

#endif // SHARED_H