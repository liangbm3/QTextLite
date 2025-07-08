#ifndef CORE_APPSETTINGS_H
#define CORE_APPSETTINGS_H

#include "utils/Singleton.h"
#include <QFont>
#include <QObject>

class QSettings;

class AppSettings : public QObject, public Singleton<AppSettings>
{
    Q_OBJECT
    friend class Singleton<AppSettings>;

public:
    // --- Getter ---
    QFont editorFont() const;

public slots:
    // --- Setter ---
    void setEditorFont(const QFont &font);

signals:
    // 当任何设置项发生改变时，发射此信号
    void settingsChanged();

private:
    AppSettings(QObject* parent = nullptr);
    void load();

    QSettings* m_settings;
    QFont m_editorFont;
};

#endif // CORE_APPSETTINGS_H