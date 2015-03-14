
#include <QApplication>
#include <QQuickView>
#include <QQmlContext>

#include <unistd.h>
#include <iostream>

#include "rsqml_main.h"

#include "models/gxsidmodel.h"
#include "models/forumgroupmodel.h"
#include "models/forummsgmodel.h"
#include "models/channelgroupmodel.h"
#include "models/channelmsgmodel.h"
#include "models/postedgroupmodel.h"
#include "models/postedmsgmodel.h"

int rsqml_main(int argc, char **argv)
{
    	QApplication app(argc, argv);

	QQuickView *view = new QQuickView;
	view->setResizeMode(QQuickView::SizeRootObjectToView);

	QQmlContext *ctxt = view->rootContext();

	GxsIdModel gxsIdModel;
	gxsIdModel.updateEntries();

	ForumGroupModel forumGroupModel;
        forumGroupModel.updateEntries();

	ChannelGroupModel channelGroupModel;
        channelGroupModel.updateEntries();

	PostedGroupModel postedGroupModel;
        postedGroupModel.updateEntries();

	ForumMsgModel forumMsgModel;
	ChannelMsgModel channelMsgModel;
	PostedMsgModel postedMsgModel;

	ctxt->setContextProperty("gxsIdModel", &gxsIdModel);
	ctxt->setContextProperty("forumGroupModel", &forumGroupModel);
	ctxt->setContextProperty("channelGroupModel", &channelGroupModel);
	ctxt->setContextProperty("postedGroupModel", &postedGroupModel);

	ctxt->setContextProperty("forumMsgModel", &forumMsgModel);
	ctxt->setContextProperty("channelMsgModel", &channelMsgModel);
	ctxt->setContextProperty("postedMsgModel", &postedMsgModel);

	view->setSource(QUrl("qrc:/qml/main.qml"));
	view->show();


	return app.exec();
}



