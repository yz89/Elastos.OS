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

#include "elastos/droid/text/method/CPasswordTransformationMethodHelper.h"
#include "elastos/droid/text/method/CPasswordTransformationMethod.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CAR_SINGLETON_IMPL(CPasswordTransformationMethodHelper)

CAR_INTERFACE_IMPL(CPasswordTransformationMethodHelper, Singleton, IPasswordTransformationMethodHelper)

ECode CPasswordTransformationMethodHelper::GetInstance(
    /* [out] */ IPasswordTransformationMethod** ret)
{
    return CPasswordTransformationMethod::GetInstance(ret);
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

