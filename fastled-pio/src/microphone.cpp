#include <arduinoFFT.h>
#include <AutoAnalogAudio.h>

AutoAnalog aaAudio;

#define SAMPLES 1024
#define SAMPLING_FREQUENCY 16000

float vReal[SAMPLES];
float vImag[SAMPLES]           = {0};
float weighingFactors[SAMPLES] = {0};

ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY, weighingFactors);

float freq, mag;

bool hadBeat = false;

#define BEAT_WINDOW 500         // Number of sample windows (each 64ms) to average for beat detection
#define BEAT_LF_COUNT 7         // Number of lowest frequencies to consider for beat detection
#define BEAT_THRESHOLD 20000.0f // Threshold for beat detection
#define BEAT_FACTOR 1.25f       // Factor to multiply the average magnitude for beat detection
#define BEAT_FACTOR_RESET 2.5f  // Factor to reset beat detection after a beat is detected
#define BEAT_RESET_WINDOW 10

void setup_mic() {
  // Startup the PDM Microphone
  aaAudio.begin(1, 0);           // PDM mic only
  aaAudio.autoAdjust       = 0;
  aaAudio.adcBitsPerSample = 16; // 16-bit audio at 16khz is the default on NRF52 and cannot be modified currently (in progress)
  aaAudio.dacBitsPerSample = 16;
  aaAudio.setSampleRate(16000);
  aaAudio.gain = 40;             // no effect
}

void run_mic() {
  if (AutoAnalog::adcReady) {
    // Get the latest ADC data
    aaAudio.getADC(SAMPLES);

    // Print the first sample for debugging
    // Serial.println(aaAudio.adcBuffer16[0]);

    // Copy the ADC data to the FFT input
    for (uint32_t i = 0; i < SAMPLES; i++) {
      // Convert unsigned 16-bit to signed 16-bit, then to float
      int16_t sample = (int16_t) aaAudio.adcBuffer16[i];
      vReal[i]       = (float) sample;
      vImag[i]       = 0.0f; // Imaginary part is zero for real input
    }
    FFT.dcRemoval();         // remove DC offset
    FFT.windowing(FFT_WIN_TYP_HANN, FFT_FORWARD);
    // Perform the FFT
    FFT.compute(FFTDirection::Forward);
    FFT.complexToMagnitude();
    // Get the major peak frequency and magnitude
    FFT.majorPeak(&freq, &mag);

    // Print the frequency and magnitude
    // Serial.print("Frequency: ");
    // Serial.print(freq);
    // Serial.print(" Hz, Magnitude: ");
    // Serial.println(mag);

    // calculate the maximum of the 10 lowest frequencies
    float avgMag = 0.0f;
    for (uint32_t i = 0; i < BEAT_LF_COUNT; i++) { avgMag += vReal[i]; }
    avgMag /= (float) BEAT_LF_COUNT;

    // calculate a rolling average of the last 10 magnitudes
    static float    rollingAvg[BEAT_WINDOW] = {0};
    static uint32_t index                   = 0;
    rollingAvg[index]                       = avgMag;
    index                                   = (index + 1) % BEAT_WINDOW;

    float avg = 0.0f;
    for (uint32_t i = 0; i < BEAT_WINDOW; i++) { avg += rollingAvg[i]; }
    avg /= (float) BEAT_WINDOW;

    Serial.print("maxMag = ");
    Serial.print(avgMag);
    Serial.print(", avg = ");
    Serial.print(avg);

    if (avg > BEAT_THRESHOLD && avgMag > avg * BEAT_FACTOR) {
      // Serial.println("Beat detected!");
      hadBeat = true;
    } else if (avgMag > avg * BEAT_FACTOR_RESET) {
      hadBeat = true;
      // calculate the average of the last 10 magnitudes from rollingAvg and index
      float resetAvg = 0.0f;
      for (uint32_t i = 0; i < BEAT_RESET_WINDOW; i++) { resetAvg += rollingAvg[(index - i + BEAT_WINDOW) % BEAT_WINDOW]; }
      resetAvg /= (float) BEAT_RESET_WINDOW;
      // fill rollingAvg array with resetAvg
      for (uint32_t i = 0; i < BEAT_WINDOW; i++) { rollingAvg[i] = resetAvg; }
    }
  }
}

bool beat_detected() {
  if (hadBeat) {
    hadBeat = false; // reset the beat detection flag
    return true;     // return true if a beat was detected
  } else {
    return false;    // no beat detected
  }
}

void foo() {}

/*
#include <mic.h>

#define SAMPLES 800
#define DEBUG 1 // Enable pin pulse during ISR

mic_config_t mic_config{
  .channel_cnt   = 1,
  .sampling_rate = 16000,
  .buf_size      = 1600,
  .debug_pin     = LED_BUILTIN // Toggles each DAC ISR (if DEBUG is set to 1)
};

NRF52840_ADC_Class Mic(&mic_config);

int16_t              recording_buf[SAMPLES];
volatile uint8_t     recording    = 0;
volatile static bool record_ready = false;

static void audio_rec_callback(uint16_t *buf, uint32_t buf_len);

void setup_mic() {
  Mic.set_callback(audio_rec_callback);

  if (!Mic.begin()) {
    Serial.println("Mic initialization failed");
    while (1);
  }

  Serial.println("Mic initialization done.");
}

void run_mic() {
  if (record_ready) {
    Serial.println("Finished sampling");

    for (int i = 0; i < SAMPLES; i++) {
      // int16_t sample = filter.step(recording_buf[i]);
      int16_t sample = recording_buf[i];
      Serial.println(sample);
    }

    record_ready = false;
  }
}

static void audio_rec_callback(uint16_t *buf, uint32_t buf_len) {
  static uint32_t idx = 0;
  // Copy samples from DMA buffer to inference buffer
  for (uint32_t i = 0; i < buf_len; i++) {
    // Convert 12-bit unsigned ADC value to 16-bit PCM (signed) audio value
    recording_buf[idx++] = buf[i];

    if (idx >= SAMPLES) {
      idx          = 0;
      recording    = 0;
      record_ready = true;
      break;
    }
  }
}
*/