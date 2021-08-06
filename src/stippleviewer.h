#ifndef STIPPLEVIEWER_H
#define STIPPLEVIEWER_H

#include <QGraphicsView>
#include <iostream>
#include <QPen>

#include "lbgstippling.h"
#include "tsp.h"

class StippleViewer : public QGraphicsView {
  Q_OBJECT

 public:
  StippleViewer(const QImage &img, QWidget *parent);
  void stipple(const LBGStippling::Params params);
  QPixmap getImage();
  void setInputImage(const QImage &img);
  void saveImageSVG(const QString &path);
  void saveImagePDF(const QString &path);
  void displayPoints(const std::vector<Stipple> &stipples);
  void displayPoints(const std::vector<Stipple> &stipples_c, const std::vector<Stipple> &stipples_m,
               const std::vector<Stipple> &stipples_y, const std::vector<Stipple> &stipples_k);
  void displayTSP(const std::vector<Stipple> &stipples, const std::vector<int> &solution);

 signals:
  void finished();
  void inputImageChanged();
  void iterationStatus(size_t iteration, size_t numberPoints, size_t splits,
                       size_t merges, float hysteresis);

 private:
  LBGStippling m_stippling;
  TSP m_TSP;
  QImage m_image;
};

#endif  // STIPPLEVIEWER_H
