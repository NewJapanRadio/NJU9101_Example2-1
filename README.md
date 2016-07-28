# NJU9101 Example 2-1 for mbed

[![Build Status](https://travis-ci.org/NewJapanRadio/NJU9101_Example2-1_mbed.svg?branch=master)](https://travis-ci.org/NewJapanRadio/NJU9101_Example2-1_mbed)

## Flowchart

![](http://g.gravizo.com/g?
    digraph G {
        graph [ranksep=0.4];
        node [shape=ellipse] START;
        node [shape=diamond] WAIT_BOOT WAIT_CLKRUN WAIT_MEAS;
        node [shape=box];
        INIT [label="Initialize Serial Port"];
        RESET [label="Chip Reset" shape="box"];
        WAIT_BOOT [label="STATUS.BOOT == 1"];
        WAKEUP_OSC [label="BLKCTRL.OSC = 1"];
        WAIT_CLKRUN [label="STATUS.CLKRUN == 1"];
        WAIT_MEAS [label="CTRL.MEAS == 0"];
        COEFF0 [label="SCAL1B = 0x2AAB"];
        COEFF1 [label="SCAL2B = 0x2AAB"];
        COEFF2 [label="SCAL3B = 0x2AAB"];
        COEFF3 [label="SCAL4B = 0x2AAB"];
        CONF0 [label="BIASSWA = 1, BIASSWB = 1"];
        CONF1 [label="OPA_BIAS = any, OPB_BIAS = any"];
        CONF2 [label="BIASSWN = 1, PAMPSEL = 1"];
        CONF3 [label="BIAS_RES = ON, OPA = ON, OPB = ON, OSC = OFF"];
        START->INIT->RESET;
        RESET->WAIT_BOOT [headport=n];
        WAIT_BOOT->WAIT_BOOT [label="no" tailport=e headport=n];
        WAIT_BOOT->WAKEUP_OSC [label="yes" tailport=s headport=n];
        WAKEUP_OSC->WAIT_CLKRUN;
        WAIT_CLKRUN->WAIT_CLKRUN [label="no" tailport=e headport=n];
        WAIT_CLKRUN->CORRECTION_EFFICIENT [label="yes" tailport=s headport=n]
        CORRECTION_EFFICIENT->CONFIGURATION;
        CONFIGURATION->MEAS_LOOP_HEAD;
        MEAS_LOOP_HEAD->START_MEAS [weight=5];
        START_MEAS->WAIT_MEAS [weight=5];
        WAIT_MEAS->WAIT_MEAS [label="no" tailport=e headport=n label="no"];
        WAIT_MEAS->MEAS_LOOP_TAIL [label="yes" tailport=s headport=n weight=5];
        MEAS_LOOP_TAIL->MEAS_LOOP_HEAD [tailport=s headport=n];
        COEFF0->COEFF1->COEFF2->COEFF3 [weight=3];
        {rank=same;COEFF0;CORRECTION_EFFICIENT}
        CORRECTION_EFFICIENT->COEFF0 [arrowhead=none style=dashed tailports=e headports=w];
        CONF0->CONF1->CONF2->CONF3 [weight=3];
        {rank=same;CONF0;CONFIGURATION}
        CONFIGURATION->CONF0 [arrowhead=none style=dashed tailports=e headports=w];
    }
)
