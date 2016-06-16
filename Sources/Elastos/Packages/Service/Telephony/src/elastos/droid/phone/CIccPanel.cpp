
#include "elastos/droid/phone/CIccPanel.h"

namespace Elastos {
namespace Droid {
namespace Phone {

const String CIccPanel::TAG("CIccPanel");// = PhoneGlobals.LOG_TAG;

CAR_INTERFACE_IMPL(CIccPanel, Dialog, IIccPanel)

CAR_OBJECT_IMPL(CIccPanel)

ECode CIccPanel::constructor(
    /* [in] */ IContext* context)
{
    return Dialog::constructor(context, R.style.IccPanel);
}

ECode CIccPanel::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    Dialog::OnCreate(savedInstanceState);
    AutoPtr<IWindow> winP;
    GetWindow((IWindow**)&winP);
    winP->SetType(IWindowManagerLayoutParams::TYPE_PRIORITY_PHONE);
    winP->SetLayout(IWindowManagerLayoutParams::MATCH_PARENT,
            IWindowManagerLayoutParams::MATCH_PARENT);
    winP->SetGravity(IGravity::CENTER);

    // TODO: Ideally, we'd like this dialog to be visible in front of the
    // keyguard, so the user will see it immediately after boot (without
    // needing to enter the lock pattern or dismiss the keyguard first.)
    //
    // However that's not easy to do.  It's not just a matter of setting
    // the FLAG_SHOW_WHEN_LOCKED and FLAG_DISMISS_KEYGUARD flags on our
    // window, since we're a Dialog (not an Activity), and the framework
    // won't ever let a dialog hide the keyguard (because there could
    // possibly be stuff behind it that shouldn't be seen.)
    //
    // So for now, we'll live with the fact that the user has to enter the
    // lock pattern (or dismiss the keyguard) *before* being able to type
    // a SIM network unlock PIN.  That's not ideal, but still OK.
    // (And eventually this will be a moot point once this UI moves
    // from the phone app to the framework; see bug 1804111).

    // TODO: we shouldn't need the mStatusBarManager calls here either,
    // once this dialog gets moved into the framework and becomes a truly
    // full-screen UI.
    AutoPtr<IPhoneGlobals> app;
    PhoneGlobals::GetInstance((IPhoneGlobals**)&app);

    AutoPtr<IInterface> obj;
    app->GetSystemService(IContext::STATUS_BAR_SERVICE, (IInterface**)&obj);
    mStatusBarManager = IStatusBarManager::Probe(obj);

    return RequestWindowFeature(IWindow::FEATURE_NO_TITLE);
}

ECode CIccPanel::OnStart()
{
    Dialog::OnStart();
    mStatusBarManager->Disable(IStatusBarManager::DISABLE_EXPAND);
}

ECode CIccPanel::OnStop()
{
    Dialog::OnStop();
    mStatusBarManager->Disable(IStatusBarManager::DISABLE_NONE);
}

ECode CIccPanel::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (keyCode == IKeyEvent::KEYCODE_BACK) {
        *result = TRUE;
        return NOERROR;
    }

    return Dialog::OnKeyDown(keyCode, event, result);
}


} // namespace Phone
} // namespace Droid
} // namespace Elastos