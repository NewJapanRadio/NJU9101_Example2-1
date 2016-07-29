# NJU9101 Example 2-1 for mbed

[![Build Status](https://travis-ci.org/NewJapanRadio/NJU9101_Example2-1_mbed.svg?branch=master)](https://travis-ci.org/NewJapanRadio/NJU9101_Example2-1_mbed)

## Flowchart

![](http://g.gravizo.com/g?
digraph G {
graph [ranksep=0.4];
node [shape=ellipse] START;
node [shape=diamond] WAIT_BOOT WAIT_CLKRUN WAIT_MEAS;
node [shape=box];
INIT [label="mbed setup\\nInitialize Serial Port"];
RESET [label="Chip Reset\\nCTRL.RST = 1"];
WAIT_BOOT [label="Wait for Boot\\nSTATUS.BOOT == 1"];
WAKEUP_OSC [label="Wakeup OSC\\nBLKCTRL.OSC = 1"];
WAIT_CLKRUN [label="Wait for OSC\\nSTATUS.CLKRUN == 1"];
WAIT_MEAS [label="Wait for End of Conversion\\nCTRL.MEAS == 0"];
MEAS_LOOP_HEAD [label="Start of Measurement" shape=house]
START_MEAS [label="Start Single Conversion"]
MEAS_LOOP_TAIL [label="End of Measurement" shape=invhouse]
READ_ADC_DATA [label="Read AMPDATA"]
RESULT [label="Display AMPDATA" shape=parallelogram]
CORRECTION_EFFICIENT [label="Correction Coefficent Setting"]
CONFIGURATION [label="Register Configuration"]
COEFF0 [label="SCAL1B = 0x2AAB"];
COEFF1 [label="SCAL2B = 0x2AAB"];
COEFF2 [label="SCAL3B = 0x2AAB"];
COEFF3 [label="SCAL4B = 0x2AAB"];
CONF0 [label="BLKCONN0.BIASSWA = 1\\nBLKCONN0.BIASSWB = 1"];
CONF1 [label="BLKCONN1.OPA_BIAS = any\\nBLKCONN1.OPB_BIAS = any"];
CONF2 [label="BLKCONN2.BIASSWN = 1\\nBLKCONN2.PAMPSEL = 1"];
CONF3 [label="BLKCTRL.BIAS_RES = ON\\nBLKCTRL.OPA = ON\\nBLKCTRL.OPB = ON\\nBLKCTRL.OSC = OFF"];
CONCAT_AMPDATA [label="AMPDATA0 << 8\\n+AMPDATA1"]
START->INIT->RESET;
RESET->WAIT_BOOT [headport=n];
WAIT_BOOT->WAIT_BOOT [label="no" tailport=e headport=n];
WAIT_BOOT->WAKEUP_OSC [label="yes" tailport=s headport=n];
WAKEUP_OSC->WAIT_CLKRUN;
WAIT_CLKRUN->WAIT_CLKRUN [label="no" tailport=e headport=n];
WAIT_CLKRUN->CORRECTION_EFFICIENT [label="yes" tailport=s headport=n]
CORRECTION_EFFICIENT->CONFIGURATION->MEAS_LOOP_HEAD;
MEAS_LOOP_HEAD->START_MEAS [weight=5];
START_MEAS->WAIT_MEAS [weight=5];
WAIT_MEAS->WAIT_MEAS [label="no" tailport=e headport=n label="no"];
WAIT_MEAS->READ_ADC_DATA [label="yes" tailport=s headport=n weight=5];
READ_ADC_DATA->RESULT [weight=5];
RESULT->MEAS_LOOP_TAIL [weight=5];
MEAS_LOOP_TAIL->MEAS_LOOP_HEAD [tailport=s headport=n];
CORRECTION_EFFICIENT->COEFF0 [arrowhead=none style=dashed tailports=e headports=w];
COEFF0->COEFF1->COEFF2->COEFF3 [weight=3];
{rank=same;CORRECTION_EFFICIENT;COEFF0}
CONFIGURATION->CONF0 [arrowhead=none style=dashed tailports=e headports=w];
CONF0->CONF1->CONF2->CONF3 [weight=3];
{rank=same;CONFIGURATION;CONF0}
READ_ADC_DATA->CONCAT_AMPDATA [arrowhead=none style=dashed tailports=e headports=w];
{rank=same;READ_ADC_DATA;CONCAT_AMPDATA}
}
)
