
#ifndef __ELASTOS_DROID_OS_CDEBUGMEMORYINFOHELPER_H__
#define __ELASTOS_DROID_OS_CDEBUGMEMORYINFOHELPER_H__

#include "_Elastos_Droid_Os_CDebugMemoryInfoHelper.h"
#include <elastos/core/Singleton.h>

using Elastos::Core::Singleton;

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CDebugMemoryInfoHelper)
    , public Singleton
    , public IDebugMemoryInfoHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI GetOtherLabel(
        /* [in] */ Int32 which,
        /* [out] */ String* label);
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_OS_CDEBUGMEMORYINFOHELPER_H__
