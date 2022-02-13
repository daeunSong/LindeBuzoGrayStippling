#ifndef OPTIMIZING_H
#define OPTIMIZING_H

#include "lbgstippling.h"
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <math.h>

class Optimizing {
 public:
  struct Path {
    std::vector<Stipple> solutionPath;
  };

  std::vector<double> city1;
  std::vector<double> city2;
  double threshold;

  Path m_Path;

  Optimizing();

  void saveRoboticPath (const std::vector<Stipple> &stipples, const std::vector<int> &solution, const QString fname, const std::string extension, const int width, const int height);
  double calDist(double x1, double y1, double x2, double y2);
};

#endif  // OPTIMIZING_H
