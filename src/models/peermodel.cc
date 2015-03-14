
#include "peermodel.h"
#include <iostream>


PeerModel::PeerModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void PeerModel::addEntry(const RsDhtNetPeer &entry)
{
	beginResetModel();

	mEntries[entry.mDhtId] = entry;

	endResetModel();
}

void PeerModel::updateEntry(const RsDhtNetPeer &entry)
{
	std::map<std::string, RsDhtNetPeer>::iterator it;
	it = mEntries.find(entry.mDhtId);
	if (it == mEntries.end())
	{
		return addEntry(entry);
	}

	/* exists already */
	it->second = entry;
	/* which entry is it? */
	int index = 0;
	for(; it != mEntries.begin(); it--, index++) ;

	QModelIndex topLeft = createIndex(index,  0);
	QModelIndex bottomRight = createIndex(index, 0);
	emit dataChanged(topLeft, bottomRight);
}


int PeerModel::rowCount(const QModelIndex & parent) const {
    return mEntries.size();
}

QVariant PeerModel::data(const QModelIndex & index, int role) const 
{
	if (index.row() < 0 || index.row() >= mEntries.size())
		return QVariant();

	std::map<std::string, RsDhtNetPeer>::const_iterator it;
	int i = 0;
	for(it = mEntries.begin(); (it != mEntries.end()) && (i < index.row()); it++, i++) ;

	if (it == mEntries.end())
		return QVariant();

	const RsDhtNetPeer &entry = it->second;

	//std::cerr << "PeerModel::data() row: " << index.row() << " b#: " << bucketno << " e#: " << entryno;
	//std::cerr << std::endl;

	if (role == DhtIdRole)
		return QString::fromStdString(entry.mDhtId);
	else if (role == AddrRole)
		return QString::fromStdString("No Addr");
	else if (role == PeerIdRole)
		return QString::fromStdString(entry.mRsId);

	return QVariant();
}

QHash<int, QByteArray> PeerModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[DhtIdRole] = "dhtid";
    roles[AddrRole] = "addr";
    roles[PeerIdRole] = "peerid";
    return roles;
}

