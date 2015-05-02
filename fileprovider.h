#ifndef FILEPROVIDER_H
#define FILEPROVIDER_H

#include <QObject>
#include "mangaviewer/plugins.h"


class FileProvider : public QObject, public ImageProvider
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ongy.mangaviewer.imageprovider")
    Q_INTERFACES(ImageProvider)

    QList<int> chapters;
    QString getPath() const;
    int backOff();

    int chapter;
    int panel;
    QString manga;
    QString path;
public:
    FileProvider(QObject *parent = 0);
    ~FileProvider();
    int loadManga(const QString & name);
    QStringList getMangas();
    QString displayChooser();
    int nextImage(QPixmap *pixmap);
    int prevImage(QPixmap *pixmap);
    virtual const QString & getName() const;
};

#endif // FILEPROVIDER_H
