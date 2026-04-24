const express = require('express');
const cors = require('cors');
const Groq = require('groq-sdk');

const app = express();
app.use(cors());
app.use(express.json());
app.use(express.static(__dirname));

const groq = new Groq({ apiKey: process.env.GROQ_API_KEY });
app.post('/chat', async (req, res) => {
  const { message, context } = req.body;

  try {
    const completion = await groq.chat.completions.create({
      model: 'llama-3.3-70b-versatile',
      messages: [
        {
          role: 'system',
          content: `You are Bloom AI, a compassionate menstrual health assistant.
User context: Phase=${context.phase}, Condition=${context.condition},
CycleDay=${context.cycleDay}, Energy=${context.energy},
Symptoms=${context.symptoms}, Moods=${context.moods}.
Give helpful, warm, evidence-based advice.`
        },
        { role: 'user', content: message }
      ],
      max_tokens: 512
    });

    const reply = completion.choices[0]?.message?.content || 'No response received.';
    res.json({ reply });

  } catch (err) {
    console.error('Server error:', err);
    res.status(500).json({ reply: 'Server error: ' + err.message });
  }
});

app.listen(8080, () => {
  console.log('Bloom AI server running at http://localhost:8080');
});