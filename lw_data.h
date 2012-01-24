// *****************************************************************************
// NICOS-NG, the Networked Instrument Control System of the FRM-II
// Copyright (c) 2009-2011 by the NICOS-NG contributors (see AUTHORS)
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
//   Tobias Weber <tobias.weber@frm2.tum.de>
//   Georg Brandl <georg.brandl@frm2.tum.de>
//
// *****************************************************************************

#ifndef LW_DATA_H
#define LW_DATA_H

#include <stdint.h>

#include <qwt_plot_spectrogram.h>

// data type used for single pixel count values
typedef uint32_t data_t;


class LWData
{
  private:
    virtual void updateRange();
    virtual void initFromBuffer(const char *data);

  protected:
    // concerning the data
    bool m_data_owned;
    int m_width, m_height, m_depth;
    data_t *m_data;
    double m_min, m_max;

    // concerning the display
    int m_cur_z;
    bool m_log10;
    bool m_custom_range;
    double m_range_min, m_range_max;

    data_t data(int x, int y, int z) const;
    int size() const { return m_width * m_height * m_depth; }

  public:
    LWData();
    LWData(int width, int height, int depth, const char *data);
    LWData(int width, int height, int depth,
           const char *format, const char *data);
    LWData(const LWData &other);
    virtual ~LWData();

    const data_t *buffer() const { return m_data; }

    int width() const { return m_width; }
    int height() const { return m_height; }
    int depth() const { return m_depth; }
    double min() const { return m_min; }
    double max() const { return m_max; }

    int currentZ() const { return m_cur_z; }
    virtual void setCurrentZ(int val);

    bool isLog10() const { return m_log10; }
    virtual void setLog10(bool val);

    bool hasCustomRange() const { return m_custom_range; }
    double customRangeMin() const;
    double customRangeMax() const;
    virtual void setCustomRange(double lower, double upper);

    /// Get current presentation value at the specified point.
    virtual double value(double x, double y) const;
    /// Get raw value without regard to presentation settings (like log10).
    virtual double valueRaw(int x, int y) const;
    /// Same, but also specifying the layer.
    virtual double valueRaw(int x, int y, int z) const;

    /// Create a histogram of the current layer.  Caller must allocate
    /// the xs and ys arrays with a length of "bins".
    virtual void histogram(int bins, double *xs, double *ys) const;
    /// Same, but creates QVectors of doubles (callable from Python).
    virtual void histogram(int bins, QVector<double> **xs,
                           QVector<double> **ys) const;
};


class LWRasterData : public QwtRasterData
{
  private:
    const LWData *m_data;

  public:
    LWRasterData() :
        QwtRasterData(QwtDoubleRect(0, 1, 0, 0)),
        m_data(new LWData()) {
    }
    LWRasterData(const LWData *data) :
        QwtRasterData(QwtDoubleRect(0, data->width(), 0, data->height())),
        m_data(data) {
    }
    LWRasterData(const LWRasterData &other) :
        QwtRasterData(other), m_data(other.m_data) {
    }

    // QwtRasterData overridables
    virtual QwtRasterData *copy() const {
        return new LWRasterData(*this);
    }
    virtual QwtDoubleInterval range() const {
        if (m_data->hasCustomRange()) {
            return QwtDoubleInterval(m_data->customRangeMin(),
                                     m_data->customRangeMax());
        } else {
            return QwtDoubleInterval(m_data->min(), m_data->max());
        }
    }
    virtual double value(double x, double y) const {
        return m_data->value(x, y);
    }
    virtual double valueRaw(double x, double y) const {
        return m_data->valueRaw(x, y);
    }

    int width() {
        return m_data->width();
    }
    int height() {
        return m_data->height();
    }
};

#endif
