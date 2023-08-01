
#include <string.h>
#include "AFSK.h"
#include "Arduino.h"
#include <Wire.h>
#include <driver/adc.h>
#include "esp_adc_cal.h"
#include "cppQueue.h"
#include "ButterworthFilter.h"
#include "fir_filter.h"

extern "C"
{
#include "soc/syscon_reg.h"
#include "soc/syscon_struct.h"
}

#define DEBUG_TNC

extern unsigned long custom_preamble;
extern unsigned long custom_tail;
int adcVal;

bool input_HPF = false;

static const adc_unit_t unit = ADC_UNIT_1;

void sample_isr();
bool hw_afsk_dac_isr = false;

static filter_t bpf;
static filter_t lpf;
static filter_t hpf;

Afsk *AFSK_modem;

// //กรองความถี่สูงผ่าน >300Hz  HPF Butterworth Filter. 0-300Hz ช่วงความถี่ต่ำใช้กับโทน CTCSS/DCS ในวิทยุสื่อสารจะถูกรองทิ้ง
// ButterworthFilter hp_filter(600, SAMPLE_RATE, ButterworthFilter::ButterworthFilter::Highpass, 1);
// //กรองความถี่ต่ำผ่าน <3KHz  LPF Butterworth Filter. ความถี่เสียงที่มากกว่า 3.5KHz ไม่ใช่ความถี่เสียงคนพูดจะถูกกรองทิ้ง
// ButterworthFilter lp_filter(3500, 9600, ButterworthFilter::ButterworthFilter::Lowpass, 2);

// // Bandpass Filter
// void bandpass_filter(float *h, int n)
// {
//   int i = 0;
//   for (i = 0; i < n; i++)
//   {
//     h[i] = lp_filter.Update(h[i]);
//   }
//   for (i = 0; i < n; i++)
//   {
//     h[i] = hp_filter.Update(h[i]);
//   }
// }

uint8_t CountOnesFromInteger(uint8_t value)
{
  uint8_t count;
  for (count = 0; value != 0; count++, value &= value - 1)
    ;
  return count;
}

#define IMPLEMENTATION FIFO

#ifndef I2S_INTERNAL
cppQueue adcq(sizeof(int16_t), 19200, IMPLEMENTATION); // Instantiate queue
#endif

// Forward declerations
int afsk_getchar(void);
void afsk_putchar(char c);

#ifdef I2S_INTERNAL
// #define ADC_PATT_LEN_MAX (16)
// #define ADC_CHECK_UNIT(unit) RTC_MODULE_CHECK(adc_unit < ADC_UNIT_2, "ADC unit error, only support ADC1 for now", ESP_ERR_INVALID_ARG)
// #define RTC_MODULE_CHECK(a, str, ret_val)                                             \
//   if (!(a))                                                                           \
//   {                                                                                   \
//     ESP_LOGE(RTC_MODULE_TAG, "%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str); \
//     return (ret_val);                                                                 \
//   }

i2s_event_t i2s_evt;
static QueueHandle_t i2s_event_queue;

// static esp_err_t adc_set_i2s_data_len(adc_unit_t adc_unit, int patt_len)
// {
//   ADC_CHECK_UNIT(adc_unit);
//   RTC_MODULE_CHECK((patt_len < ADC_PATT_LEN_MAX) && (patt_len > 0), "ADC pattern length error", ESP_ERR_INVALID_ARG);
//   // portENTER_CRITICAL(&rtc_spinlock);
//   if (adc_unit & ADC_UNIT_1)
//   {
//     SYSCON.saradc_ctrl.sar1_patt_len = patt_len - 1;
//   }
//   if (adc_unit & ADC_UNIT_2)
//   {
//     SYSCON.saradc_ctrl.sar2_patt_len = patt_len - 1;
//   }
//   // portEXIT_CRITICAL(&rtc_spinlock);
//   return ESP_OK;
// }

// static esp_err_t adc_set_i2s_data_pattern(adc_unit_t adc_unit, int seq_num, adc_channel_t channel, adc_bits_width_t bits, adc_atten_t atten)
// {
//   ADC_CHECK_UNIT(adc_unit);
//   if (adc_unit & ADC_UNIT_1)
//   {
//     RTC_MODULE_CHECK((adc1_channel_t)channel < ADC1_CHANNEL_MAX, "ADC1 channel error", ESP_ERR_INVALID_ARG);
//   }
//   RTC_MODULE_CHECK(bits < ADC_WIDTH_MAX, "ADC bit width error", ESP_ERR_INVALID_ARG);
//   RTC_MODULE_CHECK(atten < ADC_ATTEN_MAX, "ADC Atten Err", ESP_ERR_INVALID_ARG);

//   // portENTER_CRITICAL(&rtc_spinlock);
//   // Configure pattern table, each 8 bit defines one channel
//   //[7:4]-channel [3:2]-bit width [1:0]- attenuation
//   // BIT WIDTH: 3: 12BIT  2: 11BIT  1: 10BIT  0: 9BIT
//   // ATTEN: 3: ATTEN = 11dB 2: 6dB 1: 2.5dB 0: 0dB
//   uint8_t val = (channel << 4) | (bits << 2) | (atten << 0);
//   if (adc_unit & ADC_UNIT_1)
//   {
//     SYSCON.saradc_sar1_patt_tab[seq_num / 4] &= (~(0xff << ((3 - (seq_num % 4)) * 8)));
//     SYSCON.saradc_sar1_patt_tab[seq_num / 4] |= (val << ((3 - (seq_num % 4)) * 8));
//   }
//   if (adc_unit & ADC_UNIT_2)
//   {
//     SYSCON.saradc_sar2_patt_tab[seq_num / 4] &= (~(0xff << ((3 - (seq_num % 4)) * 8)));
//     SYSCON.saradc_sar2_patt_tab[seq_num / 4] |= (val << ((3 - (seq_num % 4)) * 8));
//   }
//   // portEXIT_CRITICAL(&rtc_spinlock);
//   return ESP_OK;
// }

void I2S_Init(i2s_mode_t MODE, i2s_bits_per_sample_t BPS)
{
  i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN | I2S_MODE_ADC_BUILT_IN),
      .sample_rate = SAMPLE_RATE,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = (i2s_comm_format_t)I2S_COMM_FORMAT_STAND_I2S,
      .intr_alloc_flags = 0, // ESP_INTR_FLAG_LEVEL1,
      .dma_buf_count = 3,
      .dma_buf_len = 768,
      //.tx_desc_auto_clear   = true,
      .use_apll = false // no Audio PLL ( I dont need the adc to be accurate )
  };

  if (MODE == I2S_MODE_RX || MODE == I2S_MODE_TX)
  {
    Serial.println("using I2S_MODE");
    i2s_pin_config_t pin_config;
    pin_config.bck_io_num = PIN_I2S_BCLK;
    pin_config.ws_io_num = PIN_I2S_LRC;
    if (MODE == I2S_MODE_RX)
    {
      pin_config.data_out_num = I2S_PIN_NO_CHANGE;
      pin_config.data_in_num = PIN_I2S_DIN;
    }
    else if (MODE == I2S_MODE_TX)
    {
      pin_config.data_out_num = PIN_I2S_DOUT;
      pin_config.data_in_num = I2S_PIN_NO_CHANGE;
    }

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
    i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, BPS, I2S_CHANNEL_MONO);
  }
  else if (MODE == I2S_MODE_DAC_BUILT_IN || MODE == I2S_MODE_ADC_BUILT_IN)
  {
    Serial.println("Using I2S DAC/ADC_builtin");
    // install and start i2s driver
    if (i2s_driver_install(I2S_NUM_0, &i2s_config, 5, &i2s_event_queue) != ESP_OK)
    {
      Serial.println("ERROR: Unable to install I2S drives");
    }
    // GPIO36, VP
    // init ADC pad
    i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_0);
    // i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, BPS, I2S_CHANNEL_MONO);
    i2s_adc_enable(I2S_NUM_0);
    delay(500); // required for stability of ADC

    // ***IMPORTANT*** enable continuous adc sampling
    SYSCON.saradc_ctrl2.meas_num_limit = 0;
    // sample time SAR setting
    SYSCON.saradc_ctrl.sar_clk_div = 2;
    SYSCON.saradc_fsm.sample_cycle = 2;
    // adc_set_i2s_data_pattern(ADC_UNIT_1, 0, ADC_CHANNEL_0, ADC_WIDTH_BIT_12, ADC_ATTEN_DB_0); // Input Vref 1.36V=4095,Offset 0.6324V=1744
    // adc_set_i2s_data_len(ADC_UNIT_1, 1);

    i2s_set_pin(I2S_NUM_0, NULL);
    i2s_set_dac_mode(I2S_DAC_CHANNEL_LEFT_EN); // IO26
    i2s_zero_dma_buffer(I2S_NUM_0);
    // i2s_start(I2S_NUM_0);
    //  dac_output_enable(DAC_CHANNEL_1);
    dac_output_enable(DAC_CHANNEL_2);
    dac_i2s_enable();
  }
}

#else

hw_timer_t *timer = NULL;


void AFSK_TimerEnable(bool sts)
{
  if (timer == NULL)
    return;
  if (sts == true)
    timerAlarmEnable(timer);
  else
    timerAlarmDisable(timer);
}

#endif

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIXELS_PIN, NEO_GRB + NEO_KHZ800);

void LED_Color(uint8_t r,uint8_t g,uint8_t b){
    strip.setPixelColor(0, strip.Color(r, g,b));
    strip.show();
}

void AFSK_hw_init(void)
{
  // Set up ADC
  //pinMode(RSSI_PIN, INPUT_PULLUP);
  pinMode(PTT_PIN, OUTPUT);
  //pinMode(4, OUTPUT);

  //pinMode(LED_PIN, OUTPUT);
  //digitalWrite(LED_PIN, LOW);

  digitalWrite(PTT_PIN, HIGH);

#ifdef I2S_INTERNAL
  //  Initialize the I2S peripheral
  I2S_Init(I2S_MODE_DAC_BUILT_IN, I2S_BITS_PER_SAMPLE_16BIT);
#else
  // pinMode(MIC_PIN, INPUT);
  //  Init ADC and Characteristics
  //  esp_adc_cal_characteristics_t characteristics;
  adc1_config_width(ADC_WIDTH_BIT_12);
  //adc1_config_channel_atten(SPK_PIN, ADC_ATTEN_DB_0); // Input 1.24Vp-p,Use R 33K-(10K//10K) divider input power 1.2Vref
  adc1_config_channel_atten(SPK_PIN, ADC_ATTEN_DB_11); //Input 3.3Vp-p,Use R 10K divider input power 3.3V
  // esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, &characteristics);
  // Serial.printf("v_ref routed to 3.3V\n");

  // Start a timer at 9.6kHz to sample the ADC and play the audio on the DAC.
  timer = timerBegin(0, 10, true);                // 80 MHz / 10 = 8MHz hardware clock
  timerAttachInterrupt(timer, &sample_isr, true); // Attaches the handler function to the timer
  timerAlarmWrite(timer, 834, true);              // Interrupts when counter == 834, 9592.3 times a second
  //timerAlarmWrite(timer, 417, true);              // Interrupts when counter == 834, 9592.3 times a second
  timerAlarmEnable(timer);

  pinMode(1, ANALOG);
  pinMode(17, OUTPUT);
  digitalWrite(17,1);
  pinMode(MIC_PIN, OUTPUT);
  ledcSetup(0, 64000, 8); // PWM 128KHz 8Bit
  ledcAttachPin(MIC_PIN, 0);
  //timerAlarmDisable(timer);
  //timerAlarmEnable(timer);

  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
#endif
}

void AFSK_init(Afsk *afsk)
{
  // Allocate modem struct memory
  memset(afsk, 0, sizeof(*afsk));
  AFSK_modem = afsk;
  // Set phase increment
  afsk->phaseInc = MARK_INC;
  // Initialise FIFO buffers
  fifo_init(&afsk->rxFifo, afsk->rxBuf, sizeof(afsk->rxBuf));
  fifo_init(&afsk->txFifo, afsk->txBuf, sizeof(afsk->txBuf));

  // filter initialize
  filter_param_t flt = {
      .size = FIR_LPF_N,
      .sampling_freq = SAMPLERATE,
      .pass_freq = 0,
      .cutoff_freq = 1200,
  };
  int16_t *lpf_an, *bpf_an,*hpf_an;
  // LPF
  lpf_an = filter_coeff(&flt);
  // BPF
  flt.size = FIR_BPF_N;
  flt.pass_freq = 1000;
  flt.cutoff_freq = 2500;
  bpf_an = filter_coeff(&flt);

  // LPF
  filter_init(&lpf, lpf_an, FIR_LPF_N);
  // BPF
  filter_init(&bpf, bpf_an, FIR_BPF_N);

  // HPF
  flt.size = FIR_BPF_N;
  flt.pass_freq = 1100;
  flt.cutoff_freq = 3500;
  hpf_an = filter_coeff(&flt);
  filter_init(&hpf, hpf_an, FIR_BPF_N);

  AFSK_hw_init();
}

static void AFSK_txStart(Afsk *afsk)
{
  if (!afsk->sending)
  {
    AFSK_TimerEnable(true);
    fifo_flush(&AFSK_modem->txFifo);
    // Serial.println("TX Start");
    afsk->sending = true;
    afsk->phaseInc = MARK_INC;
    afsk->phaseAcc = 0;
    afsk->bitstuffCount = 0;
    // LED_TX_ON();
    //digitalWrite(LED_TX_PIN, HIGH);
    digitalWrite(PTT_PIN, LOW);
    afsk->preambleLength = DIV_ROUND(custom_preamble * BITRATE, 4800);
    AFSK_DAC_IRQ_START();
    //LED_TX_ON();
#ifdef I2S_INTERNAL
    i2s_zero_dma_buffer(I2S_NUM_0);
    // i2s_adc_disable(I2S_NUM_0);
    dac_i2s_enable();
#endif
  }
  noInterrupts();
  afsk->tailLength = DIV_ROUND(custom_tail * BITRATE, 4800);
  interrupts();
}

void afsk_putchar(char c)
{
  AFSK_txStart(AFSK_modem);
  while (fifo_isfull_locked(&AFSK_modem->txFifo))
  {
    /* Wait */
    // delay(10);
  }
  fifo_push_locked(&AFSK_modem->txFifo, c);
}

int afsk_getchar(void)
{
  if (fifo_isempty_locked(&AFSK_modem->rxFifo))
  {
    return EOF;
  }
  else
  {
    return fifo_pop_locked(&AFSK_modem->rxFifo);
  }
}

void AFSK_transmit(char *buffer, size_t size)
{
  fifo_flush(&AFSK_modem->txFifo);
  int i = 0;
  while (size--)
  {
    afsk_putchar(buffer[i++]);
  }
}

uint8_t AFSK_dac_isr(Afsk *afsk)
{
  if (afsk->sampleIndex == 0)
  {
    // LED_RX_ON();
    // digitalWrite(LED_PIN), LOW);
    if (afsk->txBit == 0)
    {
      if (fifo_isempty(&afsk->txFifo) && afsk->tailLength == 0)
      {
        AFSK_DAC_IRQ_STOP();
        //LED_TX_OFF();
        afsk->sending = false;
        // LED_TX_OFF();
        // digitalWrite(PTT_PIN, LOW);
        return 0;
      }
      else
      {
        if (!afsk->bitStuff)
          afsk->bitstuffCount = 0;
        afsk->bitStuff = true;
        if (afsk->preambleLength == 0)
        {
          if (fifo_isempty(&afsk->txFifo))
          {
            afsk->tailLength--;
            afsk->currentOutputByte = HDLC_FLAG;
          }
          else
          {
            afsk->currentOutputByte = fifo_pop(&afsk->txFifo);
          }
        }
        else
        {
          afsk->preambleLength--;
          afsk->currentOutputByte = HDLC_FLAG;
        }
        if (afsk->currentOutputByte == AX25_ESC)
        {
          if (fifo_isempty(&afsk->txFifo))
          {
            AFSK_DAC_IRQ_STOP();
            //LED_TX_OFF();
            afsk->sending = false;
            // LED_TX_OFF();
            // digitalWrite(PTT_PIN, LOW);
            return 0;
          }
          else
          {
            afsk->currentOutputByte = fifo_pop(&afsk->txFifo);
          }
        }
        else if (afsk->currentOutputByte == HDLC_FLAG || afsk->currentOutputByte == HDLC_RESET)
        {
          afsk->bitStuff = false;
        }
      }
      afsk->txBit = 0x01;
    }

    if (afsk->bitStuff && afsk->bitstuffCount >= BIT_STUFF_LEN)
    {
      afsk->bitstuffCount = 0;
      afsk->phaseInc = SWITCH_TONE(afsk->phaseInc);
    }
    else
    {
      if (afsk->currentOutputByte & afsk->txBit)
      {
        afsk->bitstuffCount++;
      }
      else
      {
        afsk->bitstuffCount = 0;
        afsk->phaseInc = SWITCH_TONE(afsk->phaseInc);
      }
      afsk->txBit <<= 1;
    }

    afsk->sampleIndex = SAMPLESPERBIT;
  }

  afsk->phaseAcc += afsk->phaseInc;
  afsk->phaseAcc %= SIN_LEN;
  if (afsk->sampleIndex > 0)
    afsk->sampleIndex--;
  // LED_RX_OFF();

  return sinSample(afsk->phaseAcc);
}

int hdlc_flag_count = 0;
bool hdlc_flage_end = false;
static bool hdlcParse(Hdlc *hdlc, bool bit, FIFOBuffer *fifo)
{
  // Initialise a return value. We start with the
  // assumption that all is going to end well :)
  bool ret = true;

  // Bitshift our byte of demodulated bits to
  // the left by one bit, to make room for the
  // next incoming bit
  hdlc->demodulatedBits <<= 1;
  // And then put the newest bit from the
  // demodulator into the byte.
  hdlc->demodulatedBits |= bit ? 1 : 0;

  // Now we'll look at the last 8 received bits, and
  // check if we have received a HDLC flag (01111110)
  if (hdlc->demodulatedBits == HDLC_FLAG)
  {
    // If we have, check that our output buffer is
    // not full.
    if (!fifo_isfull(fifo))
    {
      // If it isn't, we'll push the HDLC_FLAG into
      // the buffer and indicate that we are now
      // receiving data. For bling we also turn
      // on the RX LED.

      hdlc->receiving = true;
      if (++hdlc_flag_count >= 3)
      {
        fifo_flush(fifo);
        LED_RX_ON();
      }
      fifo_push(fifo, HDLC_FLAG);
    }
    else
    {
      // If the buffer is full, we have a problem
      // and abort by setting the return value to
      // false and stopping the here.

      ret = false;
      hdlc->receiving = false;
      LED_RX_OFF();
      hdlc_flag_count = 0;
      hdlc_flage_end = false;
    }

    // Everytime we receive a HDLC_FLAG, we reset the
    // storage for our current incoming byte and bit
    // position in that byte. This effectively
    // synchronises our parsing to  the start and end
    // of the received bytes.
    hdlc->currentByte = 0;
    hdlc->bitIndex = 0;
    return ret;
  }

  // Check if we have received a RESET flag (01111111)
  // In this comparison we also detect when no transmission
  // (or silence) is taking place, and the demodulator
  // returns an endless stream of zeroes. Due to the NRZ
  // coding, the actual bits send to this function will
  // be an endless stream of ones, which this AND operation
  // will also detect.
  if ((hdlc->demodulatedBits & HDLC_RESET) == HDLC_RESET)
  {
    // If we have, something probably went wrong at the
    // transmitting end, and we abort the reception.
    hdlc->receiving = false;
    LED_RX_OFF();
    hdlc_flag_count = 0;
    hdlc_flage_end = false;
    return ret;
  }

  // If we have not yet seen a HDLC_FLAG indicating that
  // a transmission is actually taking place, don't bother
  // with anything.
  if (!hdlc->receiving)
    return ret;

  hdlc_flage_end = true;

  // First check if what we are seeing is a stuffed bit.
  // Since the different HDLC control characters like
  // HDLC_FLAG, HDLC_RESET and such could also occur in
  // a normal data stream, we employ a method known as
  // "bit stuffing". All control characters have more than
  // 5 ones in a row, so if the transmitting party detects
  // this sequence in the _data_ to be transmitted, it inserts
  // a zero to avoid the receiving party interpreting it as
  // a control character. Therefore, if we detect such a
  // "stuffed bit", we simply ignore it and wait for the
  // next bit to come in.
  //
  // We do the detection by applying an AND bit-mask to the
  // stream of demodulated bits. This mask is 00111111 (0x3f)
  // if the result of the operation is 00111110 (0x3e), we
  // have detected a stuffed bit.
  if ((hdlc->demodulatedBits & 0x3f) == 0x3e)
    return ret;

  // If we have an actual 1 bit, push this to the current byte
  // If it's a zero, we don't need to do anything, since the
  // bit is initialized to zero when we bitshifted earlier.
  if (hdlc->demodulatedBits & 0x01)
    hdlc->currentByte |= 0x80;

  // Increment the bitIndex and check if we have a complete byte
  if (++hdlc->bitIndex >= 8)
  {
    // If we have a HDLC control character, put a AX.25 escape
    // in the received data. We know we need to do this,
    // because at this point we must have already seen a HDLC
    // flag, meaning that this control character is the result
    // of a bitstuffed byte that is equal to said control
    // character, but is actually part of the data stream.
    // By inserting the escape character, we tell the protocol
    // layer that this is not an actual control character, but
    // data.
    if ((hdlc->currentByte == HDLC_FLAG ||
         hdlc->currentByte == HDLC_RESET ||
         hdlc->currentByte == AX25_ESC))
    {
      // We also need to check that our received data buffer
      // is not full before putting more data in
      if (!fifo_isfull(fifo))
      {
        fifo_push(fifo, AX25_ESC);
      }
      else
      {
        // If it is, abort and return false
        hdlc->receiving = false;
        LED_RX_OFF();
        hdlc_flag_count = 0;
        ret = false;
#ifdef DEBUG_TNC
        Serial.println("FIFO IS FULL");
#endif
      }
    }

    // Push the actual byte to the received data FIFO,
    // if it isn't full.
    if (!fifo_isfull(fifo))
    {
      fifo_push(fifo, hdlc->currentByte);
    }
    else
    {
      // If it is, well, you know by now!
      hdlc->receiving = false;
      LED_RX_OFF();
      hdlc_flag_count = 0;
      ret = false;
#ifdef DEBUG_TNC
      Serial.println("FIFO IS FULL");
#endif
    }

    // Wipe received byte and reset bit index to 0
    hdlc->currentByte = 0;
    hdlc->bitIndex = 0;
  }
  else
  {
    // We don't have a full byte yet, bitshift the byte
    // to make room for the next bit
    hdlc->currentByte >>= 1;
  }

  return ret;
}

#define DECODE_DELAY 4.458981479161393e-4 // sample delay
#define DELAY_DIVIDEND 325
#define DELAY_DIVISOR 728866
#define DELAYED_N ((DELAY_DIVIDEND * SAMPLERATE + DELAY_DIVISOR / 2) / DELAY_DIVISOR)

static int delayed[DELAYED_N];
static int delay_idx = 0;

void AFSK_adc_isr(Afsk *afsk, int16_t currentSample)
{
  /*
   * Frequency discrimination is achieved by simply multiplying
   * the sample with a delayed sample of (samples per bit) / 2.
   * Then the signal is lowpass filtered with a first order,
   * 600 Hz filter. The filter implementation is selectable
   */

  // deocde bell 202 AFSK from ADC value
  int m = (int)currentSample * delayed[delay_idx];
  // Put the current raw sample in the delay FIFO
  delayed[delay_idx] = (int)currentSample;
  delay_idx = (delay_idx + 1) % DELAYED_N;
  afsk->iirY[1] = filter(&lpf, m >> 7);

  // We put the sampled bit in a delay-line:
  // First we bitshift everything 1 left
  afsk->sampledBits <<= 1;
  // And then add the sampled bit to our delay line
  afsk->sampledBits |= (afsk->iirY[1] > 0) ? 1 : 0;

  // We need to check whether there is a signal transition.
  // If there is, we can recalibrate the phase of our
  // sampler to stay in sync with the transmitter. A bit of
  // explanation is required to understand how this works.
  // Since we have PHASE_MAX/PHASE_BITS = 8 samples per bit,
  // we employ a phase counter (currentPhase), that increments
  // by PHASE_BITS everytime a sample is captured. When this
  // counter reaches PHASE_MAX, it wraps around by modulus
  // PHASE_MAX. We then look at the last three samples we
  // captured and determine if the bit was a one or a zero.
  //
  // This gives us a "window" looking into the stream of
  // samples coming from the ADC. Sort of like this:
  //
  //   Past                                      Future
  //       0000000011111111000000001111111100000000
  //                   |________|
  //                       ||
  //                     Window
  //
  // Every time we detect a signal transition, we adjust
  // where this window is positioned little. How much we
  // adjust it is defined by PHASE_INC. If our current phase
  // phase counter value is less than half of PHASE_MAX (ie,
  // the window size) when a signal transition is detected,
  // add PHASE_INC to our phase counter, effectively moving
  // the window a little bit backward (to the left in the
  // illustration), inversely, if the phase counter is greater
  // than half of PHASE_MAX, we move it forward a little.
  // This way, our "window" is constantly seeking to position
  // it's center at the bit transitions. Thus, we synchronise
  // our timing to the transmitter, even if it's timing is
  // a little off compared to our own.
  if (SIGNAL_TRANSITIONED(afsk->sampledBits))
  {
    if (afsk->currentPhase < PHASE_THRESHOLD)
    {
      afsk->currentPhase += PHASE_INC;
    }
    else
    {
      afsk->currentPhase -= PHASE_INC;
    }
  }

  // We increment our phase counter
  afsk->currentPhase += PHASE_BITS;

  // Check if we have reached the end of
  // our sampling window.
  if (afsk->currentPhase >= PHASE_MAX)
  {
    // If we have, wrap around our phase
    // counter by modulus
    afsk->currentPhase %= PHASE_MAX;

    // Bitshift to make room for the next
    // bit in our stream of demodulated bits
    afsk->actualBits <<= 1;

    //// Alternative using 5 bits ////////////////
    uint8_t bits = afsk->sampledBits & 0x1f;
    uint8_t c = CountOnesFromInteger(bits);
    if (c >= 3)
      afsk->actualBits |= 1;
    /////////////////////////////////////////////////

    // Now we can pass the actual bit to the HDLC parser.
    // We are using NRZ coding, so if 2 consecutive bits
    // have the same value, we have a 1, otherwise a 0.
    // We use the TRANSITION_FOUND function to determine this.
    //
    // This is smart in combination with bit stuffing,
    // since it ensures a transmitter will never send more
    // than five consecutive 1's. When sending consecutive
    // ones, the signal stays at the same level, and if
    // this happens for longer periods of time, we would
    // not be able to synchronize our phase to the transmitter
    // and would start experiencing "bit slip".
    //
    // By combining bit-stuffing with NRZ coding, we ensure
    // that the signal will regularly make transitions
    // that we can use to synchronize our phase.
    //
    // We also check the return of the Link Control parser
    // to check if an error occured.

    if (!hdlcParse(&afsk->hdlc, !TRANSITION_FOUND(afsk->actualBits), &afsk->rxFifo))
    {
      afsk->status |= 1;
      if (fifo_isfull(&afsk->rxFifo))
      {
        fifo_flush(&afsk->rxFifo);
        afsk->status = 0;
#ifdef DEBUG_TNC
        Serial.println("FIFO IS FULL");
#endif
      }
    }
  }
}

#define ADC_SAMPLES_COUNT 192 //
int16_t abufPos = 0;
// extern TaskHandle_t taskSensorHandle;

extern void APRS_poll();
uint8_t poll_timer = 0;
// int adc_count = 0;
int offset_new = 0, offset = 2080, offset_count = 0;

#ifndef I2S_INTERNAL
// int x=0;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR  sample_isr()
{
  //portENTER_CRITICAL_ISR(&timerMux); // ISR start

  if (hw_afsk_dac_isr)
  {
    uint8_t sinwave = AFSK_dac_isr(AFSK_modem);
    //   abufPos += 45;
    //sinwave = (uint8_t)filter(&hpf, (int16_t)sinwave);
    // sinwave =(uint8_t)(127+(sin((float)abufPos/57)*128));
    // if(abufPos>=315) abufPos=0;
    // if(x++<16) Serial.printf("%d,",sinwave);
    // dacWrite(MIC_PIN, sinwave);
    ledcWrite(0, sinwave); // DAC PWM
    if (AFSK_modem->sending == false){
      digitalWrite(PTT_PIN, HIGH);
      //AFSK_TimerEnable(false);
    }
  }
  else
  {
    // digitalWrite(4, HIGH);
    adcVal = adc1_get_raw(SPK_PIN); // Read ADC1_0 From PIN 36(VP)
    //adcVal=analogRead(1);
    // Auto offset level
    offset_new += adcVal;
    offset_count++;
    if (offset_count >= 96) //192
    {
      //Serial.printf("%d,",adcVal);
      offset = offset_new / offset_count;
      offset_count = 0;
      offset_new = 0;
      if (offset > 3000 || offset < 1000) // Over dc offset to default
        offset = 2080;
    }
    // Convert unsign wave to sign wave form
    adcVal -= offset;
    // adcVal-=2030;
    //int8_t adcR = (int8_t)((int16_t)(adcVal >> 4)); // Reduce 12bit to 8bit
    // int8_t adcR = (int8_t)(adcVal / 16); // Reduce 12bit to 8bit
    adcq.push(&adcVal); // Add queue buffer
    // digitalWrite(4, LOW);
  }
  //portEXIT_CRITICAL_ISR(&timerMux); // ISR end
}
#else
bool tx_en = false;
#endif

extern int mVrms;
extern float dBV;
extern bool afskSync;

long mVsum = 0;
int mVsumCount = 0;
long lastVrms = 0;
bool VrmsFlag = false;
bool sqlActive = false;

void AFSK_Poll(bool SA818, bool RFPower)
{
  int mV;
  int x = 0;
  size_t writeByte;
  // uint8_t sintable[8] = {127, 217, 254, 217, 127, 36, 0, 36};
#ifdef I2S_INTERNAL
  size_t bytesRead;
  uint16_t pcm_in[ADC_SAMPLES_COUNT];
  uint16_t pcm_out[ADC_SAMPLES_COUNT];
#else
  int16_t adc;
#endif

  if (hw_afsk_dac_isr)
  {
#ifdef I2S_INTERNAL
    memset(pcm_out, 0, sizeof(pcm_out));
    for (x = 0; x < ADC_SAMPLES_COUNT; x++)
    {
      // LED_RX_ON();
      adcVal = (int)AFSK_dac_isr(AFSK_modem);
      // Serial.print(adcVal,HEX);
      // Serial.print(",");
      if (AFSK_modem->sending == false && adcVal == 0)
        break;

      // ไม่สามารถใช้งานในโหมด MONO ได้ จะต้องส่งข้อมูลตามลำดับซ้ายและขวา เอาต์พุต DAC บน I2S เป็นสเตอริโอเสมอ
      //  Ref: https://lang-ship.com/blog/work/esp32-i2s-dac/#toc6
      //  Left Channel GPIO 26
      pcm_out[x] = (uint16_t)adcVal; // MSB
                                     // if(SA818){
                                     //   pcm_out[x] <<= 7;
                                     //   pcm_out[x]+=10000;
                                     // }else{
      pcm_out[x] <<= 8;
      pcm_out[x] |= (uint16_t)adcVal; // MSB
      //}
      x++;
      // Right Channel GPIO 25
      pcm_out[x] = 0;
    }

    int availableBytes = x * sizeof(uint16_t);
    int buffer_position = 0;
    size_t bytesWritten = 0;
    if (x > 0)
    {
      // do
      // {

      //   // do we have something to write?
      //   if (availableBytes > 0)
      //   {
      //     // write data to the i2s peripheral
      //     i2s_write(I2S_NUM_0, buffer_position + (char *)&pcm_out,availableBytes, &bytesWritten, portMAX_DELAY);
      //     availableBytes -= bytesWritten;
      //     buffer_position += bytesWritten;
      //   }
      // } while (bytesWritten > 0);

      if (i2s_write(I2S_NUM_0, (char *)&pcm_out, (x * sizeof(uint16_t)),&writeByte, portMAX_DELAY) == ESP_OK)
      {
        Serial.println("I2S Write Error");
      }
    }

    // รอให้ I2S DAC ส่งให้หมดบัพเฟอร์ก่อนสั่งปิด DAC/PTT
    if (AFSK_modem->sending == false)
    {
      int txEvents = 0;
      memset(pcm_out, 0, sizeof(pcm_out));
      // Serial.println("TX TAIL");
      //  Clear Delay DMA Buffer
      for (int i = 0; i < 5; i++)
        i2s_write(I2S_NUM_0, (char *)&pcm_out, (ADC_SAMPLES_COUNT * sizeof(uint16_t)), &writeByte, portMAX_DELAY);
      // wait on I2S event queue until a TX_DONE is found
      while (xQueueReceive(i2s_event_queue, &i2s_evt, portMAX_DELAY) == pdPASS)
      {
        if (i2s_evt.type == I2S_EVENT_TX_DONE) // I2S DMA finish sent 1 buffer
        {
          if (++txEvents > 6)
          {
            // Serial.println("TX DONE");
            break;
          }
        }
      }
      dac_i2s_disable();
      i2s_zero_dma_buffer(I2S_NUM_0);
      // i2s_adc_enable(I2S_NUM_0);
      digitalWrite(PTT_PIN, LOW);
      if (SA818)
      {
        digitalWrite(12, LOW); // RF Power LOW
      }
    }
#endif
  }
  else
  {
#ifdef I2S_INTERNAL
    if (digitalRead(34) == LOW)
    {
      if (sqlActive == false)
      { // Falling Edge SQL
        log_d("RX Signal");
        // i2s_stop(I2S_NUM_0);
        // ac.DumpRegisters();
        //  ac.SetMode(AC101::MODE_LINE);
        //  memset(pcm_in,0,sizeof(pcm_in));
        //  i2s_zero_dma_buffer(I2S_NUM_0);
        //  i2s_start(I2S_NUM_0);
        sqlActive = true;
      }
      if (i2s_read(I2S_NUM_0, (char *)&pcm_in, (ADC_SAMPLES_COUNT * sizeof(uint16_t)), &bytesRead, portMAX_DELAY) == ESP_OK)
      {

        for (int i = 0; i < (bytesRead / sizeof(uint16_t)); i += 2)
        {
          adcVal = (int)pcm_in[i];
          offset_new += adcVal;
          offset_count++;
          if (offset_count >= ADC_SAMPLES_COUNT) // 192
          {
            offset = offset_new / offset_count;
            offset_count = 0;
            offset_new = 0;
            if (offset > 3300 || offset < 1300) // Over dc offset to default
              offset = 2303;
          }
          adcVal -= offset; // Convert unsinewave to sinewave

          if (input_HPF)
          {
            adcVal = (int)filter(&bpf, (int16_t)adcVal);
          }

          if (afskSync == false)
          {
            mV = abs((int)adcVal);         // mVp-p
            mV = (int)((float)mV / 3.68F); // mV=(mV*625)/36848;
            mVsum += powl(mV, 2);
            mVsumCount++;
          }
          // int8_t adcR = (int8_t)((int16_t)(adcVal >> 4)); // Reduce 12bit to 8bit
          // int8_t adcR=(int8_t)(adcVal/16);

          AFSK_adc_isr(AFSK_modem, (int16_t)adcVal); // Process signal IIR
          if (i % 32 == 0)
            APRS_poll(); // Poll check every 1 bit
        }
        // Get mVrms on Sync flage 0x7E
        if (afskSync == false)
        {
          if (hdlc_flag_count > 3 && hdlc_flage_end == true)
          {
            if (mVsumCount > 960)
            {
              mVrms = sqrtl(mVsum / mVsumCount);
              mVsum = 0;
              mVsumCount = 0;
              lastVrms = millis() + 500;
              VrmsFlag = true;
              // if(millis()>lastVrms){
              //  double Vrms=(double)mVrms/1000;
              //  dBV = 20.0F * log10(Vrms);
              //  //dBu = 20 * log10(Vrms / 0.7746);
              //  Serial.printf("mVrms=%d dBV=%0.1f agc=%0.2f\n",mVrms,dBV);
              // afskSync = true;
              //}
            }
          }
          if (millis() > lastVrms && VrmsFlag)
          {
            afskSync = true;
            VrmsFlag = false;
          }
        }
      }
    }
    else
    {
      // if(sqlActive==true){
      //   i2s_stop(I2S_NUM_0);
      // }
      LED_RX_OFF();
      sqlActive = false;
    }
#else
    if (adcq.getCount() >= ADC_SAMPLES_COUNT)
    {
      for (x = 0; x < ADC_SAMPLES_COUNT; x++)
      {
        if (!adcq.pop(&adc)) // Pull queue buffer
          break;
        // audiof[x] = (float)adc;
        //  Serial.printf("%02x ", (unsigned char)adc);
        //int adcFir = (int)filter(&bpf, (int16_t)adc);
        AFSK_adc_isr(AFSK_modem, adc); // Process signal IIR
        if (x % 2 == 0)
          APRS_poll(); // Poll check every 1 byte
      }
      APRS_poll();
    }
#endif
  }
}
