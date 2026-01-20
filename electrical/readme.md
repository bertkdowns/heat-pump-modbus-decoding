# HIOKI PW3365 DATA LOGGER CONNECTION

This has been reverse engineered a little from their live data logger software

Basically, it opens a TCP port 3365 and the communication looks like this:

```
:HEADER OFF
ALL RIGHT
:STATE?
RUN
:INTERVAL?
5S
:STATE?
RUN
:WIRING?
1P3WI
:MEASURE:ITEM:POWER  15,207,179,31,31,63
ALL RIGHT
:HEADER ON
ALL RIGHT
:MEASURE:POWER?
Date 2026,01,20;Time 12,20,05;Status 00000000;U1_Ins 13.6E+00;Ufnd1_Ins 13.5E+00;Udeg1_Ins 0.0E+00;Upeak1_Ins 22.3E+00;U2_Ins 0.0E+00;Ufnd2_Ins 0.0E+00;Udeg2_Ins 0.0E+00;Upeak2_Ins 0.0E+00;I1_Ins 0.00E+00;Ifnd1_Ins 0.00E+00;Ideg1_Ins 0.0E+00;Ipeak1_Ins 0.00E+00;I2_Ins 0.00E+00;Ifnd2_Ins 0.00E+00;Ideg2_Ins 0.0E+00;Ipeak2_Ins 0.00E+00;I3_Ins 0.00E+00;Ifnd3_Ins 0.00E+00;Ideg3_Ins 0.0E+00;Ipeak3_Ins 0.00E+00;P1_Ins 0.000E+03;P2_Ins 0.000E+03;P_Ins 0.000E+03;S1_Ins 0.000E+03;S2_Ins 0.000E+03;S_Ins 0.000E+03;Q1_Ins 0.000E+03;Q2_Ins 0.000E+03;Q_Ins 0.000E+03;PF1_Ins 0.000E+99;PF2_Ins 0.000E+99;PF_Ins 0.000E+99;Freq_Ins 49.97E+00;U1_Avg 0.0E+00;Ufnd1_Avg 0.0E+00;Udeg1_Avg 0.0E+00;U2_Avg 0.0E+00;Ufnd2_Avg 0.0E+00;Udeg2_Avg 0.0E+00;I1_Avg 0.00E+00;Ifnd1_Avg 0.00E+00;Ideg1_Avg 0.0E+00;I2_Avg 0.00E+00;Ifnd2_Avg 0.00E+00;Ideg2_Avg 0.0E+00;I3_Avg 0.00E+00;Ifnd3_Avg 0.00E+00;Ideg3_Avg 0.0E+00;P1_Avg 0.000E+03;P2_Avg 0.000E+03;P_Avg 0.000E+03;S1_Avg 0.000E+03;S2_Avg 0.000E+03;S_Avg 0.000E+03;Q1_Avg 0.000E+03;Q2_Avg 0.000E+03;Q_Avg 0.000E+03;PF1_Avg 0.000E+00;PF2_Avg 0.000E+00;PF_Avg 0.000E+00;Freq_Avg 0.00E+00;U1_Max 0.0E+00;Ufnd1_Max 0.0E+00;Udeg1_Max 0.0E+00;Upeak1_Max 0.0E+00;U2_Max 0.0E+00;Ufnd2_Max 0.0E+00;Udeg2_Max 0.0E+00;Upeak2_Max 0.0E+00;I1_Max 0.00E+00;Ifnd1_Max 0.00E+00;Ideg1_Max 0.0E+00;Ipeak1_Max 0.00E+00;I2_Max 0.00E+00;Ifnd2_Max 0.00E+00;Ideg2_Max 0.0E+00;Ipeak2_Max 0.00E+00;I3_Max 0.00E+00;Ifnd3_Max 0.00E+00;Ideg3_Max 0.0E+00;Ipeak3_Max 0.00E+00;P1_Max 0.000E+03;P2_Max 0.000E+03;P_Max 0.000E+03;S1_Max 0.000E+03;S2_Max 0.000E+03;S_Max 0.000E+03;Q1_Max 0.000E+03;Q2_Max 0.000E+03;Q_Max 0.000E+03;PF1_Max 0.000E+00;PF2_Max 0.000E+00;PF_Max 0.000E+00;Freq_Max 0.00E+00;U1_Min 0.0E+00;Ufnd1_Min 0.0E+00;Udeg1_Min 0.0E+00;Upeak1_Min 0.0E+00;U2_Min 0.0E+00;Ufnd2_Min 0.0E+00;Udeg2_Min 0.0E+00;Upeak2_Min 0.0E+00;I1_Min 0.00E+00;Ifnd1_Min 0.00E+00;Ideg1_Min 0.0E+00;Ipeak1_Min 0.00E+00;I2_Min 0.00E+00;Ifnd2_Min 0.00E+00;Ideg2_Min 0.0E+00;Ipeak2_Min 0.00E+00;I3_Min 0.00E+00;Ifnd3_Min 0.00E+00;Ideg3_Min 0.0E+00;Ipeak3_Min 0.00E+00;P1_Min 0.000E+03;P2_Min 0.000E+03;P_Min 0.000E+03;S1_Min 0.000E+03;S2_Min 0.000E+03;S_Min 0.000E+03;Q1_Min 0.000E+03;Q2_Min 0.000E+03;Q_Min 0.000E+03;PF1_Min 0.000E+00;PF2_Min 0.000E+00;PF_Min 0.000E+00;Freq_Min 0.00E+00;WP+ 0.0000E+03;WP- 0.0000E+03;WQLAG 0.0000E+03;WQLEAD 0.0000E+03;Ecost 0.00000E+00;WP+dem 0.0000E+03;WP-dem 0.0000E+03;WQLAGdem 0.0000E+03;WQLEADdem 0.0000E+03;Pdem+ 0.000E+03;Pdem- 0.000E+03;QdemLAG 0.000E+03;QdemLEAD 0.000E+03;PFdem 0.000E+00;Pdem_max 0.000E+03
:HEADER OFF
ALL RIGHT
:HEADER OFF
ALL RIGHT
:STATE?
RUN
:INTERVAL?
5S
:STATE?
RUN
:WIRING?
1P3WI
:MEASURE:ITEM:POWER  15,207,179,31,31,63
ALL RIGHT
:HEADER ON
ALL RIGHT
:MEASURE:POWER?
Date 2026,01,20;Time 12,20,06;Status 00000000;U1_Ins 13.5E+00;Ufnd1_Ins 13.5E+00;Udeg1_Ins 0.0E+00;Upeak1_Ins 21.6E+00;U2_Ins 0.0E+00;Ufnd2_Ins 0.0E+00;Udeg2_Ins 0.0E+00;Upeak2_Ins 0.0E+00;I1_Ins 0.00E+00;Ifnd1_Ins 0.00E+00;Ideg1_Ins 0.0E+00;Ipeak1_Ins 0.00E+00;I2_Ins 0.00E+00;Ifnd2_Ins 0.00E+00;Ideg2_Ins 0.0E+00;Ipeak2_Ins 0.00E+00;I3_Ins 0.00E+00;Ifnd3_Ins 0.00E+00;Ideg3_Ins 0.0E+00;Ipeak3_Ins 0.00E+00;P1_Ins 0.000E+03;P2_Ins 0.000E+03;P_Ins 0.000E+03;S1_Ins 0.000E+03;S2_Ins 0.000E+03;S_Ins 0.000E+03;Q1_Ins -0.000E+03;Q2_Ins 0.000E+03;Q_Ins 0.000E+03;PF1_Ins 0.000E+99;PF2_Ins 0.000E+99;PF_Ins 0.000E+99;Freq_Ins 49.96E+00;U1_Avg 13.6E+00;Ufnd1_Avg 13.5E+00;Udeg1_Avg 0.0E+00;U2_Avg 0.0E+00;Ufnd2_Avg 0.0E+00;Udeg2_Avg -2.5E+00;I1_Avg 0.00E+00;Ifnd1_Avg 0.00E+00;Ideg1_Avg -2.5E+00;I2_Avg 0.00E+00;Ifnd2_Avg 0.00E+00;Ideg2_Avg -2.5E+00;I3_Avg 0.00E+00;Ifnd3_Avg 0.00E+00;Ideg3_Avg -2.5E+00;P1_Avg 0.000E+03;P2_Avg 0.000E+03;P_Avg 0.000E+03;S1_Avg 0.000E+03;S2_Avg 0.000E+03;S_Avg 0.000E+03;Q1_Avg 0.000E+03;Q2_Avg 0.000E+03;Q_Avg 0.000E+03;PF1_Avg 0.000E+99;PF2_Avg 0.000E+99;PF_Avg 0.000E+99;Freq_Avg 50.00E+00;U1_Max 13.6E+00;Ufnd1_Max 13.6E+00;Udeg1_Max 0.0E+00;Upeak1_Max 23.0E+00;U2_Max 0.0E+00;Ufnd2_Max 0.0E+00;Udeg2_Max 0.0E+00;Upeak2_Max 0.0E+00;I1_Max 0.00E+00;Ifnd1_Max 0.00E+00;Ideg1_Max 0.0E+00;Ipeak1_Max 0.00E+00;I2_Max 0.00E+00;Ifnd2_Max 0.00E+00;Ideg2_Max 0.0E+00;Ipeak2_Max 0.00E+00;I3_Max 0.00E+00;Ifnd3_Max 0.00E+00;Ideg3_Max 0.0E+00;Ipeak3_Max 0.00E+00;P1_Max 0.000E+03;P2_Max 0.000E+03;P_Max 0.000E+03;S1_Max 0.000E+03;S2_Max 0.000E+03;S_Max 0.000E+03;Q1_Max -0.000E+03;Q2_Max 0.000E+03;Q_Max 0.000E+03;PF1_Max 0.000E+99;PF2_Max 0.000E+99;PF_Max 0.000E+99;Freq_Max 50.03E+00;U1_Min 13.5E+00;Ufnd1_Min 13.5E+00;Udeg1_Min 0.0E+00;Upeak1_Min 21.3E+00;U2_Min 0.0E+00;Ufnd2_Min 0.0E+00;Udeg2_Min 0.0E+00;Upeak2_Min 0.0E+00;I1_Min 0.00E+00;Ifnd1_Min 0.00E+00;Ideg1_Min 0.0E+00;Ipeak1_Min 0.00E+00;I2_Min 0.00E+00;Ifnd2_Min 0.00E+00;Ideg2_Min 0.0E+00;Ipeak2_Min 0.00E+00;I3_Min 0.00E+00;Ifnd3_Min 0.00E+00;Ideg3_Min 0.0E+00;Ipeak3_Min 0.00E+00;P1_Min 0.000E+03;P2_Min 0.000E+03;P_Min 0.000E+03;S1_Min 0.000E+03;S2_Min 0.000E+03;S_Min 0.000E+03;Q1_Min -0.000E+03;Q2_Min 0.000E+03;Q_Min 0.000E+03;PF1_Min 0.000E+99;PF2_Min 0.000E+99;PF_Min 0.000E+99;Freq_Min 49.97E+00;WP+ 0.0000E+03;WP- 0.0000E+03;WQLAG 0.0000E+03;WQLEAD 0.0000E+03;Ecost 0.00000E+00;WP+dem 0.0000E+03;WP-dem 0.0000E+03;WQLAGdem 0.0000E+03;WQLEADdem 0.0000E+03;Pdem+ 0.000E+03;Pdem- 0.000E+03;QdemLAG 0.000E+03;QdemLEAD 0.000E+03;PFdem 0.000E+99;Pdem_max 0.000E+03
:HEADER OFF
ALL RIGHT
:HEADER OFF
ALL RIGHT
:STATE?
RUN
:INTERVAL?
5S
:STATE?
RUN
:WIRING?
1P3WI
:MEASURE:ITEM:POWER  15,207,179,31,31,63
ALL RIGHT
:HEADER ON
ALL RIGHT
:MEASURE:POWER?
Date 2026,01,20;Time 12,20,07;Status 00000000;U1_Ins 13.6E+00;Ufnd1_Ins 13.5E+00;Udeg1_Ins 0.0E+00;Upeak1_Ins 21.8E+00;U2_Ins 0.0E+00;Ufnd2_Ins 0.0E+00;Udeg2_Ins 0.0E+00;Upeak2_Ins 0.0E+00;I1_Ins 0.00E+00;Ifnd1_Ins 0.00E+00;Ideg1_Ins 0.0E+00;Ipeak1_Ins 0.00E+00;I2_Ins 0.00E+00;Ifnd2_Ins 0.00E+00;Ideg2_Ins 0.0E+00;Ipeak2_Ins 0.00E+00;I3_Ins 0.00E+00;Ifnd3_Ins 0.00E+00;Ideg3_Ins 0.0E+00;Ipeak3_Ins 0.00E+00;P1_Ins 0.000E+03;P2_Ins 0.000E+03;P_Ins 0.000E+03;S1_Ins 0.000E+03;S2_Ins 0.000E+03;S_Ins 0.000E+03;Q1_Ins 0.000E+03;Q2_Ins -0.000E+03;Q_Ins 0.000E+03;PF1_Ins 0.000E+99;PF2_Ins 0.000E+99;PF_Ins 0.000E+99;Freq_Ins 49.95E+00;U1_Avg 13.6E+00;Ufnd1_Avg 13.5E+00;Udeg1_Avg 0.0E+00;U2_Avg 0.0E+00;Ufnd2_Avg 0.0E+00;Udeg2_Avg -2.5E+00;I1_Avg 0.00E+00;Ifnd1_Avg 0.00E+00;Ideg1_Avg -2.5E+00;I2_Avg 0.00E+00;Ifnd2_Avg 0.00E+00;Ideg2_Avg -2.5E+00;I3_Avg 0.00E+00;Ifnd3_Avg 0.00E+00;Ideg3_Avg -2.5E+00;P1_Avg 0.000E+03;P2_Avg 0.000E+03;P_Avg 0.000E+03;S1_Avg 0.000E+03;S2_Avg 0.000E+03;S_Avg 0.000E+03;Q1_Avg 0.000E+03;Q2_Avg 0.000E+03;Q_Avg 0.000E+03;PF1_Avg 0.000E+99;PF2_Avg 0.000E+99;PF_Avg 0.000E+99;Freq_Avg 50.00E+00;U1_Max 13.6E+00;Ufnd1_Max 13.6E+00;Udeg1_Max 0.0E+00;Upeak1_Max 23.0E+00;U2_Max 0.0E+00;Ufnd2_Max 0.0E+00;Udeg2_Max 0.0E+00;Upeak2_Max 0.0E+00;I1_Max 0.00E+00;Ifnd1_Max 0.00E+00;Ideg1_Max 0.0E+00;Ipeak1_Max 0.00E+00;I2_Max 0.00E+00;Ifnd2_Max 0.00E+00;Ideg2_Max 0.0E+00;Ipeak2_Max 0.00E+00;I3_Max 0.00E+00;Ifnd3_Max 0.00E+00;Ideg3_Max 0.0E+00;Ipeak3_Max 0.00E+00;P1_Max 0.000E+03;P2_Max 0.000E+03;P_Max 0.000E+03;S1_Max 0.000E+03;S2_Max 0.000E+03;S_Max 0.000E+03;Q1_Max -0.000E+03;Q2_Max 0.000E+03;Q_Max 0.000E+03;PF1_Max 0.000E+99;PF2_Max 0.000E+99;PF_Max 0.000E+99;Freq_Max 50.03E+00;U1_Min 13.5E+00;Ufnd1_Min 13.5E+00;Udeg1_Min 0.0E+00;Upeak1_Min 21.3E+00;U2_Min 0.0E+00;Ufnd2_Min 0.0E+00;Udeg2_Min 0.0E+00;Upeak2_Min 0.0E+00;I1_Min 0.00E+00;Ifnd1_Min 0.00E+00;Ideg1_Min 0.0E+00;Ipeak1_Min 0.00E+00;I2_Min 0.00E+00;Ifnd2_Min 0.00E+00;Ideg2_Min 0.0E+00;Ipeak2_Min 0.00E+00;I3_Min 0.00E+00;Ifnd3_Min 0.00E+00;Ideg3_Min 0.0E+00;Ipeak3_Min 0.00E+00;P1_Min 0.000E+03;P2_Min 0.000E+03;P_Min 0.000E+03;S1_Min 0.000E+03;S2_Min 0.000E+03;S_Min 0.000E+03;Q1_Min -0.000E+03;Q2_Min 0.000E+03;Q_Min 0.000E+03;PF1_Min 0.000E+99;PF2_Min 0.000E+99;PF_Min 0.000E+99;Freq_Min 49.97E+00;WP+ 0.0000E+03;WP- 0.0000E+03;WQLAG 0.0000E+03;WQLEAD 0.0000E+03;Ecost 0.00000E+00;WP+dem 0.0000E+03;WP-dem 0.0000E+03;WQLAGdem 0.0000E+03;WQLEADdem 0.0000E+03;Pdem+ 0.000E+03;Pdem- 0.000E+03;QdemLAG 0.000E+03;QdemLEAD 0.000E+03;PFdem 0.000E+99;Pdem_max 0.000E+03
:HEADER OFF
ALL RIGHT


```

You can find the ip address on your network by doing 

```
nmap -sn 192.168.1.0/24
```

(assuming it's on 192.168.1.something, or also just look on the device settings (keep pressing the settings button til it shows))

You can verify by going to that IP address in the browser, they host an HTTP virtual control panel where you can set it up.


to test communication manually you can do `nc -C IP_ADDR 3365` and type in one of those commands. `-C` switches to CRLF line endings.


Otherwise, this script connects to a mqtt server and periodically sends the data over mqtt. 


```
uv run electrical_mqtt.py
```