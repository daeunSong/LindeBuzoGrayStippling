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
  m_Optimizing = Optimizing();
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
    double size = static_cast<double>(2.0);//s.size);
    this->scene()->addEllipse(x, y, size, size, Qt::NoPen, s.color);
  }
  // TODO: Fix event handling
  QCoreApplication::processEvents();
}

void StippleViewer::displayPoints(const std::vector<std::vector<Stipple>> &stipples_) {
  this->scene()->clear();
  for (const auto &stipples : stipples_ ) {
    for (const auto &s : stipples) {
      double x = static_cast<double>(s.pos.x() * m_image.width() - s.size / 2.0f);
      double y =
          static_cast<double>(s.pos.y() * m_image.height() - s.size / 2.0f);
      double size = static_cast<double>(2.0);//s.size);
      this->scene()->addEllipse(x, y, size, size, Qt::NoPen, s.color);
    }
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
    double size = static_cast<double>(2.0);//s.size);
    this->scene()->addEllipse(x, y, size, size, Qt::NoPen, s.color);
  }
  for (const auto &s : stipples_m) {
    double x = static_cast<double>(s.pos.x() * m_image.width() - s.size / 2.0f);
    double y =
        static_cast<double>(s.pos.y() * m_image.height() - s.size / 2.0f);
    double size = static_cast<double>(2.0);//s.size);
    this->scene()->addEllipse(x, y, size, size, Qt::NoPen, s.color);
  }
  for (const auto &s : stipples_y) {
    double x = static_cast<double>(s.pos.x() * m_image.width() - s.size / 2.0f);
    double y =
        static_cast<double>(s.pos.y() * m_image.height() - s.size / 2.0f);
    double size = static_cast<double>(2.0);//s.size);
    this->scene()->addEllipse(x, y, size, size, Qt::NoPen, s.color);
  }
//  for (const auto &s : stipples_k) {
//    double x = static_cast<double>(s.pos.x() * m_image.width() - s.size / 2.0f);
//    double y =
//        static_cast<double>(s.pos.y() * m_image.height() - s.size / 2.0f);
//    double size = static_cast<double>(2.0);//s.size);
//    this->scene()->addEllipse(x, y, size, size, Qt::NoPen, s.color);
//  }
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

void StippleViewer::displayTSP(const std::vector<std::vector<Stipple>> &s, const std::vector<std::vector<int>> &t) {
  for (int i = 0; i < s.size(); i++)
     displayTSP(s[i], t[i]);
}

QPixmap StippleViewer::getImage() {
  QPixmap pixmap(this->scene()->sceneRect().size().toSize());
  pixmap.fill(Qt::white);

  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing, true);
  this->scene()->render(&painter);
  return pixmap;
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
//
//void StippleViewer::colorPick(const LBGStippling::Params params) {
//  cv::Mat img, res, points, labels, centers;
//  int n, cIndex, iTemp;
////  img = cv::imread(params.fileName);
//  int width = m_image.width();
//  int height = m_image.height();
//  int nPoints = width * height;
//
//  points.create(nPoints, 1, CV_32FC3);
//  centers.create(params.colorNum, 1, points.type());
//  res.create(height, width, CV_32FC3);
//
//  for(int j = 0, n = 0; j < height; j++) {
//    for(int i = 0; i < width; i++, n++) {
//      points.at<cv:Vec3f>(n)[0] =
//    }
//  }
//
////  double compactness = cv::kmeans(points, clusterCount, labels,
////    TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 10, 1.0),
////       3, KMEANS_PP_CENTERS, centers);
//}
//
//cv::Mat StippleViewer::QImage2Mat(QImage const &img, int format = CV_8UC3) {
//  return cv::Mat(img.height(), img.width(), format,
//                (void *)(img.constBits()), img.bytesPerLine());
//}
//
QImage StippleViewer::Mat2QImage(cv::Mat const &mat, QImage::Format format = QImage::Format_RGB888) {
    return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), format).copy();
}

std::tuple<std::vector<cv::Mat>, std::vector<std::vector<int>>> StippleViewer::kmeans_cluster(const LBGStippling::Params params) {
  int K = params.colorNum;
//  m_image.convertToFormat(QImage::Format_RGB888);
//  cv::Mat img = QImage2Mat(m_image);
//  cv::cvtColor(img, img, CV_BGR2RGB);
  cv::Mat img = cv::imread(params.path.toStdString());
  std::vector<cv::Mat> imgs;

  cv::Mat bestLabels, centers;
  cv::Mat clustered(img.size(), img.type());

  for (int i = 0; i < K ; i ++){
    cv::Mat splitted(img.size(), img.type());
    splitted = cv::Scalar(255,255,255);
    imgs.push_back(splitted);
  }

  // samples
	cv::Mat samples(img.rows * img.cols, img.channels(), CV_32F);
	for (int y = 0; y < img.rows; y++)
		for (int x = 0; x < img.cols; x++)
			for (int z = 0; z < img.channels(); z++)
				if (img.channels() == 3) {
					samples.at<float>(y + x * img.rows, z) = img.at<cv::Vec3b>(y, x)[z];
				}
				else {
					samples.at<float>(y + x * img.rows, z) = img.at<uchar>(y, x);
				}
  // kmeans clustering
  cv::kmeans(samples, K, bestLabels,
            cv::TermCriteria( CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0),
            10, cv::KMEANS_PP_CENTERS, centers);

  std::vector<std::vector<int>> colors; // r, g, b colors
  for (int i = 0; i < K ; i ++){
    std::vector<int> color;
    for (int j = 2; j >= 0 ;j--)
      color.push_back(centers.at<float>(i,j));
    colors.push_back(color);
  }

  for (int y = 0; y < clustered.rows; y++)  {
    for (int x = 0; x < clustered.cols; x++)  {
      int index = y + x * clustered.rows;
      int cluster_index = bestLabels.at<int>(index, 0);
      // set new color
      for (int i = 0; i < 3; i++) {
        clustered.at<cv::Vec3b>(y, x)[i] = centers.at<float>(cluster_index, i);
        imgs[cluster_index].at<cv::Vec3b>(y, x)[i] = centers.at<float>(cluster_index, i);
      }
    }
  }
  if (params.saveLog)
      cv::imwrite("logs/"+params.fileName.toStdString()+"_flat.png",clustered);
      for (int i = 0; i < K; i ++){
        cv::imwrite("logs/"+params.fileName.toStdString()+"_flat_"+std::to_string(i)+".png",imgs[i]);
      }

  return {imgs, colors};
}

void StippleViewer::stipple(const LBGStippling::Params params) {

  // perform k-means cluster and split img
  std::vector<cv::Mat> splitted;
  std::vector<std::vector<int>> colors;
  tie(splitted, colors) = kmeans_cluster(params);

  std::vector<std::vector<Stipple>> stipples;
  std::vector<double> times_stipple;
  std::vector<std::vector<int>> tsps;
  std::vector<double> times_tsp;

  for (int i = 0; i < params.colorNum; i++){
    std::vector<Stipple> stipple; double time_stipple;
    QImage img = Mat2QImage(splitted[i]);
    tie(stipple, time_stipple) = m_stippling.stipple(img, params, qRgb(colors[i][0],colors[i][1],colors[i][2]));
    stipples.push_back(stipple); times_stipple.push_back(time_stipple);
    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple_")+qstr(std::to_string(i))+".png");
  }
  displayPoints(stipples);
  this->scene()->clear();

  if (params.solveTSP) {
    for (int i = 0; i < params.colorNum; i++){
      std::vector<int> tsp; double time_tsp;
      tie(tsp, time_tsp) = m_TSP.solve(stipples[i]);
      tsps.push_back(tsp); times_tsp.push_back(time_tsp);
      if (params.interactiveDisplay) displayTSP(stipples[i], tsp);
      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp_")+qstr(std::to_string(i))+".png");
    }
  }
  displayTSP(stipples, tsps);
  if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp.png"));

  // LOGGING
  if (params.saveLog) {
    std::ofstream log("logs/"+std::string(params.fileName.toUtf8().constData())+"_log.txt");
    log << params.fileName.toUtf8().constData() << std::endl;
    log << "COLORS :\n";
    for (int i = 0; i < params.colorNum; i++){
      log << "Color " << std::to_string(i) << ": [" << colors[i][0] << "\t" << colors[i][1] << "\t" << colors[i][2] << "]\n";
    }
    log << "SIZE & TIME :\n";
    for (int i = 0; i < params.colorNum; i++){
      log << "Color " << std::to_string(i) << stipples[i].size() << "\t" << times_stipple[i] << " ms\t" << times_tsp[i] << "ms\n";
    }
    log.close();

    // save the robotic path
    for (int i = 0; i < params.colorNum; i++)
      m_Optimizing.saveRoboticPath (stipples[i], tsps[i], params.fileName, std::to_string(i), m_image.width(), m_image.height());

  }
/*

  if (params.colorSplit) {
    tie(stipple_1, time_stipple_1) = m_stippling.stipple(m_image_1, params, qRgb(params.color1_r, params.color1_g, params.color1_b)); //cyan
    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple_1.png"));
    tie(stipple_2, time_stipple_2) = m_stippling.stipple(m_image_2, params, qRgb(params.color2_r, params.color2_g, params.color2_b)); //magenta
    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple_2.png"));
    tie(stipple_3, time_stipple_3) = m_stippling.stipple(m_image_3, params, qRgb(params.color3_r, params.color3_g, params.color3_b)); //yellow
    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple_3.png"));
//    tie(stipple_k, time_stipple_k) = m_stippling.stipple(m_image_k, params, QColor(0,0,0,150)); //black
//    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple_k.png"));
    displayPoints(stipple_1, stipple_2, stipple_3, stipple_1);
    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple.png"));

    if (params.solveTSP) {
      this->scene()->clear();
      tie(tsp_1, time_tsp_1) = m_TSP.solve(stipple_1);
      if (params.interactiveDisplay) displayTSP(stipple_1, tsp_1);
      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp_1.png"));
      tie(tsp_2, time_tsp_2) = m_TSP.solve(stipple_2);
      if (params.interactiveDisplay) this->scene()->clear(); displayTSP(stipple_2, tsp_2);
      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp_2.png"));
      tie(tsp_3, time_tsp_3) = m_TSP.solve(stipple_3);
      if (params.interactiveDisplay) this->scene()->clear(); displayTSP(stipple_3, tsp_3);
      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp_3.png"));
//      tie(tsp_k, time_tsp_k) = m_TSP.solve(stipple_k);
//      if (params.interactiveDisplay) this->scene()->clear(); displayTSP(stipple_k, tsp_k);
//      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp_k.png"));
      displayTSP(stipple_1, tsp_1, stipple_2, tsp_2, stipple_3, tsp_3, stipple_1, tsp_1);
      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp.png"));
    }

    // LOGGING
    if (params.saveLog) {
      std::ofstream log("logs/"+std::string(params.fileName.toUtf8().constData())+"_log.txt");
      log << params.fileName.toUtf8().constData() << std::endl;
      log << "COLOR :\n";
      log << "Color1: " << params.color1_r << "\t" << params.color1_g << "\t" << params.color1_b << "\n";
      log << "Color2: " << params.color2_r << "\t" << params.color2_g << "\t" << params.color2_b << "\n";
      log << "Color3: " << params.color3_r << "\t" << params.color3_g << "\t" << params.color3_b << "\n\n";

      log << "SIZE & TIME :\n";
      log << "Color1 \t" << stipple_1.size() << "\t" << time_stipple_1  << " ms\t" << time_tsp_1 << "ms\n";
      log << "Color2 \t" << stipple_2.size() << "\t" << time_stipple_2 << " ms\t" << time_tsp_2 << "ms\n";
      log << "Color3 \t" << stipple_3.size() << "\t" << time_stipple_3 << " ms\t" << time_tsp_3 << "ms\n";
//      log << "K \t" << stipple_k.size() << "\t" << time_stipple_k << " ms\t" << time_tsp_k << "ms\n";
      log.close();

      // save the robotic path
      m_Optimizing.saveRoboticPath (stipple_1, tsp_1, params.fileName, "1", m_image.width(), m_image.height());
      m_Optimizing.saveRoboticPath (stipple_2, tsp_2, params.fileName, "2", m_image.width(), m_image.height());
      m_Optimizing.saveRoboticPath (stipple_3, tsp_3, params.fileName, "3", m_image.width(), m_image.height());
//      m_Optimizing.saveRoboticPath (stipple_k, tsp_k, params.fileName, "k", m_image.width(), m_image.height());

    }
  }


  // Split images in CMYK
//  m_image.convertToFormat(QImage::Format_RGBA8888);
  QImage m_image_1 = m_image.copy();
  QImage m_image_2 = m_image.copy();
  QImage m_image_3 = m_image.copy();

  for(int j = 0; j < m_image.height(); j++) {
    for(int i = 0; i < m_image.width(); i++) {
      int r, g, b;
      QColor pixel_color = m_image.pixel(i, j);
      pixel_color.getRgb(&r, &g, &b);

      if (abs(int(r - params.color1_r)) < 10 && abs(int(g - params.color1_g)) < 10 && abs(int(b - params.color1_b) < 10)) {
        m_image_1.setPixel(i, j, qRgb(r,g,b));
        m_image_2.setPixel(i, j, qRgb(255, 255, 255));
        m_image_3.setPixel(i, j, qRgb(255, 255, 255));
      }
      else if (abs(int(r - params.color2_r)) < 10 && abs(int(g - params.color2_g)) < 10 && abs(int(b - params.color2_b)) < 10) {
        m_image_1.setPixel(i, j, qRgb(255, 255, 255));
        m_image_2.setPixel(i, j, qRgb(r,g,b));
        m_image_3.setPixel(i, j, qRgb(255, 255, 255));
      }
      else if (abs(int(r - params.color3_r)) < 10 && abs(int(g - params.color3_g)) < 10 && abs(int(b - params.color3_b)) < 10) {
        m_image_1.setPixel(i, j, qRgb(255, 255, 255));
        m_image_2.setPixel(i, j, qRgb(255, 255, 255));
        m_image_3.setPixel(i, j, qRgb(r,g,b));
      }
      else {
        m_image_1.setPixel(i, j, qRgb(255, 255, 255));
        m_image_2.setPixel(i, j, qRgb(255, 255, 255));
        m_image_3.setPixel(i, j, qRgb(255, 255, 255));
      }
    }
  }

  std::vector<Stipple> stipple_1, stipple_2, stipple_3;
  double time_stipple_1 = 0, time_stipple_2 = 0, time_stipple_3 = 0;
  std::vector<int> tsp_1, tsp_2, tsp_3;
  double time_tsp_1 = 0, time_tsp_2= 0 , time_tsp_3 = 0;

  // TODO: Handle return value
  if (!params.colorSplit) {
//    if (params.saveLog) m_image_k.save(qstr("logs/")+params.fileName+qstr("_k.png"));
//
//    tie(stipple_k, time_stipple_k) =  m_stippling.stipple(m_image_k, params, Qt::black); //black
//    if (!params.interactiveDisplay) {
//      displayPoints(stipple_k);
//    }
//    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple.png"));
//
//    if (params.solveTSP) {
//      this->scene()->clear();
//      tie(tsp_k, time_tsp_k) = m_TSP.solve(stipple_k);
//      displayTSP(stipple_k, tsp_k);
//      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp.png"));
//    }
//
//    // LOGGING
//    if (params.saveLog) {
//      std::ofstream log("logs/"+std::string(params.fileName.toUtf8().constData())+"_log.txt");
//      log << params.fileName.toUtf8().constData() << std::endl;
//      log << "COLOR : BLACK\n";
//      log << "K \t" << stipple_k.size() << "\t" << time_stipple_k << " ms\t" << time_tsp_k << "ms\n";
//      log.close();
//
//      m_Optimizing.saveRoboticPath (stipple_k, tsp_k, params.fileName, "k", m_image.width(), m_image.height());
//    }
  }
  else {
    if (params.saveLog){
      m_image_1.save(qstr("logs/")+params.fileName+qstr("_1.png"));
      m_image_2.save(qstr("logs/")+params.fileName+qstr("_2.png"));
      m_image_3.save(qstr("logs/")+params.fileName+qstr("_3.png"));
    }

    tie(stipple_1, time_stipple_1) = m_stippling.stipple(m_image_1, params, qRgb(params.color1_r, params.color1_g, params.color1_b)); //cyan
    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple_1.png"));
    tie(stipple_2, time_stipple_2) = m_stippling.stipple(m_image_2, params, qRgb(params.color2_r, params.color2_g, params.color2_b)); //magenta
    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple_2.png"));
    tie(stipple_3, time_stipple_3) = m_stippling.stipple(m_image_3, params, qRgb(params.color3_r, params.color3_g, params.color3_b)); //yellow
    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple_3.png"));
//    tie(stipple_k, time_stipple_k) = m_stippling.stipple(m_image_k, params, QColor(0,0,0,150)); //black
//    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple_k.png"));
    displayPoints(stipple_1, stipple_2, stipple_3, stipple_1);
    if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_stipple.png"));

    if (params.solveTSP) {
      this->scene()->clear();
      tie(tsp_1, time_tsp_1) = m_TSP.solve(stipple_1);
      if (params.interactiveDisplay) displayTSP(stipple_1, tsp_1);
      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp_1.png"));
      tie(tsp_2, time_tsp_2) = m_TSP.solve(stipple_2);
      if (params.interactiveDisplay) this->scene()->clear(); displayTSP(stipple_2, tsp_2);
      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp_2.png"));
      tie(tsp_3, time_tsp_3) = m_TSP.solve(stipple_3);
      if (params.interactiveDisplay) this->scene()->clear(); displayTSP(stipple_3, tsp_3);
      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp_3.png"));
//      tie(tsp_k, time_tsp_k) = m_TSP.solve(stipple_k);
//      if (params.interactiveDisplay) this->scene()->clear(); displayTSP(stipple_k, tsp_k);
//      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp_k.png"));
      displayTSP(stipple_1, tsp_1, stipple_2, tsp_2, stipple_3, tsp_3, stipple_1, tsp_1);
      if (params.saveLog) saveImagePNG(qstr("logs/")+params.fileName+qstr("_tsp.png"));
    }

    // LOGGING
    if (params.saveLog) {
      std::ofstream log("logs/"+std::string(params.fileName.toUtf8().constData())+"_log.txt");
      log << params.fileName.toUtf8().constData() << std::endl;
      log << "COLOR :\n";
      log << "Color1: " << params.color1_r << "\t" << params.color1_g << "\t" << params.color1_b << "\n";
      log << "Color2: " << params.color2_r << "\t" << params.color2_g << "\t" << params.color2_b << "\n";
      log << "Color3: " << params.color3_r << "\t" << params.color3_g << "\t" << params.color3_b << "\n\n";

      log << "SIZE & TIME :\n";
      log << "Color1 \t" << stipple_1.size() << "\t" << time_stipple_1  << " ms\t" << time_tsp_1 << "ms\n";
      log << "Color2 \t" << stipple_2.size() << "\t" << time_stipple_2 << " ms\t" << time_tsp_2 << "ms\n";
      log << "Color3 \t" << stipple_3.size() << "\t" << time_stipple_3 << " ms\t" << time_tsp_3 << "ms\n";
//      log << "K \t" << stipple_k.size() << "\t" << time_stipple_k << " ms\t" << time_tsp_k << "ms\n";
      log.close();

      // save the robotic path
      m_Optimizing.saveRoboticPath (stipple_1, tsp_1, params.fileName, "1", m_image.width(), m_image.height());
      m_Optimizing.saveRoboticPath (stipple_2, tsp_2, params.fileName, "2", m_image.width(), m_image.height());
      m_Optimizing.saveRoboticPath (stipple_3, tsp_3, params.fileName, "3", m_image.width(), m_image.height());
//      m_Optimizing.saveRoboticPath (stipple_k, tsp_k, params.fileName, "k", m_image.width(), m_image.height());

    }
  }
*/
  emit finished();
//  m_TSPSolver.solve(stipples);

}
