/*
 * ============================================================
 *  BLOOM – AI Chat Backend Server
 *  Language  : C++17
 *  Library   : cpp-httplib (single header — see SETUP below)
 *  AI API    : Anthropic Claude  (or swap to OpenAI — see comments)
 *  Port      : 8080
 * ============================================================
 *
 *  SETUP (do this once):
 *  ─────────────────────
 *  1. Download httplib.h (single file, no install):
 *       curl -O https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h
 *     Put httplib.h in the same folder as this file.
 *
 *  2. Paste your API key on the line marked  ← PASTE YOUR KEY HERE
 *
 *  3. Compile:
 *       Linux/Mac:
 *         g++ -std=c++17 -o bloom_server bloom_server.cpp -lssl -lcrypto -lpthread
 *       Windows (MSVC):
 *         cl /std:c++17 bloom_server.cpp
 *       (Install OpenSSL first if missing: sudo apt install libssl-dev   or   brew install openssl)
 *
 *  4. Run:
 *       ./bloom_server
 *     You should see:  🌸 Bloom server running on http://localhost:8080
 *
 *  5. The frontend's chat widget will connect automatically — no changes needed.
 *
 * ============================================================
 *  HOW IT WORKS (full explanation at the bottom of this file)
 * ============================================================
 */

#define CPPHTTPLIB_OPENSSL_SUPPORT      // enable HTTPS so we can call Anthropic
#include "httplib.h"                    // the entire HTTP server + client in one header

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

// ============================================================
//  ★  YOUR API KEY — paste between the quotes
// ============================================================
const string ANTHROPIC_API_KEY = ""
// ============================================================

const string ANTHROPIC_API_URL  = "api.anthropic.com";
const string ANTHROPIC_ENDPOINT = "/v1/messages";
const string CLAUDE_MODEL       = "claude-sonnet-4-6";  // fast + smart, good for chat

// ============================================================
//  CYCLE / HEALTH KNOWLEDGE (used to build the AI system prompt)
//  These match the data already in your frontend so the AI
//  always gives phase-appropriate, contextually accurate answers.
// ============================================================

const map<string, string> PHASE_DESCRIPTIONS = {
    {"menstrual",  "Days 1-5. Oestrogen and progesterone are at their lowest. Energy is low, the uterine lining is shedding. Focus on rest, iron, warmth, and gentleness."},
    {"follicular", "Days 6-13. FSH rises, follicles grow, oestrogen climbs. Energy rebuilds, mood lifts, cognition sharpens. Good time to start new things."},
    {"ovulation",  "Days 14-16. LH surges, egg is released. Oestrogen peaks, testosterone briefly rises. Peak energy, social confidence, and creativity."},
    {"luteal",     "Days 17-28. Progesterone dominates. If no fertilisation occurs, both hormones drop causing PMS — bloating, mood swings, cravings, insomnia."}
};

const map<string, string> CONDITION_ADVICE = {
    {"Dysmenorrhea (period pain)", "Omega-3 foods, magnesium, chamomile tea, Child's Pose, 4-7-8 breathing"},
    {"PMS / Anxiety",              "B6 foods, magnesium, reduce caffeine, Box Breathing, Legs Up the Wall yoga"},
    {"Cycle fatigue",              "Iron-rich foods, Kapalabhati energising breath, gentle Sun Salutations"},
    {"Bloating & fluid retention", "Dandelion tea, asparagus, cucumber, Cat-Cow yoga, diaphragmatic breathing"},
    {"Emotional sensitivity",      "Tryptophan foods like bananas and chickpeas, Alternate Nostril Breathing"},
    {"Peak energy phase",          "High-intensity yoga, complex cognitive tasks, social activities"},
    {"Hormonal skin / hair changes","Zinc-rich foods like pumpkin seeds, reduce dairy, green tea"},
};

// ============================================================
//  MINI JSON HELPERS
//  (avoids needing a JSON library — keeps the file self-contained)
// ============================================================

// Extract a string value from a simple JSON key like "key":"value"
string jsonGetString(const string& json, const string& key) {
    string search = "\"" + key + "\":\"";
    size_t pos = json.find(search);
    if (pos == string::npos) return "";
    pos += search.size();
    size_t end = json.find("\"", pos);
    if (end == string::npos) return "";
    return json.substr(pos, end - pos);
}

// Extract a number value from a JSON key like "key":42
string jsonGetNumber(const string& json, const string& key) {
    string search = "\"" + key + "\":";
    size_t pos = json.find(search);
    if (pos == string::npos) return "";
    pos += search.size();
    size_t end = json.find_first_of(",}", pos);
    if (end == string::npos) return "";
    string val = json.substr(pos, end - pos);
    // trim whitespace
    val.erase(0, val.find_first_not_of(" \t\n\r"));
    val.erase(val.find_last_not_of(" \t\n\r") + 1);
    return val;
}

// Escape a string so it's safe to put inside a JSON string value
string jsonEscape(const string& s) {
    string out;
    for (char c : s) {
        if      (c == '"')  out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else                out += c;
    }
    return out;
}

// ============================================================
//  BUILD THE SYSTEM PROMPT
//  This is the personality + knowledge we inject into Claude
//  so it answers as a menstrual health expert, not a generic AI.
// ============================================================

string buildSystemPrompt(const string& phase, const string& condition,
                         const string& cycleDay, const string& energy,
                         const string& symptoms, const string& moods) {

    string phaseDesc = "";
    if (PHASE_DESCRIPTIONS.count(phase))
        phaseDesc = PHASE_DESCRIPTIONS.at(phase);

    string condAdvice = "";
    if (CONDITION_ADVICE.count(condition))
        condAdvice = CONDITION_ADVICE.at(condition);

    ostringstream prompt;
    prompt <<
        "You are Bloom AI, a compassionate and knowledgeable menstrual health assistant "
        "built into the Bloom app. You specialise in menstrual cycle health, nutrition, "
        "yoga, breathing exercises, and hormonal wellbeing. "
        "You are warm, non-judgmental, evidence-based, and speak like a knowledgeable friend — "
        "never clinical or cold. Keep responses concise (2-4 sentences unless the user asks for detail). "
        "Never diagnose medical conditions. Always recommend seeing a doctor for persistent or severe symptoms.\n\n"

        "CURRENT USER CONTEXT (use this to personalise every answer):\n"
        "- Cycle phase: "    << (phase.empty()     ? "unknown" : phase)     << "\n"
        "- Phase info: "     << (phaseDesc.empty()  ? "not available" : phaseDesc) << "\n"
        "- Cycle day: "      << (cycleDay.empty()   ? "unknown" : cycleDay + " of 28") << "\n"
        "- Condition: "      << (condition.empty()  ? "unknown" : condition)  << "\n"
        "- Energy level: "   << (energy.empty()     ? "unknown" : energy + "%") << "\n"
        "- Symptoms: "       << (symptoms.empty()   ? "none selected" : symptoms) << "\n"
        "- Moods: "          << (moods.empty()      ? "none selected" : moods)    << "\n"
        "- Suggested focus: "<< (condAdvice.empty() ? "general wellness" : condAdvice) << "\n\n"

        "Always refer to this context when answering. "
        "If the user asks what phase they're in, tell them. "
        "If they ask about foods, yoga, or breathing, tailor your answer to their current phase and condition. "
        "If context fields say 'unknown', gently ask the user to fill in their details in the app.";

    return prompt.str();
}

// ============================================================
//  CALL THE CLAUDE API
//  Sends the user's message + context to Anthropic and
//  returns Claude's text reply.
// ============================================================

string callClaude(const string& userMessage, const string& systemPrompt) {

    // Build the JSON request body for Anthropic's /v1/messages endpoint
    string requestBody =
        "{"
        "\"model\":\""        + CLAUDE_MODEL + "\","
        "\"max_tokens\":500,"                           // keep replies short and fast
        "\"system\":\""       + jsonEscape(systemPrompt) + "\","
        "\"messages\":["
          "{"
            "\"role\":\"user\","
            "\"content\":\"" + jsonEscape(userMessage) + "\""
          "}"
        "]"
        "}";

    // Create an HTTPS client pointing at Anthropic's API
    httplib::SSLClient cli(ANTHROPIC_API_URL, 443);
    cli.set_connection_timeout(10);   // 10 second timeout
    cli.set_read_timeout(30);         // Claude can take a moment

    // Set the required Anthropic headers
    httplib::Headers headers = {
        {"x-api-key",         ANTHROPIC_API_KEY},
        {"anthropic-version", "2023-06-01"},
        {"content-type",      "application/json"}
    };

    // Make the POST request
    auto res = cli.Post(ANTHROPIC_ENDPOINT, headers, requestBody, "application/json");

    if (!res) {
        // Network error — httplib couldn't connect
        return "Sorry, I couldn't connect to the AI service. Please check your internet connection.";
    }

    if (res->status != 200) {
        // API returned an error (wrong key, quota exceeded, etc.)
        cerr << "Claude API error " << res->status << ": " << res->body << "\n";

        if (res->status == 401)
            return "API key error — please check your key in bloom_server.cpp and restart the server.";
        if (res->status == 429)
            return "Rate limit reached. Please wait a moment and try again.";
        if (res->status == 529)
            return "Claude is overloaded right now. Please try again in a moment.";

        return "AI service error (" + to_string(res->status) + "). Please try again.";
    }

    /*  Anthropic response JSON looks like:
        {
          "content": [
            { "type": "text", "text": "Here is Claude's reply..." }
          ],
          ...
        }
        We find the first "text" block and extract it.
    */
    const string& body = res->body;

    // Find  "text":"..."  inside the content array
    string textKey = "\"text\":\"";
    size_t pos = body.find(textKey);
    if (pos == string::npos)
        return "I got a response but couldn't read it. Please try again.";

    pos += textKey.size();

    // Extract until the closing quote, handling escape sequences
    string reply;
    bool escaped = false;
    for (size_t i = pos; i < body.size(); i++) {
        char c = body[i];
        if (escaped) {
            if      (c == 'n')  reply += '\n';
            else if (c == 't')  reply += '\t';
            else if (c == '"')  reply += '"';
            else if (c == '\\') reply += '\\';
            else                reply += c;
            escaped = false;
        } else {
            if      (c == '\\') escaped = true;
            else if (c == '"')  break;   // end of string
            else                reply += c;
        }
    }

    return reply.empty() ? "I received an empty response. Please try again." : reply;
}

// ============================================================
//  ALTERNATIVE: OPENAI / GPT
//  If you prefer OpenAI, comment out callClaude() above
//  and uncomment this version. Change your key accordingly.
// ============================================================
/*
const string OPENAI_API_KEY = "sk-proj-PASTE_YOUR_OPENAI_KEY_HERE";

string callOpenAI(const string& userMessage, const string& systemPrompt) {
    string requestBody =
        "{"
        "\"model\":\"gpt-4o-mini\","
        "\"max_tokens\":500,"
        "\"messages\":["
          "{\"role\":\"system\",\"content\":\"" + jsonEscape(systemPrompt) + "\"},"
          "{\"role\":\"user\",\"content\":\""   + jsonEscape(userMessage)  + "\"}"
        "]"
        "}";

    httplib::SSLClient cli("api.openai.com", 443);
    cli.set_connection_timeout(10);
    cli.set_read_timeout(30);

    httplib::Headers headers = {
        {"Authorization", "Bearer " + OPENAI_API_KEY},
        {"Content-Type",  "application/json"}
    };

    auto res = cli.Post("/v1/chat/completions", headers, requestBody, "application/json");
    if (!res || res->status != 200) return "AI service error. Check your OpenAI key.";

    // OpenAI response: choices[0].message.content
    string textKey = "\"content\":\"";
    size_t pos = res->body.rfind(textKey);  // rfind gets the message content, not system
    if (pos == string::npos) return "Could not read response.";
    pos += textKey.size();
    string reply;
    bool escaped = false;
    for (size_t i = pos; i < res->body.size(); i++) {
        char c = res->body[i];
        if (escaped) { reply += (c == 'n' ? '\n' : c == '"' ? '"' : c); escaped = false; }
        else if (c == '\\') escaped = true;
        else if (c == '"') break;
        else reply += c;
    }
    return reply;
}
*/

// ============================================================
//  MAIN — HTTP SERVER
// ============================================================

int main() {
    // Validate API key before starting
    if (ANTHROPIC_API_KEY == "PASTE_YOUR_KEY_HERE" || ANTHROPIC_API_KEY.empty()) {
        cerr << "\n❌  No API key set!\n";
        cerr << "    Open bloom_server.cpp, find the line:\n";
        cerr << "      const string ANTHROPIC_API_KEY = \"PASTE_YOUR_KEY_HERE\";\n";
        cerr << "    Replace PASTE_YOUR_KEY_HERE with your key from console.anthropic.com\n\n";
        return 1;
    }

    httplib::Server svr;

    // ── CORS preflight ──────────────────────────────────────
    // Browsers send an OPTIONS request before cross-origin POSTs.
    // We must respond with the right headers or the browser blocks the request.
    svr.Options("/.*", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin",  "*");
        res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });

    // ── POST /chat ──────────────────────────────────────────
    // Frontend sends:
    //   { "message": "...", "context": { "phase": "...", "condition": "...", ... } }
    // We return:
    //   { "reply": "..." }
    svr.Post("/chat", [](const httplib::Request& req, httplib::Response& res) {

        // CORS header — required for browser to accept the response
        res.set_header("Access-Control-Allow-Origin", "*");

        const string& body = req.body;

        // Parse fields from the incoming JSON
        string message   = jsonGetString(body, "message");

        // Context is nested — find the "context":{...} block first
        string ctxBlock  = "";
        size_t ctxPos = body.find("\"context\":");
        if (ctxPos != string::npos) {
            size_t open  = body.find("{", ctxPos);
            size_t close = body.find("}", open);
            if (open != string::npos && close != string::npos)
                ctxBlock = body.substr(open, close - open + 1);
        }

        string phase     = jsonGetString(ctxBlock,  "phase");
        string condition = jsonGetString(ctxBlock,  "condition");
        string cycleDay  = jsonGetNumber(ctxBlock,  "cycleDay");
        string energy    = jsonGetNumber(ctxBlock,  "energy");
        string symptoms  = jsonGetString(ctxBlock,  "symptoms");
        string moods     = jsonGetString(ctxBlock,  "moods");

        if (message.empty()) {
            res.set_content("{\"reply\":\"I didn't receive a message. Please try again.\"}", "application/json");
            return;
        }

        cout << "[chat] phase=" << phase << " day=" << cycleDay
             << " | message: " << message.substr(0, 60) << "...\n";

        // Build the personalised system prompt and call Claude
        string systemPrompt = buildSystemPrompt(phase, condition, cycleDay, energy, symptoms, moods);
        string reply        = callClaude(message, systemPrompt);

        // Return JSON response
        string responseJson = "{\"reply\":\"" + jsonEscape(reply) + "\"}";
        res.set_content(responseJson, "application/json");
    });

    // ── GET /health ─────────────────────────────────────────
    // Simple ping endpoint — lets you check the server is running
    // Open http://localhost:8080/health in your browser to test
    svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("{\"status\":\"ok\",\"server\":\"Bloom AI\",\"model\":\"" + CLAUDE_MODEL + "\"}", "application/json");
    });

    cout << "\n🌸 Bloom AI server running on http://localhost:8080\n";
    cout << "   Model : " << CLAUDE_MODEL << "\n";
    cout << "   Endpoints:\n";
    cout << "     POST http://localhost:8080/chat    — AI chat\n";
    cout << "     GET  http://localhost:8080/health  — ping test\n";
    cout << "\n   Waiting for requests...\n\n";

    svr.listen("0.0.0.0", 8080);

    return 0;
}

/*
 * ============================================================
 *  HOW THE WHOLE THING WORKS — FULL EXPLANATION
 * ============================================================
 *
 *  THE FLOW (what happens when the user types in the chat box):
 *  ─────────────────────────────────────────────────────────
 *
 *  1. USER TYPES a message in the Bloom chat widget (index.html)
 *
 *  2. FRONTEND (JavaScript in index.html) collects:
 *       - The message text
 *       - The user's current health context from app state:
 *           phase, condition, cycleDay, energy, symptoms, moods
 *     Then sends a POST request to http://localhost:8080/chat
 *     with all of that as JSON.
 *
 *  3. THIS SERVER (bloom_server.cpp) receives the request.
 *     It parses the JSON body to extract the message and context fields.
 *
 *  4. buildSystemPrompt() constructs a detailed instruction block
 *     that tells Claude:
 *       - Who it is (Bloom AI, health companion)
 *       - The user's current phase, cycle day, condition, symptoms, moods
 *       - How to tone its responses (warm, concise, evidence-based)
 *     This means Claude's reply is always personalised, not generic.
 *
 *  5. callClaude() makes an HTTPS POST to api.anthropic.com/v1/messages
 *     with your API key in the header and the message + system prompt as body.
 *     Claude processes it and returns a JSON response.
 *
 *  6. We parse the "text" field out of Claude's JSON response.
 *
 *  7. We send that text back to the frontend as:
 *       { "reply": "Claude's answer here..." }
 *
 *  8. The frontend's sendChat() function receives this, removes the
 *     "typing..." indicator, and displays the reply as a chat bubble.
 *
 *  ─────────────────────────────────────────────────────────
 *  WHY cpp-httplib?
 *  ─────────────────────────────────────────────────────────
 *  It's a single .h file — no CMake, no package manager, no install.
 *  It handles both the HTTP server (listening for frontend requests)
 *  AND the HTTPS client (calling Anthropic's API).
 *  Used in production at companies like Stability AI and others.
 *
 *  ─────────────────────────────────────────────────────────
 *  WHY DOES THE SYSTEM PROMPT MATTER?
 *  ─────────────────────────────────────────────────────────
 *  Without the system prompt, Claude would answer like a generic AI.
 *  With it, Claude knows the user is in the Luteal phase, has cramps
 *  and anxiety, and is on day 22 — so it answers specifically:
 *  "Try 4-7-8 breathing tonight, your progesterone is dropping which
 *  amplifies anxiety in this phase..."
 *  That's what makes this feel like a real health companion vs a chatbot.
 *
 *  ─────────────────────────────────────────────────────────
 *  WHAT THE API KEY DOES
 *  ─────────────────────────────────────────────────────────
 *  Anthropic's API is a paid service (very cheap — ~$0.001 per message).
 *  The key is how Anthropic knows which account to bill.
 *  It goes in the HTTP header:  x-api-key: sk-ant-api03-xxxxx
 *  NEVER put this key in your frontend JS — anyone could steal it
 *  from the browser's Network tab. Keeping it here in the C++ server
 *  means it stays on your machine, safe.
 *
 *  ─────────────────────────────────────────────────────────
 *  QUICK CHECKLIST BEFORE RUNNING:
 *  ─────────────────────────────────────────────────────────
 *  [ ] httplib.h downloaded and in the same folder as this file
 *  [ ] API key pasted on line ~50 (ANTHROPIC_API_KEY = "sk-ant-...")
 *  [ ] Compiled with:  g++ -std=c++17 -o bloom_server bloom_server.cpp -lssl -lcrypto -lpthread
 *  [ ] Server running: ./bloom_server
 *  [ ] Test it:        open http://localhost:8080/health in browser → should show {"status":"ok"}
 *  [ ] Open index.html, type in chat → should get real AI replies
 * ============================================================
 */
