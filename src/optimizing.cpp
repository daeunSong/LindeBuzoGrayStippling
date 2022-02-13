#include "optimizing.h"

Optimizing::Optimizing () {

};

double Optimizing::calDist(double x1, double y1, double x2, double y2) {
  return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void Optimizing::saveRoboticPath (const std::vector<Stipple> &stipples, const std::vector<int> &solution, const QString fname, const std::string extension, const int width, const int height) {
  std::ofstream out("logs/"+std::string(fname.toUtf8().constData())+"_path_"+extension+".txt");
  out << width << " " << height << std::endl; // save the drawing size
  double ratio = width/height;
  double target_height = 0.5;

  int count = 0;
  double x1 = stipples[0].pos.x() * target_height * ratio;
  double y1 = stipples[0].pos.y() * target_height;
  for (auto i: solution) {
    out << stipples[i].pos.x() << " " << stipples[i].pos.y() << std::endl;
    m_Path.solutionPath.push_back(stipples[i]);
    count ++;
    if (count > 4999) {
      out << "End" << std::endl;
      out << stipples[i].pos.x() << " " << stipples[i].pos.y() << std::endl;
      count = 0;
    }
    double x2 = stipples[i].pos.x() * target_height * ratio;
    double y2 = stipples[i].pos.y() * target_height;

//    if (calDist(x1, y1, x2, y2) < 0.05) {
//      out << "End" << std::endl;
//      out << stipples[i].pos.x() << " " << stipples[i].pos.y() << std::endl;
//    }
    x1 = x2; y1 = y2;
  }
  out << "End" << std::endl;
  out.close();
}
