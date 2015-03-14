
#include "gxsidmodel.h"
#include <retroshare/rsidentity.h>

#include <iostream>

#include <QPixmap>


GxsIdModel::GxsIdModel(QObject *parent)
    : GxsGroupModel(rsIdentity->getTokenService(), parent)
{
}


void GxsIdModel::load(uint32_t token)
{
	beginResetModel();

	mEntries.clear();

	// sort them into Subscription order.
        std::vector<RsGxsIdGroup> entries;
	rsIdentity->getGroupData(mToken, entries);

        std::vector<RsGxsIdGroup>::iterator it;
        for(it = entries.begin(); it != entries.end();)
        {
		if (IS_GROUP_ADMIN(it->mMeta.mSubscribeFlags)) {
			mEntries.push_back(*it);
			it = entries.erase(it);
		} else {
			it++;
		}
	}

        for(it = entries.begin(); it != entries.end();)
        {
		if (IS_GROUP_SUBSCRIBED(it->mMeta.mSubscribeFlags)) {
			mEntries.push_back(*it);
			it = entries.erase(it);
		} else {
			it++;
		}
	}

	/* add in the last */
	mEntries.insert(mEntries.end(), entries.begin(), entries.end());

	endResetModel();
}


int GxsIdModel::rowCount(const QModelIndex & parent) const 
{
	return mEntries.size();
}


QVariant GxsIdModel::data(const QModelIndex & index, int role) const 
{
	int idx = index.row();
	if (idx < 0 || idx >= mEntries.size())
		return QVariant();

	if (role < GxsGroupModel::GroupExtraRole)
	{
		const RsGroupMetaData &peerData = mEntries[idx].mMeta;
		return GxsGroupModel::metadata(peerData, role);
	}

	const RsGxsIdGroup &entry = mEntries[idx];

	//std::cerr << "GxsIdModel::data() row: " << index.row() << " b#: " << bucketno << " e#: " << entryno;
	//std::cerr << std::endl;

	if (role == PgpKnownRole)
	{
		return QVariant(entry.mPgpKnown);
	}
	else if (role == PgpIdRole)
	{
		return QString::fromStdString(entry.mPgpId.toStdString());
	}
	else if (role == AvatarRole)
	{
		QPixmap pix ;
    		if (entry.mImage.mSize == 0 || !pix.loadFromData(entry.mImage.mData, entry.mImage.mSize, "PNG"))
		{
			QVariant();
		}
		return pix;
	}
	else if (role == ReputationRole)
	{
		return QVariant(entry.mReputation.mOverallScore);
	}
 
	return QVariant();
}


QHash<int, QByteArray> GxsIdModel::roleNames() const 
{
    QHash<int, QByteArray> roles = GxsGroupModel::roleNames();
    roles[PgpKnownRole] = "PgpKnown";
    roles[PgpIdRole] = "PgpId";
    roles[AvatarRole] = "Avatar";
    roles[ReputationRole] = "Reputation";

    return roles;
}

