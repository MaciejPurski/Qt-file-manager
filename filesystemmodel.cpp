#include "filesystemmodel.h"

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

    for (int i = 0; i < 9; i++) {
        if (perm & (1 << i)) {
            switch (i % 3) {
            case 0:
                result[8 - i] = 'x';
                break;
            case 1:
                result[8 - i] = 'w';
                break;
            case 2:
                result[8 - i] = 'r';
                break;
            }
        }
    }

    return result;
}
