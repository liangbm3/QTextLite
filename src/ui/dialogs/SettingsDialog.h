#ifndef UI_DIALOGS_SETTINGSDIALOG_H
#define UI_DIALOGS_SETTINGSDIALOG_H

#include <QDialog>

class QFontComboBox;
class QPushButton;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget* parent = nullptr);

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void applyChanges();

private:
    void loadCurrentSettings();
    
    QFontComboBox* m_fontComboBox;
    QPushButton* m_applyButton;
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
};

#endif // UI_DIALOGS_SETTINGSDIALOG_H