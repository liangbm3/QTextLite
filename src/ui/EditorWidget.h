#ifndef UI_EDITORWIDGET_H
#define UI_EDITORWIDGET_H

#include <QPlainTextEdit>
#include <QObject>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class LineNumberArea; // 前向声明 LineNumberArea 类
class QWheelEvent;

class EditorWidget : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit EditorWidget(QWidget *parent = nullptr);

    //公共接口，供LineNumberArea回调
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
protected:
    //重写事件处理函数
    void resizeEvent(QResizeEvent *event) override;
    //重写鼠标滚轮事件处理函数
    void wheelEvent(QWheelEvent *event) override;
public slots:
    void zoomIn();   //放大字体
    void zoomOut();  //缩小字体
    void resetZoom();//用于重置的槽函数
private slots:
    //行号区域大小改变时的处理函数
    void updateLineNumberAreaWidth(int newBlockCount);
    //文本内容改变时的处理函数
    void updateLineNumberArea(const QRect &rect, int dy);
    //高亮当前行
    void highlightCurrentLine();
private:
    QWidget *m_lineNumberArea; // 行号区域
    QFont m_defaultFont; // 默认字体
};

#endif // UI_EDITORWIDGET_H