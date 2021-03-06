#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QObject>
#include <QFileSystemModel>

class FileSystemModel : public QFileSystemModel
{
    Q_OBJECT

public:
    FileSystemModel(QObject *parent = nullptr);

    static QString toString(QFile::Permissions perm);

    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool copy(QModelIndex src, QModelIndex dst);


    virtual ~FileSystemModel() { }

private:
    enum ColumnSection { NAME, SIZE, TYPE, MODIFIED, PERMISSIONS, OWNER, GROUP };
    bool copyPath(QString sourceDir, QString destinationDir, bool overWriteDirectory);
};

#endif // FILESYSTEMMODEL_H
