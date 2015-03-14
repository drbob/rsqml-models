
#include "dhtmodel.h"
#include <iostream>

#define DHT_BUCKET_COUNT 160U
#define DHT_BUCKET_SIZE  10U

DhtModel::DhtModel(QObject *parent)
    : QAbstractListModel(parent)
{
	mEntries.resize(DHT_BUCKET_COUNT);
	std::vector<std::vector<RsDhtPeer> >::iterator it;
	int i = 0;
	for(it = mEntries.begin(); it != mEntries.end(); it++, i++)
	{
		it->resize(DHT_BUCKET_SIZE);
		for(int j = 0; j < DHT_BUCKET_SIZE; j++)
		{
			RsDhtPeer peer;
			peer.mBucket = i;
			peer.mDhtId = "n/a";
			peer.mAddr = "None";
			(*it)[j] = peer;
		}
	}
}

void DhtModel::updateEntries(uint16_t bucketno, std::list<RsDhtPeer> &entries)
{
	if (bucketno >= DHT_BUCKET_COUNT)
	{
		return;
	}

	std::vector<RsDhtPeer> &bucket = mEntries[bucketno];

	/* sort policy ??? */
	std::list<RsDhtPeer>::iterator it;
	uint16_t i = 0;
	for(it = entries.begin(); (it != entries.end()) && (i < DHT_BUCKET_SIZE); it++, i++)
	{
		bucket[i] = *it;
	}

	// clear rest of the bucket.
	for(; i < DHT_BUCKET_SIZE; i++)
	{
		RsDhtPeer peer;
		peer.mBucket = bucketno;
		bucket[i] = peer;
	}

	QModelIndex topLeft = createIndex((DHT_BUCKET_COUNT - bucketno - 1) * DHT_BUCKET_SIZE, 0);
	QModelIndex bottomRight = createIndex((DHT_BUCKET_COUNT - bucketno) * DHT_BUCKET_SIZE, 0);
	emit dataChanged(topLeft, bottomRight);
}


int DhtModel::rowCount(const QModelIndex & parent) const {
    return mEntries.size() * DHT_BUCKET_SIZE;
}

QVariant DhtModel::data(const QModelIndex & index, int role) const 
{
	if (index.row() < 0 || index.row() >= mEntries.size() * DHT_BUCKET_SIZE)
		return QVariant();

	int bucketno = index.row() / DHT_BUCKET_SIZE;
	int entryno = index.row() - (bucketno * DHT_BUCKET_SIZE);

	//std::cerr << "DhtModel::data() row: " << index.row() << " b#: " << bucketno << " e#: " << entryno;
	//std::cerr << std::endl;

    	const std::vector<RsDhtPeer> &bucket = mEntries[DHT_BUCKET_COUNT - 1 - bucketno];
	const RsDhtPeer &entry = bucket[entryno];

	if (role == IdRole)
		return QString::fromStdString(entry.mDhtId);
	else if (role == AddrRole)
		return QString::fromStdString(entry.mAddr);
	else if (role == BucketRole)
		return QString::number(entry.mBucket);
	else if (role == PeerFlagsRole)
		return QString::number(entry.mPeerFlags, 16);
	else if (role == ExtraFlagsRole)
		return QString::number(entry.mExtraFlags, 16);

	return QVariant();
}



QHash<int, QByteArray> DhtModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[AddrRole] = "addr";
    roles[BucketRole] = "bucket";
    roles[PeerFlagsRole] = "peerflags";
    roles[ExtraFlagsRole] = "extraflags";
    return roles;
}

