#ifndef STIPPLEVIEWER_H
#define STIPPLEVIEWER_H

#include <QGraphicsView>
#include <iostream>
#include <fstream>
#include <QPen>
#include <math.h>

#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

#include "lbgstippling.h"
#include "tsp.h"
#include "optimizing.h"

class StippleViewer : public QGraphicsView {
  Q_OBJECT

 public:
  StippleViewer(const QImage &img, QWidget *parent);
  void colorPick(const LBGStippling::Params params);
  void stipple(const LBGStippling::Params params);
  QPixmap getImage();
  void setInputImage(const QImage &img);
  void saveImagePNG(const QString &path);
  void saveImageSVG(const QString &path);
  void saveImagePDF(const QString &path);
  void saveRoboticPath(const std::vector<Stipple> &stipples, const std::vector<int> &solution, const QString fname, const std::string extension);
  void displayPoints(const std::vector<Stipple> &stipples);
  void displayPoints(const std::vector<std::vector<Stipple>> &stipples);
  void displayPoints(const std::vector<Stipple> &stipples_c, const std::vector<Stipple> &stipples_m,
               const std::vector<Stipple> &stipples_y, const std::vector<Stipple> &stipples_k);
  void displayTSP(const std::vector<Stipple> &stipples, const std::vector<int> &solution);
  void displayTSP(const std::vector<std::vector<Stipple>> &stipples, const std::vector<std::vector<int>> &solutions);
  void displayTSP(const std::vector<Stipple> &c, const std::vector<int> &cc,
              const std::vector<Stipple> &m, const std::vector<int> &mm,
              const std::vector<Stipple> &y, const std::vector<int> &yy,
              const std::vector<Stipple> &k, const std::vector<int> &kk);
  double calDist(double x1, double y1, double x2, double y2);

  cv::Mat QImage2Mat(QImage const &img, int format);
  QImage Mat2QImage(cv::Mat const &mat, QImage::Format format);

  std::tuple<std::vector<cv::Mat>, std::vector<std::vector<int>>> kmeans_cluster(const LBGStippling::Params params);

 signals:
  void finished();
  void inputImageChanged();
  void iterationStatus(size_t iteration, size_t numberPoints, size_t splits,
                       size_t merges, float hysteresis);

 private:
  LBGStippling m_stippling;
  TSP m_TSP;
  Optimizing m_Optimizing;
  QImage m_image;
};

#endif  // STIPPLEVIEWER_H
