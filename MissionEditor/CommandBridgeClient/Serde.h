#pragma once

#include <set>
#include <afxstr.h>
#include "CommandBridge.h"
#include "TriggerDef.h"

namespace Serde {

    using StrViewVec = std::vector<RPCB_StrView>;

    class CacheBuilder {
    private:
        StrViewVec items;
        std::set<CString> stringPool;

    protected:
        template<size_t N>
        void pushKeyValue(const char(&key)[N], RPCB_StrView val) {
            items.emplace_back(key, N - 1);
            items.emplace_back(val);
        }
        void pushKeyValue(CString&& key, const CString& val) {
            items.emplace_back(ToStr(key));
            items.emplace_back(val, val.GetLength());
        }

        inline RPCB_StrView toStr(const CString& s) {
            auto const [result, _] = stringPool.insert(s);
            auto const& str = *result;
            return { str, static_cast<size_t>(str.GetLength()) };
        }

    public:
        CacheBuilder(size_t capcity = 16) {
            items.reserve(capcity);
        }

        const size_t Size() const { return items.size(); }
        const StrViewVec& Build() const { return items; }

        template <typename T>
        RPCB_StrView ToStr(const T& val);

        template<size_t N>
        void PushKeyValue(const char(&key)[N], const CString& val) {
            pushKeyValue(key, { val, static_cast<size_t>(val.GetLength()) });
        }
        template<size_t N>
        void PushKeyValue(const char(&key)[N], int val) {
            pushKeyValue(key, ToStr(val));
        }
        template<size_t N>
        void PushKeyValue(const char(&key)[N], size_t val) {
            pushKeyValue(key, ToStr(val));
        }
        template<size_t N>
        void PushKeyValue(const char(&key)[N], bool val) {
            pushKeyValue(key, ToStr(val));
        }

        template <>
        inline RPCB_StrView ToStr<CString>(const CString& s) {
            return toStr(s);
        }

        template <>
        inline RPCB_StrView ToStr<int>(const int& val) {
            CString s;
            s.Format(_T("%d"), val);
            return toStr(s);
        }
        template <>
        inline RPCB_StrView ToStr<size_t>(const size_t& val) {
            CString s;
            s.Format(_T("%llu"), val);
            return toStr(s);
        }
        template <>
        inline RPCB_StrView ToStr<bool>(const bool& isTrue) { return ToStrView(isTrue ? "1" : "0"); }
    };

    class TriggerCacheBuilder : public CacheBuilder {
    public:
        using CacheBuilder::CacheBuilder;

        inline void WriteControls(const TriggerOptions& opts) {
            pushKeyValue("disabled", ToStr(opts.controls[TriggerOptions::Disable]));
            pushKeyValue("easy", ToStr(opts.controls[TriggerOptions::Easy]));
            pushKeyValue("medium", ToStr(opts.controls[TriggerOptions::Medium]));
            pushKeyValue("hard", ToStr(opts.controls[TriggerOptions::Hard]));
            pushKeyValue("mustTransfer", ToStr(opts.controls[TriggerOptions::MustTransfer]));
        }

        inline void WriteEvent(const TriggerEvent& ev) {
            pushKeyValue("eventType", ToStr(ev.eventType));
            PushKeyValue("param1", ev.param1);
            if (ev.param2) {
                PushKeyValue("param2", *ev.param2);
            }
        }

        inline void WriteAction(const TriggerAction& act) {
            pushKeyValue("actionType", ToStr(act.ActionType()));
            pushKeyValue("actionCode", ToStr(act.ActionCode()));
            pushKeyValue("waypoint", ToStr(act.Waypoint()));

            for (int pi = 0; pi < act.Params().size(); ++pi) {
                CString keyName;
                keyName.Format("param%d", pi);
                pushKeyValue(std::move(keyName), act.Params()[pi]);
            }
        }
    };
    
} // namespace Serde
