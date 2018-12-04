#include "filesystemmodel.h"
#include <QDebug>
#include <iostream>

FileSystemModel::FileSystemModel(QObject *parent) : QFileSystemModel (parent)
{
    this->setFilter(QDir::AllEntries);
}

int FileSystemModel::columnCount(const QModelIndex& parent) const
{
    return QFileSystemModel::columnCount(parent) + 3;
}

QVariant FileSystemModel::data(const QModelIndex& index, int role) const
{
   if (!index.isValid()) {
       return QFileSystemModel::data(index, role);
   }

   if (role == Qt::TextAlignmentRole)
       return Qt::AlignHCenter;

   /* For now we handle only display model */
   if (role != Qt::DisplayRole)
       return QFileSystemModel::data(index, role);

   switch (index.column()) {
   case SIZE:
       if (fileInfo(index).isDir())
           return 4096; // one block size
        break;
   case PERMISSIONS:
        return toString(fileInfo(index).permissions());
   case OWNER:
       return fileInfo(index).owner();
   case GROUP:
       return fileInfo(index).owner();
   default:
       return QFileSystemModel::data(index, role);
   }

   return QFileSystemModel::data(index, role);
}

QVariant FileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QFileSystemModel::headerData(section, orientation, role);

    switch (section) {
    case PERMISSIONS:
        return "Permissions";
    case OWNER:
        return "Owner";
    case GROUP:
        return "Group";
    default:
        return QFileSystemModel::headerData(section, orientation, role);
    }
}


QString FileSystemModel::toString(QFile::Permissions perm) {
    QString result = "---------";

    if (perm.testFlag(QFileDevice::ReadOwner)) {
        result[0] = 'r';
    }

    if (perm.testFlag(QFileDevice::WriteOwner)) {
        result[1] = 'w';
    }

    if (perm.testFlag(QFileDevice::ExeOwner)) {
        result[2] = 'x';
    }

    if (perm.testFlag(QFileDevice::ReadGroup)) {
        result[3] = 'r';
    }

    if (perm.testFlag(QFileDevice::WriteGroup)) {
        result[4] = 'w';
    }

    if (perm.testFlag(QFileDevice::ExeGroup)) {
        result[5] = 'x';
    }

    if (perm.testFlag(QFileDevice::ReadOther)) {
        result[6] = 'r';
    }

    if (perm.testFlag(QFileDevice::WriteOther)) {
        result[7] = 'w';
    }

    if (perm.testFlag(QFileDevice::ExeOther)) {
        result[8] = 'x';
    }

    return result;
}

bool FileSystemModel::copy(QModelIndex src, QModelIndex dst)
{
    if (!isDir(src)) {
        QString nPath = filePath(dst) + "/" + fileName(src);

        return QFile::copy(filePath(src), nPath);
    } else {
        // TODO: match regexp for recursive copying
        return copyPath(filePath(src), filePath(dst) + "/" + fileName(src), false);
    }
}

bool FileSystemModel::copyPath(QString sourceDir, QString destinationDir, bool overWriteDirectory)
{
    QDir originDirectory(sourceDir);

    if (!originDirectory.exists())
    {
        return false;
    }

    QDir destinationDirectory(destinationDir);

    if (destinationDirectory.exists() && !overWriteDirectory)
    {
        return false;
    }
    else if (destinationDirectory.exists() && overWriteDirectory)
    {
        destinationDirectory.removeRecursively();
    }

    originDirectory.mkpath(destinationDir);

    foreach (QString directoryName, originDirectory.entryList(QDir::Dirs |
                                                              QDir::NoDotAndDotDot))
    {
        QString destinationPath = destinationDir + "/" + directoryName;
        copyPath(sourceDir + "/" + directoryName, destinationPath, overWriteDirectory);
    }

    foreach (QString fileName, originDirectory.entryList(QDir::Files))
    {
        QFile::copy(sourceDir + "/" + fileName, destinationDir + "/" + fileName);
    }

    /*! Possible race-condition mitigation? */
    QDir finalDestination(destinationDir);
    finalDestination.refresh();

    if (finalDestination.exists())
    {
        return true;
    }

    return false;
}
