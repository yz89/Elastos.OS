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

#ifndef __ELASTOS_DROID_SERVER_WM_APPWINDOWTOKEN_H__
#define __ELASTOS_DROID_SERVER_WM_APPWINDOWTOKEN_H__

#include "Elastos.CoreLibrary.h"
#include "elastos/droid/server/wm/WindowToken.h"
#include "elastos/droid/server/wm/StartingData.h"
#include "elastos/droid/server/input/InputApplicationHandle.h"
#include <elastos/utility/etl/List.h>

using Elastos::Droid::View::IApplicationToken;
using Elastos::Droid::View::IView;
using Elastos::Droid::Server::Input::InputApplicationHandle;
using Elastos::Utility::IArrayList;
using Elastos::Utility::Etl::List;

typedef IArrayList WindowList;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

class AppWindowAnimator;
class WindowAnimator;
class WindowState;

class AppWindowToken
    : public WindowToken
{
public:
    AppWindowToken();

    ~AppWindowToken();

    CARAPI constructor(
        /* [in] */ CWindowManagerService* service,
        /* [in] */ IApplicationToken* token,
        /* [in] */ Boolean voiceInteraction);

    CARAPI_(void) SendAppVisibilityToClients();

    CARAPI_(void) UpdateReportedVisibilityLocked();

    CARAPI_(AutoPtr<WindowState>) FindMainWindow();

    CARAPI_(Boolean) IsVisible();

    CARAPI_(void) RemoveAllWindows();

    CARAPI ToString(
        /* [out] */ String* str);

public:
    AutoPtr<IApplicationToken> mAppToken;

    // All of the windows and child windows that are included in this
    // application token.  Note this list is NOT sorted!
    AutoPtr<WindowList> mAllAppWindows;
    AutoPtr<AppWindowAnimator> mAppAnimator;

    WindowAnimator* mAnimator;

    Boolean mVoiceInteraction;

    Int32 mGroupId;
    Boolean mAppFullscreen;
    Int32 mRequestedOrientation;
    Boolean mLayoutConfigChanges;
    Boolean mShowWhenLocked;

    // The input dispatching timeout for this application token in nanoseconds.
    Int64 mInputDispatchingTimeoutNanos;

    // These are used for determining when all windows associated with
    // an activity have been drawn, so they can be made visible together
    // at the same time.
    Int64 mLastTransactionSequence;
    Int32 mNumInterestingWindows;
    Int32 mNumDrawnWindows;
    Boolean mInPendingTransaction;
    Boolean mAllDrawn;
    // Set to true when this app creates a surface while in the middle of an animation. In that
    // case do not clear allDrawn until the animation completes.
    Boolean mDeferClearAllDrawn;

    // Is this token going to be hidden in a little while?  If so, it
    // won't be taken into account for setting the screen orientation.
    Boolean mWillBeHidden;

    // Is this window's surface needed?  This is almost like hidden, except
    // it will sometimes be true a little earlier: when the token has
    // been shown, but is still waiting for its app transition to execute
    // before making its windows shown.
    Boolean mHiddenRequested;

    // Have we told the window clients to hide themselves?
    Boolean mClientHidden;

    // Last visibility state we reported to the app token.
    Boolean mReportedVisible;

    // Last drawn state we reported to the app token.
    Boolean mReportedDrawn;

    // Set to true when the token has been removed from the window mgr.
    Boolean mRemoved;

    // Information about an application starting window if displayed.
    AutoPtr<StartingData> mStartingData;
    WindowState* mStartingWindow;
    IView* mStartingView;
    Boolean mStartingDisplayed;
    Boolean mStartingMoved;
    Boolean mFirstWindowDrawn;

    // Input application handle used by the input dispatcher.
    AutoPtr<InputApplicationHandle> mInputApplicationHandle;

    Boolean mDeferRemoval;

    Boolean mLaunchTaskBehind;
    Boolean mEnteringAnimation;
};

} // Wm
} // Server
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_SERVER_WM_APPWINDOWTOKEN_H__
