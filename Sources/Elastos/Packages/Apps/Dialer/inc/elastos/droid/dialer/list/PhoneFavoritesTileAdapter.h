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

#ifndef __ELASTOS_DROID_DIALER_LIST_PHONEFAVORITESTILEADAPTER_H__
#define __ELASTOS_DROID_DIALER_LIST_PHONEFAVORITESTILEADAPTER_H__

#include "_Elastos.Droid.Dialer.h"
#include "elastos/droid/widget/BaseAdapter.h"
#include "elastos/droid/contacts/common/list/ContactEntry.h"

using Elastos::Droid::Contacts::Common::IContactPhotoManager;
using Elastos::Droid::Contacts::Common::List::IContactTileViewListener;
using Elastos::Droid::Contacts::Common::List::ContactEntry;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::Widget::BaseAdapter;
using Elastos::Core::IComparator;
using Elastos::Utility::IArrayList;

namespace Elastos {
namespace Droid {
namespace Dialer {
namespace List {

/**
 * Also allows for a configurable number of columns as well as a maximum row of tiled contacts.
 */
class PhoneFavoritesTileAdapter
    : public BaseAdapter
    , public IOnDragDropListener
{
private:
    class ContactEntryComparator
        : public Object
        , public IComparator
    {
    public:
        CAR_INTERFACE_DECL()

        ContactEntryComparator(
            /* [in] */ PhoneFavoritesTileAdapter* host)
            : mHost(host)
        {}

        // @Override
        CARAPI Compare(
            /* [in] */ IInterface* lhs,
            /* [in] */ IInterface* rhs,
            /* [out] */ Int32* result);

    private:
        PhoneFavoritesTileAdapter* mHost;
    };

protected:
    class ViewTypes
    {
    public:
        static const Int32 TILE = 0;
        static const Int32 COUNT = 1;
    };

public:
    CAR_INTERFACE_DECL()

    PhoneFavoritesTileAdapter(
        /* [in] */ IContext* context,
        /* [in] */ IContactTileViewListener* listener,
        /* [in] */ IOnDataSetChangedForAnimationListener* dataSetChangedListener);

    CARAPI_(void) SetPhotoLoader(
        /* [in] */ IContactPhotoManager* photoLoader);

    /**
     * Indicates whether a drag is in process.
     *
     * @param inDragging Boolean variable indicating whether there is a drag in process.
     */
    CARAPI_(void) SetInDragging(
        /* [in] */ Boolean inDragging);

    /** Gets whether the drag is in process. */
    CARAPI_(Boolean) GetInDragging();

    /**
     * Creates {@link ContactTileView}s for each item in {@link Cursor}.
     *
     * Else use {@link ContactTileLoaderFactory}
     */
    CARAPI_(void) SetContactCursor(
        /* [in] */ ICursor* cursor);

    /**
     * Returns the number of frequents that will be displayed in the list.
     */
    CARAPI_(Int32) GetNumFrequents();

    // @Override
    CARAPI GetCount(
        /* [out] */ Int32* count);

    /**
     * Returns an ArrayList of the {@link ContactEntry}s that are to appear
     * on the row for the given position.
     */
    // @Override
    CARAPI GetItem(
        /* [in] */ Int32 position,
        /* [out] */ IInterface** item);

    /**
     * For the top row of tiled contacts, the item id is the position of the row of
     * contacts.
     * For frequent contacts, the item id is the maximum number of rows of tiled contacts +
     * the actual contact id. Since contact ids are always greater than 0, this guarantees that
     * all items within this adapter will always have unique ids.
     */
    // @Override
    CARAPI GetItemId(
        /* [in] */ Int32 position,
        /* [out] */ Int64* id);

    // @Override
    CARAPI HasStableIds(
        /* [out] */ Boolean* result);

    // @Override
    CARAPI AreAllItemsEnabled(
        /* [out] */ Boolean* result);

    // @Override
    CARAPI IsEnabled(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* result);

    // @Override
    CARAPI NotifyDataSetChanged();

    // @Override
    CARAPI GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    // @Override
    CARAPI GetViewTypeCount(
        /* [out] */ Int32* count);

    // @Override
    CARAPI GetItemViewType(
        /* [in] */ Int32 position,
        /* [out] */ Int32* type);

    /**
     * Temporarily removes a contact from the list for UI refresh. Stores data for this contact
     * in the back-up variable.
     *
     * @param index Position of the contact to be removed.
     */
    CARAPI_(void) PopContactEntry(
        /* [in] */ Int32 index);

    /**
     * Drops the temporarily removed contact to the desired location in the list.
     */
    CARAPI_(void) HandleDrop();

    /**
     * Invoked when the dragged item is dropped to unsupported location. We will then move the
     * contact back to where it was dragged from.
     */
    CARAPI_(void) DropToUnsupportedView();

    /**
     * Clears all temporary variables at a new interaction.
     */
    CARAPI_(void) CleanTempVariables();

    /**
     * Given a list of contacts that each have pinned positions, rearrange the list (destructive)
     * such that all pinned contacts are in their defined pinned positions, and unpinned contacts
     * take the spaces between those pinned contacts. Demoted contacts should not appear in the
     * resulting list.
     *
     * This method also updates the pinned positions of pinned contacts so that they are all
     * unique positive integers within range from 0 to toArrange.size() - 1. This is because
     * when the contact entries are read from the database, it is possible for them to have
     * overlapping pin positions due to sync or modifications by third party apps.
     */
    // @VisibleForTesting
    /* package */
     CARAPI_(void) ArrangeContactsByPinnedPosition(
        /* [in] */ IArrayList* toArrange);

    /**
     * Given an existing list of contact entries and a single entry that is to be pinned at a
     * particular position, return a list of {@link ContentProviderOperation}s that contains new
     * pinned positions for all contacts that are forced to be pinned at new positions, trying as
     * much as possible to keep pinned contacts at their original location.
     *
     * At this point in time the pinned position of each contact in the list has already been
     * updated by {@link #arrangeContactsByPinnedPosition}, so we can assume that all pinned
     * positions(within {@link #PIN_LIMIT} are unique positive integers.
     */
    // @VisibleForTesting
    /* package */
    CARAPI_(AutoPtr<IArrayList>) GetReflowedPinningOperations(
        /* [in] */ IArrayList* list,
        /* [in] */ Int32 oldPos,
        /* [in] */ Int32 newPinPos);

    // @Override
    CARAPI OnDragStarted(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ IPhoneFavoriteSquareTileView* view);

    // @Override
    CARAPI OnDragHovered(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ IPhoneFavoriteSquareTileView* view);

    // @Override
    CARAPI OnDragFinished(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    // @Override
    CARAPI OnDroppedOnRemove();

protected:
    /**
     * Sets the column indices for expected {@link Cursor}
     * based on {@link DisplayType}.
     */
    CARAPI_(void) BindColumnIndices();

    /**
     * Gets the number of frequents from the passed in cursor.
     *
     * This methods is needed so the GroupMemberTileAdapter can override this.
     *
     * @param cursor The cursor to get number of frequents from.
     */
    CARAPI_(void) SaveNumFrequentsFromCursor(
        /* [in] */ ICursor* cursor);

    /**
     * Iterates over the {@link Cursor}
     * Returns position of the first NON Starred Contact
     * Returns -1 if {@link DisplayType#STARRED_ONLY}
     * Returns 0 if {@link DisplayType#FREQUENT_ONLY}
     */
    CARAPI_(Int32) GetNumStarredContacts(
        /* [in] */ ICursor* cursor);
private:
    /**
     * Saves the cursor data to the cache, to speed up UI changes.
     *
     * @param cursor Returned cursor with data to populate the view.
     */
    CARAPI_(void) SaveCursorToCache(
        /* [in] */ ICursor* cursor);

    /**
     * @param itemIndex Position of the contact in {@link #mContactEntries}.
     * @return True if the given index is valid for {@link #mContactEntries}.
     */
    CARAPI_(Boolean) IsIndexInBound(
        /* [in] */ Int32 itemIndex);

    /**
     * Mark the tile as drop area by given the item index in {@link #mContactEntries}.
     *
     * @param itemIndex Position of the contact in {@link #mContactEntries}.
     */
    CARAPI_(void) MarkDropArea(
        /* [in] */ Int32 itemIndex);

    /**
     * Used when a contact is removed from speeddial. This will both unstar and set pinned position
     * of the contact to PinnedPosition.DEMOTED so that it doesn't show up anymore in the favorites
     * list.
     */
    CARAPI_(void) UnstarAndUnpinContact(
        /* [in] */ IUri* contactUri);

public:
    // Pinned positions start from 1, so there are a total of 20 maximum pinned contacts
    static const Int32 PIN_LIMIT = 21;

protected:
    /** Contact data stored in cache. This is used to populate the associated view. */
    AutoPtr<IArrayList> mContactEntries;

    Int32 mNumFrequents;
    Int32 mNumStarred;

    Int32 mIdIndex;
    Int32 mLookupIndex;
    Int32 mPhotoUriIndex;
    Int32 mNameIndex;
    Int32 mPresenceIndex;
    Int32 mStatusIndex;

private:
    static const String TAG;
    static const Boolean DEBUG = FALSE;

    /**
     * The soft limit on how many contact tiles to show.
     * NOTE This soft limit would not restrict the number of starred contacts to show, rather
     * 1. If the count of starred contacts is less than this limit, show 20 tiles total.
     * 2. If the count of starred contacts is more than or equal to this limit,
     * show all starred tiles and no frequents.
     */
    static const Int32 TILES_SOFT_LIMIT = 20;

    AutoPtr<IContactTileViewListener> mListener;
    AutoPtr<IOnDataSetChangedForAnimationListener> mDataSetChangedListener;

    AutoPtr<IContext> mContext;
    AutoPtr<IResources> mResources;

    /** Back up of the temporarily removed Contact during dragging. */
    AutoPtr<ContactEntry> mDraggedEntry;
    /** Position of the temporarily removed contact in the cache. */
    Int32 mDraggedEntryIndex;
    /** New position of the temporarily removed contact in the cache. */
    Int32 mDropEntryIndex;
    /** New position of the temporarily entered contact in the cache. */
    Int32 mDragEnteredEntryIndex;

    Boolean mAwaitingRemove;
    Boolean mDelayCursorUpdates;

    AutoPtr<IContactPhotoManager> mPhotoManager;

    Int32 mPhoneNumberIndex;
    Int32 mPhoneNumberTypeIndex;
    Int32 mPhoneNumberLabelIndex;
    Int32 mIsDefaultNumberIndex;
    Int32 mStarredIndex;
    Int32 mPinnedIndex;
    Int32 mContactIdIndex;

    /** Indicates whether a drag is in process. */
    Boolean mInDragging;

    AutoPtr<IComparator> mContactEntryComparator;
};

} // List
} // Dialer
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_DIALER_LIST_PHONEFAVORITESTILEADAPTER_H__
