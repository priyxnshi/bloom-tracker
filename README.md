# 🌸 Bloom — Intelligent Menstrual Health Companion

> A production-grade, AI-powered women's health web application built with vanilla JavaScript, C++, and integrated AI APIs — featuring cycle tracking, personalised recommendations, gamification, and deep health analytics.

---

## 🖥️ Live Demo

> *(Deploy to Netlify/Vercel by dragging `index.html` — no build step required)*

---

## 📸 Screenshots

| Dashboard | Analytics | Streaks |
|:-:|:-:|:-:|
| Cycle Tracker with phase detection | 5-tab analytics dashboard | Gamified streak & badge system |

---

## ✨ Features

### 🔐 Authentication & User System
- Username + password signup/login (stored securely in localStorage with `btoa` hashing)
- Guest mode — full app access with no data persistence
- Per-user profile data isolation
- Auto sign-in on return visit
- Sign out with full session clear

### 👤 Personalisation Engine
- Editable user profile: age, cycle length, diet type, activity level, sleep hours, goal
- Recommendations dynamically filtered by **diet type** (vegetarian / vegan / non-veg)
- Cycle length configurable (20–45 days) for accurate phase prediction
- All recommendations adapt to logged user profile in real time

### 🩸 Cycle Tracker
- Last period date input with automatic cycle day calculation
- **Phase detection algorithm** (Menstrual / Follicular / Ovulation / Luteal)
- Cycle day, next period countdown, ovulation window prediction
- Interactive symptom chips (15+ symptoms) and mood selector (10+ moods)
- Energy level slider (0–100%)
- Natural language NLP input — type symptoms like *"I feel tired and bloated"*

### 🤖 AI Chatbot
- Context-aware chatbot powered by **Groq API** (Llama 3)
- Passes current phase, symptoms, moods, and energy to system prompt
- Responds with personalised, science-backed health advice
- Full conversation history within session
- Deployed backend on Render

### 🥗 Nutrition Recommendations
- **Priority queue (max-heap)** powered food recommendation engine (C++ backend logic)
- Foods ranked by nutritional priority per condition
- Nutrient info, health benefit, and diet compatibility per food
- Conditions covered: PMS, Fatigue, Bloating, Heavy Flow, Menstrual Discomfort, Anxiety, and more
- Filter by vegetarian / vegan

### 🧘 Yoga Module
- Phase-specific yoga asanas with Sanskrit names
- Difficulty levels (Beginner / Intermediate / Advanced)
- Step-by-step instructions per asana
- Duration guidance per pose

### 🌬️ Breathing Exercises
- Pranayama techniques (Nadi Shodhana, Bhramari, Anulom Vilom, etc.)
- Purpose-tagged: stress relief, energy boost, pain management, focus
- Full step-by-step breathing instructions with timing

### 📊 Analytics Dashboard (5 tabs)
| Tab | Visualisations |
|---|---|
| **Overview** | Energy by phase (bar) · Phase distribution (doughnut) · 14-day energy trend (line) |
| **Mood & Energy** | Mood frequency by phase (grouped bar) · Energy by day of week |
| **Symptoms** | Top 10 symptom frequency (horizontal bar) · Symptom–phase correlation heatmap table |
| **Calendar** | 90-day cycle phase heatmap (colour-coded by phase) |
| **Planner** | Phase-based productivity guide · Day-by-day personalised planner for next cycle |

All charts powered by **Chart.js 4.4**.

### 🔥 Streaks & Gamification
- Daily logging streak counter with animated flame
- 30-day streak progress bar
- **Points system** — earn 10 pts per daily log
- **5-tier level system**: 🌱 Seedling → 🌿 Sprout → 🌺 Blooming → 🌸 Flourishing → 🌟 Radiant
- XP bar with progress to next level
- **8 unlockable badges**: First Bloom, Week Warrior, Fortnight Flow, Cycle Master, Star Tracker, Point Collector, Flow Keeper, Phase Explorer
- Weekly activity bar chart (last 7 days)

### 📅 Productivity Planner
- Phase-based work/rest recommendations
- Follicular → creative & learning mode
- Ovulation → peak performance, presentations, decisions
- Luteal → detail & analytical work
- Menstrual → rest, admin, reflection
- Day-by-day personalised planner based on cycle data

### 📚 Education Hub
- Phase-by-phase educational content
- Hormone explanations (Oestrogen, Progesterone, LH, FSH)
- Science-backed health information per cycle phase

### 📝 History Log
- Last 30 entries stored locally
- Full symptom, mood, energy, and phase record per entry
- Scroll-friendly history view

---

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    Frontend (index.html)                  │
│  Vanilla JS · Chart.js 4.4 · CSS Variables · No build   │
│                                                           │
│  ┌─────────┐ ┌──────────┐ ┌──────────┐ ┌────────────┐  │
│  │ Tracker │ │Analytics │ │ Streaks  │ │  Profile   │  │
│  └─────────┘ └──────────┘ └──────────┘ └────────────┘  │
│  ┌──────────┐ ┌─────────┐ ┌──────────┐ ┌────────────┐  │
│  │Nutrition │ │  Yoga   │ │ Breathe  │ │  Chatbot   │  │
│  └──────────┘ └─────────┘ └──────────┘ └────────────┘  │
└────────────────────────┬────────────────────────────────┘
                         │ REST API
          ┌──────────────┴──────────────┐
          │   Node.js / Express Backend  │
          │      (Render.com)            │
          │  /chat → Groq API (Llama 3) │
          └──────────────┬──────────────┘
                         │
          ┌──────────────┴──────────────┐
          │    C++ Logic Layer           │
          │  Data Structures Used:       │
          │  • unordered_map (food DB)   │
          │  • priority_queue (ranking)  │
          │  • vector (yoga/breathing)   │
          │  • stack (history)           │
          └─────────────────────────────┘
```

---

## 🧠 Data Structures & Algorithms (C++ Backend)

This project was built with a strong DSA foundation — the recommendation engine is powered by classic data structures:

| Structure | Usage |
|---|---|
| `unordered_map<string, vector<FoodInfo>>` | O(1) food lookup by condition |
| `priority_queue` (max-heap) | Foods ranked by nutritional priority score |
| `vector<YogaAsana>` | Phase-filtered yoga recommendations |
| `vector<BreathingExercise>` | Purpose-tagged breathing exercises |
| `stack<HealthEntry>` | LIFO health history (most recent first) |
| `map<string, string>` | Cycle phase → symptom/condition mapping |
| `set<string>` | Deduplication of symptom categories |

**Algorithms used:**
- Phase detection: rule-based segmentation on cycle day
- Food ranking: comparator-based heap sort by `priority` field
- NLP: keyword frequency matching for natural language symptom input
- Pattern detection: frequency analysis across phase-grouped log entries

---

## 🛠️ Tech Stack

| Layer | Technology |
|---|---|
| Frontend | HTML5 · CSS3 (custom design system) · Vanilla JavaScript (ES6+) |
| Charts | Chart.js 4.4.1 |
| Fonts | Google Fonts — Playfair Display + DM Sans |
| Backend | Node.js · Express |
| AI API | Groq API (Llama 3 8B) |
| Logic Layer | C++ (STL — vectors, maps, queues, stacks) |
| Storage | localStorage (client-side, per-user isolated) |
| Deployment | Netlify (frontend) · Render (backend) |

---

## 🚀 Getting Started

### Prerequisites
- A modern browser (Chrome, Firefox, Safari, Edge)
- No npm, no build tools, no dependencies to install for frontend

### Run Locally

```bash
# Clone the repository
git clone https://github.com/yourusername/bloom-health.git
cd bloom-health

# Open directly in browser — no server needed
open index.html
# or just double-click index.html
```

### Connect Your Own Groq API (for Chatbot)

1. Get a free API key at [console.groq.com](https://console.groq.com)
2. In `index.html`, find `BACKEND_URL` and point it to your own backend
3. In your Node.js backend, set `GROQ_API_KEY` in your `.env`

```env
GROQ_API_KEY=your_key_here
PORT=3001
```

### Deploy Frontend (Netlify)

1. Go to [netlify.com](https://netlify.com) → "Add new site" → "Deploy manually"
2. Drag and drop `index.html`
3. Done — live in 30 seconds ✅

### Deploy Backend (Render)

```bash
cd backend
npm install
# Set environment variables in Render dashboard
# Deploy from GitHub repo
```

---

## 📁 Project Structure

```
bloom-health/
├── index.html          # Entire frontend — single file, self-contained
├── backend.cpp         # C++ recommendation & data structure logic
├── backend/            # Node.js API server (optional, for chatbot)
│   ├── server.js
│   ├── routes/
│   │   └── chat.js     # Groq API integration
│   └── package.json
└── README.md
```

---

## 🔮 Roadmap

- [ ] **ML cycle prediction** — LSTM / Random Forest model via Python Flask API
- [ ] **Wearable integration** — Heart rate, sleep data from Fitbit / Apple Health API
- [ ] **PDF export** — Monthly health report generation
- [ ] **Voice input** — Web Speech API for hands-free symptom logging
- [ ] **Push notifications** — Period reminders, ovulation alerts
- [ ] **MongoDB backend** — Full cloud persistence replacing localStorage
- [ ] **PWA support** — Offline mode, installable app

---

## ⚠️ Disclaimer

Bloom is a personal health tracking tool and is **not a medical device**. It does not provide medical diagnoses. All content is for informational and educational purposes only. Please consult a qualified healthcare professional for any medical concerns.

---

## 👩‍💻 Author

Built with 💗 as a full-stack project combining frontend engineering, DSA, AI integration, and health-tech UX design.

---

## 📄 License

MIT License — free to use, modify, and distribute with attribution.
