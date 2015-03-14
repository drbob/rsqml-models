#pragma once

#include <QAbstractListModel>
#include <QStringList>
#include <tryst/rsdht.h>

class DhtModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DhtRoles {
        IdRole = Qt::UserRole + 1,
        AddrRole,
        BucketRole,
    	PeerFlagsRole,
    	ExtraFlagsRole
    };

    DhtModel(QObject *parent = 0);

    void updateEntries(uint16_t bucket, std::list<RsDhtPeer> &entries);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:

    std::vector<std::vector<RsDhtPeer> > mEntries;
};


