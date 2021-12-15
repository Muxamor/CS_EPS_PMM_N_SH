#ifndef MAX17320_H
#define MAX17320_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX17320_ON_FET       	0x00  		// ON charge/discharge keys MAX17320.
#define MAX17320_OFF_FET        0x01   		// OFF charge/discharge keys MAX17320.
#define MAX17320_2S_batt        0x02   		// 2s1p configuration battery in branch.
#define MAX17320_3S_batt        0x03   		// 3s1p configuration battery in branch.
#define MAX17320_4S_batt        0x04   		// 4s1p configuration battery in branch.
#define MAX17320_0_termistors	0			// 0 termistors in branch.
#define MAX17320_1_termistors	1			// 1 termistors in branch.
#define MAX17320_2_termistors	2			// 2 termistors in branch.
#define MAX17320_3_termistors	3			// 3 termistors in branch.
#define MAX17320_4_termistors	4			// 4 termistors in branch.
#define MAX17320_LSB_mV        	0.078125   	// LSB for voltage 0.078125mV per bit
#define MAX17320_LSB_degree		0.00390625	// LSB for temperature and %.
#define MAX17320_LSB_mA			1.5625		// LSB for current 0.0015625mV/RSENSE per bit
#define MAX17320_LSB_mAh		5			// LSB for capacity 0.005mAh/RSENSE per bit
#define MAX17320_LSB_mOhm		0.244140625	// LSB for resistance 1/4096 mOhm per bit
#define MAX17320_LSB_mVrip		0.009765625	// LSB for  voltage Vripple 1.25mV/128 per bit


typedef struct // The register data
{
	uint8_t PermFail :1; 	// Permanent Failure. This bit is set if any permanent failure is detected.
	uint8_t OVPF :1; 		// Severe Overvoltage Failure: If any cell voltage exceeds nOVPrtTh.OVPPermFail.
	uint8_t OTPF :1; 		// Severe Overtemperature Failure. If Temp exceeds nTPrtTh3.TpermFailHot.
	uint8_t CFETFs :1; 		// ChargeFET failure-short detected. If the IC detects that the charge FET is shorted and cannot be opened, it sets the CFETFs bit and the PermFail bit.
	uint8_t DFETFs :1; 		// DischargeFET failure-short detected. If the IC detects that the discharge FET is shorted and cannot be opened, it sets the DFETFs and the PermFail bit.
	uint8_t FETFo :1; 		// FET Failure Open. If the IC detects an open FET failure it sets FETFo. In this case, if the IC detects either CHG or DIS FET to have failed open, then it sets FETFo.
	uint8_t LDet :1; 		// Leakage Detection Fault. This bit is set when a leakage detection fault has been detected.
	uint8_t ChksumF :1; 	// Checksum Failure. ChksumF protection related NVM configuration registers checksum failure.

	uint8_t ODCP :1; 		// Overdischarge current faults.
	uint8_t UVP :1; 		// Undervoltage faults.
	uint8_t Imbalance :1; 	// Multicell imbalance faults.
	uint8_t DieHot :1; 		// Overtemperature for die temperature faults.
	uint8_t OCCP :1; 		// Overcharge current faults.
	uint8_t OVP :1; 		// Overvoltage faults.
	uint8_t TooColdC :1; 	// Undertemperature for charging faults.
	uint8_t TooHotC :1; 	// Overtemperature for discharging faults.

	uint8_t CHGs :1; 		// CHG FET Status, 1 = On, 0 = Off.
	uint8_t DISs :1; 		// DIS FET Status, 1 = On, 0 = Off.
	uint8_t PBEN :1; 		// Pushbutton Enable. 1 = Pushbutton wakeup on ALRT pin is enabled. 0 = Disabled.
	uint8_t CPCfg :2; 		// Charge Pump Gate Drive Voltage Configuration. 00b = 6V, 01b = 8V, 10b = 10V.
	uint8_t CommOvrd :1; 	// Command Override Enable. 1 = FET override function enabled. Allows communication to turn off CHG, DIS	FETs by writing the CommStat register.
	uint8_t AOLDO :2; 		// Always-on LDO Configuration 00b = off, 01b = 3.4V, 10b = 1.8V, 11b = 3.4V.

	uint8_t Fib :3; 		// Fib is a reset counter which controls Fibonacci-saving reset accelleration.
	uint8_t TAlrtEn :1; 	// Temperature Alert Enable.
	uint8_t dSOCen :1; 		// SOC Change Alert Enable.
	uint8_t Aen :1; 		// Enable Alert on Fuel-Gauge Outputs.
	uint8_t FTHRM :1; 		// Force Thermistor Bias Switch.
	uint8_t ALSH :1; 		// Note that if Aen bit is set to 1, the ALSH bit should be set to 0 to prevent an alert condition from causing the device to enter shutdown mode.

	uint8_t COMMSH :1; 		// Communication Shutdown.
	uint8_t AtRateEn :1; 	// ???
	uint8_t DisBlockRead :1;// Disable SBS Block Read.
	uint8_t PBen :1; 		// PushButton Enable.
	uint8_t FIFOen :1; 		// ADC FIFO Enable.
	uint8_t VS :1; 			// Voltage ALRT Sticky.
	uint8_t TS :1; 			// Temperature ALRT Sticky.
	uint8_t SS :1; 			// SOC ALRT Sticky.

	uint8_t PAen :1; 		// Protection Alert Enable. Set PAen = 1 to enable this feature that saves the protector faults.
	uint8_t enSBS :1; 		// Enable SBS.
	uint8_t enAF :1; 		// Enable Age Forecasting.
	uint8_t enMC :1; 		// Enable MiscCfg restore.
	uint8_t enDC :1; 		// Enable DesignCap restore.
	uint8_t enVE :1; 		// Enable VEmpty restore.
	uint8_t enDP :1; 		// Enable Dynamic Power.
	uint8_t enICT :1; 		// Enable IChgTerm restore.

	uint8_t enLCfg :1; 		// Enable LearnCfg restore.
	uint8_t enRCfg :1; 		// Enable RelaxCfg restore.
	uint8_t enFCfg :1; 		// Enable FilterCfg restore.
	uint8_t enSHA :1; 		// Set to 1 to configure the MTP at address 0x1DC to 0x1DF as SHA space. Set to 0 to configure address 0x1DC to 0x1DF as user MTP.
	uint8_t enX :1; 		// Enable XTable restore.
	uint8_t enOCV :1; 		// Enable OCVTable restore.
	uint8_t enCTE :1; 		// Enable Converge-to-Empty.
	uint8_t enAT :1; 		// Enable Alert Thresholds.

	uint8_t enTTF :1; 		// Enable time-to-full configuration.
	uint8_t enProtChksm :1;	// Enable protector checksum function.
	uint8_t enJ :1; 		// Enable ChargingCurrent and ChargingVoltage.
	uint8_t enProt :1; 		// Enable Protector.
	uint8_t enSC :1; 		// Enable special chemistry model.
	uint8_t enJP :1; 		// Enable Protection with JEITA (temperature region dependent).
	uint8_t enFTh :1; 		// Enable FullSOCThr configuration restore.
	uint8_t enMtl :1; 		// Enable CGTempCo restore.

	uint8_t FibScl :2; 		// Set the FibMax and FibScl "Fibonacci Saving" interval to provide recurring log-saving according to the expected battery lifespan.
	uint8_t FibMax :3; 		// Set the FibMax and FibScl "Fibonacci Saving" interval to provide recurring log-saving according to the expected battery lifespan.
	uint8_t enFL :1; 		// Enable Fault Logging.
	uint8_t enMet :1; 		// Enable metal current sensing.
	uint8_t enFC :1; 		// Enable FullCap and FullCapRep backup and restore.

	uint8_t enVT :1; 		// Enable Voltage and Temperature backup.
	uint8_t enMMC :1; 		// Enable MaxMinCurr Backup.
	uint8_t enMMV :1; 		// Enable MaxMinVolt Backup.
	uint8_t enMMT :1; 		// Enable MaxMinTemp Backup.
	uint8_t enT :1; 		// Enable TimerH backup and restore.
	uint8_t HibScalar :3; 	// Sets the task period while in hibernate mode based.

	uint8_t HibThreshold :4;// Sets the threshold level for entering or exiting hibernate mode.
	uint8_t HibEnterTime :3;// Sets the time period that consecutive current readings.
	uint8_t EnHib :1; 		// Enable Hibernate Mode.

	uint8_t HibExitTime :2; // Sets the required time period of consecutive current readings.
	uint8_t VOLT :3; 		// Sets the time constant for the AvgVCell register.
	uint8_t TEMP :3; 		// Sets the time constant for the AvgTA register.

	uint8_t CURR :4; 		// Sets the time constant for the AvgCurrent register.
	uint8_t MIX :4; 		// Sets the time constant for the mixing algorithm.

	uint8_t MR :4; 			// Mixing Rate. This value sets the strength of the servo mixing rate.
	uint8_t FUS :4; 		// Full Update Slope. This field prevents jumps in the RepSOC and FullCapRep registers.

	uint8_t SACFG :2; 		// SOC Alert Config. SOC Alerts can be generated by monitoring any of the SOC registers as follows.
	uint8_t dt :4; 			// Sets the time period over which change in VCell is compared against dV.
	uint8_t :2; 			//

	uint8_t dV :5; 			// Sets the threshold, which VCell is compared against.
	uint8_t LS :3; 			// Learn Stage. The Learn Stage value controls the influence of the voltage fuel gauge on the mixing algorithm.

	uint8_t RepL_per_stg :3;// Adjusts the RepLow threshold setting depending on the present learn stage.
	uint8_t VoltLowOff :5;	// When the AvgVCell register value drops below the VoltLow threshold, RepCap begins to bend downwards.

	uint8_t MinSlopeX :4; 	// Sets the amount of slope shallowing which occurs when RepSOC falls below RepLow.
	uint8_t RepLow :4; 		// Sets the threshold below which RepCap begins to bend upwards.

	uint8_t NR :3; 			// Sets the filter magnitude for ripple observation.
	uint8_t NCELLS :2; 		// Number of Cells.
	uint8_t NThrms :3; 		// Number of Thermistor Channels.

	uint8_t THType :1; 		// If using 10kΩ NTC thermistor, set THType = 0.
	uint8_t BtPkEn :1; 		// Enable Pckp and Batt Channels update.
	uint8_t AOCfg :2; 		// Always-on LDO Configuration 00b = off, 01b = 3.4V, 10b = 1.8V, 11b = 3.4V.
	uint8_t UVShdn :4; 		// UnderVoltage Shutdown Threshold.

	uint8_t UOCVP :5; 		// Under Open Circuit Voltage Protection Threshold.
	uint8_t dHotChargeV :3; // HotChargeV defines the delta charge voltage (relative to WarmChargeV) between temperatures T3 and T4.

	uint8_t UVPTh :5; 		// UnderVoltage Protection threshold.
	uint8_t dColdChargeV :3;// ColdChargeV defines the delta charge voltage (relative to RoomChargeV) between temperatures T1 and T2.

	uint8_t dWarmChargeV :2;// WarmChargeV defines the delta charge voltage (relative to RoomChargeV) between temperatures TWarm and T3.
	uint8_t ChgDetTh :3;	// Charger Detection Threshold.
	uint8_t BALCFG :3;		// Balancing Configuration.

	uint8_t dOVPR :4;		// Delta from Overvoltage Protection to the Overvoltage-Release Threshold.
	uint8_t dOVP :4;		// Delta from ChargeVoltage to Overvoltage Protection.

	uint8_t OVPPermFail :4;	// Permanent Failure OVP (permanent overvoltage protection) Threshold.
	uint8_t Imbal :4;		// Cell Imbalance Protection Threshold.

	uint8_t Rmismatch :4;	// Choose Rmismatch in mΩ approximately 20% of nominal battery resistance.
	uint8_t Zener :1;		// Zener Balancing Enable.
	uint8_t HotCOEF :3;		// Coefficient 12.5% to 100% relative to ChargingCurrent for controlling the charge current at hot.

	uint8_t ODTH :4;		// Overdischarge Threshold Setting.
	uint8_t SCTH :4;		// Short-Circuit Threshold Setting.

	uint8_t OCTH :5;		// Overcharge Threshold Setting.
	uint8_t ColdCOEF :3;	// Coefficient 12.5% to 100% relative to ChargingCurrent for controlling the charge current at cold.

	uint8_t OCDLY :4;		// Overdischarge and Overcharge Current Delay.
	uint8_t SCDLY :4;		// Short-Circuit Delay.

	uint8_t DieTempTh  :4;	// Sets the Dietemp Overtemperature-Protection Threshold.
	uint8_t CurrDet :4;		// It is a threshold to detect discharging and charging events.

	uint8_t TooHotDchg :4;	// Sets the Overtemperature-Protection Threshold Associated with Discharge.
	uint8_t QovflwTh  :4;	// Capacity Overflow Threshold.

	uint8_t WarmCOEF :2; 	// Coefficient 62.5% to 100% relative to ChargingCurrent for controlling the charge current at warm.
	uint8_t UVPTimer :2; 	// Set UVPTimer to configure the Undervoltage-Protection timer.
	uint8_t TempTimer :2; 	// Set TempTimer to configure the fault-timing for the following faults: TooColdC, TooHotC, Die-Hot, and TooHotD.
	uint8_t PermFailTimer :2;// Set PermFailTimer to configure the fault-timing for permanent failure detection.

	uint8_t OverCurrTimer :3;// It configures the fault timing for the slow overcharge-current detection (OCCP) as well as	overdischarge-current detection (ODCP).
	uint8_t OVPTimer :2; 	// Set OVPTimer to configure the fault timing for Overvoltage-Protection.
	uint8_t FullTimer :3; 	// Set FullTimer to configure the timing for full-detection.

	uint8_t CHGWDT :2; 		// Set CHGWDT to configure the charger communication watchdog timer.
	uint8_t LeakCurrTh  :4;	// Leakage Current Threshold is an unsigned 4-bit threshold for leakage current alert and fault generation.
	uint8_t CEEn :1; 		// Coulombic-efficiency (CE) method enable.
	uint8_t  :1; 			//

	uint8_t LeakFaultCfg :3;// Leakage Fault Configuration.
	uint8_t FetPFEn :1;		// FET PermFail Enable.
	uint8_t UVRdy :1;		// Undervoltage-Ready.
	uint8_t OvrdEn :1;		// Override Enable. Set OvrdEn = 1 to enable the Alert pin to be an input to turn disable the protection FETs.
	uint8_t DeepShpEn :1;	// Deepship Enable.
	uint8_t PFEn :1;		// PermFail Enable.

	uint8_t PreqEn :1;		// PreQual Enable. Set PreqEn = 1 to enable the Pre-Qual enable functionality.
	uint8_t CmOvrdEn :1;	// Comm Override Enable.
	uint8_t ChgWDTEn :1;	// Charger WatchDog Enable.
	uint8_t StepdV0 :4;		// This  register bit-fields scale the JEITA zone charge current down by a 4-bit ratio from 1/16 to 16/16.
	uint8_t :1;				//

	uint8_t PreChgCurr :5;	// Sets the precharging current for the ChargingCurrent register.
	uint8_t HeatLim :3;		// Set HeatLim to limit the thermal dissipation in the protection FETs during prequal regulation.

	uint8_t PreQualVolt :5;	// Sets the prequal voltage.
	uint8_t SCTest :2;		// Set SCTest = 01b to source 30μA from BATT to PCKP for testing the presence/removal of any overload/shortcircuit at PCKP.
	uint8_t FullEn :1;		// Full Charge Protection Enable.

	uint8_t StepdV1 :4;		// This  register bit-fields scale the JEITA zone charge current down by a 4-bit ratio from 1/16 to 16/16.
	uint8_t StepCurr1 :4;	// This register bit-fields configure StepVolt0 and StepVolt1 relative to the JEITA zone charge voltage.

	uint8_t StepCurr2 :4;	// This register bit-fields configure StepVolt0 and StepVolt1 relative to the JEITA zone charge voltage.
	uint8_t DeadTrgRt :4;	// Sets the remaining percentage of initial cell capacity where the cell is considered fully aged.

	uint8_t LeakCurr; 		// Leakage Current Register. Leakage current is an unsigned 8-bit result of leakage current from self-discharge in a cell.
	uint16_t Cycle; 		// Cycles Register. The Cycles register maintains a total count of the number of charge/discharge cycles of the cell that have occurred.
	uint16_t FullCapNom; 	// FullCapNom Register. This register holds the calculated full capacity of the cell, not including temperature and empty compensation.
	uint16_t AvgVCell; 		// AvgVCell Register. This register provides nonvolatile back up of the AvgVCell register.
	uint8_t AvgTA; 			// AvgTA Register. This register provides nonvolatile back up of the AvgTA register.
	uint16_t XTable0;		// XTable0 Register. This table comes from battery characterization data.
	uint16_t XTable1;		// XTable1 Register. This table comes from battery characterization data.
	uint16_t XTable2;		// XTable2 Register. This table comes from battery characterization data.
	uint16_t XTable3;		// XTable3 Register. This table comes from battery characterization data.
	uint16_t XTable4;		// XTable4 Register. This table comes from battery characterization data.
	uint16_t XTable5;		// XTable5 Register. This table comes from battery characterization data.
	uint16_t XTable6;		// XTable6 Register. This table comes from battery characterization data.
	uint16_t XTable7;		// XTable7 Register. This table comes from battery characterization data.
	uint16_t XTable8;		// XTable8 Register. This table comes from battery characterization data.
	uint16_t XTable9;		// XTable9 Register. This table comes from battery characterization data.
	uint16_t XTable10;		// XTable10 Register. This table comes from battery characterization data.
	uint16_t XTable11;		// XTable11 Register. This table comes from battery characterization data.
	uint16_t OCVTable0;		// OCVTable0 Register. This table comes from battery characterization data.
	uint16_t OCVTable1;		// OCVTable1 Register. This table comes from battery characterization data.
	uint16_t OCVTable2;		// OCVTable2 Register. This table comes from battery characterization data.
	uint16_t OCVTable3;		// OCVTable3 Register. This table comes from battery characterization data.
	uint16_t OCVTable4;		// OCVTable4 Register. This table comes from battery characterization data.
	uint16_t OCVTable5;		// OCVTable5 Register. This table comes from battery characterization data.
	uint16_t OCVTable6;		// OCVTable6 Register. This table comes from battery characterization data.
	uint16_t OCVTable7;		// OCVTable7 Register. This table comes from battery characterization data.
	uint16_t OCVTable8;		// OCVTable8 Register. This table comes from battery characterization data.
	uint16_t OCVTable9;		// OCVTable9 Register. This table comes from battery characterization data.
	uint16_t OCVTable10;	// OCVTable10 Register. This table comes from battery characterization data.
	uint16_t OCVTable11;	// OCVTable11 Register. This table comes from battery characterization data.
	uint16_t QRTable00;		// QRTable00 Register. This register contain characterization information regarding cell capacity.
	uint16_t QRTable10;		// QRTable10 Register. This register contain characterization information regarding cell capacity.
	uint16_t QRTable20;		// QRTable20 Register. This register contain characterization information regarding cell capacity.
	uint16_t QRTable30;		// QRTable30 Register. This register contain characterization information regarding cell capacity.
	uint16_t FullSOCThr;	// FullSOCThr Register. This register gates detection of end-of-charge.
	uint16_t VE;			// Empty Voltage Register. This register sets the voltage level for detecting empty.
	uint8_t VR;				// Recovery Voltage Register. This register sets the voltage level for clearing empty detection.
	uint16_t DesignCap;		// DesignCap Register. This register holds the expected capacity of the cell.
	uint16_t IChgTerm;		// IChgTerm Register. This register r allows the device to detect when a charge cycle of the cell has completed.
	uint16_t RComp0;		// RComp0 Register. This register holds characterization information critical to computing the open circuit voltage of a cell under loaded conditions.
	uint16_t TempCo;		// TempCo Register. This register register holds temperature compensation information for the nRComp0 register value.
	uint8_t LOAD; 			// Load Register. Sets the threshold, which the AvgCurrent register is compared against.
	uint8_t CVMixCapRatio;	// CVMixCapRatio Register. Register is defined as the approximate state of charge where the charge transitions from the constant current portion.
	uint8_t CVHalfTime;		// CVHalfTime Register. Register is defined as the amount of time in the constant voltage portion of the charge cycle.
	uint16_t kDV;			// Sets the corresponding amount of capacity to compensate proportional to the ripple.
	uint16_t ROMID0;		// Contains a unique 64-bit identification value that is contained in the nROMID registers.
	uint16_t ROMID1;		// Contains a unique 64-bit identification value that is contained in the nROMID registers.
	uint16_t ROMID2;		// Contains a unique 64-bit identification value that is contained in the nROMID registers.
	uint16_t ROMID3;		// Contains a unique 64-bit identification value that is contained in the nROMID registers.
	uint8_t VMIN; 			// VMIN Register. Minimum voltage reading. An alert is generated if the VCell register reading falls below this value.
	uint8_t VMAX; 			// VMAX Register. Maximum voltage reading. An alert is generated if the maximum cell voltage reading exceeds this value.
	uint8_t TMIN; 			// TMIN Register. Minimum temperature reading. An alert is generated if the Temp register reading falls below this value.
	uint8_t TMAX; 			// TMAX Register. Maximum temperature reading. An alert is generated if any temperature channel reading exceeds this value.
	uint8_t CURRMIN; 		// CURRMIN Register. Minimum Current Threshold. An alert is generated if the current register reading falls below this value.
	uint8_t CURRMAX; 		// CURRMAX Register. Maximum Current Threshold. An alert is generated if the current register reading exceeds this value.
	uint8_t SMIN; 			// SMIN Register. Minimum state-of-charge reading. An alert is generated if the selected SOC register reading falls below this value.
	uint8_t SMAX; 			// SMAX Register. Maximum state-of-charge reading. An alert is generated if the selected SOC register reading exceeds this value.
	uint8_t RoomChargeV; 	// RoomChargeV Register. It defines the charge voltage between temperatures T2 "Cold" and T3 "Warm", relative to a standard 4.2V setting.
	uint8_t ODCPTh; 		// Overdischarge Current-Protection Threshold Register.
	uint8_t OCCPTh; 		// Overcharge Current-Protection Threshold at Room Temperature.
	uint8_t RoomChgCurrent; // RoomChargingCurrent Register. Sets the nominal room-temperature charging current.
	uint8_t T1; 			// Too-Cold temperature threshold Register. When Temp < T1, charging is considered unsafe and unhealthy, and the IC blocks charging.
	uint8_t T2; 			// Cold temperature threshold Register. When Temp < T2, charging current/voltage should be reduced, and the chargeprotection thresholds are adjusted accordingly.
	uint8_t T3; 			// Hot temperature threshold Register. When Temp > T3, charging current/voltage should be reduced and the chargeprotection thresholds are adjusted accordingly.
	uint8_t T4; 			// Too-Hot temperature threshold Register. When Temp > T4, charging is blocked by the IC.
	uint8_t Twarm ; 		// Twarm Register. Warm temperature threshold, giving an extra temperature region for changing charging current and charging voltage control.
	uint8_t TpermFailHot; 	// TpermFailHot Register. If enabled, the IC goes into permanent failure mode, and permanently disables the charge FET or blows the fuse (if installed).
	uint8_t CheckSum; 		// CheckSum Register. Protector NVM CheckSum.
	uint8_t LeakCurrent; 	// LeakCurrent Register. Reported Leak Current is an unsigned 15-bit leakage current.
	uint16_t FullCapRep; 	// FullCapRep Register. This register reports the full capacity that goes with RepCap.
	int8_t MaxCurr; 		// MaxCurr Register. Maximum Current register reading (0.40mV resolution).
	int8_t MinCurr; 		// MinCurr Register. Minimum Current register reading (0.40mV resolution).
	uint8_t MaxVCELL; 		// MaxVCELL Register. Maximum channel voltage reading (20mV resolution).
	uint8_t MinVCELL; 		// MinVCELL Register. Minimum VCell register reading (20mV resolution).
	int8_t MaxTemp; 		// MaxTemp Register. Maximum Temp register reading (1ºC resolution).
	int8_t MinTemp; 		// MinTemp Register. Minimum Temp register reading (1ºC resolution).
	uint16_t TimerH; 		// TimerH Register. This register allows the IC to track the age of the cell.
	uint16_t ChgCtl; 		// ChgCtl Register.
	uint16_t RGain; 		// RGain Register.
	uint16_t PackRes; 		// PackRes Register.
	uint8_t COff; 			// COff Register. COff has a range of -32 to +31 LSbs.
	uint16_t CGain; 		// CGain Register. The recommended default value of CGain = 0x100 corresponds to a gain of 1.
	uint16_t ThermCfg; 		// ThermCfg Register. Set nThermCfg register to compensate thermistor for accurate translation of temperature.
	uint16_t RSense; 		// RSense Register. The register is the designated location to store the nominal sense resistor value used by the application.
	uint16_t DPLimit; 		// DPLimit Register.
	uint16_t ScOcvLim; 		// ScOcvLim Register.
	uint16_t ManfctrDate; 	// ManfctrDate Register.
	uint16_t FirstUsed; 	// FirstUsed Register.
	uint8_t CycleStart; 	// CycleStart Register. Sets the number of cell cycles before age forecasting calculations begin.
	uint8_t Vdesign; 		// Vdesign  Register. 'Design voltage' specification for the design.
	uint8_t Vminsys; 		// Vminsys Register. 'Minimum system voltage' specification for the design. Generates MinSysVoltage value.
	uint8_t ManfName0; 		// ManfName0 Register.
	uint8_t ManfName1; 		// ManfName1 Register.
	uint8_t ManfName2; 		// ManfName2 Register.
	uint8_t SerNum0; 		// SerNum0 Register.
	uint8_t SerNum1; 		// SerNum1 Register.
	uint8_t SerNum2; 		// SerNum2 Register.
	uint8_t DevName0; 		// DevName0 Register.
	uint8_t DevName1; 		// DevName1 Register.
	uint8_t DevName2; 		// DevName2 Register.
	uint8_t DevName3; 		// DevName3 Register.
	uint8_t DevName4; 		// DevName4 Register.

} MAX17320_RegDataEEPROME;

typedef struct // The register data
{
	uint8_t POR :1; 		// Power-On Reset. This bit is set to a 1 when the device detects that a software or hardware POR event has	occurred.
	uint8_t Imn :1; 		// Minimum Current Alert Threshold Exceeded. This bit is set to a 1 whenever a Current register reading is below the minimum IAlrtTh value.
	uint8_t Imx :1; 		// Maximum Current Alert Threshold Exceeded. This bit is set to a 1 whenever a Current register reading is above the maximum IAlrtTh value.
	uint8_t dSOCi :1; 		// State-of-Charge 1% Change Alert. This is set to 1 whenever the RepSOC register have been change 1% .
	uint8_t Vmn :1; 		// Minimum Voltage Alert Threshold Exceeded. This bit is set to a 1 whenever a VCell register reading is below the	minimum VAlrtTh value.
	uint8_t Tmn :1; 		// Minimum Temperature Alert Threshold Exceeded. This bit is set to a 1 whenever a Temperature register reading is below the minimum TAlrtTh value.
	uint8_t Smn :1; 		// Minimum SOC Alert Threshold Exceeded. This bit is set to a 1 whenever SOC falls below the minimum SAlrtTh value.
	uint8_t Vmx :1; 		// Maximum Voltage Alert Threshold Exceeded. This bit is set to a 1 whenever a VCell register reading is above the maximum VAlrtTh value.

	uint8_t Tmx :1; 		// Maximum Temperature Alert Threshold Exceeded. This bit is set to a 1 whenever a Temperature register reading is above the maximum TAlrtTh value.
	uint8_t Smx :1; 		// Maximum SOC Alert Threshold Exceeded. This bit is set to a 1 whenever SOC rises above the maximum SAlrtTh value.
	uint8_t PA :1; 			// Protection Alert. This bit is set to a 1 when there is a protection event.
	uint8_t Hib :1; 		// Hibernate Status. This bit is set to a 1 when the device is in hibernate mode or 0 when the device is in active mode.
	uint8_t Ship :1; 		// A flag to indicate the ship state.
	uint8_t ResDFault :1; 	// ???
	uint8_t ODCP :1; 		// Overdischarge current faults.
	uint8_t UVP :1; 		// Undervoltage faults.

	uint8_t TooHotD :1; 	// Overtemperature for discharging faults.
	uint8_t DieHot :1; 		// Overtemperature for die temperature faults.
	uint8_t PermFail :1; 	// Permanent Failure Detected.
	uint8_t Imbalance :1; 	// Multicell imbalance faults.
	uint8_t PreqF :1; 		// Prequal timeout faults.
	uint8_t Qovflw :1; 		// Capacity overflow faults.
	uint8_t OCCP :1; 		// Overcharge current faults.
	uint8_t OVP :1; 		// Overvoltage faults.

	uint8_t TooColdC :1; 	// Undertemperature for charging faults.
	uint8_t Full :1; 		// Full detection faults.
	uint8_t TooHotC :1; 	// Overtemperature for discharging faults.
	uint8_t ChgWDT :1; 		// Charge communication watchdog timer faults.
	uint8_t HisLDet :1; 	// History Leakage Detection Fault.
	uint8_t HisResDFault :1;// History ???
	uint8_t HisODCP :1; 	// History Overdischarge current faults.
	uint8_t HisUVP :1; 		// History Undervoltage faults.

	uint8_t HisTooHotD :1; 	// History Overtemperature for discharging faults.
	uint8_t HisDieHot :1; 	// History Overtemperature for die temperature faults.
	uint8_t HisPermFail :1; // History Permanent Failure Detected.
	uint8_t HisImbalance :1;// History Multicell imbalance faults.
	uint8_t HisPreqF :1; 	// History Prequal timeout faults.
	uint8_t HisQovflw :1; 	// History Capacity overflow faults.
	uint8_t HisOCCP :1; 	// History Overcharge current faults.
	uint8_t HisOVP :1; 		// History Overvoltage faults.

	uint8_t HisTooColdC :1; // History Undertemperature for charging faults.
	uint8_t HisFull :1; 	// History Full detection faults.
	uint8_t HisTooHotC :1; 	// History Overtemperature for discharging faults.
	uint8_t HisChgWDT :1; 	// History Charge communication watchdog timer faults.
	uint8_t PAen :1; 		// Protection Alert Enable. Set PAen = 1 to enable this feature that saves the protector faults.
	uint8_t Aen :1; 		// Enable Alert on Fuel-Gauge Outputs.
	uint8_t FTHRM :1; 		// Force Thermistor Bias Switch.
	uint8_t ETHRM :1; 		// Enable Thermistor.

	uint8_t COMMSH :1; 		// Communication Shutdown.
	uint8_t SHIP :1; 		// Ship or Deepship Command.
	uint8_t DisBlockRead :1;// Disable SBS Block Read.
	uint8_t PBen :1; 		// PushButton Enable.
	uint8_t DisLDO :1; 		// Disable AOLDO.
	uint8_t VS :1; 			// Voltage ALRT Sticky.
	uint8_t TS :1; 			// Temperature ALRT Sticky.
	uint8_t SS :1; 			// SOC ALRT Sticky.

	uint8_t DRCfg :2; 		// Deep Relax Time Configuration.
	uint8_t TAlrtEn :1; 	// Temperature Alert Enable.
	uint8_t dSOCen :1; 		// SOC Change Alert Enable.
	uint8_t POWR :4; 		// Sets the time constant for the AvgPower register.

	uint8_t ADCFIFOen :1; 	// Set 1 to enable	continuous acquisition mode for ADC FIFO.
	uint8_t POR_CMD :1; 	// Firmware Restart.
	uint8_t VOLT :3; 		// Sets the time constant for the AvgVCell register.
	uint8_t TEMP :3; 		// Sets the time constant for the AvgTA register.

	uint8_t CURR :4; 		// Sets the time constant for the AvgCurrent register.
	uint8_t MIX :4; 		// Sets the time constant for the mixing algorithm.

	uint8_t AtRtEn :1; 		// ???
	uint8_t dt :4; 			// Sets the time period over which change in VCell is compared against dV.
	uint8_t LS :3; 			// Learn Stage. The Learn Stage value controls the influence of the voltage fuel gauge on the mixing algorithm.

	uint8_t dV :5; 			// Sets the threshold, which VCell is compared against.
	uint8_t DNR :1; 		// Data Not Ready.
	uint8_t RelDt2 :1; 		// Long Relaxation.
	uint8_t EDet :1; 		// Empty Detection.

	uint8_t EmptySocHold :5;// It is the RepSOC at which RepSOC is held constant.
	uint8_t RelDt :1; 		// Relaxed cell detection.
	uint8_t  :2; 			//

	uint8_t EmptyVHold :7;	// The positive voltage offset that is added to VEmpty.
	uint8_t HoldEn :1; 		// Enable bit for 99% hold feature during charging.

	uint8_t MR :4; 			// Mixing Rate. This value sets the strength of the servo mixing rate.
	uint8_t FUS :4; 		// Full Update Slope. This field prevents jumps in the RepSOC and FullCapRep registers.

	uint8_t SACFG :2; 		// SOC Alert Config. SOC Alerts can be generated by monitoring any of the SOC registers as follows.
	uint8_t BalCell1 :1;	// Flag balancing cell1.
	uint8_t BalCell2 :1;	// Flag balancing cell2.
	uint8_t BalCell3 :1;	// Flag balancing cell3.
	uint8_t BalCell4 :1;	// Flag balancing cell4.
	uint8_t  :2;

	uint16_t VCell; 		// VCell Register. Each update cycle, the lowest reading from all cell voltage measurements is placed in the VCell register.
	uint16_t AvgVCell; 		// AvgVCell Register. The AvgVCell register reports an average of the VCell register readings.
	uint16_t Cell1; 		// Cell1 Register. Each update cycle, the cell voltage measurement for each cell is placed in  Cell1 register.
	uint16_t Cell2; 		// Cell2 Register. Each update cycle, the cell voltage measurement for each cell is placed in  Cell2 register.
	uint16_t Cell3; 		// Cell3 Register. Each update cycle, the cell voltage measurement for each cell is placed in  Cell3 register.
	uint16_t Cell4; 		// Cell4 Register. Each update cycle, the cell voltage measurement for each cell is placed in  Cell4 register.
	uint16_t AvgCell1; 		// AvgCell1 Register. Register report an 8-sample filtered average of Cell1 register readings.
	uint16_t AvgCell2; 		// AvgCell2 Register. Register report an 8-sample filtered average of Cell2 register readings.
	uint16_t AvgCell3; 		// AvgCell3 Register. Register report an 8-sample filtered average of Cell3 register readings.
	uint16_t AvgCell4; 		// AvgCell4 Register. Register report an 8-sample filtered average of Cell4 register readings.
	uint16_t Batt; 			// Batt Register. Registers contains the total pack voltage measured inside the protector on a 20.48V scale with an LSB of	0.3125mV.
	uint16_t PCKP; 			// PCKP Register. Register contains the voltage between PACK+ and GND on a 20.48V scale with an LSB of 0.3125mV.
	uint8_t MaxVCELL; 		// MaxVCELL Register. Maximum channel voltage reading (20mV resolution).
	uint8_t MinVCELL; 		// MinVCELL Register. Minimum VCell register reading (20mV resolution).
	int16_t Curr; 			// Curr Register. Register contains measure the voltage between the CSP and CSN pins.
	int16_t AvgCurr; 		// AvgCurr Register. Register reports an average of Current register readings over a configurable 0.7 second to 6.4 hour time period.
	int8_t MaxCurr; 		// MaxCurr Register. Maximum Current register reading (0.40mV resolution).
	int8_t MinCurr; 		// MinCurr Register. Minimum Current register reading (0.40mV resolution).
	int16_t Temp; 			// Temp Register. The register reflects the highest thermistor temperature, or the die-temperature if the thermistors are disabled.
	int16_t AvgTA; 			// AvgTA Register. Register reports an average of the readings from the Temp register.
	int8_t MaxTemp; 		// MaxTemp Register. Maximum Temp register reading (1ºC resolution).
	int8_t MinTemp; 		// MinTemp Register. Minimum Temp register reading (1ºC resolution).
	int16_t DieTemp; 		// DieTemp Register. This register displays die temperature in degrees Celsius.
	int16_t AvgDieTemp; 	// AvgDieTemp Register. The register reports a 4-sample filtered average of the DieTemp register.
	int16_t Temp1; 			// Temp1 Register. The register display temperature readings from thermistor 1 (if enabled) in degrees Celsius.
	int16_t Temp2; 			// Temp2 Register. The register display temperature readings from thermistor 2 (if enabled) in degrees Celsius.
	int16_t Temp3; 			// Temp3 Register. The register display temperature readings from thermistor 3 (if enabled) in degrees Celsius.
	int16_t Temp4; 			// Temp4 Register. The register display temperature readings from thermistor 4 (if enabled) in degrees Celsius.
	int16_t AvgTemp1; 		// AvgTemp1 Register. The register report a 4-sample filtered average of the Temp1 registers.
	int16_t AvgTemp2; 		// AvgTemp1 Register. The register report a 4-sample filtered average of the Temp2 registers.
	int16_t AvgTemp3; 		// AvgTemp1 Register. The register report a 4-sample filtered average of the Temp3 registers.
	int16_t AvgTemp4; 		// AvgTemp1 Register. The register report a 4-sample filtered average of the Temp4 registers.
	int16_t Pwr; 			// Pwr Register. Registers contains instantaneous power calculation from immediate current and voltage.
	int16_t AvgPwr; 		// AvgPwr Register. Registers contains filtered Average Power from the power register.
	int16_t ChgCurr; 		// ChgCurr Register. Registers reports the prescribed charging current.
	int16_t ChgVolt; 		// ChgVolt Register. Registers reports the prescribed charging voltage.
	uint16_t RepCap; 		// RepCap Register. Reported Capacity is a filtered version of the AvCap register that prevents large jumps.
	uint16_t RepSOC; 		// RepSOC Register. RepSOC is a filtered version of the AvSOC register that prevents large jumps.
	uint16_t FullCapRep; 	// FullCapRep Register. This register reports the full capacity that goes with RepCap.
	uint16_t TTE; 			// TTE Register. The TTE register holds the estimated time-to-empty for the application under present temperature and load conditions.
	uint16_t TTF; 			// TTF Register. The TTF register holds the estimated time-to-full for the application under present conditions.
	uint16_t Age; 			// Age Register. The Age register contains a calculated percentage value of the application’s present cell capacity.
	uint16_t Cycles; 		// Cycles Register. The Cycles register maintains a total count of the number of charge/discharge cycles of the cell that have occurred.
	uint16_t Timer; 		// Timer Register. This register holds timing information for the fuel gauge.
	uint16_t TimerH; 		// TimerH Register. This register allows the IC to track the age of the cell.
	uint16_t FullCapNom; 	// FullCapNom Register. This register holds the calculated full capacity of the cell based on all inputs from the ModelGauge m5 algorithm.
	uint16_t RCell; 		// RCell Register. The register displays the calculated internal resistance of the cell or the average internal resistance.
	uint16_t VRipple; 		// VRipple Register. The register holds the slow average RMS value of the VCell register reading variation compared to the AvgVCell	register.
	uint8_t LOAD; 			// Load Register. Sets the threshold, which the AvgCurrent register is compared against.
	uint16_t CVMixCapRatio;	// CVMixCapRatio Register. Register is defined as the approximate state of charge where the charge transitions from the constant current portion.
	uint16_t CVHalfTime;	// CVHalfTime Register. Register is defined as the amount of time in the constant voltage portion of the charge cycle.
	uint8_t Device;			// Device Register. Register holds the device name.
	uint16_t Revision;		// Revision Register. Register holds the device revision number.
	uint16_t QRes;			// QResidual Register. Register is displays the calculated amount of charge in mAh that is presently inside.
	uint16_t VFSOC;			// VFSOC Register. Register holds the calculated present state-of-charge of the battery according to the voltage fuel gauge.
	uint16_t VFOCV;			// VFOCV Register. Register contains the calculated open-circuit voltage of the cell as determined by the voltage fuel gauge.
	uint16_t QH;			// QH Register. Register displays the raw coulomb count generated by the device.
	uint16_t QL;			// QL Register. Register displays the raw coulomb count generated by the device.
	uint16_t AvCap;			// AvCap Register. Register holds the calculated available capacity of the cell pack based on all inputs from the ModelGauge.
	uint16_t AvSOC;			// AvSOC Register. Register holds the calculated available state of charge of the cell based on all inputs from the ModelGauge.
	uint16_t MixSOC;		// MixSOC Register. Register holds the calculated present state-of-charge of the cell before any empty compensation	adjustments are performed.
	uint16_t MixCap;		// MixCap Register. Register holds the calculated remaining capacity of the cell before any empty compensation adjustments are performed.
	uint16_t VFRemCap;		// VFRemCap Register. Register holds the remaining capacity of the cell as determined by the voltage fuel gauge.
	int16_t AtRate;			// AtRate Register. Host software should write the AtRate register with a negative two’s-complement 16-bit value of a theoretical load current.
	uint16_t AtQRes;		// AtQRes Register. Register displays the residual charge held by the cell at the theoretical load-current level entered into the AtRate register.
	uint16_t AtTTE;			// AtTTE Register. Register estimate time-to-empty for any theoretical current load entered into the AtRate	register.
	uint16_t AtAvSOC;		// AtAvSOC Register. Register r holds the theoretical state of charge of the cell based on the theoretical current load of the AtRate register.
	uint16_t AtAvCap;		// AtAvCap Register. Register holds the estimated remaining capacity of the cell based on the theoretical load current value of	the AtRate register.
	uint8_t VMIN; 			// VMIN Register. Minimum voltage reading. An alert is generated if the VCell register reading falls below this value.
	uint8_t VMAX; 			// VMAX Register. Maximum voltage reading. An alert is generated if the maximum cell voltage reading exceeds this value.
	uint8_t TMIN; 			// TMIN Register. Minimum temperature reading. An alert is generated if the Temp register reading falls below this value.
	uint8_t TMAX; 			// TMAX Register. Maximum temperature reading. An alert is generated if any temperature channel reading exceeds this value.
	uint8_t CURRMIN; 		// CURRMIN Register. Minimum Current Threshold. An alert is generated if the current register reading falls below this value.
	uint8_t CURRMAX; 		// CURRMAX Register. Maximum Current Threshold. An alert is generated if the current register reading exceeds this value.
	uint8_t SMIN; 			// SMIN Register. Minimum state-of-charge reading. An alert is generated if the selected SOC register reading falls below this value.
	uint8_t SMAX; 			// SMAX Register. Maximum state-of-charge reading. An alert is generated if the selected SOC register reading exceeds this value.
	uint16_t FullSOCThr;	// FullSOCThr Register. This register gates detection of end-of-charge.
	uint16_t DesignCap;		// DesignCap Register. This register holds the expected capacity of the cell.
	uint16_t IChgTerm;		// IChgTerm Register. This register r allows the device to detect when a charge cycle of the cell has completed.
	uint16_t VE;			// Empty Voltage Register. This register sets the voltage level for detecting empty.
	uint8_t VR;				// Recovery Voltage Register. This register sets the voltage level for clearing empty detection.
	uint16_t QRTable00;		// QRTable00 Register. This register contain characterization information regarding cell capacity.
	uint16_t QRTable10;		// QRTable10 Register. This register contain characterization information regarding cell capacity.
	uint16_t QRTable20;		// QRTable20 Register. This register contain characterization information regarding cell capacity.
	uint16_t QRTable30;		// QRTable30 Register. This register contain characterization information regarding cell capacity.
	uint16_t CGTempCo;		// CGTempCo Register. This register adjust current measurements for temperature.
	uint16_t AgeForecast;	// AgeForecast Register. This register displays the estimated cycle life of the application cell.
	uint16_t IAvgEmpty;		// IAvgEmpty Register.
	uint8_t CVMixCap;		// CVMixCapRatio Register. Register is defined as the approximate state of charge where the charge transitions from the constant current portion.
	uint8_t CVHalfT;		// CVHalfTime Register. Register is defined as the amount of time in the constant voltage portion of the charge cycle.
	uint16_t MinVolt;		// MinVolt Register. Register is defined as the approximate state of charge where the charge transitions from the constant current portion.
	int16_t MinCurrent;		// MinCurrent Register. Register is defined as the amount of time in the constant voltage portion of the charge cycle.
	uint16_t MaxPeakPwr;	// MaxPeakPwr Register.
	uint16_t SusPeakPwr;	// SusPeakPwr Register.
	uint16_t PackResist;	// PackResist Register.
	uint16_t SysResist;		// SysResist Register.
	uint16_t MinSysV;		// MinSysV Register.
	uint16_t MPPCurr;		// MPPCurr Register.
	uint16_t SPPCurr;		// IAvgEmpty Register.

} MAX17320_RegData;

typedef struct // The Battery Branch Data
{
	uint8_t Ship :1; 		// A flag to indicate the ship state.
	uint8_t ResDFault :1; 	// ???
	uint8_t ODCP :1; 		// Overdischarge current faults.
	uint8_t UVP :1; 		// Undervoltage faults.
	uint8_t TooHotD :1; 	// Overtemperature for discharging faults.
	uint8_t DieHot :1; 		// Overtemperature for die temperature faults.
	uint8_t PermFail :1; 	// Permanent Failure Detected.
	uint8_t Imbalance :1; 	// Multicell imbalance faults.

	uint8_t PreqF :1; 		// Prequal timeout faults.
	uint8_t Qovflw :1; 		// Capacity overflow faults.
	uint8_t OCCP :1; 		// Overcharge current faults.
	uint8_t OVP :1; 		// Overvoltage faults.
	uint8_t TooColdC :1; 	// Undertemperature for charging faults.
	uint8_t Full :1; 		// Full detection faults.
	uint8_t TooHotC :1; 	// Overtemperature for discharging faults.
	uint8_t ChgWDT :1; 		// Charge communication watchdog timer faults.

	uint8_t HisLDet :1; 	// History Leakage Detection Fault.
	uint8_t HisResDFault :1;// History ???
	uint8_t HisODCP :1; 	// History Overdischarge current faults.
	uint8_t HisUVP :1; 		// History Undervoltage faults.
	uint8_t HisTooHotD :1; 	// History Overtemperature for discharging faults.
	uint8_t HisDieHot :1; 	// History Overtemperature for die temperature faults.
	uint8_t HisPermFail :1; // History Permanent Failure Detected.
	uint8_t HisImbalance :1;// History Multicell imbalance faults.

	uint8_t HisPreqF :1; 	// History Prequal timeout faults.
	uint8_t HisQovflw :1; 	// History Capacity overflow faults.
	uint8_t HisOCCP :1; 	// History Overcharge current faults.
	uint8_t HisOVP :1; 		// History Overvoltage faults.
	uint8_t HisTooColdC :1; // History Undertemperature for charging faults.
	uint8_t HisFull :1; 	// History Full detection faults.
	uint8_t HisTooHotC :1; 	// History Overtemperature for discharging faults.
	uint8_t HisChgWDT :1; 	// History Charge communication watchdog timer faults.

	uint8_t BalCell1 :1;	// Flag balancing cell1.
	uint8_t BalCell2 :1;	// Flag balancing cell2.
	uint8_t BalCell3 :1;	// Flag balancing cell3.
	uint8_t BalCell4 :1;	// Flag balancing cell4.
	uint8_t LDet :1; 		// Leakage Detection Fault. This bit is set when a leakage detection fault has been detected.
	uint8_t  :3;

	uint16_t Cell1_Reg; 	// Cell1 Register. Each update cycle, the cell voltage measurement for each cell is placed in  Cell1 register.
	uint16_t Cell2_Reg; 	// Cell2 Register. Each update cycle, the cell voltage measurement for each cell is placed in  Cell2 register.
	uint16_t Cell3_Reg; 	// Cell3 Register. Each update cycle, the cell voltage measurement for each cell is placed in  Cell3 register.
	uint16_t Cell4_Reg; 	// Cell4 Register. Each update cycle, the cell voltage measurement for each cell is placed in  Cell4 register.
	uint16_t AvgCell1_Reg; 	// AvgCell1 Register. Register report an 8-sample filtered average of Cell1 register readings.
	uint16_t AvgCell2_Reg; 	// AvgCell2 Register. Register report an 8-sample filtered average of Cell2 register readings.
	uint16_t AvgCell3_Reg; 	// AvgCell3 Register. Register report an 8-sample filtered average of Cell3 register readings.
	uint16_t AvgCell4_Reg; 	// AvgCell4 Register. Register report an 8-sample filtered average of Cell4 register readings.
	uint16_t Batt_Reg; 		// Batt Register. Registers contains the total pack voltage measured inside the protector on a 20.48V scale with an LSB of	0.3125mV.
	uint8_t MaxVCELL_Reg; 	// MaxVCELL Register. Maximum channel voltage reading (20mV resolution).
	uint8_t MinVCELL_Reg; 	// MinVCELL Register. Minimum VCell register reading (20mV resolution).
	uint16_t Cell1_mV; 		// Cell1 value. Each update cycle, the cell voltage measurement for each cell is placed in  Cell1 register.
	uint16_t Cell2_mV; 		// Cell2 value. Each update cycle, the cell voltage measurement for each cell is placed in  Cell2 register.
	uint16_t Cell3_mV; 		// Cell3 value. Each update cycle, the cell voltage measurement for each cell is placed in  Cell3 register.
	uint16_t Cell4_mV; 		// Cell4 value. Each update cycle, the cell voltage measurement for each cell is placed in  Cell4 register.
	uint16_t AvgCell1_mV; 	// AvgCell1 value. Register report an 8-sample filtered average of Cell1 register readings.
	uint16_t AvgCell2_mV; 	// AvgCell2 value. Register report an 8-sample filtered average of Cell2 register readings.
	uint16_t AvgCell3_mV; 	// AvgCell3 value. Register report an 8-sample filtered average of Cell3 register readings.
	uint16_t AvgCell4_mV; 	// AvgCell4 value. Register report an 8-sample filtered average of Cell4 register readings.
	uint16_t Batt_mV; 		// Batt value. Registers contains the total pack voltage measured inside the protector on a 20.48V scale with an LSB of	0.3125mV.
	uint32_t MaxVCELL_mV; 	// MaxVCELL value. Maximum channel voltage reading (20mV resolution).
	uint32_t MinVCELL_mV; 	// MinVCELL value. Minimum VCell register reading (20mV resolution).
	int16_t Curr_Reg; 		// Curr Register. Register contains measure the voltage between the CSP and CSN pins.
	int16_t AvgCurr_Reg; 	// AvgCurr Register. Register reports an average of Current register readings over a configurable 0.7 second to 6.4 hour time period.
	int8_t MaxCurr_Reg; 	// MaxCurr Register. Maximum Current register reading (0.40mV resolution).
	int8_t MinCurr_Reg; 	// MinCurr Register. Minimum Current register reading (0.40mV resolution).
	int16_t Curr_mA; 		// Curr value. Register contains measure the voltage between the CSP and CSN pins.
	int16_t AvgCurr_mA; 	// AvgCurr value. Register reports an average of Current register readings over a configurable 0.7 second to 6.4 hour time period.
	int32_t MaxCurr_mA;		// MaxCurr value. Maximum Current register reading (0.40mV resolution).
	int32_t MinCurr_mA;		// MinCurr value. Minimum Current register reading (0.40mV resolution).
	int16_t Pwr_Reg; 		// Pwr Register. Registers contains instantaneous power calculation from immediate current and voltage.
	int16_t AvgPwr_Reg; 	// AvgPwr Register. Registers contains filtered Average Power from the power register.
	int16_t Pwr_mW; 		// Pwr value. Registers contains instantaneous power calculation from immediate current and voltage.
	int16_t AvgPwr_mW; 		// AvgPwr value. Registers contains filtered Average Power from the power register.
	int8_t MaxTemp_Reg; 	// MaxTemp Register. Maximum Temp register reading (1ºC resolution).
	int8_t MinTemp_Reg; 	// MinTemp Register. Minimum Temp register reading (1ºC resolution).
	int16_t DieTemp_Reg; 	// DieTemp Register. This register displays die temperature in degrees Celsius.
	int16_t AvgDieTemp_Reg; // AvgDieTemp Register. The register reports a 4-sample filtered average of the DieTemp register.
	int16_t Temp1_Reg; 		// Temp1 Register. The register display temperature readings from thermistor 1 (if enabled) in degrees Celsius.
	int16_t Temp2_Reg; 		// Temp2 Register. The register display temperature readings from thermistor 2 (if enabled) in degrees Celsius.
	int16_t Temp3_Reg; 		// Temp3 Register. The register display temperature readings from thermistor 3 (if enabled) in degrees Celsius.
	int16_t Temp4_Reg; 		// Temp4 Register. The register display temperature readings from thermistor 4 (if enabled) in degrees Celsius.
	int16_t AvgTemp1_Reg; 	// AvgTemp1 Register. The register report a 4-sample filtered average of the Temp1 registers.
	int16_t AvgTemp2_Reg; 	// AvgTemp1 Register. The register report a 4-sample filtered average of the Temp2 registers.
	int16_t AvgTemp3_Reg; 	// AvgTemp1 Register. The register report a 4-sample filtered average of the Temp3 registers.
	int16_t AvgTemp4_Reg; 	// AvgTemp1 Register. The register report a 4-sample filtered average of the Temp4 registers.
	int8_t MaxTemp_Dg; 		// MaxTemp value. Maximum Temp register reading (1ºC resolution).
	int8_t MinTemp_Dg; 		// MinTemp value. Minimum Temp register reading (1ºC resolution).
	int16_t DieTemp_Dg; 	// DieTemp value. This register displays die temperature in degrees Celsius.
	int16_t AvgDieTemp_Dg; 	// AvgDieTemp value. The register reports a 4-sample filtered average of the DieTemp register.
	int16_t Temp1_Dg; 		// Temp1 value. The register display temperature readings from thermistor 1 (if enabled) in degrees Celsius.
	int16_t Temp2_Dg; 		// Temp2 value. The register display temperature readings from thermistor 2 (if enabled) in degrees Celsius.
	int16_t Temp3_Dg; 		// Temp3 value. The register display temperature readings from thermistor 3 (if enabled) in degrees Celsius.
	int16_t Temp4_Dg; 		// Temp4 value. The register display temperature readings from thermistor 4 (if enabled) in degrees Celsius.
	int16_t AvgTemp1_Dg; 	// AvgTemp1 value. The register report a 4-sample filtered average of the Temp1 registers.
	int16_t AvgTemp2_Dg; 	// AvgTemp1 value. The register report a 4-sample filtered average of the Temp2 registers.
	int16_t AvgTemp3_Dg; 	// AvgTemp1 value. The register report a 4-sample filtered average of the Temp3 registers.
	int16_t AvgTemp4_Dg; 	// AvgTemp1 value. The register report a 4-sample filtered average of the Temp4 registers.
	uint16_t RepCap_Reg; 	// RepCap Register. Reported Capacity is a filtered version of the AvCap register that prevents large jumps.
	uint16_t RepSOC_Reg; 	// RepSOC Register. RepSOC is a filtered version of the AvSOC register that prevents large jumps.
	uint16_t Age_Reg; 		// Age Register. The Age register contains a calculated percentage value of the application’s present cell capacity.
	uint16_t Cycles_Reg; 	// Cycles Register. The Cycles register maintains a total count of the number of charge/discharge cycles of the cell that have occurred.
	uint16_t RCell_Reg; 	// RCell Register. The register displays the calculated internal resistance of the cell or the average internal resistance.
	uint16_t VRipple_Reg; 	// VRipple Register. The register holds the slow average RMS value of the VCell register reading variation compared to the AvgVCell	register.
	uint8_t LeakCurr_Reg; 	// Leakage Current Register. Leakage current is an unsigned 8-bit result of leakage current from self-discharge in a cell.
	uint16_t RepCap_mAh; 	// RepCap value. Reported Capacity is a filtered version of the AvCap register that prevents large jumps.
	uint16_t RepSOC_Per; 	// RepSOC value. RepSOC is a filtered version of the AvSOC register that prevents large jumps.
	uint16_t Age_Dg; 		// Age value. The Age register contains a calculated percentage value of the application’s present cell capacity.
	uint16_t Cycles_Cn; 	// Cycles value. The Cycles register maintains a total count of the number of charge/discharge cycles of the cell that have occurred.
	uint16_t RCell_mOhm; 	// RCell value. The register displays the calculated internal resistance of the cell or the average internal resistance.
	uint16_t VRipple_mV; 	// VRipple value. The register holds the slow average RMS value of the VCell register reading variation compared to the AvgVCell	register.
	uint16_t LeakCurr_mA;	// Leakage Current value. Leakage current is an unsigned 8-bit result of leakage current from self-discharge in a cell.

} MAX17320_BranchData;

ErrorStatus I2C_Read_MAX17320(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint32_t reg_addr, uint16_t *data);

ErrorStatus I2C_Write_MAX17320(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr, uint32_t reg_addr, uint16_t data);

ErrorStatus MAX17320_Read_StatusReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct);

ErrorStatus MAX17320_Read_nBattStatusReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct);

ErrorStatus MAX17320_Read_nFaultLogReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct);

ErrorStatus MAX17320_Read_ProtStatusReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct);

ErrorStatus MAX17320_Read_HistoryProtStatusReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct);

ErrorStatus MAX17320_Read_Balancing (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct, uint8_t Battery_number);

ErrorStatus MAX17320_Clear_HistoryProtStatusReg (I2C_TypeDef *I2Cx);

ErrorStatus MAX17320_Read_HProtCfg2Reg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct);

ErrorStatus MAX17320_Read_VoltageReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct);

ErrorStatus MAX17320_Read_CurrentReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct);

ErrorStatus MAX17320_Read_TempReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct);

ErrorStatus MAX17320_Read_PwrReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct);

ErrorStatus MAX17320_Read_ChgParamReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct);

ErrorStatus MAX17320_Read_GaugeOutReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct);

ErrorStatus MAX17320_Read_nGaugeOutReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct);

ErrorStatus MAX17320_Read_nXTableReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct);

ErrorStatus MAX17320_Read_nOCVTableReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct);

ErrorStatus MAX17320_Read_nQRTableReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct);

ErrorStatus MAX17320_Read_nGaugeInReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct);

ErrorStatus MAX17320_Read_nGaugeConfigReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct);

ErrorStatus MAX17320_Read_ConfigReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct);

ErrorStatus MAX17320_Read_GaugeAddReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct);

ErrorStatus MAX17320_Read_nIDReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct);

ErrorStatus MAX17320_Read_AtRateReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct);

ErrorStatus MAX17320_Read_nAlertReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct);

ErrorStatus MAX17320_Read_AlertReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct);

ErrorStatus MAX17320_Read_nProtectReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct);

ErrorStatus MAX17320_Read_nTempProtReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct);

ErrorStatus MAX17320_Read_nFaultTimerReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct);

ErrorStatus MAX17320_Read_nProtCfgReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct);

ErrorStatus MAX17320_Read_DoubleReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct);

ErrorStatus MAX17320_Read_nDoubleReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct);

ErrorStatus MAX17320_Write_FullReset (I2C_TypeDef *I2Cx);

ErrorStatus MAX17320_Write_FuelGaugeReset (I2C_TypeDef *I2Cx);

ErrorStatus MAX17320_Write_ON_OFF_CHRG_FET (I2C_TypeDef *I2Cx, uint8_t Chg_key);

ErrorStatus MAX17320_Write_ON_OFF_DCHRG_FET (I2C_TypeDef *I2Cx, uint8_t Dchg_key);

ErrorStatus MAX17320_Read_Br_Data_Reg (I2C_TypeDef *I2Cx);

ErrorStatus MAX17320_Read_Br_ProtStatus_Reg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct);

ErrorStatus MAX17320_Read_Br_HistoryProtStatus_Reg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct);

ErrorStatus MAX17320_Read_Balancing_Reg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct, uint8_t Battery_number);

ErrorStatus MAX17320_Read_Br_Voltage_Reg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct, uint8_t ConfigBattBr);

ErrorStatus MAX17320_Read_Br_Voltage_mV (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct, uint8_t ConfigBattBr);

ErrorStatus MAX17320_Read_Br_Current_Reg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct);

ErrorStatus MAX17320_Read_Br_Current_mA (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct, uint8_t Rsense);

ErrorStatus MAX17320_Read_Br_Pwr_Reg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct);

ErrorStatus MAX17320_Read_Br_Pwr_mW (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct, uint8_t Rsense);

ErrorStatus MAX17320_Read_Br_Temp_Reg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct, uint8_t NumTermistors);

ErrorStatus MAX17320_Read_Br_Temp_Dg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct, uint8_t NumTermistors);

ErrorStatus MAX17320_Read_Br_GaugeOut_Reg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct);

ErrorStatus MAX17320_Read_Br_GaugeOut_mAh (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct, uint8_t Rsense);

ErrorStatus MAX17320_WriteAccmCharge (I2C_TypeDef *I2Cx, uint16_t AccmCharge, uint8_t Rsense);

#ifdef __cplusplus
}
#endif

#endif /* MAX17320_H */
