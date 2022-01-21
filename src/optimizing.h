#ifndef OPTIMIZING_H
#define OPTIMIZING_H

#include "lbgstippling.h"
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>

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

};

#endif  // OPTIMIZING_H
