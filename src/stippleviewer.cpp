#include "stippleviewer.h"

#include <QCoreApplication>
#include <QPrinter>
#include <QSvgGenerator>
#include <QImage>

#define qstr QString::fromStdString

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

void StippleViewer::displayTSP(const std::vector<Stipple> &c, const std::vector<int> &cc,
                               const std::vector<Stipple> &m, const std::vector<int> &mm,
                               const std::vector<Stipple> &y, const std::vector<int> &yy,
                               const std::vector<Stipple> &k, const std::vector<int> &kk) {
  displayTSP(c, cc);
  displayTSP(m, mm);
  displayTSP(y, yy);
//  displayTSP(k, kk);
}

QPixmap StippleViewer::getImage() {
  QPixmap pixmap(this->scene()->sceneRect().size().toSize());
  pixmap.fill(Qt::white);

  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing, true);
  this->scene()->render(&painter);
  return pixmap;
}

void StippleViewer::saveRoboticPath (const std::vector<Stipple> &stipples, const std::vector<int> &solution, const QString fname, const std::string extension) {
  std::ofstream out("logs/"+std::string(fname.toUtf8().constData())+"_path_"+extension+".txt");
  out << m_image.width() << " " << m_image.height() << std::endl; // save the drawing size
  int count = 0;
  for (auto i: solution) {
    out << stipples[i].pos.x() << " " << stipples[i].pos.y() << std::endl;
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

void StippleViewer::saveImagePNG(const QString &path) {
  QPixmap map = getImage();
  map.save(path);
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
//      std::cout << c << " " << m << " " << y << " " << k << std::endl;
      // thresholding the values
//      c > 30? c = c : c = 0;
//      m > 50? m = m : m = 0;
//      y > 5? y = y : y = 0;
//      k > 70? k = k : k = 0;
      m_image_c.setPixel(i, j, QColor::fromCmyk(c,0,0,0).toRgb().rgba());
      m_image_m.setPixel(i, j, QColor::fromCmyk(0,m,0,0).toRgb().rgba());
      m_image_y.setPixel(i, j, QColor::fromCmyk(0,0,y,0).toRgb().rgba());
      m_image_k.setPixel(i, j, QColor::fromCmyk(0,0,0,k).toRgb().rgba());
    }
  }

  std::vector<Stipple> stipple_c, stipple_m, stipple_y, stipple_k;
  double time_stipple_c = 0, time_stipple_m = 0, time_stipple_y = 0, time_stipple_k = 0;
  std::vector<int> tsp_c, tsp_m, tsp_y, tsp_k;
  double time_tsp_c = 0, time_tsp_m= 0 , time_tsp_y = 0, time_tsp_k = 0;

  // TODO: Handle return value
  if (!params.colorSplit) {
    if (params.saveLog) m_image_k.save(qstr("logs/")+params.fileName+qstr("_k.png"));

    tie(stipple_k, time_stipple_k) =  m_stippling.stipple(m_image_k, params, Qt::black); //black
    if (!params.interactiveDisplay) {
      displayPoints(stipple_k);
    }
    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple.png"));

    if (params.solveTSP) {
      this->scene()->clear();
      tie(tsp_k, time_tsp_k) = m_TSP.solve(stipple_k);
      displayTSP(stipple_k, tsp_k);
      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp.png"));
    }

    // LOGGING
    if (params.saveLog) {
      std::ofstream log("logs/"+std::string(params.fileName.toUtf8().constData())+"_log.txt");
      log << params.fileName.toUtf8().constData() << std::endl;
      log << "COLOR : BLACK\n";
      log << "K \t" << stipple_k.size() << "\t" << time_stipple_k << " ms\t" << time_tsp_k << "ms\n";
      log.close();

      saveRoboticPath (stipple_k, tsp_k, params.fileName, "k");
    }
  }
  else {
    if (params.saveLog){
      m_image_c.save(qstr("logs/")+params.fileName+qstr("_c.png"));
      m_image_m.save(qstr("logs/")+params.fileName+qstr("_m.png"));
      m_image_y.save(qstr("logs/")+params.fileName+qstr("_y.png"));
      m_image_k.save(qstr("logs/")+params.fileName+qstr("_k.png"));
    }

    tie(stipple_c, time_stipple_c) = m_stippling.stipple(m_image_c, params, QColor(0,255,255,180)); //cyan
    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple_c.png"));
    tie(stipple_m, time_stipple_m) = m_stippling.stipple(m_image_m, params, QColor(255,0,255,180)); //magenta
    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple_m.png"));
    tie(stipple_y, time_stipple_y) = m_stippling.stipple(m_image_y, params, QColor(255,255,0,180)); //yellow
    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple_y.png"));
    tie(stipple_k, time_stipple_k) = m_stippling.stipple(m_image_k, params, QColor(0,0,0,180)); //black
    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple_k.png"));
    displayPoints(stipple_c, stipple_m, stipple_y, stipple_k);
    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple.png"));

    if (params.solveTSP) {
      this->scene()->clear();
      tie(tsp_c, time_tsp_c) = m_TSP.solve(stipple_c);
      if (params.interactiveDisplay) displayTSP(stipple_c, tsp_c);
      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp_c.png"));
      tie(tsp_m, time_tsp_m) = m_TSP.solve(stipple_m);
      if (params.interactiveDisplay) this->scene()->clear(); displayTSP(stipple_m, tsp_m);
      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp_m.png"));
      tie(tsp_y, time_tsp_y) = m_TSP.solve(stipple_y);
      if (params.interactiveDisplay) this->scene()->clear(); displayTSP(stipple_y, tsp_y);
      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp_y.png"));
      tie(tsp_k, time_tsp_k) = m_TSP.solve(stipple_k);
      if (params.interactiveDisplay) this->scene()->clear(); displayTSP(stipple_k, tsp_k);
      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp_k.png"));
      displayTSP(stipple_c, tsp_c, stipple_m, tsp_m, stipple_y, tsp_y, stipple_k, tsp_k);
      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp.png"));
    }

    // LOGGING
    if (params.saveLog) {
      std::ofstream log("logs/"+std::string(params.fileName.toUtf8().constData())+"_log.txt");
      log << params.fileName.toUtf8().constData() << std::endl;
      log << "COLOR : CMYK\n";
      log << "C \t" << stipple_c.size() << "\t" << time_stipple_c  << " ms\t" << time_tsp_c << "ms\n";
      log << "M \t" << stipple_m.size() << "\t" << time_stipple_m << " ms\t" << time_tsp_m << "ms\n";
      log << "Y \t" << stipple_y.size() << "\t" << time_stipple_y << " ms\t" << time_tsp_y << "ms\n";
      log << "K \t" << stipple_k.size() << "\t" << time_stipple_k << " ms\t" << time_tsp_k << "ms\n";
      log.close();

      // save the robotic path
      saveRoboticPath (stipple_c, tsp_c, params.fileName, "c");
      saveRoboticPath (stipple_m, tsp_m, params.fileName, "m");
      saveRoboticPath (stipple_y, tsp_y, params.fileName, "y");
      saveRoboticPath (stipple_k, tsp_k, params.fileName, "k");

    }
  }

  emit finished();
//  m_TSPSolver.solve(stipples);

}
