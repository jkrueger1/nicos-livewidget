// *****************************************************************************
// NICOS, the Networked Instrument Control System of the FRM-II
// Copyright (c) 2009-2014 by the NICOS contributors (see AUTHORS)
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Module authors:
//   Georg Brandl <georg.brandl@frm2.tum.de>
//   Philipp Schmakat <philipp.schmakat@frm2.tum.de>
//
// *****************************************************************************

#ifndef LW_CONTROLS_H
#define LW_CONTROLS_H

#include "lw_common.h"

#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSlider>
#include <QSplitter>
#include <QSpinBox>
#include <QComboBox>
#include <QDir>
#include <QFile>
#include <QLineEdit>
#include <QListView>
#include <QFileSystemModel>

#include "qwt_plot_curve.h"

class LWProfileWindow;

#include "lw_widget.h"
#include "lw_histogram.h"
#include "lw_profile.h"
#include "lw_imageproc.h"

class LWControls : public QWidget
{
    Q_OBJECT

    friend class LWWidget;

  private:
    LWWidget *m_widget;
    bool m_sliderupdating;
    double m_absmin, m_absmax, m_absrange;
    double m_curmin, m_curmax, m_currange;

    double m_range_x[2];
    double m_range_y[2];

    double m_histogram_x[257];
    double m_histogram_y[257];

    double m_prof_x[2];
    double m_prof_y[2];
    int m_prof_type;

    void showProfWindow(const char *title);

  protected:
    QVBoxLayout *mainLayout;

    QCheckBox *gridBox;
    QCheckBox *logscaleBox;
    QCheckBox *grayscaleBox;
    QCheckBox *cyclicBox;
    QCheckBox *normalizeBox;
    QLineEdit *normalizedFile;
    QCheckBox *darkfieldBox;
    QLineEdit *darkfieldFile;
    QCheckBox *despeckleBox;
    QLabel *despeckleValueLabel;
    QSpinBox *despeckleValue;

    QComboBox *filterSelector;
    QComboBox *operationSelector;

    QPushButton *profileButton;
    QPushButton *profileHideButton;
    QSpinBox *profileWidth;
    QLabel *profileWidthLabel;
    QSpinBox *profileBins;
    QLabel *profileBinsLabel;

    QPushButton *xsumButton;
    QPushButton *ysumButton;

    QSlider *minSlider;
    QLabel *minSliderLabel;
    QSlider *maxSlider;
    QLabel *maxSliderLabel;
    QSlider *brtSlider;
    QLabel *brtSliderLabel;
    QSlider *ctrSlider;
    QLabel *ctrSliderLabel;

    QwtPlot *histoPlot;
    LWHistogramItem *histogram;
    QwtPlotCurve *histoRange;
    QwtPlotPicker *histoPicker;

    QLabel *spacerLabel;

    QLabel *filelistLabel;
    QLineEdit *filelistDirectory;
    QListView *filelistView;
    QFileSystemModel *filelistModel;

    QwtPlotCurve *profLine0;
    QwtPlotCurve *profLine1;
    QwtPlotCurve *profLine2;
    LWProfileWindow *profWindow;

    void setupUi();
    void setControls(LWCtrl which);
    void setAxisNames(const char *xaxis, const char *yaxis);

  protected slots:
    void pickRange(const QwtDoubleRect &);
    void updateMinMax();
    void updateBrightness(int);
    void updateContrast(int);
    void updateNormalizedFile();
    void updateDarkfieldFile();
    void updateDespeckleValue();
    void updateFilterSelector(int comboBoxValue);
    void updateOperationSelector(int comboBoxValue);
    void setLogscale(bool);
    void setColorMap();
    void setGrid(bool);
    void setNormalize(bool);
    void setDarkfieldSubtract(bool);
    void setDespeckle(bool);
    void dataUpdated(LWData *);
    void pickProfile();
    void hideProfileLine();
    void createProfile(const QwtArray<QwtDoublePoint> &);
    void updateProfWidth(int);
    void updateProfBins(int);
    void updateProfLineWidth(int);
    void zoomAdjusted();
    void createXSum();
    void createYSum();
    void listFiles();
    void selectFile(QModelIndex);
    void model_directoryLoaded(QString);

  public:
    LWControls(QWidget *parent = NULL);
    virtual ~LWControls();
};

#endif
