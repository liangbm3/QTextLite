#include "ui/widgets/LineNumberArea.h"
#include "ui/EditorWidget.h"
#include <QPainter>
#include <QTextBlock>
#include <QDebug>

LineNumberArea::LineNumberArea(EditorWidget* editor)
    : QWidget(editor), m_editor(editor)
{
    //设置背景色等属性
    setBackgroundRole(QPalette::ColorRole::Light);
    setAutoFillBackground(true);
}

QSize LineNumberArea::sizeHint() const
{
    //调用编辑器来计算并返回推荐的宽度
    return QSize(m_editor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    //调用编辑器的绘制函数来绘制行号
    m_editor->lineNumberAreaPaintEvent(event);
}