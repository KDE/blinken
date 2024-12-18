/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "maskedmousearea.h"

#include <QGuiApplication>
#include <QStyleHints>
#include <qqmlfile.h>


MaskedMouseArea::MaskedMouseArea(QQuickItem *parent)
    : QQuickItem(parent), m_pressed(false), m_alphaThreshold(0.0),
      m_containsMouse(false) {
  setAcceptHoverEvents(true);
  setAcceptedMouseButtons(Qt::LeftButton);
}

void MaskedMouseArea::setPressed(bool pressed) {
  if (m_pressed != pressed) {
    m_pressed = pressed;
    Q_EMIT pressedChanged();
  }
}

void MaskedMouseArea::setContainsMouse(bool containsMouse) {
  if (m_containsMouse != containsMouse) {
    m_containsMouse = containsMouse;
    Q_EMIT containsMouseChanged();
  }
}

void MaskedMouseArea::setMaskSource(const QUrl &source) {
  if (m_maskSource != source) {
    m_maskSource = source;
    m_maskImage = QImage(QQmlFile::urlToLocalFileOrQrc(source));
    Q_EMIT maskSourceChanged();
  }
}

void MaskedMouseArea::setAlphaThreshold(qreal threshold) {
  if (m_alphaThreshold != threshold) {
    m_alphaThreshold = threshold;
    Q_EMIT alphaThresholdChanged();
  }
}

bool MaskedMouseArea::contains(const QPointF &point) const {
  if (!QQuickItem::contains(point) || m_maskImage.isNull())
    return false;
  
  QPoint p = point.toPoint();

  //get the scaled image
  QImage transImage=m_maskImage.scaled(QSize(this->width(),this->height()),Qt::IgnoreAspectRatio,Qt::FastTransformation);
    if (p.x() < 0 || p.x() >= transImage.width() || p.y() < 0 ||
      p.y() >= transImage.height())
    return false;

  qreal r = qBound<int>(0, m_alphaThreshold * 255, 255);
 
  return qAlpha(transImage.pixel(p)) > r;
}

void MaskedMouseArea::mousePressEvent(QMouseEvent *event) {
  setPressed(true);
  m_pressPoint = event->position().toPoint();
  Q_EMIT pressed();
}

void MaskedMouseArea::mouseReleaseEvent(QMouseEvent *event) {
  setPressed(false);
  Q_EMIT released();

  const int threshold = qApp->styleHints()->startDragDistance();
  const bool isClick =
      (threshold >= qAbs(event->position().toPoint().x() - m_pressPoint.x()) &&
       threshold >= qAbs(event->position().toPoint().y() - m_pressPoint.y()));

  if (isClick)
    Q_EMIT clicked();
}

void MaskedMouseArea::mouseUngrabEvent() {
  setPressed(false);
  Q_EMIT canceled();
}

void MaskedMouseArea::hoverEnterEvent(QHoverEvent *event) {
  Q_UNUSED(event);
  setContainsMouse(true);
}

void MaskedMouseArea::hoverLeaveEvent(QHoverEvent *event) {
  Q_UNUSED(event);
  setContainsMouse(false);
}

#include "moc_maskedmousearea.cpp"
