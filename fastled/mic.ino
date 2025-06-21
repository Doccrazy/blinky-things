#define ARDUINO_ARCH_NRF52840 1

#include <mic.h>

#define SAMPLES 800
#define DEBUG 1                 // Enable pin pulse during ISR  

mic_config_t mic_config{
  .channel_cnt = 1,
  .sampling_rate = 16000,
  .buf_size = 1600,
  .debug_pin = LED_BUILTIN                // Toggles each DAC ISR (if DEBUG is set to 1)
};

NRF52840_ADC_Class Mic(&mic_config);

int16_t recording_buf[SAMPLES];
volatile uint8_t recording = 0;
volatile static bool record_ready = false;

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
      //int16_t sample = filter.step(recording_buf[i]);
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
      idx = 0;
      recording = 0;
      record_ready = true;
      break;
    } 
  }
}
