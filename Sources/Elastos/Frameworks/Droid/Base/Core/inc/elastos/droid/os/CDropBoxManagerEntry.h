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

#ifndef __ELASTOS_DROID_OS_CDROPBOXMANAGERENTRY_H__
#define __ELASTOS_DROID_OS_CDROPBOXMANAGERENTRY_H__

#include "_Elastos_Droid_Os_CDropBoxManagerEntry.h"
#include "elastos/droid/ext/frameworkdef.h"
#include <elastos/utility/logging/Slogger.h>
#include "elastos/droid/os/ParcelFileDescriptor.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/Math.h>

using Elastos::Core::Math;
using Elastos::Core::StringBuilder;
using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Droid::Os::ParcelFileDescriptor;
using Elastos::Droid::Os::IDropBoxManager;
using Elastos::IO::IInputStream;
using Elastos::IO::IByteArrayInputStream;
using Elastos::IO::CByteArrayInputStream;
using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::Zip::IGZIPInputStream;
using Elastos::Utility::Zip::CGZIPInputStream;

namespace Elastos {
namespace Droid {
namespace Os {
/**
 * A single entry retrieved from the drop box.
 * This may include a reference to a stream, so you must call
 * {@link #close()} when you are done using it.
 */
CarClass(CDropBoxManagerEntry)
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor();

    /** Create a new empty Entry with no contents. */
    CARAPI constructor(
        /* [in] */ const String& tag,
        /* [in] */ Int64 millis);

    /** Create a new Entry with plain text contents. */
    CARAPI constructor(
        /* [in] */ const String& tag,
        /* [in] */ Int64 millis,
        /* [in] */ const String& text);

    /**
     * Create a new Entry with byte array contents.
     * The data array must not be modified after creating this entry.
     */
    CARAPI constructor(
        /* [in] */ const String& tag,
        /* [in] */ Int64 millis,
        /* [in] */ ArrayOf<Byte>* arrayData,
        /* [in] */ Int32 flags);

    /**
     * Create a new Entry with streaming data contents.
     * Takes ownership of the ParcelFileDescriptor.
     */
    CARAPI constructor(
        /* [in] */ const String& tag,
        /* [in] */ Int64 millis,
        /* [in] */ IParcelFileDescriptor* pfdData,
        /* [in] */ Int32 flags);

    /**
     * Create a new Entry with the contents read from a file.
     * The file will be read when the entry's contents are requested.
     */
    CARAPI constructor(
        /* [in] */ const String& tag,
        /* [in] */ Int64 millis,
        /* [in] */ IFile* fileData,
        /* [in] */ Int32 flags);

    /** Close the input stream associated with this entry. */
    CARAPI Close();

    /** @return the tag originally attached to the entry. */
    CARAPI GetTag(
        /* [out] */ String* tag);

    /** @return time when the entry was originally created. */
    CARAPI GetTimeMillis(
        /* [out] */ Int64* timeMillis);

    /** @return flags describing the content returned by {@link #getInputStream()}. */
    CARAPI GetFlags(
        /* [out] */ Int32* flags); // getInputStream() decompresses.

    /**
     * @param maxBytes of string to return (will truncate at this length).
     * @return the uncompressed text contents of the entry, null if the entry is not text.
     */
    CARAPI GetText(
        /* [in] */ Int32 maxBytes,
        /* [out] */ String* text);

    /** @return the uncompressed contents of the entry, or null if the contents were lost */
    CARAPI GetInputStream(
        /* [out] */ IInputStream** stream);

    // public static final Parcelable.Creator<Entry> CREATOR = new Parcelable.Creator() {
    //     public Entry[] newArray(int size) { return new Entry[size]; }
    //     public Entry createFromParcel(Parcel in) {
    //         String tag = in.readString();
    //         long millis = in.readLong();
    //         int flags = in.readInt();
    //         if ((flags & HAS_BYTE_ARRAY) != 0) {
    //             return new Entry(tag, millis, in.createByteArray(), flags & ~HAS_BYTE_ARRAY);
    //         } else {
    //             return new Entry(tag, millis, in.readFileDescriptor(), flags);
    //         }
    //     }
    // };

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* out);

private:
    String mTag;
    Int64 mTimeMillis;

    AutoPtr<ArrayOf<Byte> > mData;
    AutoPtr<IParcelFileDescriptor> mFileDescriptor;
    Int32 mFlags;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_CDROPBOXMANAGERENTRY_H__
