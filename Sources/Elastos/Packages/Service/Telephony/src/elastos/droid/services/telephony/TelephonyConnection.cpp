
#include "elastos/droid/services/telephony/TelephonyConnection.h"

namespace Elastos {
namespace Droid {
namespace Services {
namespace Telephony {

TelephonyConnection::MyHandler::MyHandler(
    /* [in] */ TelephonyConnection* host)
    : mHost(host)
{
    Handler::constructor();
}

ECode TelephonyConnection::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case MSG_PRECISE_CALL_STATE_CHANGED:
            Logger::V("TelephonyConnection", "MSG_PRECISE_CALL_STATE_CHANGED");
            UpdateState();
            break;
        case MSG_HANDOVER_STATE_CHANGED:
            Logger::V("TelephonyConnection", "MSG_HANDOVER_STATE_CHANGED");
            AsyncResult ar = (AsyncResult) msg.obj;
            com.android.internal.telephony.Connection connection =
                 (com.android.internal.telephony.Connection) ar.result;
            SetOriginalConnection(connection);
            break;
        case MSG_RINGBACK_TONE:
            Logger::V("TelephonyConnection", "MSG_RINGBACK_TONE");
            // TODO: This code assumes that there is only one connection in the foreground
            // call, in other words, it punts on network-mediated conference calling.
            AutoPtr<com.android.internal.telephony.Connection> ocon;
            getOriginalConnection((om.android.internal.telephony.Connection**)&ocon);
            AutoPtr<com.android.internal.telephony.Connection> fcon =  GetForegroundConnection();
            if (TO_IINTERFACE(ocon) != TO_IINTERFACE(fcon)) {
                Logger::V("TelephonyConnection", "handleMessage, original connection is " +
                        "not foreground connection, skipping");
                return NOERROR;
            }
            SetRingbackRequested((Boolean) ((AsyncResult) msg.obj).result);
            break;
        case MSG_DISCONNECT:
            UpdateState();
            break;
    }
    return NOERROR;
}

ECode TelephonyConnection::MyConnectionPostDialListener::OnPostDialWait()
{
    Logger::V("TelephonyConnection", "onPostDialWait");
    if (mOriginalConnection != NULL) {
        String remaining;
        mOriginalConnection->GetRemainingPostDialString(&remaining);
        SetPostDialWait(remaining);
    }
    return NOERROR;
}

ECode TelephonyConnection::MyConnectionListener::OnVideoStateChanged(
    /* [in] */ Int32 videoState)
{
    return mHost->SetVideoState(videoState);
}

ECode TelephonyConnection::MyConnectionListener::OnLocalVideoCapabilityChanged(
    /* [in] */ Boolean capable)
{
    return mHost->SetLocalVideoCapable(capable);
}

ECode TelephonyConnection::MyConnectionListener::OnRemoteVideoCapabilityChanged(
    /* [in] */ Boolean capable)
{
    return mHost->SetRemoteVideoCapable(capable);
}

ECode TelephonyConnection::MyConnectionListener::OnVideoProviderChanged(
    /* [in] */ IConnectionVideoProvider* videoProvider)
{
    return mHost->SetVideoProvider(videoProvider);
}

ECode TelephonyConnection::MyConnectionListener::OnAudioQualityChanged(
    /* [in] */ Int32 audioQuality)
{
    return mHost->SetAudioQuality(audioQuality);
}

const Int32 TelephonyConnection::MSG_PRECISE_CALL_STATE_CHANGED = 1;
const Int32 TelephonyConnection::MSG_RINGBACK_TONE = 2;
const Int32 TelephonyConnection::MSG_HANDOVER_STATE_CHANGED = 3;
const Int32 TelephonyConnection::MSG_DISCONNECT = 4;

CAR_INTERFACE_IMPL(TelephonyConnection, Connection, ITelephonyConnection)

TelephonyConnection::TelephonyConnection()
    : mOriginalConnectionState(ICallState_IDLE)
    , mLocalVideoCapable(FALSE)
    , mRemoteVideoCapable(FALSE)
    , mAudioQuality(0)
{
    mHandler = new MyHandler(this);

    mPostDialListener = new MyConnectionPostDialListener(this);

    mOriginalConnectionListener = new MyConnectionPostDialListener(this);
}

ECode TelephonyConnection::constructor(
    /* [in] */ com.android.internal.telephony.Connection originalConnection)
{
    if (originalConnection != NULL) {
        SetOriginalConnection(originalConnection);
    }
    return NOERROR;
}

ECode TelephonyConnection::OnAudioStateChanged(
    /* [in] */ IAudioState* audioState)
{
    // TODO: update TTY mode.
    AutoPtr<IPhone> phone;
    GetPhone((IPhone**)&phone);
    if (phone != null) {
        phone->SetEchoSuppressionEnabled();
    }
    return NOERROR;
}

ECode TelephonyConnection::OnStateChanged(
    /* [in] */ Int32 state)
{
    Logger::V("TelephonyConnection", "onStateChanged, state: " + Connection.stateToString(state));
}

ECode TelephonyConnection::OnDisconnect()
{
    Logger::V("TelephonyConnection", "onDisconnect");
    Hangup(android.telephony.DisconnectCause.LOCAL);
    return NOERROR;
}

ECode TelephonyConnection::OnSeparate()
{
    Logger::V("TelephonyConnection", "onSeparate");
    if (mOriginalConnection != NULL) {
        //try {
        ECode ec = mOriginalConnection->Separate();
        //} catch (CallStateException e) {
        if (ec == (ECode)CallStateException) {
            Logger::E("TelephonyConnection", "Call to Connection.separate failed with exception %d", ec);
        }
        //}
    }
    return NOERROR;
}

ECode TelephonyConnection::OnAbort()
{
    Logger::V("TelephonyConnection", "onAbort");
    Hangup(android.telephony.DisconnectCause.LOCAL);
    return NOERROR;
}

ECode TelephonyConnection::OnHold()
{
    PerformHold();
    return NOERROR;
}

ECode TelephonyConnection::OnUnhold()
{
    PerformUnhold();
    return NOERROR;
}

ECode TelephonyConnection::OnAnswer(
    /* [in] */ Int videoState)
{
    Logger::V("TelephonyConnection", "onAnswer");
    AutoPtr<IPhone> phone;
    GetPhone((IPhone**)&phone);
    if (IsValidRingingCall() && phone != NULL) {
        //try {
        ECode ec = phone->AcceptCall(videoState);
        //} catch (CallStateException e) {
        if (ec == (ECode)CallStateException) {
            Logger::E("TelephonyConnection", "Failed to accept call. %d", ec);
        }
        //}
    }
    return NOERROR;
}

ECode TelephonyConnection::OnReject()
{
    Logger::V("TelephonyConnection", "onReject");
    if (IsValidRingingCall()) {
        Hangup(android.telephony.DisconnectCause.INCOMING_REJECTED);
    }
    return Connection::OnReject();
}

ECode TelephonyConnection::OnPostDialContinue(
    /* [in] */ Boolean proceed)
{
    Logger::V("TelephonyConnection", "onPostDialContinue, proceed: %d", proceed);
    if (mOriginalConnection != NULL) {
        if (proceed) {
            mOriginalConnection->ProceedAfterWaitChar();
        }
        else {
            mOriginalConnection->CancelPostDial();
        }
    }
    return NOERROR;
}

ECode TelephonyConnection::PerformHold()
{
    Logger::V("TelephonyConnection", "performHold");
    // TODO: Can dialing calls be put on hold as well since they take up the
    // foreground call slot?
    if (ICallState_ACTIVE == mOriginalConnectionState) {
        Logger::V("TelephonyConnection", "Holding active call");
        //try
        {
            AutoPtr<ICall> call;
            FAIL_GOTO(mOriginalConnection->GetCall((ICall**)&call), ERROR)
            AutoPtr<IPhone> phone;
            FAIL_GOTO(call->GetPhone((IPhone**)&phone), ERROR)
            AutoPtr<ICall> ringingCall;
            FAIL_GOTO(phone->GetRingingCall((ICall**)&ringingCall), ERROR)

            // Although the method says switchHoldingAndActive, it eventually calls a RIL method
            // called switchWaitingOrHoldingAndActive. What this means is that if we try to put
            // a call on hold while a call-waiting call exists, it'll end up accepting the
            // call-waiting call, which is bad if that was not the user's intention. We are
            // cheating here and simply skipping it because we know any attempt to hold a call
            // while a call-waiting call is happening is likely a request from Telecom prior to
            // accepting the call-waiting call.
            // TODO: Investigate a better solution. It would be great here if we
            // could "fake" hold by silencing the audio and microphone streams for this call
            // instead of actually putting it on hold.
            Int32 state;
            FAIL_GOTO(ringingCall->GetState(&state), ERROR)
            if (state != ICallState_WAITING) {
                FAIL_GOTO(phone->SwitchHoldingAndActive(), ERROR)
            }

            // TODO: Cdma calls are slightly different.
        }
        //} catch (CallStateException e) {
    ERROR:
        if (ec == (ECode)CallStateException) {
            Logger::E("TelephonyConnection", "Exception occurred while trying to put call on hold. %d", ec);
        }
        //}
    }
    else {
        Logger::W("TelephonyConnection", "Cannot put a call that is not currently active on hold.");
    }
    return NOERROR;
}

ECode TelephonyConnection::PerformUnhold()
{
    Logger::V("TelephonyConnection", "performUnhold");
    if (ICallState_HOLDING == mOriginalConnectionState) {
        //try
        {
            // Here's the deal--Telephony hold/unhold is weird because whenever there exists
            // more than one call, one of them must always be active. In other words, if you
            // have an active call and holding call, and you put the active call on hold, it
            // will automatically activate the holding call. This is weird with how Telecom
            // sends its commands. When a user opts to "unhold" a background call, telecom
            // issues hold commands to all active calls, and then the unhold command to the
            // background call. This means that we get two commands...each of which reduces to
            // switchHoldingAndActive(). The result is that they simply cancel each other out.
            // To fix this so that it works well with telecom we add a minor hack. If we
            // have one telephony call, everything works as normally expected. But if we have
            // two or more calls, we will ignore all requests to "unhold" knowing that the hold
            // requests already do what we want. If you've read up to this point, I'm very sorry
            // that we are doing this. I didn't think of a better solution that wouldn't also
            // make the Telecom APIs very ugly.
            if (!HasMultipleTopLevelCalls()) {
                AutoPtr<ICall> call;
                FAIL_GOTO(mOriginalConnection->GetCall((ICall**)&call), ERROR)
                AutoPtr<IPhone> phone;
                FAIL_GOTO(call->GetPhone((IPhone**)&phone), ERROR)
                FAIL_GOTO(phone->SwitchHoldingAndActive(), ERROR)
            }
            else {
                Logger::I("TelephonyConnection", "Skipping unhold command for %s", TO_CSTR(this));
            }
        }
        //} catch (CallStateException e) {
    ERROR:
        if (ec == (ECode)CallStateException) {
            Logger::E("TelephonyConnection", "Exception occurred while trying to release call from hold. %d", ec);
        }
        //}
    }
    else {
        Logger::W("TelephonyConnection", "Cannot release a call that is not already on hold from hold.");
    }
}

ECode TelephonyConnection::PerformConference(
    /* [in] */ ITelephonyConnection* otherConnection)
{
    return NOERROR;
}

ECode TelephonyConnection::UpdateCallCapabilities()
{
    Int32 newCallCapabilities = BuildCallCapabilities();
    newCallCapabilities = ApplyVideoCapabilities(newCallCapabilities);
    newCallCapabilities = ApplyAudioQualityCapabilities(newCallCapabilities);
    newCallCapabilities = ApplyConferenceTerminationCapabilities(newCallCapabilities);

    Int32 capabilities;
    GetCallCapabilities(&capabilities);
    if (capabilities != newCallCapabilities) {
        SetCallCapabilities(newCallCapabilities);
    }
    return NOERROR;
}

ECode TelephonyConnection::UpdateAddress()
{
    UpdateCallCapabilities();
    if (mOriginalConnection != NULL) {
        String str;
        mOriginalConnection->GetAddress(&str);
        AutoPtr<IUri> address = GetAddressFromNumber(str);
        Int32 presentation;
        mOriginalConnection->GetNumberPresentation(&presentation);

        AutoPtr<IUri> tmp;
        GetAddress((IUri**)&tmp);
        Boolean res;
        if ((IObjects::Probe(address)->Equals(tmp, &res), !res) ||
                presentation != GetAddressPresentation()) {
            Logger::V("TelephonyConnection", "updateAddress, address changed");
            SetAddress(address, presentation);
        }

        String name;
        mOriginalConnection->GetCnapName(&name);
        Int32 namePresentation;
        mOriginalConnection->GetCnapNamePresentation(&namePresentation);

        String str2;
        GetCallerDisplayName(&str2);
        if (!name.Equals(str2) ||
                namePresentation != GetCallerDisplayNamePresentation()) {
            Logger::V("TelephonyConnection", "updateAddress, caller display name changed");
            SetCallerDisplayName(name, namePresentation);
        }
    }
    return NOERROR;
}

ECode TelephonyConnection::OnRemovedFromCallService()
{
    return NOERROR;
}

ECode TelephonyConnection::SetOriginalConnection(
    /* [in] */ com.android.internal.telephony.Connection* originalConnection)
{
    Logger::V("TelephonyConnection", "new TelephonyConnection, originalConnection: %s", TO_CSTR(originalConnection));
    AutoPtr<IPhone> phone;
    GetPhone((IPhone**)&phone);
    if (mOriginalConnection != NULL) {
        phone->UnregisterForPreciseCallStateChanged(mHandler);
        phone->UnregisterForRingbackTone(mHandler);
        phone->UnregisterForHandoverStateChanged(mHandler);
        phone->UnregisterForDisconnect(mHandler);
    }
    mOriginalConnection = originalConnection;
    phone->RegisterForPreciseCallStateChanged(
            mHandler, MSG_PRECISE_CALL_STATE_CHANGED, NULL);
    phone->RegisterForHandoverStateChanged(
            mHandler, MSG_HANDOVER_STATE_CHANGED, NULL);
    phone->RegisterForRingbackTone(mHandler, MSG_RINGBACK_TONE, NULL);
    phone->RegisterForDisconnect(mHandler, MSG_DISCONNECT, NULL);
    mOriginalConnection->AddPostDialListener(mPostDialListener);
    mOriginalConnection->AddListener(mOriginalConnectionListener);

    // Set video state and capabilities
    Int32 state;
    mOriginalConnection->GetVideoState(&state);
    SetVideoState(state);
    Boolean res;
    mOriginalConnection->IsLocalVideoCapable(&res);
    SetLocalVideoCapable(res);
    mOriginalConnection->IsRemoteVideoCapable(&res);
    SetRemoteVideoCapable(res);
    AutoPtr<IConnectionVideoProvider> provider;
    mOriginalConnection->GetVideoProvider((IConnectionVideoProvider**)&provider);
    SetVideoProvider(provider);
    Int32 quality;
    mOriginalConnection->GetAudioQuality(&quality);
    SetAudioQuality(quality);

    return UpdateAddress();
}

ECode TelephonyConnection::Hangup(
    /* [in] */ Int32 telephonyDisconnectCode)
{
    if (mOriginalConnection != NULL) {
        //try
        {
            // Hanging up a ringing call requires that we invoke call.hangup() as opposed to
            // connection.hangup(). Without this change, the party originating the call will not
            // get sent to voicemail if the user opts to reject the call.
            if (IsValidRingingCall()) {
                AutoPtr<ICall> call;
                GetCall((ICall**)&call);
                if (call != NULL) {
                    FAIL_GOTO(call->Hangup(), ERROR)
                }
                else {
                    Logger::W("TelephonyConnection", "Attempting to hangup a connection without backing call.");
                }
            }
            else {
                // We still prefer to call connection.hangup() for non-ringing calls in order
                // to support hanging-up specific calls within a conference call. If we invoked
                // call.hangup() while in a conference, we would end up hanging up the entire
                // conference call instead of the specific connection.
                FAIL_GOTO(mOriginalConnection->Hangup(), ERROR)
            }
        }
        //} catch (CallStateException e) {
    ERROR:
        if (ec == (ECode)CallStateException) {
            Logger::E("TelephonyConnection", "Call to Connection.hangup failed with exception %d", ec);
        }
        //}
    }
    return NOERROR;
}

ECode TelephonyConnection::GetOriginalConnection(
    /* [out] */ com.android.internal.telephony.Connection** con)
{
    VALIDATE_NOT_NULL(con)

    *con = mOriginalConnection;
    REFCOUNT_ADD(*con)
    return NOERROR;
}

ECode TelephonyConnection::GetCall(
    /* [out] */ ICall** call)
{
    VALIDATE_NOT_NULL(con)

    if (mOriginalConnection != NULL) {
        return mOriginalConnection->GetCall(call);
    }
    return NULL;
}

ECode TelephonyConnection::GetPhone(
    /* [out] */ IPhone** phone)
{
    VALIDATE_NOT_NULL(con)

    AutoPtr<ICall> call;
    GetCall((ICall**)&call);
    if (call != NULL) {
        return call->GetPhone(phone);
    }
    return NULL;
}

Boolean TelephonyConnection::HasMultipleTopLevelCalls()
{
    Int32 numCalls = 0;
    AutoPtr<IPhone> phone;
    GetPhone((IPhone**)&phone);
    if (phone != NULL) {
        AutoPtr<ICall> call;
        phone->GetRingingCall((ICall**)&call);
        Boolean res;
        if (call->IsIdle(&res), !res) {
            numCalls++;
        }

        AutoPtr<ICall> call2;
        phone->GetForegroundCall((ICall**)&call2);
        if (call2->IsIdle(&res), !res) {
            numCalls++;
        }

        AutoPtr<ICall> call3;
        phone->GetBackgroundCall((ICall**)&call3);
        if (call3->IsIdle(&res), !res) {
            numCalls++;
        }
    }
    return numCalls > 1;
}

AutoPtr<com.android.internal.telephony.Connection> TelephonyConnection::GetForegroundConnection()
{
    AutoPtr<IPhone> phone;
    GetPhone((IPhone**)&phone);
    if (phone != NULL) {
        AutoPtr<ICall> call;
        phone->GetForegroundCall((ICall**)&call);
        AutoPtr<com.android.internal.telephony.Connection> con;
        call->GetEarliestConnection((com.android.internal.telephony.Connection**)&con);
        return con;
    }
    return NULL;
}

Boolean TelephonyConnection::IsValidRingingCall()
{
    AutoPtr<IPhone> phone;
    GetPhone((IPhone**)&phone);
    if (phone == NULL) {
        Logger::V("TelephonyConnection", "isValidRingingCall, phone is null");
        return FALSE;
    }

    AutoPtr<ICall> ringingCall;
    phone->GetRingingCall((ICall**)&ringingCall);
    AutoPtr<ICallState> state;
    ringingCall->GetState((ICallState**)&state);
    Boolean res;
    if (state->IsRinging(&res), !res) {
        Logger::V("TelephonyConnection", "isValidRingingCall, ringing call is not in ringing state");
        return FALSE;
    }

    AutoPtr<com.android.internal.telephony.Connection> tmp;
    ringingCall->GetEarliestConnection((com.android.internal.telephony.Connection**)&tmp);
    if (TO_IINTERFACE(tmp) != TO_IINTERFACE(mOriginalConnection)) {
        Logger::V("TelephonyConnection", "isValidRingingCall, ringing call connection does not match");
        return FALSE;
    }

    Logger::V("TelephonyConnection", "isValidRingingCall, returning true");
    return TRUE;
}

ECode TelephonyConnection::UpdateState()
{
    if (mOriginalConnection == NULL) {
        return NOERROR;
    }

    Int32 newState;
    mOriginalConnection->GetState(&newState);
    Logger::V("TelephonyConnection", "Update state from %d to %d for %s", mOriginalConnectionState,
            newState, TO_CSTR(this));
    if (mOriginalConnectionState != newState) {
        mOriginalConnectionState = newState;
        switch (newState) {
            case IDLE:
                break;
            case ACTIVE:
                SetActiveInternal();
                break;
            case HOLDING:
                SetOnHold();
                break;
            case DIALING:
            case ALERTING:
                SetDialing();
                break;
            case INCOMING:
            case WAITING:
                SetRinging();
                break;
            case DISCONNECTED:
            {
                Int32 telephonyDisconnectCause;
                mOriginalConnection->GetDisconnectCause(&telephonyDisconnectCause);
                AutoPtr<IDisconnectCause> cause = DisconnectCauseUtil::ToTelecomDisconnectCause(
                        telephonyDisconnectCause);
                SetDisconnected(cause);
                Close();
                break;
            }
            case DISCONNECTING:
                break;
        }
    }
    UpdateCallCapabilities();
    UpdateAddress();
    return NOERROR;
}

void TelephonyConnection::SetActiveInternal()
{
    Int32 state;
    GetState(&state);
    if (state == STATE_ACTIVE) {
        Logger::W("TelephonyConnection", "Should not be called if this is already ACTIVE");
        return;
    }

    // When we set a call to active, we need to make sure that there are no other active
    // calls. However, the ordering of state updates to connections can be non-deterministic
    // since all connections register for state changes on the phone independently.
    // To "optimize", we check here to see if there already exists any active calls.  If so,
    // we issue an update for those calls first to make sure we only have one top-level
    // active call.
    AutoPtr<IConnectionService> service;
    GetConnectionService((IConnectionService**)service);
    if (service != NULL) {
        AutoPtr<ICollection> coll;
        service->GetAllConnections((ICollection**)&coll);
        AutoPtr<ArrayOf<IInterface*> > array;
        coll->ToArray((ArrayOf<IInterface*>**)&array);

        for (Int32 i = 0; i < array->GetLength(); i++) {
            AutoPtr<IConnection> current = IConnection::Probe((*array)[i]);

            if (TO_IINTERFACE(current) != TO_IINTERFACE(this) && ITelephonyConnection::Probe(current) != NULL) {
                AutoPtr<ITelephonyConnection> other = ITelephonyConnection::Probe(current);
                Int32 _state;
                other->GetState(&_state);
                if (_state == STATE_ACTIVE) {
                    other->UpdateState();
                }
            }
        }
    }
    SetActive();
}

void TelephonyConnection::Close()
{
    Logger::V("TelephonyConnection", "close");
    AutoPtr<IPhone> phone;
    GetPhone((IPhone**)&phone);
    if (phone != NULL) {
        phone->UnregisterForPreciseCallStateChanged(mHandler);
        phone->UnregisterForRingbackTone(mHandler);
        phone->UnregisterForHandoverStateChanged(mHandler);
    }
    mOriginalConnection = NULL;
    Destroy();
}

Int32 TelephonyConnection::ApplyVideoCapabilities(
        /* [in] */ Int32 capabilities)
{
    Int32 currentCapabilities = capabilities;
    if (mRemoteVideoCapable) {
        currentCapabilities = ApplyCapability(currentCapabilities,
                IPhoneCapabilities::SUPPORTS_VT_REMOTE);
    }
    else {
        currentCapabilities = RemoveCapability(currentCapabilities,
                IPhoneCapabilities::SUPPORTS_VT_REMOTE);
    }

    if (mLocalVideoCapable) {
        currentCapabilities = ApplyCapability(currentCapabilities,
                IPhoneCapabilities::SUPPORTS_VT_LOCAL);
    }
    else {
        currentCapabilities = RemoveCapability(currentCapabilities,
                IPhoneCapabilities::SUPPORTS_VT_LOCAL);
    }
    return currentCapabilities;
}

Int32 TelephonyConnection::ApplyAudioQualityCapabilities(
        /* [in] */ Int32 callCapabilities)
{
    Int32 currentCapabilities = callCapabilities;

    if (mAudioQuality ==
            com.android.internal.telephony.Connection.AUDIO_QUALITY_HIGH_DEFINITION) {
        currentCapabilities = ApplyCapability(currentCapabilities, IPhoneCapabilities::VoLTE);
    }
    else {
        currentCapabilities = RemoveCapability(currentCapabilities, IPhoneCapabilities::VoLTE);
    }

    return currentCapabilities;
}

Int32 TelephonyConnection::ApplyConferenceTerminationCapabilities(
        /* [in] */ Int32 callCapabilities)
{
    Int32 currentCapabilities = callCapabilities;

    // An IMS call cannot be individually disconnected or separated from its parent conference
    AutoPtr<com.android.internal.telephony.Connection> con;
    GetOriginalConnection((com.android.internal.telephony.Connection**)&con);
    Boolean isImsCall = IImsPhoneConnection::Probe(con) != NULL;
    if (!isImsCall) {
        currentCapabilities |=
                IPhoneCapabilities::DISCONNECT_FROM_CONFERENCE
                | IPhoneCapabilities::SEPARATE_FROM_CONFERENCE;
    }

    return currentCapabilities;
}

ECode TelephonyConnection::IsLocalVideoCapable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mLocalVideoCapable;
    return NOERROR;
}

ECode TelephonyConnection::IsRemoteVideoCapable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mRemoteVideoCapable;
    return NOERROR;
}

ECode TelephonyConnection::SetLocalVideoCapable(
    /* [in] */ Boolean capable)
{
    mLocalVideoCapable = capable;
    UpdateCallCapabilities();
    return NOERROR;
}

ECode TelephonyConnection::SetRemoteVideoCapable(
    /* [in] */ Boolean capable)
{
    mRemoteVideoCapable = capable;
    UpdateCallCapabilities();
    return NOERROR;
}

ECode TelephonyConnection::SetAudioQuality(
    /* [in] */ Int32 audioQuality)
{
    mAudioQuality = audioQuality;
    UpdateCallCapabilities();
    return NOERROR;
}

ECode TelephonyConnection::GetAudioQuality(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mAudioQuality;
    return NOERROR;
}

ECode TelephonyConnection::ResetStateForConference()
{
    Int32 state;
    GetState(&state);
    if (state == IConnection::STATE_HOLDING) {
        Int32 _state;
        mOriginalConnection->GetState(&_state);
        if (_state == ICallState_ACTIVE) {
            SetActive();
        }
    }
    return NOERROR;
}

ECode TelephonyConnection::SetHoldingForConference(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    Int32 state;
    GetState(&state);
    if (state == IConnection::STATE_ACTIVE) {
        SetOnHold();
        *result = TRUE;
        return NOERRORl
    }
    *result = FALSE;
    return NOERRORl
}

AutoPtr<IUri> TelephonyConnection::GetAddressFromNumber(
        /* [in] */ const String& number)
{
    // Address can be null for blocked calls.
    if (number.IsNull()) {
        number = String("");
    }

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    AutoPtr<IUri> uri;
    return helper->FromParts(IPhoneAccount::SCHEME_TEL, number, NULL, (IUri**)&uri);
}

Int32 TelephonyConnection::ApplyCapability(
    /* [in] */ Int32 capabilities,
    /* [in] */ Int32 capability)
{
    Int32 newCapabilities = capabilities | capability;
    return newCapabilities;
}

Int32 TelephonyConnection::RemoveCapability(
    /* [in] */ Int32 capabilities,
    /* [in] */ Int32 capability)
{
    Int32 newCapabilities = capabilities & ~capability;
    return newCapabilities;
}

} // namespace Telephony
} // namespace Services
} // namespace Droid
} // namespace Elastos