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

#include "elastos/droid/server/am/GrantUri.h"
#include "Elastos.Droid.Content.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Droid::Content::CContentProviderHelper;
using Elastos::Droid::Content::IContentProviderHelper;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

GrantUri::GrantUri(
    /* [in] */ Int32 sourceUserId,
    /* [in] */ IUri* uri,
    /* [in] */ Boolean prefix)
    : mSourceUserId(sourceUserId)
    , mUri(uri)
    , mPrefix(prefix)
{
}

ECode GrantUri::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    *hashCode = ToString().GetHashCode();
    return NOERROR;
}

ECode GrantUri::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = Equals((GrantUri*)IObject::Probe(other));
    return NOERROR;
}

Boolean GrantUri::Equals(
    /* [in] */ GrantUri* other)
{
    if (other != NULL) {
        Boolean result;
        IObject::Probe(mUri)->Equals(other->mUri, &result);
        return result && (mSourceUserId == other->mSourceUserId)
                && mPrefix == other->mPrefix;
    }
    return FALSE;
}

ECode GrantUri::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info)
    *info = ToString();
    return NOERROR;
}

String GrantUri::ToString()
{
    StringBuilder result;
    result += mSourceUserId;
    result += " @ ";
    result += mUri;
    if (mPrefix)
        result += " [mPrefix]";
    return result.ToString();
}

String GrantUri::ToSafeString()
{
    StringBuilder result;
    result += mSourceUserId;
    result += " @ ";
    String str;
    mUri->ToSafeString(&str);
    result += str;
    if (mPrefix)
        result += " [mPrefix]";
    return result.ToString();
}

AutoPtr<GrantUri> GrantUri::Resolve(
    /* [in] */ Int32 defaultSourceUserHandle,
    /* [in] */ IUri* uri)
{
    AutoPtr<IContentProviderHelper> cpHelper;
    CContentProviderHelper::AcquireSingleton((IContentProviderHelper**)&cpHelper);
    Int32 userId;
    cpHelper->GetUserIdFromUri(uri, defaultSourceUserHandle, &userId);
    AutoPtr<IUri> retUri;
    cpHelper->GetUriWithoutUserId(uri, (IUri**)&retUri);
    return new GrantUri(userId, retUri, FALSE);
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
