#include "optimizing.h"

Optimizing::Optimizing () {

};

void Optimizing::saveRoboticPath (const std::vector<Stipple> &stipples, const std::vector<int> &solution, const QString fname, const std::string extension, const int width, const int height) {
  std::ofstream out("logs/"+std::string(fname.toUtf8().constData())+"_path_"+extension+".txt");
  out << width << " " << height << std::endl; // save the drawing size
  int count = 0;
  for (auto i: solution) {
    out << stipples[i].pos.x() << " " << stipples[i].pos.y() << std::endl;
    m_Path.solutionPath.push_back(stipples[i]);
    count ++;
    if (count > 4999) {
      out << "End" << std::endl;
      out << stipples[i].pos.x() << " " << stipples[i].pos.y() << std::endl;
      count = 0;
    }
  }
  out << "End" << std::endl;
  out.close();
}
