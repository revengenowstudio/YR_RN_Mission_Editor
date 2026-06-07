#pragma once

#include <map>
#include <string>
#include <string_view>
#include <vector>
#include <functional>
#include "CommandBridge.h"

/* ── Input extraction ────────────────────────────────────────────────────

   Since nlohmann::json uses std::map, the bridge delivers params in
   alphabetical key order.  We store field definitions in a std::map
   (also sorted) so that a single linear scan over the params can
   extract every known field — no O(n²) FindParam loop.
   ----------------------------------------------------------------------- */

class ParamExtractor {
public:
    using Handler = std::function<void(std::string_view value)>;

    /* Define a field to extract from the params. */
    void Add(const char* key, Handler handler) {
        m_fields.emplace(key, std::move(handler));
    }

    /* Walk params once; for each known key, call its handler. */
    void Extract(const RPCB_StrViewList& params) const {
        auto fieldIt = m_fields.begin();
        for (size_t i = 0; i + 1 < params.count && fieldIt != m_fields.end(); i += 2) {
            std::string_view key(params.items[i].data, params.items[i].len);
            // Advance field iterator to match or pass this key
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
