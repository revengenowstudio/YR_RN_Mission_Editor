#pragma once

#include <map>
#include <string>
#include <string_view>
#include <vector>
#include <functional>
#include "Serde.h"
#include "CommandBridge.h"
#include "CommandBridgeClient.h"

/* ── Callback helpers ──────────────────────────────────────────────────── */

inline void SendError(RPCB_CallContext* ctx, RPCB_StrView err) {
    RPCB_Response resp = {RPCB_PARAM_STRING_LIST, err, {}};
    CommandBridgeClient::Instance().SendResponse(ctx->uniqueId, &resp);
}

inline void SendOK(RPCB_CallContext* ctx, const Serde::StrViewVec& items) {
    RPCB_Response resp = {RPCB_PARAM_STRING_LIST, EmptyStrView(), { items.data(), items.size() }};
    CommandBridgeClient::Instance().SendResponse(ctx->uniqueId, &resp);
}

inline void SendOKEmpty(RPCB_CallContext* ctx) {
    RPCB_Response resp = {RPCB_PARAM_STRING_LIST, EmptyStrView(), {}};
    CommandBridgeClient::Instance().SendResponse(ctx->uniqueId, &resp);
}

inline void SendArrayOK(RPCB_CallContext* ctx, RPCB_StrView* items, size_t n) {
    RPCB_Response resp = {RPCB_PARAM_STRING_ARRAY, EmptyStrView(), {items, n}};
    CommandBridgeClient::Instance().SendResponse(ctx->uniqueId, &resp);
}

/* ── ParamExtractor ──────────────────────────────────────────────────────

   Since nlohmann::json uses std::map, the bridge delivers params in
   alphabetical key order.  We store field definitions in a std::map
   (also sorted) so that a single linear scan over the params can
   extract every known field — no O(n^2) FindParam loop.
   ----------------------------------------------------------------------- */

class ParamExtractor {
public:
    using Handler = std::function<void(std::string_view value)>;

    void Add(const char* key, Handler handler) {
        m_fields.emplace(key, std::move(handler));
    }

    void Extract(const RPCB_StrViewList& params) const {
        auto fieldIt = m_fields.begin();
        for (size_t i = 0; i + 1 < params.count && fieldIt != m_fields.end(); i += 2) {
            std::string_view key(params.items[i].data, params.items[i].len);
            while (fieldIt != m_fields.end() && fieldIt->first < key) {
                ++fieldIt;
            }
            if (fieldIt != m_fields.end() && fieldIt->first == key) {
                fieldIt->second(std::string_view(params.items[i + 1].data,
                                                  params.items[i + 1].len));
                ++fieldIt;
            }
        }
    }

private:
    std::map<std::string, Handler, std::less<>> m_fields;
};
