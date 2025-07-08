#include "ui/widgets/LineNumberArea.h"
#include "ui/EditorWidget.h"
#include <QPainter>
#include <QTextBlock>
#include <QDebug>

EditorWidget::EditorWidget(QWidget *parent)
    : QPlainTextEdit(parent)
{
    m_lineNumberArea = new LineNumberArea(this);

    // 连接信号和槽
    // 文本块的总行数发生变化时
    connect(this, &EditorWidget::blockCountChanged, this, &EditorWidget::updateLineNumberAreaWidth);
    // 视口内容更新时发出，滚动时，dy是垂直滚动的像素距离，或者文本的布局或格式发生变化时
    connect(this, &EditorWidget::updateRequest, this, &EditorWidget::updateLineNumberArea);
    // 光标位置发生变化时，用来实时行背景高亮
    connect(this, &EditorWidget::cursorPositionChanged, this, &EditorWidget::highlightCurrentLine);

    // 初始化边距和高亮
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    m_defaultFont=this->font(); // 保存默认字体
}

// 计算行号和所需宽度
int EditorWidget::lineNumberAreaWidth()
{
    int digits = 1;
    // blockCount() 返回当前文本的总行数
    int max = qMax(1, blockCount()); // 保证至少为1行
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }
    // 宽度 = 数字宽度 * 位数 + 一点点边距
    // 获取数字9的宽度，数字宽度都一样
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

// 更新行号区域的宽度，并设置编辑器的左边距
void EditorWidget::updateLineNumberAreaWidth(int /*newBlockCount*/)
{
    // 设置左边距的宽度，其他为0
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

// 当视口需要更新时（例如滚动时），此槽被调用
void EditorWidget::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
    {
        // 如果有垂直滚动，滚动行号区域
        m_lineNumberArea->scroll(0, dy);
    }
    else
    {
        // 否则重新绘制
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
    }

    // 如果行号区域包含视口的矩形，则更新行号区域的宽度
    if (rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth(0);
    }
}

// 当窗口大小改变时，调整行号区域的几何尺寸
// 重写了QPlainTextEdit的resizeEvent事件处理函数
void EditorWidget::resizeEvent(QResizeEvent *event)
{
    // 调用父类保证编辑器本身的布局和内容正常
    QPlainTextEdit::resizeEvent(event);
    // 获得编辑器的内容区域
    QRect cr = contentsRect();
    // 用这个区域重新设置行号区域
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

// 高亮光标所在的当前行
void EditorWidget::highlightCurrentLine()
{
    // 用于存储所有需要高亮的区域
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) // 如果编辑器不是只读状态
    {
        QTextEdit::ExtraSelection selection;        // 创建一个ExtraSelection对象
        QColor lineColor = QColor(255, 255, 0, 80); // 设置带透明度的淡黄色高亮，80为透明度

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true); // 整行高亮
        selection.cursor = textCursor();                                     // 让高亮区域与光标一致
        selection.cursor.clearSelection();                                   // 保证只高亮当前行，而不是选中内容
        extraSelections.append(selection);                                   // 把这个高亮区域加入列表
    }

    // 让编辑器显示高亮效果
    setExtraSelections(extraSelections);
}

// 被 LineNumberArea 回调的绘制函数
// 负责绘制编辑器左侧的行号区域
void EditorWidget::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray); // 设置背景颜色

    QTextBlock block = firstVisibleBlock(); // 获取第一个可见文本块
    // 计算每个可见文本块在屏幕上的垂直位置
    int blockNumber = block.blockNumber(); // 获取第一行文本的行号
    // 获取文本块的几何信息，相对于文本左上角，并把它转换为视口坐标
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingRect(block).height();
    // 遍历所有可见文本块，只要当前行还在重绘的区域，即还没有超出可见区域底部
    while (block.isValid() && top <= event->rect().bottom())
    {
        // 只有当该行是可见的，并且它的底部还在需要重绘的区域内，才绘制行号。
        if (block.isVisible() && bottom >= event->rect().top())
        {
            // 生成当前行号字符串
            QString number = QString::number(blockNumber + 1);
            // 设置画笔颜色为黑色
            painter.setPen(Qt::black);
            // 在行号的对应位置绘制行号文本
            painter.drawText(0, top, m_lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }
        block = block.next(); // 移动到下一个文本块
        // 更新下一行的顶部和底部像素位置，行号加1
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void EditorWidget::wheelEvent(QWheelEvent *e)
{
    // 检查滚轮事件发生时，Ctrl键是否被按下
    if (e->modifiers() & Qt::ControlModifier)
    {
        // 检查滚轮滚动的方向
        if (e->angleDelta().y() > 0)
        {
            // 正值表示向上滚动，我们调用 zoomIn() 放大
            this->zoomIn();
        }
        else
        {
            // 负值表示向下滚动，我们调用 zoomOut() 缩小
            this->zoomOut();
        }
        // 接受该事件，表示我们已经处理了它，它不应再被传递
        e->accept();
    }
    else
    {
        // 如果没有按下Ctrl键，我们调用基类的实现来处理正常的滚动
        QPlainTextEdit::wheelEvent(e);
    }
}

void EditorWidget::zoomIn()
{
    QFont font = this->font();
    int pointSize = font.pointSize();
    if (pointSize < 72) // 限制最大字体大小
    {
        font.setPointSize(pointSize + 1);
        this->setFont(font);
    }
}

void EditorWidget::zoomOut()
{
    QFont font = this->font();
    int pointSize = font.pointSize();
    if (pointSize > 6) // 限制最小字体大小
    {
        font.setPointSize(pointSize - 1);
        this->setFont(font);
    }
}

void EditorWidget::resetZoom()
{
    this->setFont(m_defaultFont); // 重置缩放级别为默认值
}