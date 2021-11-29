#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <QObject>

class Localization : public QObject
{
    Q_OBJECT
public:
    explicit Localization(QObject *parent = nullptr);

signals:

};

#endif // LOCALIZATION_H
