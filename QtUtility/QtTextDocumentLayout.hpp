/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef _qt_QTEXTDOCUMENTLAYOUT_P_H
#define _qt_QTEXTDOCUMENTLAYOUT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qtutility_global.hpp"
#include <QtGui/qabstracttextdocumentlayout.h>
#include <QtGui/qtextoption.h>
#include <QtGui/qtextobject.h>

//QT_BEGIN_NAMESPACE

class QTextListFormat;
class QTextTableCell;
class QTextDocumentLayoutPrivate;

class QTUTILITYSHARED_EXPORT QtTextDocumentLayout : 
    public QAbstractTextDocumentLayout
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QTextDocumentLayout)
    Q_PROPERTY(int cursorWidth READ cursorWidth WRITE setCursorWidth)
    Q_PROPERTY(qreal idealWidth READ idealWidth)
    Q_PROPERTY(bool contentHasAlignment READ contentHasAlignment)
public:
    explicit QtTextDocumentLayout(QTextDocument *doc);

    // from the abstract layout
    void draw(QPainter *painter, const PaintContext &context) Q_DECL_OVERRIDE;
    int hitTest(const QPointF &point, Qt::HitTestAccuracy accuracy) const Q_DECL_OVERRIDE;

    int pageCount() const Q_DECL_OVERRIDE;
    QSizeF documentSize() const Q_DECL_OVERRIDE;

    void setCursorWidth(int width);
    int cursorWidth() const;

    // internal, to support the ugly FixedColumnWidth wordwrap mode in QTextEdit
    void setFixedColumnWidth(int width);

    // internal for QTextEdit's NoWrap mode
    void setViewport(const QRectF &viewport);

    virtual QRectF frameBoundingRect(QTextFrame *frame) const Q_DECL_OVERRIDE;
    virtual QRectF blockBoundingRect(const QTextBlock &block) const Q_DECL_OVERRIDE;
    QRectF tableBoundingRect(QTextTable *table) const;
    QRectF tableCellBoundingRect(QTextTable *table, const QTextTableCell &cell) const;

    // ####
    int layoutStatus() const;
    int dynamicPageCount() const;
    QSizeF dynamicDocumentSize() const;
    void ensureLayouted(qreal);

    qreal idealWidth() const;

    bool contentHasAlignment() const;

protected:
    void documentChanged(int from, int oldLength, int length) override;
    void resizeInlineObject(QTextInlineObject item, int posInDocument, const QTextFormat &format) override;
    void positionInlineObject(QTextInlineObject item, int posInDocument, const QTextFormat &format) override;
    void drawInlineObject(QPainter *p, const QRectF &rect, QTextInlineObject item,
                          int posInDocument, const QTextFormat &format) override;
    virtual void timerEvent(QTimerEvent *e) override;
protected:
    virtual QRectF doLayout(int from, int oldLength, int length);
    virtual void layoutFinished();
protected:
    virtual void drawBorder(
        QPainter *painter,
        const QRectF &rect,
        qreal topMargin,
        qreal bottomMargin,
        qreal border,
        const QBrush &brush,
        QTextFrameFormat::BorderStyle style) const;
    virtual void drawBorderDetail(
        QPainter * painter,
        QTextFrameFormat::BorderStyle style,
        const QRectF &rect,
        qreal border,
        const QBrush &brush)const;
};

//QT_END_NAMESPACE

#endif // QTEXTDOCUMENTLAYOUT_P_H
