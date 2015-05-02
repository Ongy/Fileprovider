#include "fileprovider.h"

#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QDir>
#include <QFileDialog>


FileProvider::FileProvider(QObject *parent) :
    QObject(parent)
  , chapter(1), panel(0)
  , manga("")
  , path("")
{

}

int FileProvider::loadManga(const QString & name)
{
    bool ok;
    this->path = name;
    QFile info(this->path + "/.info");
    if(info.open(QIODevice::ReadOnly)) {
        QTextStream in(&info);
        this->manga = in.readLine();
        while(!in.atEnd()) {
            QString line = in.readLine();
            this->chapters.append(line.toInt(&ok));
            if(!ok) {
                qDebug("Could not convert %s int a number", line.toStdString().c_str());
                return -1;
            }
        }
    }
    else {
        qDebug("Could not find info file");
        return -1;
    }
    QSettings settings(APPNAME, "fileprovider");
    settings.beginGroup(this->manga);
    this->chapter = settings.value("chapter", 1).toInt();
    this->panel = settings.value("panel", 0).toInt();
    settings.endGroup();
    return 0;
}

QStringList FileProvider::getMangas()
{
    return QStringList();
}

QString FileProvider::getPath() const
{
    char buffer[128];
    QString path = this->path;
    snprintf(buffer, sizeof(buffer), "/%d/%d.jpg", this->chapter, this->panel);
    path.append(buffer);
    return path;
}

int FileProvider::nextImage(QPixmap * pixmap)
{
    int tmp = this->panel;
    if(++this->panel > this->chapters[this->chapter -1]) {
        this->panel = 1;
        if(++this->chapter > this->chapters.length()) {
            --this->chapter;
            this->panel = tmp;
            return -1;
        }
    }

    QString path = getPath();
    new(pixmap) QPixmap(path);
    return 0;
}

int FileProvider::prevImage(QPixmap *pixmap)
{
    if(this->backOff() < 0)
        return -1;
    QString path = getPath();
    new(pixmap) QPixmap(path);
    return 0;
}

int FileProvider::backOff()
{
    if(!--this->panel) {
        if(!--this->chapter) {
            this->panel = 1;
            this->chapter = 1;
            return -1;
        }
        this->panel = this->chapters[this->chapter-1];
    }
    return 0;
}

FileProvider::~FileProvider()
{
    this->backOff();
    QSettings settings(APPNAME, "fileprovider");
    settings.beginGroup(this->manga);
    settings.setValue("chapter", this->chapter);
    settings.setValue("panel", this->panel);
    settings.endGroup();
}

const QString & FileProvider::getName() const
{
    static QString name = "File provider";
    return name;
}

QString FileProvider::displayChooser()
{

    QString home = QDir::homePath();
    QString manga = QFileDialog::getExistingDirectory(NULL, "Choose which manga to view", home);
    return manga;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Fileprovider, Fileprovider)
#endif // QT_VERSION < 0x050000
