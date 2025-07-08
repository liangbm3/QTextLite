#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

class QLineEdit;
class QCheckBox;
class QPushButton;

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();
    QString findText() const;
    bool isCaseSensitive() const;
    void setFindText(const QString &text);
    void focusOnFindLineEdit();

signals:
    // 定义信号，用于通知主窗口执行操作
    // 参数包含：查找字符串、查找选项
    void findNext(const QString &str, Qt::CaseSensitivity cs);
    void findPrevious(const QString &str, Qt::CaseSensitivity cs);
    void replace(const QString &str);
    void replaceAll(const QString &findStr, const QString &replaceStr, Qt::CaseSensitivity cs);
private slots:
    // 对话框内部的私有槽函数
    void onFindClicked();
    void onFindPreviousClicked();
    void onReplaceClicked();
    void onReplaceAllClicked();
    // 当查找文本改变时，控制按钮的可用状态
    void onTextChanged();

private:
    // UI控件
    QLineEdit *m_findLineEdit;
    QLineEdit *m_replaceLineEdit;
    QCheckBox *m_caseSensitiveCheckBox;
    QPushButton *m_findButton;
    QPushButton *m_findPreviousButton;
    QPushButton *m_replaceButton;
    QPushButton *m_replaceAllButton;
    QPushButton *m_closeButton;
};

#endif // FINDDIALOG_H