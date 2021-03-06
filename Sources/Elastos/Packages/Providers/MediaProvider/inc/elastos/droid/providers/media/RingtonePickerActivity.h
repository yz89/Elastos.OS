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

#ifndef _ELASTOS_DROID_PROVIDERS_MEDIA_RINGTONEPICKERACTIVITY_H__
#define _ELASTOS_DROID_PROVIDERS_MEDIA_RINGTONEPICKERACTIVITY_H__

#include "_Elastos.Droid.Providers.Media.h"
#include "Elastos.Droid.Widget.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.Internal.h"
#include <elastos/core/Runnable.h>
#include <elastos/droid/internal/app/AlertActivity.h>

using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Internal::App::AlertActivity;
using Elastos::Droid::Internal::App::IAlertControllerAlertParamsOnPrepareListViewListener;
using Elastos::Droid::Media::IRingtone;
using Elastos::Droid::Media::IRingtoneManager;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IBundle;
using Elastos::Core::Runnable;
using Elastos::Droid::View::IView;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Widget::IAdapterViewOnItemSelectedListener;
using Elastos::Droid::Widget::IListView;
using Elastos::Core::IRunnable;

namespace Elastos {
namespace Droid {
namespace Providers {
namespace Media {

class RingtonePickerActivity
    : public AlertActivity
    , public IRingtonePickerActivity
{
private:
    class InnerListener
        : public Object
        , public IDialogInterfaceOnClickListener
        , public IAdapterViewOnItemSelectedListener
        , public IAlertControllerAlertParamsOnPrepareListViewListener
    {
    public:
        CAR_INTERFACE_DECL()

        InnerListener(
            /* [in] */ RingtonePickerActivity* host);

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

        CARAPI OnPrepareListView(
            /* [in] */ IListView* listView);

        CARAPI OnItemSelected(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

        CARAPI OnNothingSelected(
            /* [in] */ IAdapterView* parent);
    private:
        RingtonePickerActivity* mHost;
    };

    class InnerRunnable
        : public Runnable
    {
    public:
        InnerRunnable(
            /* [in] */ RingtonePickerActivity* host);

        CARAPI Run();

    private:
        RingtonePickerActivity* mHost;
    };

    class RingtoneClickListener
        : public Object
        , public IDialogInterfaceOnClickListener
    {
    public:
        CAR_INTERFACE_DECL()

        RingtoneClickListener(
            /* [in] */ RingtonePickerActivity* host);

        /*
         * On item clicked
         */
        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        RingtonePickerActivity* mHost;
    };

    class MyRunnable
        : public Runnable
    {
        friend class RingtonePickerActivity;
    public:
        MyRunnable(
            /* [in] */ RingtonePickerActivity* host);

        CARAPI Run();

    private:
        RingtonePickerActivity* mHost;
    };

public:
    CAR_INTERFACE_DECL()

    RingtonePickerActivity();

    virtual ~RingtonePickerActivity();

    CARAPI constructor();

    CARAPI OnSaveInstanceState(
        /* [in] */ IBundle* outState);

    CARAPI OnPrepareListView(
        /* [in] */ IListView* listView);

    CARAPI OnClick(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 which);

    CARAPI OnItemSelected(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    CARAPI OnNothingSelected(
        /* [in] */ IAdapterView* parent);

    CARAPI Run();

protected:
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStop();

    CARAPI OnDestroy();

    CARAPI OnPause();

private:
    CARAPI_(Int32) AddStaticItem(
        /* [in] */ IListView* listView,
        /* [in] */ Int32 textResId);

    CARAPI_(Int32) AddDefaultRingtoneItem(
        /* [in] */ IListView* listView);

    CARAPI_(Int32) AddSilentItem(
        /* [in] */ IListView* listView);

    CARAPI_(void) PlayRingtone(
        /* [in] */ Int32 position,
        /* [in] */ Int32 delayMs);

    CARAPI_(void) SaveAnyPlayingRingtone();

    CARAPI_(void) StopAnyPlayingRingtone();

    CARAPI_(Int32) GetRingtoneManagerPosition(
        /* [in] */ Int32 listPos);

    CARAPI_(Int32) GetListPosition(
        /* [in] */ Int32 ringtoneManagerPos);

private:
    static const Int32 POS_UNKNOWN;

    static const String TAG;

    static const Int32 DELAY_MS_SELECTION_PLAYED;

    static const String SAVE_CLICKED_POS;

    AutoPtr<IRingtoneManager> mRingtoneManager;
    Int32 mType;

    AutoPtr<ICursor> mCursor;
    AutoPtr<IHandler> mHandler;

    /** The position in the list of the 'Silent' item. */
    Int32 mSilentPos;

    /** The position in the list of the 'Default' item. */
    Int32 mDefaultRingtonePos;

    /** The position in the list of the last clicked item. */
    Int32 mClickedPos;

    /** The position in the list of the ringtone to sample. */
    Int32 mSampleRingtonePos;

    /** Whether this list has the 'Silent' item. */
    Boolean mHasSilentItem;

    /** The Uri to place a checkmark next to. */
    AutoPtr<IUri> mExistingUri;

    /** The number of static items in the list. */
    Int32 mStaticItemCount;

    /** Whether this list has the 'Default' item. */
    Boolean mHasDefaultItem;

    /** The Uri to play when the 'Default' item is clicked. */
    AutoPtr<IUri> mUriForDefaultItem;

    /**
     * A Ringtone for the default ringtone. In most cases, the RingtoneManager
     * will stop the previous ringtone. However, the RingtoneManager doesn't
     * manage the default ringtone for us, so we should stop this one manually.
     */
    AutoPtr<IRingtone> mDefaultRingtone;

    /**
     * The ringtone that's currently playing, unless the currently playing one is the default
     * ringtone.
     */
    AutoPtr<IRingtone> mCurrentRingtone;

    /**
     * Keep the currently playing ringtone around when changing orientation, so that it
     * can be stopped later, after the activity is recreated.
     */
    static AutoPtr<IRingtone> sPlayingRingtone;

    // Whether we have tap the "OK" or "Cancel" button.
    Boolean mIsHasClick;// = false;

    AutoPtr<InnerRunnable> mInnerRunnable;
    AutoPtr<InnerListener> mInnerListener;
    AutoPtr<RingtoneClickListener> mRingtoneClickListener;
};

} // namespace Media
} // namespace Providers
} // namespace Droid
} // namespace Elastos

#endif //_ELASTOS_DROID_PROVIDERS_MEDIA_RINGTONEPICKERACTIVITY_H__