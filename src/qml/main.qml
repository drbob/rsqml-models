import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1
import "."

Rectangle {
    id: page
    width: 600; height: 400
    color: "#FFFFFF"

    Rectangle {
        id: header
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        height: 50

        ApplicationBar {
            id: status

            AppButton {
                icon: "icons/contacts-128.png"
                onButtonClicked : {
                    tabView.currentIndex = 0
                }
            }

            AppButton {
                icon: "icons/settings-4-128.png"
                onButtonClicked : {
                    tabView.currentIndex = 1
                }
            }

            AppButton {
                icon: "icons/email-128.png"
                onButtonClicked : {
                    tabView.currentIndex = 2
                }
            }

            AppButton {
                icon: "icons/star-2-128.png"
                onButtonClicked : {
                    tabView.currentIndex = 3
                }
            }

        }

    }

    TabView {
        id: tabView
        width: parent.width
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        tabsVisible: false

        Tab {
            id: gxsIds

            GxsService {
                title: "Friends"

                groupDelegate: GxsIdDelegate {}
                groupModel: gxsIdModel
            }
        }

        Tab {
            id: forum

            GxsService {
                title: "Forums"

                // This one uses the default GxsGroupDelegate.
                groupModel: forumGroupModel

                msgDelegate: ForumMsgDelegate {}
                msgModel: forumMsgModel
            }
        }

        Tab {
            id: channelLinks
            GxsService {
                title: "Channels"

                // custom GroupDelegate.
                groupDelegate: ChannelGroupDelegate {}
                groupModel: channelGroupModel

                msgDelegate: ChannelMsgDelegate {}
                msgModel: channelMsgModel
            }
        }

        Tab {
            id: postedLinks

            GxsService {
                title: "Posted"

                // This one uses the default GxsGroupDelegate.
                groupModel: postedGroupModel

                msgDelegate: PostedMsgDelegate {}
                msgModel: postedMsgModel
            }
        }
    }
}
    
    
    
    
