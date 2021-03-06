//=========================================================================
// Copyright (C) 2012 The Elastos Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=========================================================================

#ifndef __ELASTOS_DROID_EXT_FRAMEWORKHASH_H__
#define __ELASTOS_DROID_EXT_FRAMEWORKHASH_H__

#include <elastos/core/Object.h>

//
// Runtime
//
DEFINE_OBJECT_HASH_FUNC_FOR(IInterface)
DEFINE_OBJECT_HASH_FUNC_FOR(IWeakReference)

// Accounts
//
#ifdef HASH_FOR_ACCOUNTS
#include "Elastos.Droid.Accounts.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Accounts::IAccount)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Accounts::IAccountAndUser)
#endif

//
// App
//
#ifdef HASH_FOR_APP
#include "Elastos.Droid.App.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::App::IActivity)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::App::IAppOpsManagerOnOpChangedListener)
#endif

//
// Content
//
#ifdef HASH_FOR_CONTENT
#include "Elastos.Droid.Content.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::IBroadcastReceiver)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::IComponentName)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::IContext)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::IIntentFilterComparison)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::IServiceConnection)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::ISharedPreferencesOnSharedPreferenceChangeListener)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::Pm::ISignature)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::Res::IThemeChangeListener)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::Res::IThemeProcessingListener)
#endif

//
// Location
//
#ifdef HASH_FOR_LOCATION
#include "Elastos.Droid.Location.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Location::ICountryListener)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Location::IGpsStatusListener)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Location::IGpsStatusNmeaListener)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Location::ILocationListener)
#endif

//
// Net
//
#ifdef HASH_FOR_NET
#include "Elastos.Droid.Net.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Net::IUri)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Net::INetworkIdentity)
#endif

//
// Os
//
#ifdef HASH_FOR_OS
#include "Elastos.Droid.Os.h"
DEFINE_OBJECT_HASH_FUNC_USING_ADDR_FOR(Elastos::Droid::Os::IBinder)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Os::IMessenger)
#endif

//
// Graphics
//
#ifdef HASH_FOR_GRAPHICS
#include "Elastos.Droid.Graphics.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Graphics::IDrawable)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Graphics::IBitmap)
#endif

//
// View
//
#ifdef HASH_FOR_VIEW
#include "Elastos.Droid.View.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::View::IView)
#endif

//
// Widget
//
#ifdef HASH_FOR_WIDGET
#include "Elastos.Droid.Widget.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Widget::IGridLayoutSpec)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Widget::IInterval)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Widget::IRemoteViewsCacheKey)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Widget::IRemoteViewsFrameLayout)
#endif

//
//Text::Style
//
#ifdef HASH_FOR_TEXT_STYLE
#include "Elastos.Droid.Text.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Text::Style::ISuggestionSpan)
#endif

//
//TELECOM
//
#ifdef HASH_FOR_TELECOM
#include "Elastos.Droid.Telecom.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Telecom::ICall)
#endif

#endif // __ELASTOS_DROID_EXT_FRAMEWORKHASH_H__


