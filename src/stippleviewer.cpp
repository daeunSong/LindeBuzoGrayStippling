#include "stippleviewer.h"

#include <QCoreApplication>
#include <QPrinter>
#include <QSvgGenerator>
#include <QImage>

StippleViewer::StippleViewer(const QImage &img, QWidget *parent)
    : QGraphicsView(parent), m_image(img) {
  setInteractive(false);
  setRenderHint(QPainter::Antialiasing, true);
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  setOptimizationFlag(QGraphicsView::DontSavePainterState);
  setFrameStyle(0);
  setAttribute(Qt::WA_TranslucentBackground, false);
  setCacheMode(QGraphicsView::CacheBackground);

  setScene(new QGraphicsScene(this));
  this->scene()->setSceneRect(m_image.rect());
  this->scene()->setItemIndexMethod(QGraphicsScene::NoIndex);
  this->scene()->addPixmap(QPixmap::fromImage(m_image));

  m_stippling = LBGStippling();
  m_TSP = TSP();
  m_stippling.setStatusCallback([this](const auto &status) {
    emit iterationStatus(status.iteration + 1, status.size, status.splits,
                         status.merges, status.hysteresis);
  });

  m_stippling.setStippleCallback(
      [this](const auto &stipples) { displayPoints(stipples); });
}

void StippleViewer::displayPoints(const std::vector<Stipple> &stipples) {
  this->scene()->clear();
  for (const auto &s : stipples) {
    double x = static_cast<double>(s.pos.x() * m_image.width() - s.size / 2.0f);
    double y =
        static_cast<double>(s.pos.y() * m_image.height() - s.size / 2.0f);
    double size = static_cast<double>(s.size);
    this->scene()->addEllipse(x, y, size, size, Qt::NoPen, s.color);
  }
  // TODO: Fix event handling
  QCoreApplication::processEvents();
}

void StippleViewer::displayPoints(const std::vector<Stipple> &stipples_c, const std::vector<Stipple> &stipples_m,
                          const std::vector<Stipple> &stipples_y, const std::vector<Stipple> &stipples_k) {
  this->scene()->clear();
  for (const auto &s : stipples_c) {
    double x = static_cast<double>(s.pos.x() * m_image.width() - s.size / 2.0f);
    double y =
        static_cast<double>(s.pos.y() * m_image.height() - s.size / 2.0f);
    double size = static_cast<double>(s.size);
    this->scene()->addEllipse(x, y, size, size, Qt::NoPen, s.color);
  }
  for (const auto &s : stipples_m) {
    double x = static_cast<double>(s.pos.x() * m_image.width() - s.size / 2.0f);
    double y =
        static_cast<double>(s.pos.y() * m_image.height() - s.size / 2.0f);
    double size = static_cast<double>(s.size);
    this->scene()->addEllipse(x, y, size, size, Qt::NoPen, s.color);
  }
  for (const auto &s : stipples_y) {
    double x = static_cast<double>(s.pos.x() * m_image.width() - s.size / 2.0f);
    double y =
        static_cast<double>(s.pos.y() * m_image.height() - s.size / 2.0f);
    double size = static_cast<double>(s.size);
    this->scene()->addEllipse(x, y, size, size, Qt::NoPen, s.color);
  }
  for (const auto &s : stipples_k) {
    double x = static_cast<double>(s.pos.x() * m_image.width() - s.size / 2.0f);
    double y =
        static_cast<double>(s.pos.y() * m_image.height() - s.size / 2.0f);
    double size = static_cast<double>(s.size);
    this->scene()->addEllipse(x, y, size, size, Qt::NoPen, s.color);
  }
  // TODO: Fix event handling
  QCoreApplication::processEvents();
}

void StippleViewer::displayTSP(const std::vector<Stipple> &stipples, const std::vector<int> &solution) {
//  this->scene()->clear();
  const QPen pen = QPen(stipples[0].color);

  double x1 = stipples[solution[0]].pos.x() * m_image.width() - stipples[solution[0]].size / 2.0f;
  double y1 = stipples[solution[0]].pos.y() * m_image.height() - stipples[solution[0]].size / 2.0f;

  for (int i = 1; i < solution.size(); i ++) {
    double x2 = static_cast<double>(stipples[solution[i]].pos.x() * m_image.width()
                                                                - stipples[solution[i]].size / 2.0f);
    double y2 =
        static_cast<double>(stipples[solution[i]].pos.y() * m_image.height()
                                                                - stipples[solution[i]].size / 2.0f);
    this->scene()->addLine(x1, y1, x2, y2, pen);
    x1 = x2; y1 = y2;
  }
  // TODO: Fix event handling
  QCoreApplication::processEvents();
}

QPixmap StippleViewer::getImage() {
  QPixmap pixmap(this->scene()->sceneRect().size().toSize());
  pixmap.fill(Qt::white);

  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing, true);
  this->scene()->render(&painter);
  return pixmap;
}

void StippleViewer::saveImageSVG(const QString &path) {
  QSvgGenerator generator;
  generator.setFileName(path);
  generator.setSize(m_image.size());
  generator.setViewBox(this->scene()->sceneRect());
  generator.setTitle("Stippling Result");
  generator.setDescription(
      "SVG File created by Weighted Linde-Buzo-Gray Stippling");

  QPainter painter;
  painter.begin(&generator);
  this->scene()->render(&painter);
  painter.end();
}

void StippleViewer::saveImagePDF(const QString &path) {
  adjustSize();

  QPrinter printer(QPrinter::HighResolution);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setCreator("Weighted Linde-Buzo-Gray Stippling");
  printer.setOutputFileName(path);
  printer.setPaperSize(m_image.size(), QPrinter::Point);
  printer.setFullPage(true);

  QPainter painter(&printer);
  this->render(&painter);
}

void StippleViewer::setInputImage(const QImage &img) {
  m_image = img;
  this->scene()->clear();
  this->scene()->addPixmap(QPixmap::fromImage(m_image));
  this->scene()->setSceneRect(m_image.rect());

  auto w = m_image.width();
  auto h = m_image.height();
  setMinimumSize(w, h);
  adjustSize();

  emit inputImageChanged();
}

void StippleViewer::stipple(const LBGStippling::Params params) {
  // Split images in CMYK
//  m_image.convertToFormat(QImage::Format_RGBA8888);
  QImage m_image_c = m_image.copy();
  QImage m_image_m = m_image.copy();
  QImage m_image_y = m_image.copy();
  QImage m_image_k = m_image.copy();

  for(int j = 0; j < m_image.height(); j++) {
    for(int i = 0; i < m_image.width(); i++) {
      int c = 0, m = 0, y = 0, k = 0;
      QColor oldColor = m_image.pixel(i, j);
      oldColor = oldColor.toCmyk();
      oldColor.getCmyk(&c, &m, &y, &k);
      m_image_c.setPixel(i, j, QColor::fromCmyk(c,0,0,0).toRgb().rgba());
      m_image_m.setPixel(i, j, QColor::fromCmyk(0,m,0,0).toRgb().rgba());
      m_image_y.setPixel(i, j, QColor::fromCmyk(0,0,y,0).toRgb().rgba());
      m_image_k.setPixel(i, j, QColor::fromCmyk(0,0,0,k).toRgb().rgba());
    }
  }

  std::vector<Stipple> stipples;

  // TODO: Handle return value
  if (!params.colorSplit) {
    std::vector<Stipple> black =  m_stippling.stipple(m_image, params, Qt::black); //black
    if (!params.interactiveDisplay) {
      displayPoints(black);
    }
//    std::copy(black.begin(), black.end(), stipples.begin());
    if (params.solveTSP) {
      this->scene()->clear();
      m_TSP.solve(black);
      displayTSP(black, m_TSP.solution);
    }
  }
  else {
    std::vector<Stipple> cyan, magenta, yellow, black;

    clock_t begin_time = clock();

    cyan = m_stippling.stipple(m_image_c, params, QColor(0,255,255,180)); //cyan
    magenta = m_stippling.stipple(m_image_m, params, QColor(255,0,255,180)); //magenta
    yellow = m_stippling.stipple(m_image_y, params, QColor(255,255,0,180)); //yellow
    black = m_stippling.stipple(m_image_k, params, QColor(0,0,0,180)); //black
/*
    #pragma omp parallel sections
    {
      #pragma omp section
      cyan = m_stippling.stipple(m_image_c, params, QColor(0,255,255,180)); //cyan

      #pragma omp section
      magenta = m_stippling.stipple(m_image_m, params, QColor(255,0,255,180)); //magenta

      #pragma omp section
      yellow = m_stippling.stipple(m_image_y, params, QColor(255,255,0,180)); //yellow

      #pragma omp section
      black = m_stippling.stipple(m_image_k, params, QColor(0,0,0,180)); //black
    }*/

    clock_t end_time = clock();
    double time = double(end_time - begin_time)/CLOCKS_PER_SEC*1000;

    std::cout << "stippleviewer.cpp CMYK TIME: " << time << " ms" << std::endl;

    displayPoints(cyan, magenta, yellow, black);
//    std::copy(black.begin(), black.end(), stipples.begin());
    if (params.solveTSP) {
      this->scene()->clear();
      clock_t begin_time = clock();

      #pragma omp parallel sections
      {
        #pragma omp section
        m_TSP.solve(cyan);

        #pragma omp section
        m_TSP.solve(magenta);

        #pragma omp section
        m_TSP.solve(yellow);

        #pragma omp section
        m_TSP.solve(black);
      }
      
      /*
      m_TSP.solve(cyan);
      displayTSP(cyan, m_TSP.solution);
      m_TSP.solve(magenta);
      displayTSP(magenta, m_TSP.solution);
      m_TSP.solve(yellow);
      displayTSP(yellow, m_TSP.solution);
      m_TSP.solve(black);
      displayTSP(black, m_TSP.solution);
      */

      clock_t end_time = clock();
      double time = double(end_time - begin_time)/CLOCKS_PER_SEC*1000;
      std::cout << "stippleviewer.cpp TSP TIME: " << time << " ms" << std::endl;
    }
  }

  emit finished();
  # pragma omp parallel
  {
    printf("Hello OpenMP thread id %d\n", omp_get_thread_num());
  }
//  m_TSPSolver.solve(stipples);

}
