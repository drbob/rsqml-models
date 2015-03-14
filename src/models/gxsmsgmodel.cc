
#include "gxsmsgmodel.h"
#include <iostream>

#include <QTimer>


GxsMsgModel::GxsMsgModel(RsTokenService *service, QObject *parent)
:QAbstractListModel(parent), mService(service)
{ }


void GxsMsgModel::updateEntries(QString grpStrId)
{

	RsGxsGroupId grpId(grpStrId.toStdString());
	std::list<RsGxsGroupId> groupIds;
	groupIds.push_back(grpId);

	uint32_t token;
	RsTokReqOptions opts;
	opts.mReqType = GXS_REQUEST_TYPE_MSG_DATA;
        mService->requestMsgInfo(token, RS_TOKREQ_ANSTYPE_DATA, opts, groupIds);

	refresh(token);
}


void GxsMsgModel::refresh(uint32_t token)
{
        mToken = token;
	QTimer::singleShot(100, this, SLOT(check_load()));
}


void GxsMsgModel::check_load()
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


QVariant GxsMsgModel::metadata(const RsMsgMetaData &data, int role) const
{
	switch(role)
	{
		case GroupIdRole:
			return QString::fromStdString(data.mGroupId.toStdString());
			break;
		case MsgIdRole:
			return QString::fromStdString(data.mMsgId.toStdString());
			break;
		case ThreadIdRole:
			return QString::fromStdString(data.mThreadId.toStdString());
			break;
		case ParentIdRole:
			return QString::fromStdString(data.mParentId.toStdString());
			break;
		case OrigMsgIdRole:
			return QString::fromStdString(data.mOrigMsgId.toStdString());
			break;
		case AuthorIdRole:
			return QString::fromStdString(data.mAuthorId.toStdString());
			break;

		case MsgNameRole:
			return QString::fromStdString(data.mMsgName);
			break;
		case MsgFlagsRole:
			return QVariant(data.mMsgFlags);
			break;
		case MsgStatusRole:
			return QVariant(data.mMsgStatus);
			break;
		case PublishTsRole:
			return QVariant((int) data.mPublishTs);
			break;
		case ChildTsRole:
			return QVariant((int) data.mChildTs);
			break;
		default:
			break;
	}

	return QVariant();
}


QHash<int, QByteArray> GxsMsgModel::roleNames() const 
{
	QHash<int, QByteArray> roles;

	roles[GroupIdRole] = "GroupId";
	roles[MsgIdRole] = "MsgId";
	roles[ThreadIdRole] = "ThreadId";
	roles[ParentIdRole] = "ParentId";
	roles[OrigMsgIdRole] = "OrigMsgId";
	roles[AuthorIdRole] = "AuthorId";

	roles[MsgNameRole] = "MsgName";
	roles[PublishTsRole] = "PublishTs";
	roles[MsgFlagsRole] = "MsgFlags";
	roles[MsgStatusRole] = "MsgStatus";
	roles[ChildTsRole] = "ChildTs";

	return roles;
}

