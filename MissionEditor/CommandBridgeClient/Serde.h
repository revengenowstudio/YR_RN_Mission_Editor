#pragma once

#include "CommandBridge.h"
#include "TriggerDef.h"

namespace Serde {

inline void SetKeyValue(RPCB_StrView* items, size_t& n, const char* k, const CString& v) {
    items[n].data = k;
    items[n].len  = strlen(k);
    n++;
    items[n].data = v;
    items[n].len  = v.GetLength();
    n++;
}

inline void WriteControls(RPCB_StrView* items, size_t& n, const TriggerOptions& opts) {
    CString s;
    s.Format("%d", opts.controls[TriggerOptions::Disable] ? 1 : 0);
    SetKeyValue(items, n, "disabled", s);
    s.Format("%d", opts.controls[TriggerOptions::Easy] ? 1 : 0);
    SetKeyValue(items, n, "easy", s);
    s.Format("%d", opts.controls[TriggerOptions::Medium] ? 1 : 0);
    SetKeyValue(items, n, "medium", s);
    s.Format("%d", opts.controls[TriggerOptions::Hard] ? 1 : 0);
    SetKeyValue(items, n, "hard", s);
    s.Format("%d", opts.controls[TriggerOptions::MustTransfer] ? 1 : 0);
    SetKeyValue(items, n, "mustTransfer", s);
}

inline void WriteEvent(RPCB_StrView* items, size_t& n, const TriggerEvent& ev) {
    CString s;
    s.Format("%d", ev.eventType);
    SetKeyValue(items, n, "eventType", s);
    SetKeyValue(items, n, "param1", ev.param1);
    if (ev.param2) {
        SetKeyValue(items, n, "param2", *ev.param2);
    }
}

inline void WriteAction(RPCB_StrView* items, size_t& n, const TriggerAction& act) {
    CString s;
    s.Format("%d", act.ActionType());
    SetKeyValue(items, n, "actionType", s);
    s.Format("%d", act.ActionCode());
    SetKeyValue(items, n, "actionCode", s);
    s.Format("%d", act.Waypoint());
    SetKeyValue(items, n, "waypoint", s);
    char key[8];
    for (int pi = 0; pi < 5; ++pi) {
        sprintf_s(key, "param%d", pi);
        SetKeyValue(items, n, key, act.Params()[pi]);
    }
}

} // namespace Serde
