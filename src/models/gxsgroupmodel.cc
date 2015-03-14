
#include "gxsgroupmodel.h"
#include <retroshare/rsgxsflags.h>
#include <iostream>

#include <QTimer>


GxsGroupModel::GxsGroupModel(RsTokenService *service, QObject *parent)
:QAbstractListModel(parent), mService(service)
{ }



void GxsGroupModel::updateEntries()
{
	uint32_t token;
	RsTokReqOptions opts;
	opts.mReqType = GXS_REQUEST_TYPE_GROUP_DATA;
        mService->requestGroupInfo(token, RS_TOKREQ_ANSTYPE_DATA, opts);

	refresh(token);
}


void GxsGroupModel::refresh(uint32_t token)
{
        mToken = token;
	QTimer::singleShot(100, this, SLOT(check_load()));
}


void GxsGroupModel::check_load()
{
        /* check token */
        uint32_t status =  mService->requestStatus(mToken);
	bool complete = (RsTokenService::GXS_REQUEST_V2_STATUS_FAILED == status) ||
                         (RsTokenService::GXS_REQUEST_V2_STATUS_COMPLETE == status);

	if (complete)
	{
		load(mToken);
	}
	else
	{
		QTimer::singleShot(100, this, SLOT(check_load()));
	}
}


QVariant GxsGroupModel::metadata(const RsGroupMetaData &data, int role) const
{
	switch(role)
	{
		case GroupIdRole:
			return QString::fromStdString(data.mGroupId.toStdString());
			break;
		case GroupNameRole:
			return QString::fromStdString(data.mGroupName);
			break;
		case GroupFlagsRole:
			return QVariant(data.mGroupFlags);
			break;
		case SignFlagsRole:
			return QVariant(data.mSignFlags);
			break;
		case PublishTsRole:
			return QVariant((int) data.mPublishTs);
			break;
		case AuthorIdRole:
			return QString::fromStdString(data.mAuthorId.toStdString());
			break;
		case CircleIdRole:
			return QString::fromStdString(data.mCircleId.toStdString());
			break;
		case CircleTypeRole:
			return QVariant(data.mCircleType);
			break;
		case AuthenFlagsRole:
			return QVariant(data.mAuthenFlags);
			break;
		case ParentIdRole:
			return QString::fromStdString(data.mParentGrpId.toStdString());
			break;
		case SubscribeFlagsRole:
			return QVariant(data.mSubscribeFlags);
			break;
		case SubscribeStatusRole:
			if (IS_GROUP_ADMIN(data.mSubscribeFlags))
			{
				return QString("Own Group");
			}
			else if (IS_GROUP_SUBSCRIBED(data.mSubscribeFlags))
			{
				return QString("Subscribed");
			}
			else 
			{
				return QString("Other");
			}
			break;
		default:
			break;
	}
	return QVariant();
}


QHash<int, QByteArray> GxsGroupModel::roleNames() const 
{
	QHash<int, QByteArray> roles;

	roles[GroupIdRole] = "GroupId";
	roles[GroupNameRole] = "GroupName";
	roles[GroupFlagsRole] = "GroupFlags";
	roles[SignFlagsRole] = "SignFlags";
	roles[PublishTsRole] = "PublishTs";
	roles[AuthorIdRole] = "AuthorId";
	roles[CircleIdRole] = "CircleId";
	roles[CircleTypeRole] = "CircleType";
	roles[AuthenFlagsRole] = "AuthenFlags";
	roles[ParentIdRole] = "ParentId";
	roles[SubscribeFlagsRole] = "SubscribeFlags";
	roles[SubscribeStatusRole] = "SubscribeStatus";

	return roles;
}

