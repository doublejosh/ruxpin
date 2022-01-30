const fs = require('fs');
const util = require('util');
const textToSpeech = require('@google-cloud/text-to-speech');
const client = new textToSpeech.TextToSpeechClient();
const fireBaseData = require('./fortunes.json');
const fortunes = Object.values(fireBaseData.fortunes).map(f => f.text);

const phrases = [
  `A coin! <break strength="weak"/> Hi. <break strength="strong"/> Time for another fortune! <break strength="strong"/> Think about what you're uncertain about, please. <break strength="strong"/> <break strength="strong"/> I wonder what you're wondering`,
  `I'm saving our meeting. <break strength="strong"/> Future fortunes are now adjusted.`,
  `I picked one. You took too long.`,
  `Was that fortune on target?`,
  `Ok, good. Thanks! I'll remember.`,
  `Huh, ok, <break strength="weak"/> sorry. I'll remember.`,
  `I'm calibrating your fortune now. Let me think.`,
  `Huh. A freebie. <break strength="strong"/> Ok. I guess?`,
  `Ok. You picked yes.`,
  `Oh, ok. You picked no.`,
  `Hello? <break strength="strong"/> Do you have another coin?`
];

const questions = [
  `Are you thinking about something practical?`,
  `Awe, is there love in this story?`,
  `Would you be ok without it? <break strength="medium"/> That thing you're thinking about?`,
  `Is your question about, you? <break strength="medium"/> If not, that's ok too.`,
  `Do you ever answer yes to anything?`,
  `Do you ever answer no to anything?`,
  `Is the universe being obvious at you?`,
];

const killSpaces = (list) => list.map(f => f.replace(/  /g, " "))
const fortuneList = killSpaces(killSpaces(killSpaces(fortunes)))

const funny = [
  `I'm Teddy Ruckspin! <break strength="weak"/> Shelly Farnam, Do you want to go on an adventure?`
]

const quickStart = async (list, prefix) => {
  list.map(async (text, index) => {
    const [response] = await client.synthesizeSpeech({
      input: { ssml: `<speak>${text}</speak>` },
      voice: {
        languageCode: 'en-US',
        ssmlGender: 'MALE',
        name: 'en-US-Wavenet-I'
      },
      audioConfig: {
        speakingRate: 0.75,
        pitch: 8,
        audioEncoding: 'MP3',
      },
    });
  
    const writeFile = util.promisify(fs.writeFile);
    await writeFile(`./data/audio-${prefix}-${index}.mp3`, response.audioContent, 'binary');
  });

  console.log('Writing new audio files');
}

quickStart(funny, 'funny');
