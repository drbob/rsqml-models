#pragma once

#include <QAbstractListModel>
#include <QStringList>
#include <tryst/rsdht.h>

class PeerModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum PeerRoles {
        DhtIdRole = Qt::UserRole + 1,
        AddrRole,
        PeerIdRole
    };

    PeerModel(QObject *parent = 0);

    void addEntry(const RsDhtNetPeer &id);
    void updateEntry(const RsDhtNetPeer &update);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:

    std::map<std::string, RsDhtNetPeer> mEntries;
};


