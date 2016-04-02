/*
  * Copyright (C) 2013 The Android Open Source Project
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *      http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  */

#ifndef __ELASTOS_DROID_SERVER_WIFI_WIFINOTIFICATIONCONTROLLER_H__
#define __ELASTOS_DROID_SERVER_WIFI_WIFINOTIFICATIONCONTROLLER_H__

#include "elastos/droid/ext/frameworkext.h"

// package com.android.server.wifi;
// import android.app.Notification;
// import android.app.NotificationManager;
// import android.app.TaskStackBuilder;
// import android.content.BroadcastReceiver;
// import android.content.ContentResolver;
// import android.content.Context;
// import android.content.Intent;
// import android.content.IntentFilter;
// import android.database.ContentObserver;
// import android.net.NetworkInfo;
// import android.net.wifi.ScanResult;
// import android.net.wifi.WifiManager;
// import android.os.Handler;
// import android.os.Message;
// import android.os.UserHandle;
// import android.provider.Settings;
// import java.io.FileDescriptor;
// import java.io.PrintWriter;
// import java.util.List;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Os::IHandler;
using Elastos::Io::IFileDescriptor;
using Elastos::Io::IPrintWriter;
using Elastos::Droid::Net::INetworkInfo;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wifi {

/* Takes care of handling the "open wi-fi network available" notification @hide */
class WifiNotificationController
    : public Object
{
private:
    class InnerBroadcastReceiver1
        : public Object
        , public IBroadcastReceiver
    {
    public:
        InnerBroadcastReceiver1(
            /* [in] */ WifiNotificationController* owner);

        // @Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        WifiNotificationController* mOwner;
    };

    class NotificationEnabledSettingObserver
        : public Object
        , public IContentObserver
    {
    public:
        NotificationEnabledSettingObserver(
            /* [in] */ IHandler* handler);

        virtual CARAPI Register();

        // @Override
        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:
        CARAPI_(Boolean) GetValue();
    };

public:
    WifiNotificationController(
        /* [in] */ IContext* context,
        /* [in] */ WifiStateMachine* wsm);

    virtual CARAPI Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args);

private:
    // synchronized
    CARAPI_(void) CheckAndSetNotification(
        /* [in] */ INetworkInfo* networkInfo,
        /* [in] */ IList<IScanResult>* scanResults);

    /**
      * Clears variables related to tracking whether a notification has been
      * shown recently and clears the current notification.
      */
    // synchronized
    CARAPI_(void) ResetNotification();

    /**
      * Display or don't display a notification that there are open Wi-Fi networks.
      * @param visible {@code true} if notification should be visible, {@code false} otherwise
      * @param numNetworks the number networks seen
      * @param force {@code true} to force notification to be shown/not-shown,
      * even if it is already shown/not-shown.
      * @param delay time in milliseconds after which the notification should be made
      * visible or invisible.
      */
    CARAPI_(void) SetNotificationVisible(
        /* [in] */ Boolean visible,
        /* [in] */ Int32 numNetworks,
        /* [in] */ Boolean force,
        /* [in] */ Int32 delay);

private:
    /**
      * The icon to show in the 'available networks' notification. This will also
      * be the ID of the Notification given to the NotificationManager.
      */
    static const Int32 ICON_NETWORKS_AVAILABLE = com.android.internal.R.drawable.stat_notify_wifi_in_range;
    /**
      * When a notification is shown, we wait this amount before possibly showing it again.
      */
    /*const*/ Int64 NOTIFICATION_REPEAT_DELAY_MS;
    /**
      * Whether the user has set the setting to show the 'available networks' notification.
      */
    Boolean mNotificationEnabled;
    /**
      * Observes the user setting to keep {@link #mNotificationEnabled} in sync.
      */
    AutoPtr<NotificationEnabledSettingObserver> mNotificationEnabledSettingObserver;
    /**
      * The {@link System#currentTimeMillis()} must be at least this value for us
      * to show the notification again.
      */
    Int64 mNotificationRepeatTime;
    /**
      * The Notification object given to the NotificationManager.
      */
    AutoPtr<INotification> mNotification;
    /**
      * Whether the notification is being shown, as set by us. That is, if the
      * user cancels the notification, we will not receive the callback so this
      * will still be true. We only guarantee if this is false, then the
      * notification is not showing.
      */
    Boolean mNotificationShown;
    /**
      * The number of continuous scans that must occur before consider the
      * supplicant in a scanning state. This allows supplicant to associate with
      * remembered networks that are in the scan results.
      */
    static const Int32 NUM_SCANS_BEFORE_ACTUALLY_SCANNING = 3;
    /**
      * The number of scans since the last network state change. When this
      * exceeds {@link #NUM_SCANS_BEFORE_ACTUALLY_SCANNING}, we consider the
      * supplicant to actually be scanning. When the network state changes to
      * something other than scanning, we reset this to 0.
      */
    Int32 mNumScansSinceNetworkStateChange;
    /*const*/ AutoPtr<IContext> mContext;
    /*const*/ AutoPtr<WifiStateMachine> mWifiStateMachine;
    AutoPtr<INetworkInfo> mNetworkInfo;
    AutoPtr<volatile> Int32 mWifiState;
};

} // namespace Wifi
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_WIFI_WIFINOTIFICATIONCONTROLLER_H__
