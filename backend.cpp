#include <iostream>
#include <vector>      
#include <queue>       
#include <stack>       
#include <map>         
#include <unordered_map> 
#include <set>         
#include <string>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

struct FoodInfo {
    string name;
    string nutrients;
    string benefit;
    int priority;
    bool isVegetarian;
    bool isVegan;

    FoodInfo() : name(""), nutrients(""), benefit(""), priority(0), isVegetarian(true), isVegan(true) {}
    FoodInfo(string n, string nut, string ben, int p, bool veg, bool vgn)
        : name(n), nutrients(nut), benefit(ben), priority(p), isVegetarian(veg), isVegan(vgn) {}
};

struct CompareFoodPriority {
    bool operator()(const FoodInfo& a, const FoodInfo& b) {
        return a.priority < b.priority;
    }
};

struct YogaAsana {
    string name;
    string sanskritName;
    string phase;
    string benefits;
    string difficulty;
    int duration;
    string steps;

    YogaAsana() : difficulty("beginner"), duration(0) {}
    YogaAsana(string n, string sn, string p, string b, string d, int dur, string st)
        : name(n), sanskritName(sn), phase(p), benefits(b), difficulty(d), duration(dur), steps(st) {}
};

struct BreathingExercise {
    string name;
    string sanskritName;
    string purpose;
    string benefits;
    int duration;
    string steps;

    BreathingExercise() : duration(0) {}
    BreathingExercise(string n, string sn, string p, string b, int d, string st)
        : name(n), sanskritName(sn), purpose(p), benefits(b), duration(d), steps(st) {}
};

struct HealthEntry {
    string date;
    string phase;
    int dayOfCycle;
    vector<int> symptoms;           
    vector<int> moods;              
    int energyLevel;
    string notes;
    string condition;

    HealthEntry() : dayOfCycle(0), energyLevel(5) {}
};

string cyclePhaseArray[28] = {
    "Menstrual","Menstrual","Menstrual","Menstrual","Menstrual",
    "Follicular","Follicular","Follicular","Follicular","Follicular",
    "Follicular","Follicular","Follicular",
    "Ovulation","Ovulation",
    "Luteal","Luteal","Luteal","Luteal","Luteal",
    "Luteal","Luteal","Luteal","Luteal","Luteal","Luteal","Luteal","Luteal"
};

map<string, string> cycleGraph = {
    {"Menstrual","Follicular"},
    {"Follicular","Ovulation"},
    {"Ovulation","Luteal"},
    {"Luteal","Menstrual"}
};

string symptomMenu[20] = {
    "Cramps", "Bloating", "Headache", "Fatigue", "Back Pain",
    "Breast Tenderness", "Nausea", "Diarrhea", "Constipation", "Acne",
    "Joint Pain", "Muscle Aches", "Hot Flashes", "Dizziness", "Insomnia",
    "Food Cravings", "Increased Appetite", "Water Retention", "Leg Pain", "Numbness"
};

string moodMenu[15] = {
    "Irritable", "Anxious", "Depressed", "Mood Swings", "Angry",
    "Sad", "Happy", "Energetic", "Calm", "Focused",
    "Stressed", "Overwhelmed", "Confident", "Social", "Withdrawn"
};

set<string> symptomCategories = {"Physical", "Emotional", "Digestive", "Sleep"};
set<string> conditionCategories = {"Normal", "PMS", "Fatigue", "Anxious", "Menstrual Discomfort", "Heavy Flow", "Bloating", "Mood Swings", "Breakouts", "Headaches"};

unordered_map<string, vector<FoodInfo>> foodHashTable = {
    {"PMS", {
        FoodInfo("Dark Leafy Greens (Spinach, Kale)", "Magnesium (40mg/cup), Iron (2.7mg), Calcium (100mg)", "Magnesium reduces muscle tension and cramps, iron prevents fatigue", 10, true, true),
        FoodInfo("Salmon", "Omega-3 fatty acids (2000mg), Vitamin D (450IU), B6", "Omega-3s reduce prostaglandin production (inflammation), regulate mood hormones", 10, false, false),
        FoodInfo("Dark Chocolate (70%+ cocoa)", "Magnesium (64mg/oz), Iron, Antioxidants (flavonoids)", "Magnesium relaxes muscles, releases endorphins for mood", 9, true, true),
        FoodInfo("Bananas", "Vitamin B6 (0.4mg), Potassium (422mg), Fiber", "B6 produces serotonin for mood, potassium reduces water retention", 9, true, true),
        FoodInfo("Whole Grains (Brown rice, Oats)", "Complex carbs, B vitamins, Magnesium, Fiber", "Stabilize blood sugar to prevent mood swings and cravings", 8, true, true),
        FoodInfo("Greek Yogurt", "Calcium (300mg), Probiotics, Protein, B12", "Calcium reduces PMS symptoms by 30%, probiotics aid gut-brain axis", 9, true, false),
        FoodInfo("Chickpeas", "B6 (0.5mg), Magnesium, Protein, Fiber", "B6 regulates neurotransmitters for mood stability", 8, true, true),
        FoodInfo("Pumpkin Seeds", "Magnesium (150mg/oz), Zinc, Iron, Omega-3", "Supports hormone balance and reduces inflammation", 8, true, true)
    }},
    {"Fatigue / Low Energy", {
        FoodInfo("Red Meat (Lean beef)", "Heme iron (2.5mg/3oz), B12 (2.4mcg), Protein, Zinc", "Heme iron is 2-3x more absorbable, B12 crucial for energy metabolism", 10, false, false),
        FoodInfo("Lentils", "Iron (3.3mg/cup), Protein (18g), Folate (358mcg), Fiber", "Plant iron plus folate for red blood cell production and sustained energy", 10, true, true),
        FoodInfo("Eggs", "B12 (0.6mcg), Protein (6g), Iron, Choline", "Complete protein with iron for hemoglobin production", 9, true, false),
        FoodInfo("Almonds & Cashews", "Magnesium (80mg/oz), Healthy fats, Vitamin E, Iron", "Magnesium converts food to energy, prevents muscle fatigue", 8, true, true),
        FoodInfo("Berries (Strawberries, Blueberries)", "Antioxidants, Vitamin C (85mg), Fiber", "Vitamin C increases iron absorption by up to 300%", 8, true, true),
        FoodInfo("Quinoa", "Iron (2.8mg/cup), Complete protein (8g), Magnesium", "All 9 essential amino acids for sustained energy", 9, true, true),
        FoodInfo("Spinach (Cooked)", "Iron (6.4mg/cup), Folate, Vitamin C", "Highest plant iron source, folate prevents anemia", 9, true, true),
        FoodInfo("Beetroot", "Nitrates, Iron, Folate, Vitamin C", "Nitrates improve oxygen delivery to muscles by 16%", 8, true, true),
        FoodInfo("Chia Seeds", "Omega-3 (5g/oz), Iron (2.2mg), Protein, Fiber", "Sustained energy release, omega-3 reduces fatigue", 8, true, true),
        FoodInfo("Spirulina", "Iron (2mg/tsp), Protein (4g), B12", "One of most iron-dense foods, boosts energy quickly", 9, true, true)
    }},
    {"Menstrual Discomfort", {
        FoodInfo("Ginger Tea", "Gingerol (anti-inflammatory compound)", "Blocks prostaglandin synthesis as effectively as ibuprofen", 10, true, true),
        FoodInfo("Turmeric (with black pepper)", "Curcumin (200mg), Piperine", "Curcumin reduces inflammation, piperine increases absorption 2000%", 10, true, true),
        FoodInfo("Chamomile Tea", "Apigenin (relaxant), Glycine", "Relaxes uterine muscles, reduces cramping pain by 25%", 9, true, true),
        FoodInfo("Pineapple", "Bromelain (enzyme), Vitamin C, Manganese", "Bromelain reduces heavy bleeding and menstrual pain", 8, true, true),
        FoodInfo("Flaxseeds (Ground)", "Lignans, Omega-3 (2.3g/tbsp), Fiber", "Lignans balance estrogen, omega-3 reduces cramping", 9, true, true),
        FoodInfo("Water (8+ glasses)", "Hydration", "Reduces bloating and helps flush excess sodium", 10, true, true),
        FoodInfo("Cinnamon", "Cinnamaldehyde (anti-inflammatory)", "Reduces menstrual pain and heavy bleeding", 8, true, true),
        FoodInfo("Papaya", "Papain enzyme, Carotene, Vitamin C", "Helps regulate periods and ease muscle contractions", 8, true, true),
        FoodInfo("Fennel Seeds", "Anethole, Phytoestrogens", "Reduces cramping intensity by 30%, antispasmodic properties", 8, true, true),
        FoodInfo("Mint Tea", "Menthol, Rosmarinic acid", "Natural muscle relaxant, eases digestive discomfort", 8, true, true)
    }},
    {"Heavy Flow", {
        FoodInfo("Red Meat (Grass-fed beef)", "Heme iron (3mg/3oz), B12, Zinc, Protein", "Replenishes iron lost during heavy bleeding (50-80mg per cycle)", 10, false, false),
        FoodInfo("Spinach (Cooked)", "Iron (6.4mg/cup), Vitamin K (888mcg), Folate", "Vitamin K aids blood clotting, iron prevents anemia", 10, true, true),
        FoodInfo("Lentils & Beans", "Iron (6.6mg/cup), Folate (358mcg), Protein", "High non-heme iron for hemoglobin production", 10, true, true),
        FoodInfo("Beetroot", "Iron (1.1mg), Folate (148mcg), Nitrates", "Increases hemoglobin and red blood cell count", 9, true, true),
        FoodInfo("Pumpkin Seeds", "Iron (4.2mg/oz), Zinc (2.2mg), Magnesium", "Excellent iron source plus zinc for blood health", 9, true, true),
        FoodInfo("Blackstrap Molasses", "Iron (3.5mg/tbsp), Calcium (172mg), B6", "Most concentrated plant iron source", 8, true, true),
        FoodInfo("Vitamin C Foods (Oranges, Bell peppers)", "Vitamin C (95mg)", "Enhances non-heme iron absorption by 300%", 9, true, true),
        FoodInfo("Dates", "Iron (1mg/date), Fiber, Natural sugars", "Quick energy plus iron in concentrated form", 8, true, true),
        FoodInfo("Liver (Chicken/Beef)", "Heme iron (10mg/3oz), B12, Folate, Vitamin A", "Highest natural iron source, rapidly replenishes stores", 10, false, false)
    }},
    {"Bloating", {
        FoodInfo("Asparagus", "Natural diuretic, Folate, Fiber, Vitamin K", "Reduces water retention and bloating naturally", 9, true, true),
        FoodInfo("Cucumber", "95% water, Potassium, Silica", "Flushes excess water, reduces puffiness", 8, true, true),
        FoodInfo("Watermelon", "92% water, Lycopene, Potassium", "Natural diuretic, hydrates while reducing bloating", 8, true, true),
        FoodInfo("Dandelion Tea", "Natural diuretic properties", "Increases urine output by 50%, reduces water weight", 9, true, true),
        FoodInfo("Peppermint Tea", "Menthol, Antispasmodic compounds", "Relaxes digestive muscles, reduces gas and bloating", 9, true, true),
        FoodInfo("Papaya", "Papain enzyme, Fiber", "Aids digestion, reduces gas and bloating", 8, true, true),
        FoodInfo("Celery", "Natural diuretic, Fiber, Potassium", "Reduces bloating and water retention", 7, true, true),
        FoodInfo("Lemon Water", "Vitamin C, Antioxidants", "Stimulates digestion, reduces bloating", 8, true, true)
    }},
    {"Anxious", {
        FoodInfo("Chamomile Tea", "Apigenin, Antioxidants", "Natural calming effect, reduces anxiety and promotes sleep", 10, true, true),
        FoodInfo("Yogurt with Probiotics", "Probiotics, Calcium, Protein", "Gut-brain axis health, mood regulation", 9, true, false),
        FoodInfo("Oats", "Complex carbohydrates, Fiber", "Steady mood through stable blood sugar", 8, true, true),
        FoodInfo("Fatty Fish (Salmon, Mackerel)", "Omega-3 fatty acids", "Reduces anxiety, supports brain health", 9, false, false),
        FoodInfo("Avocado", "Healthy fats, B vitamins, Potassium", "Stress reduction, hormone balance", 8, true, true),
        FoodInfo("Almonds", "Magnesium (80mg/oz), Vitamin E, Zinc", "Magnesium regulates stress hormones and neurotransmitters", 8, true, true),
        FoodInfo("Green Tea", "L-theanine, Antioxidants", "L-theanine promotes relaxation without drowsiness", 9, true, true),
        FoodInfo("Dark Chocolate", "Magnesium, Flavonoids", "Reduces cortisol (stress hormone), improves mood", 8, true, true)
    }},
    {"Mood Swings", {
        FoodInfo("Whole Grains", "Complex carbs, B vitamins, Fiber", "Stabilize blood sugar and serotonin production", 9, true, true),
        FoodInfo("Bananas", "Tryptophan, B6, Potassium", "Tryptophan converts to serotonin (mood regulator)", 9, true, true),
        FoodInfo("Walnuts", "Omega-3 (2.5g/oz), Magnesium, B vitamins", "Omega-3 supports brain health and emotional regulation", 8, true, true),
        FoodInfo("Sweet Potatoes", "Complex carbs, B6, Fiber", "Steady energy release prevents mood crashes", 8, true, true),
        FoodInfo("Eggs", "Choline, B12, Protein", "Choline supports neurotransmitter production", 8, true, false),
        FoodInfo("Turkey", "Tryptophan, Protein, B vitamins", "High tryptophan content boosts serotonin naturally", 8, false, false),
        FoodInfo("Greek Yogurt", "Probiotics, Calcium, Protein", "Gut bacteria influence mood and emotional health", 9, true, false)
    }},
    {"Breakouts", {
        FoodInfo("Pumpkin", "Vitamin A (200%), Zinc, Vitamin E", "Vitamin A regulates oil production, zinc fights bacteria", 9, true, true),
        FoodInfo("Sweet Potatoes", "Beta-carotene, Vitamin C, Fiber", "Beta-carotene converts to vitamin A for skin health", 9, true, true),
        FoodInfo("Green Tea", "EGCG (antioxidant), Polyphenols", "Reduces sebum production and inflammation by 50%", 10, true, true),
        FoodInfo("Fatty Fish", "Omega-3, Vitamin D, Selenium", "Anti-inflammatory omega-3 reduces acne severity", 9, false, false),
        FoodInfo("Berries", "Antioxidants, Vitamin C, Fiber", "Fight free radicals, reduce inflammation", 8, true, true),
        FoodInfo("Zinc-rich foods (Pumpkin seeds)", "Zinc (2.2mg/oz)", "Zinc reduces acne lesions by 40%", 9, true, true),
        FoodInfo("Probiotic foods (Kimchi, Sauerkraut)", "Probiotics, Vitamin K", "Gut health directly affects skin health", 8, true, true),
        FoodInfo("Turmeric", "Curcumin (anti-inflammatory)", "Reduces inflammatory acne and scarring", 8, true, true)
    }},
    {"Headaches", {
        FoodInfo("Water (10+ glasses)", "Hydration", "Dehydration causes 70% of headaches", 10, true, true),
        FoodInfo("Magnesium-rich foods (Almonds, Spinach)", "Magnesium (80mg/oz)", "Magnesium deficiency linked to migraines", 10, true, true),
        FoodInfo("Ginger", "Gingerol (anti-inflammatory)", "Blocks prostaglandins causing headache pain", 9, true, true),
        FoodInfo("Watermelon", "92% water, Electrolytes", "Hydrates and replenishes electrolytes", 8, true, true),
        FoodInfo("Peppermint Tea", "Menthol", "Relaxes blood vessels, reduces tension headaches", 8, true, true),
        FoodInfo("Salmon", "Omega-3, Vitamin D, B2", "Omega-3 reduces inflammation and headache frequency", 9, false, false),
        FoodInfo("Bananas", "Potassium, Magnesium, B6", "Electrolyte balance prevents headaches", 8, true, true),
        FoodInfo("Caffeine (moderate)", "Caffeine", "Constricts blood vessels, relieves headache (but don't overdo)", 7, true, true)
    }},
    {"Normal", {
        FoodInfo("Mixed Vegetables (Broccoli, Carrots, Bell peppers)", "Vitamins A, C, K, Fiber", "General health maintenance, immune support", 8, true, true),
        FoodInfo("Quinoa", "Complete protein, Fiber, Iron", "Sustained energy, muscle repair", 8, true, true),
        FoodInfo("Nuts (Almonds, Walnuts)", "Healthy fats, Protein, Vitamin E", "Brain health, heart health, satiety", 8, true, true),
        FoodInfo("Greek Yogurt", "Probiotics, Protein, Calcium", "Gut health, bone strength, digestive support", 8, true, false),
        FoodInfo("Berries (Blueberries, Strawberries)", "Antioxidants, Vitamin C, Fiber", "Anti-aging, immune boost, low calorie", 7, true, true),
        FoodInfo("Salmon", "Omega-3, Protein, Vitamin D", "Heart health, brain function, anti-inflammatory", 9, false, false),
        FoodInfo("Leafy Greens", "Vitamins, Minerals, Fiber", "Overall health and vitality", 8, true, true),
        FoodInfo("Whole Grains", "Complex carbs, B vitamins, Fiber", "Sustained energy, digestive health", 7, true, true)
    }}
};

vector<YogaAsana> yogaDatabase = {
   
    YogaAsana("Child's Pose", "Balasana", "Menstrual", "Relieves cramps and lower back pain, calms nervous system, reduces stress, gentle hip opener", "beginner", 5, "Kneel on floor, knees wide. Sit back on heels. Fold forward, forehead to floor. Arms extended forward or alongside body. Breathe deeply into lower back. Hold 3-5 minutes. Most restorative pose for menstruation."),
    YogaAsana("Cat-Cow Stretch", "Marjaryasana-Bitilasana", "Menstrual", "Increases spinal flexibility, massages abdominal organs, reduces menstrual cramps, improves circulation", "beginner", 3, "Start on hands and knees. Inhale, arch back (cow). Exhale, round spine (cat). Flow smoothly between poses. Coordinate with breath. Repeat 10-15 times. Gentle and soothing."),
    YogaAsana("Reclining Bound Angle", "Supta Baddha Konasana", "Menstrual", "Opens hips and pelvis, relieves menstrual discomfort, deeply relaxing, reduces bloating", "beginner", 10, "Lie on back. Bring soles of feet together. Let knees fall open. Place hands on belly or overhead. Use props under knees if needed. Breathe deeply. Hold 5-15 minutes. Perfect for heavy bleeding days."),
    YogaAsana("Supine Twist", "Supta Matsyendrasana", "Menstrual", "Massages digestive organs, relieves bloating and gas, gentle detox, calms nervous system", "beginner", 5, "Lie on back. Hug right knee to chest. Drop right knee across body to left. Extend right arm. Look right. Breathe into twist. Hold 2-3 minutes. Switch sides. Very gentle during menstruation."),
    
    YogaAsana("Downward Facing Dog", "Adho Mukha Svanasana", "Follicular", "Energizes entire body, strengthens arms and legs, improves circulation, increases energy", "beginner", 3, "Start on hands and knees. Lift hips up and back. Form inverted V shape. Press heels toward floor. Spread fingers wide. Relax head between arms. Hold 1-3 minutes. Building strength returns."),
    YogaAsana("Warrior I", "Virabhadrasana I", "Follicular", "Builds strength and stamina, improves focus and balance, increases confidence, energizing", "intermediate", 3, "From standing, step left foot back. Bend right knee 90°. Square hips forward. Raise arms overhead. Gaze upward. Ground through back heel. Hold 30-60 seconds. Switch sides. Empowering pose."),
    YogaAsana("Tree Pose", "Vrksasana", "Follicular", "Improves balance and focus, strengthens legs and core, increases concentration, grounding", "beginner", 2, "Stand on left leg. Place right foot on left inner thigh. Hands at heart or overhead. Find focal point. Engage core. Breathe steadily. Hold 30-60 seconds. Switch sides. Energy and balance peak."),
    YogaAsana("Warrior II", "Virabhadrasana II", "Follicular", "Builds leg and core strength, improves stamina, increases focus and determination", "beginner", 3, "Step feet wide apart. Turn right foot out 90°. Bend right knee over ankle. Extend arms parallel to floor. Gaze over right hand. Ground through both feet. Hold 30-60 seconds. Switch sides."),
    
     
    YogaAsana("Bridge Pose", "Setu Bandhasana", "Ovulation", "Opens chest and heart, energizes body, strengthens back and glutes, improves mood", "beginner", 3, "Lie on back, knees bent, feet hip-width. Press feet down, lift hips up. Interlace fingers under back. Roll shoulders under. Breathe into chest. Hold 30-60 seconds. Release slowly. Peak energy pose."),
    YogaAsana("Camel Pose", "Ustrasana", "Ovulation", "Deep backbend, opens entire front body, boosts energy and confidence, heart chakra opener", "advanced", 2, "Kneel with knees hip-width. Place hands on lower back. Lift chest, arch back. Reach for heels if comfortable. Drop head back. Hold 30 seconds. Come up slowly. Very energizing. Take breaks."),
    YogaAsana("Dancer's Pose", "Natarajasana", "Ovulation", "Improves balance and flexibility, opens chest, strengthens legs, graceful and energizing", "intermediate", 2, "Stand on left leg. Bend right knee, catch right foot. Kick foot into hand. Reach left arm forward. Arch back slightly. Hold 30 seconds. Switch sides. Peak physical performance."),
    YogaAsana("Wheel Pose", "Urdhva Dhanurasana", "Ovulation", "Full backbend, extreme energy boost, opens chest and shoulders, builds strength", "advanced", 1, "Lie on back. Bend knees, feet near hips. Hands by ears, fingers toward shoulders. Press up into full backbend. Straighten arms. Hold 10-30 seconds. Lower slowly. Only if experienced."),
    
   
    YogaAsana("Pigeon Pose", "Eka Pada Rajakapotasana", "Luteal", "Deep hip opener, releases stored emotions, reduces PMS tension, stretches hip flexors", "intermediate", 5, "From downward dog, bring right knee forward behind right wrist. Extend left leg back. Square hips forward. Fold forward over right leg. Rest on forearms or forehead. Breathe into hip. Hold 2-3 minutes. Release. Switch sides."),
    YogaAsana("Legs Up the Wall", "Viparita Karani", "Luteal", "Reduces swelling and bloating, improves circulation, calms nervous system, relieves tired legs", "beginner", 10, "Sit sideways against wall. Swing legs up wall as you lie back. Hips close to wall or few inches away. Arms relaxed at sides. Close eyes. Breathe deeply. Hold 5-15 minutes. Perfect before bed."),
    YogaAsana("Seated Forward Bend", "Paschimottanasana", "Luteal", "Calms mind, stretches spine and hamstrings, reduces stress and anxiety, introspective", "intermediate", 4, "Sit with legs extended. Inhale lengthen spine. Exhale fold forward from hips. Reach for feet or shins. Relax head and shoulders. Breathe steadily. Hold 2-4 minutes. Calming for luteal phase."),
    YogaAsana("Corpse Pose", "Savasana", "Luteal", "Deep relaxation, reduces stress and anxiety, lowers cortisol, promotes rest and restoration", "beginner", 10, "Lie flat on back. Legs extended, feet fall open naturally. Arms at sides, palms up. Close eyes. Scan body, release all tension. Breathe naturally. Stay 5-15 minutes. Essential for restoration. Perfect end to practice."),
    YogaAsana("Gentle Spinal Twist", "Bharadvajasana", "Luteal", "Massages digestive organs, relieves bloating, calms nervous system, gentle detox", "beginner", 4, "Sit with legs to left side. Place right hand behind. Left hand on right knee. Inhale lengthen. Exhale twist right. Look over right shoulder. Breathe into twist. Hold 1-2 minutes. Switch sides. Very soothing.")
};

vector<BreathingExercise> breathingDatabase = {
    BreathingExercise("4-7-8 Breathing", "Relaxing Breath", "anxiety", "Calms nervous system instantly, reduces heart rate by 10-15 bpm, improves sleep quality", 8, "Exhale completely through mouth with whoosh sound. Close mouth, inhale quietly through nose for 4 counts. Hold breath for 7 counts. Exhale completely through mouth for 8 counts. Repeat 4-8 times. Activates parasympathetic nervous system, reduces cortisol by 25% in 5 minutes."),
    BreathingExercise("Alternate Nostril Breathing", "Nadi Shodhana", "anxiety", "Balances hormones, reduces mood swings, improves focus, calms anxiety", 10, "Sit comfortably, spine straight. Close right nostril with thumb. Inhale slowly through left nostril (4 counts). Close left nostril with ring finger, open right. Exhale through right nostril (4 counts). Inhale right, exhale left - one round. Complete 5-10 rounds. Balances left and right brain hemispheres."),
    BreathingExercise("Box Breathing", "Sama Vritti", "stress", "Reduces stress immediately, improves focus and decision-making, lowers blood pressure", 10, "Exhale completely. Inhale through nose for 4 counts. Hold breath for 4 counts. Exhale through nose for 4 counts. Hold empty lungs for 4 counts. Repeat 5-10 minutes. Used by Navy SEALs, reduces stress hormones by 30%."),
    BreathingExercise("Cooling Breath", "Sitali Pranayama", "stress", "Reduces hot flashes, cools body during luteal phase, reduces inflammation", 5, "Sit comfortably with spine straight. Roll tongue into tube (or press between lips if can't roll). Inhale slowly through rolled tongue. Close mouth, exhale through nose. Repeat 10-15 times. Cools body temperature by 2-3°F."),
    BreathingExercise("Bellows Breath", "Bhastrika", "energy", "Boosts energy instantly, increases blood flow, improves metabolism, relieves fatigue", 3, "Sit with straight spine. Take deep inhale through nose. Exhale forcefully through nose. Repeat rapid cycles (1 per second). Do 10-20 breaths, then rest. Repeat 2-3 rounds. Increases oxygen by 15%. Avoid during menstruation or if dizzy."),
    BreathingExercise("Ocean Breath", "Ujjayi Pranayama", "anxiety", "Reduces anxiety, warms body, improves focus during yoga, regulates nervous system", 10, "Inhale deeply through nose. Slightly contract throat (like fogging mirror). Create ocean-like sound. Exhale through nose with same sound. Continue 5-10 minutes. Increases oxygenation, activates relaxation response."),
    BreathingExercise("Bee Breath", "Bhramari", "anxiety", "Instantly calms mind, reduces anger and anxiety, lowers blood pressure, improves focus", 5, "Sit comfortably, close eyes. Place index fingers on ears. Inhale deeply through nose. Exhale making humming bee sound. Feel vibration in head. Repeat 5-10 times. Scientifically proven to reduce stress in minutes."),
    BreathingExercise("Lion's Breath", "Simhasana", "stress", "Releases tension in face and jaw, relieves stress, energizing, fun and playful", 3, "Sit comfortably or kneel. Inhale deeply through nose. Exhale forcefully through mouth, stick tongue out, eyes wide, make 'ha' sound. Repeat 3-5 times. Releases pent-up tension. Good for frustration."),
    BreathingExercise("Breath of Fire", "Kapalabhati", "energy", "Energizing, detoxifying, clears sinuses, strengthens core, increases alertness", 5, "Sit with straight spine. Passive inhale through nose. Forceful exhale by contracting belly. Start slow, increase speed. Do 30-60 breaths, then rest. Repeat 2-3 rounds. Very energizing. Avoid during menstruation."),
    BreathingExercise("Three-Part Breath", "Dirga Pranayama", "sleep", "Deep relaxation, reduces stress, improves lung capacity, promotes better sleep", 10, "Sit or lie comfortably. Inhale into belly (expands). Continue into ribcage (expands). Finish into upper chest. Exhale from chest, ribs, belly. Repeat 5-10 minutes. Full yogic breath, very calming."),
    BreathingExercise("Humming Bee Breath Extended", "Bhramari Variations", "sleep", "Powerful for insomnia, calms mind chatter, reduces heart rate, promotes deep relaxation", 8, "Sit comfortably. Close eyes, relax face. Inhale deeply. Exhale making long humming sound. Feel vibration throughout head. Focus on sound. Repeat 10-15 times. Excellent before bed. Scientifically proven for sleep."),
    BreathingExercise("Pursed Lip Breathing", "Controlled Exhale", "stress", "Helps with breathlessness, reduces anxiety, improves breathing efficiency, calming", 5, "Inhale through nose (2 counts). Purse lips like blowing out candle. Exhale slowly through pursed lips (4-6 counts). Keep exhale longer than inhale. Repeat 5-10 minutes. Keeps airways open longer.")
};


vector<HealthEntry> historyStack;


stack<HealthEntry> redoStack;

int currentHistoryIndex = -1;


void printLine(char ch = '=', int length = 80) {
    cout << string(length, ch) << "\n";
}

void printHeader(string title) {
    printLine();
    cout << "  " << title << "\n";
    printLine();
}

void displaySymptomMenu() {
    printHeader("SYMPTOM MENU (20 SYMPTOMS)");
    cout << left;
    for (int i = 0; i < 20; i++) {
        cout << "  [" << setw(2) << i << "] " << setw(30) << symptomMenu[i];
        if (i % 2 == 1) cout << "\n";
    }
    if (20 % 2 != 0) cout << "\n";
    printLine('-');
}

void displayMoodMenu() {
    printHeader("MOOD MENU (15 MOODS)");
    cout << left;
    for (int i = 0; i < 15; i++) {
        cout << "  [" << setw(2) << i << "] " << setw(30) << moodMenu[i];
        if (i % 2 == 1) cout << "\n";
    }
    if (15 % 2 != 0) cout << "\n";
    printLine('-');
}

string getPhaseByDay(int day) {
   
    if (day < 1 || day > 28) return "Invalid";
    return cyclePhaseArray[day - 1];
}

string detectCondition(vector<int>& symptoms) {
    
    if (symptoms.empty()) return "Normal";
    
   
    set<int> symptomSet(symptoms.begin(), symptoms.end());
    
    
    if (symptomSet.count(0)) return "Menstrual Discomfort";
 
    if (symptomSet.count(1)) return "Bloating";
  
    if (symptomSet.count(3)) return "Fatigue / Low Energy";
   
    if (symptomSet.count(2)) return "Headaches";
  
    if (symptomSet.count(9)) return "Breakouts";
    
    if (symptoms.size() >= 3) return "PMS";
    
    return "Normal";
}

string formatDuration(int minutes) {
    if (minutes < 60) {
        return to_string(minutes) + " minutes";
    } else {
        int hours = minutes / 60;
        int mins = minutes % 60;
        string result = to_string(hours) + " hour" + (hours > 1 ? "s" : "");
        if (mins > 0) result += " " + to_string(mins) + " min";
        return result;
    }
}

void displayFoodRecommendations(string condition, bool vegetarian, bool vegan) {
  
    priority_queue<FoodInfo, vector<FoodInfo>, CompareFoodPriority> pq;
    
    printHeader("NUTRITION RECOMMENDATIONS: " + condition);
    

    if (foodHashTable.find(condition) != foodHashTable.end()) {
        for (const auto& food : foodHashTable[condition]) {
            if (vegan && !food.isVegan) continue;
            if (vegetarian && !food.isVegetarian) continue;
            pq.push(food);
        }
    }

    int count = 1;
    while (!pq.empty()) {
        FoodInfo food = pq.top();
        pq.pop();
        
        cout << "\n  " << count++ << ". " << food.name << "\n";
        cout << "     Nutrients:  " << food.nutrients << "\n";
        cout << "     Benefits:   " << food.benefit << "\n";
        cout << "     Priority:   " << string(food.priority, '*') << " (" << food.priority << "/10)\n";
        cout << "     Diet Type:  " << (food.isVegan ? "Vegan" : food.isVegetarian ? "Vegetarian" : "Non-Vegetarian") << "\n";
    }
    
    if (count == 1) {
        cout << "\n  No foods found matching your dietary preferences.\n";
    }
    printLine();
}

void displayYogaRecommendations(string phase, string difficulty) {
  
    printHeader("YOGA RECOMMENDATIONS");
    cout << "  Filter - Phase: " << phase << " | Difficulty: " << difficulty << "\n";
    printLine('-');
    
    int count = 1;
    for (const auto& yoga : yogaDatabase) {
        if (phase != "All" && yoga.phase != phase) continue;
        if (difficulty != "all" && yoga.difficulty != difficulty) continue;
        
        cout << "\n  " << count++ << ". " << yoga.name << " (" << yoga.sanskritName << ")\n";
        cout << "     Phase:       " << yoga.phase << "\n";
        cout << "     Difficulty:  " << yoga.difficulty << "\n";
        cout << "     Duration:    " << formatDuration(yoga.duration) << "\n";
        cout << "     Benefits:    " << yoga.benefits << "\n";
        cout << "     How to do:   " << yoga.steps << "\n";
    }
    
    if (count == 1) {
        cout << "\n  No yoga poses found for the selected filters.\n";
    }
    printLine();
}

void displayBreathingExercises(string purpose) {
 
    printHeader("BREATHING EXERCISES (PRANAYAMA)");
    cout << "  Purpose Filter: " << (purpose == "all" ? "All Purposes" : purpose) << "\n";
    printLine('-');
    
    int count = 1;
    for (const auto& breathing : breathingDatabase) {
        if (purpose != "all" && breathing.purpose != purpose) continue;
        
        cout << "\n  " << count++ << ". " << breathing.name << " (" << breathing.sanskritName << ")\n";
        cout << "     Purpose:     " << breathing.purpose << "\n";
        cout << "     Duration:    " << formatDuration(breathing.duration) << "\n";
        cout << "     Benefits:    " << breathing.benefits << "\n";
        cout << "     How to do:   " << breathing.steps << "\n";
    }
    
    if (count == 1) {
        cout << "\n  No breathing exercises found for the selected purpose.\n";
    }
    printLine();
}

void displayHistory(int count) {
   
    printHeader("HEALTH HISTORY");
    
    if (currentHistoryIndex < 0) {
        cout << "  No history entries found. Start tracking your health!\n";
        printLine();
        return;
    }
    
    int start = max(0, currentHistoryIndex - count + 1);
    
    cout << left;
    cout << "  " << setw(15) << "Date" << setw(15) << "Phase" 
         << setw(22) << "Condition" << setw(10) << "Energy" << "\n";
    printLine('-');
    
    for (int i = start; i <= currentHistoryIndex; i++) {
        cout << "  " << setw(14) << historyStack[i].date 
             << setw(14) << historyStack[i].phase
             << setw(21) << historyStack[i].condition
             << setw(9) << (to_string(historyStack[i].energyLevel) + "/10") << "\n";
        
      
        if (!historyStack[i].symptoms.empty()) {
            cout << "     Symptoms: ";
            for (size_t j = 0; j < historyStack[i].symptoms.size(); j++) {
                cout << symptomMenu[historyStack[i].symptoms[j]];
                if (j < historyStack[i].symptoms.size() - 1) cout << ", ";
            }
            cout << "\n";
        }
        
     
        if (!historyStack[i].moods.empty()) {
            cout << "     Moods: ";
            for (size_t j = 0; j < historyStack[i].moods.size(); j++) {
                cout << moodMenu[historyStack[i].moods[j]];
                if (j < historyStack[i].moods.size() - 1) cout << ", ";
            }
            cout << "\n";
        }
        
        if (!historyStack[i].notes.empty()) {
            cout << "     Notes: " << historyStack[i].notes << "\n";
        }
        cout << "\n";
    }
    printLine();
}

void saveToHistory(HealthEntry& entry) {
   
    if (currentHistoryIndex + 1 < (int)historyStack.size()) {
        historyStack.resize(currentHistoryIndex + 1);
    }
    historyStack.push_back(entry);
    currentHistoryIndex++;
    
    while (!redoStack.empty()) {
        redoStack.pop();
    }
}

void displayMainMenu() {
    printHeader("🌸 MENSTRUAL HEALTH TRACKER 🌸");
    cout << "  Available Commands:\n\n";
    cout << "  report     - Log your daily health entry (symptoms, moods, energy)\n";
    cout << "  foods      - Get personalized nutrition recommendations\n";
    cout << "  yoga       - Find yoga poses for your cycle phase\n";
    cout << "  breathing  - Discover breathing exercises (pranayama)\n";
    cout << "  history    - View your health history\n";
    cout << "  cycle      - View cycle phase information\n";
    cout << "  exit       - Exit the program\n";
    printLine();
}

void displayCycleInfo() {
  
    printHeader("MENSTRUAL CYCLE PHASES & TRANSITIONS");
    
    cout << "  Phase Transitions (Graph Structure):\n";
    for (const auto& transition : cycleGraph) {
        cout << "    " << transition.first << " → " << transition.second << "\n";
    }
    
    cout << "\n  Phase Duration:\n";
    cout << "    🩸 Menstrual Phase:  Days 1-5   (5 days)  - Bleeding, low energy\n";
    cout << "    🌱 Follicular Phase: Days 6-13  (8 days)  - Energy rising, building\n";
    cout << "    ✨ Ovulation Phase:  Days 14-15 (2 days)  - Peak energy & confidence\n";
    cout << "    🌙 Luteal Phase:     Days 16-28 (13 days) - Energy declining, PMS\n";
    printLine();
}

int main() {
    displayMainMenu();

    string command;
    while (true) {
        cout << "\n> Enter command: ";
        cin >> command;

        if (command == "exit") {
            printHeader("GOODBYE!");
            cout << "  Thank you for using Menstrual Health Tracker.\n";
            cout << "  Take care of yourself! 🌸\n";
            printLine();
            break;
        }

        else if (command == "report") {
            HealthEntry entry;
            
            printHeader("DAILY HEALTH REPORT");
            
            cout << "  Day of cycle (1-28): ";
            cin >> entry.dayOfCycle;
            
            if (entry.dayOfCycle < 1 || entry.dayOfCycle > 28) {
                cout << "\n  ❌ Invalid! Please enter day between 1-28.\n";
                continue;
            }

            cout << "  Energy level (1-10): ";
            cin >> entry.energyLevel;

            displaySymptomMenu();
            
            int symptomCount;
            cout << "  How many symptoms do you have? ";
            cin >> symptomCount;

            for (int i = 0; i < symptomCount; i++) {
                int symptomId;
                cout << "  Enter symptom ID " << (i+1) << ": ";
                cin >> symptomId;
                
                if (symptomId >= 0 && symptomId < 20) {
                    entry.symptoms.push_back(symptomId);
                } else {
                    cout << "  ⚠️  Invalid ID! Must be 0-19. Try again.\n";
                    i--;
                }
            }

            displayMoodMenu();
            
            int moodCount;
            cout << "  How many moods are you experiencing? ";
            cin >> moodCount;

            for (int i = 0; i < moodCount; i++) {
                int moodId;
                cout << "  Enter mood ID " << (i+1) << ": ";
                cin >> moodId;
                
                if (moodId >= 0 && moodId < 15) {
                    entry.moods.push_back(moodId);
                } else {
                    cout << "  ⚠️  Invalid ID! Must be 0-14. Try again.\n";
                    i--;
                }
            }

            entry.phase = getPhaseByDay(entry.dayOfCycle);
            entry.condition = detectCondition(entry.symptoms);

            time_t now = time(0);
            tm* timeInfo = localtime(&now);
            stringstream ss;
            ss << (1900 + timeInfo->tm_year) << "-" 
               << setfill('0') << setw(2) << (1 + timeInfo->tm_mon) << "-" 
               << setw(2) << timeInfo->tm_mday;
            entry.date = ss.str();

            cin.ignore();
            cout << "\n  Additional notes (optional): ";
            getline(cin, entry.notes);

            saveToHistory(entry);

            printLine();
            cout << "  ✅ Report saved successfully!\n\n";
            cout << "  📅 Date:       " << entry.date << "\n";
            cout << "  🌙 Phase:      " << entry.phase << " (Day " << entry.dayOfCycle << ")\n";
            cout << "  💊 Condition:  " << entry.condition << "\n";
            cout << "  ⚡ Energy:     " << entry.energyLevel << "/10\n";
            printLine();
        }
        
        else if (command == "foods") {
            string condition;
            int veg, vegan;
            
            cout << "\n  Available conditions:\n";
            cout << "  - Normal\n  - PMS\n  - Fatigue / Low Energy\n  - Menstrual Discomfort\n";
            cout << "  - Heavy Flow\n  - Bloating\n  - Anxious\n  - Mood Swings\n";
            cout << "  - Breakouts\n  - Headaches\n\n";
            
            cout << "  Enter condition: ";
            cin.ignore();
            getline(cin, condition);
            
            cout << "  Vegetarian? (1=yes, 0=no): ";
            cin >> veg;
            cout << "  Vegan? (1=yes, 0=no): ";
            cin >> vegan;
            
            cout << "\n";
            displayFoodRecommendations(condition, veg == 1, vegan == 1);
        }
        
        else if (command == "yoga") {
            string phase, difficulty;
            
            cout << "\n  Phase (Menstrual/Follicular/Ovulation/Luteal/All): ";
            cin >> phase;
            cout << "  Difficulty (beginner/intermediate/advanced/all): ";
            cin >> difficulty;
            
            cout << "\n";
            displayYogaRecommendations(phase, difficulty);
        }
        
        else if (command == "breathing") {
            string purpose;
            
            cout << "\n  Purpose (anxiety/stress/sleep/energy/all): ";
            cin >> purpose;
            
            cout << "\n";
            displayBreathingExercises(purpose);
        }
        
        else if (command == "history") {
            int count;
            cout << "\n  Number of entries to show: ";
            cin >> count;
            
            cout << "\n";
            displayHistory(count);
        }
        
        else if (command == "cycle") {
            cout << "\n";
            displayCycleInfo();
        }
        
        else {
            cout << "\n  ❌ Invalid command!\n";
            cout << "  Available: report, foods, yoga, breathing, history, cycle, exit\n";
        }
    }

    return 0;
}