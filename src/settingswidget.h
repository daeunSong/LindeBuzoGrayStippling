#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QtWidgets>

#include "lbgstippling.h"

class StippleViewer;

class SettingsWidget : public QWidget {
 public:
  explicit SettingsWidget(StippleViewer *stippleViewer,
                          QWidget *parent = nullptr);

 private:
  LBGStippling::Params m_params;
  StippleViewer *m_stippleViewer;

  QSpinBox *m_spinCyan;
  QSpinBox *m_spinMagenta;
  QSpinBox *m_spinYellow;

  QPushButton *m_savePNG;
  QPushButton *m_saveSVG;
  QPushButton *m_savePDF;

  void enableSaveButtons();
  void disableSaveButtons();
  void toggleThresholds(bool checked);
};

#endif  // SETTINGSWIDGET_H
