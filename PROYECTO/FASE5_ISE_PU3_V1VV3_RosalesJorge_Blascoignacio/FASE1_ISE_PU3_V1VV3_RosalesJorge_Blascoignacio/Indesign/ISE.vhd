LIBRARY IEEE;
USE IEEE.std_logic_1164.all;

ENTITY SCHEMATIC1 IS 

END SCHEMATIC1;



ARCHITECTURE STRUCTURE OF SCHEMATIC1 IS

-- COMPONENTS

COMPONENT INA122
	PORT (
	INP : INOUT std_logic;
	INN : INOUT std_logic;
	VDD : INOUT std_logic;
	VEE : INOUT std_logic;
	\OUT\ : INOUT std_logic;
	RG1 : INOUT std_logic;
	RG2 : INOUT std_logic;
	REF : INOUT std_logic
	); END COMPONENT;

COMPONENT \{Rtd}\
	PORT (
	\2\ : INOUT std_logic;
	\1\ : INOUT std_logic
	); END COMPONENT;

COMPONENT TLV75533P_TRANS
	PORT (
	\IN\ : IN std_logic;
	GND : IN std_logic;
	EN : IN std_logic;
	NC : INOUT std_logic;
	\OUT\ : IN std_logic
	); END COMPONENT;

COMPONENT \0.1u\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic
	); END COMPONENT;

-- SIGNALS

SIGNAL \0\ : std_logic;
	ATTRIBUTE \BiasValue Voltage\:string;
	ATTRIBUTE \BiasValue Voltage\ of \0\ : signal is "0V";
	ATTRIBUTE \BiasValue Current\:string;
	ATTRIBUTE \BiasValue Current\ of \0\ : signal is "-5.685uA";
SIGNAL \0V4\ : std_logic;
	ATTRIBUTE \BiasValue Voltage\ of \0V4\ : signal is "1.491mV";
	ATTRIBUTE \BiasValue Current\ of \0V4\ : signal is "-42.37nA";
SIGNAL \2V5\ : std_logic;
	ATTRIBUTE \BiasValue Voltage\ of \2V5\ : signal is "9.315mV";
	ATTRIBUTE \BiasValue Current\ of \2V5\ : signal is "4.804uA";
SIGNAL \3V3\ : std_logic;
	ATTRIBUTE \BiasValue Voltage\ of \3V3\ : signal is "12.36mV";
	ATTRIBUTE \BiasValue Current\ of \3V3\ : signal is "-224.7mA";
SIGNAL \5V\ : std_logic;
	ATTRIBUTE \BiasValue Voltage\ of \5V\ : signal is "5.000V";
	ATTRIBUTE \BiasValue Current\ of \5V\ : signal is "-224.2mA";
SIGNAL N05834 : std_logic;
	ATTRIBUTE \BiasValue Voltage\ of N05834 : signal is "5.729mV";
	ATTRIBUTE \BiasValue Current\ of N05834 : signal is "14.24nA";
SIGNAL N05894 : std_logic;
	ATTRIBUTE \BiasValue Voltage\ of N05894 : signal is "4.489mV";
	ATTRIBUTE \BiasValue Current\ of N05894 : signal is "4.814uA";
SIGNAL N05980 : std_logic;
	ATTRIBUTE \BiasValue Voltage\ of N05980 : signal is "6.048mV";
	ATTRIBUTE \BiasValue Current\ of N05980 : signal is "-14.24nA";
SIGNAL N06018 : std_logic;
	ATTRIBUTE \BiasValue Voltage\ of N06018 : signal is "45.25uV";
	ATTRIBUTE \BiasValue Current\ of N06018 : signal is "9.628uA";
SIGNAL N06106 : std_logic;
	ATTRIBUTE \BiasValue Voltage\ of N06106 : signal is "67.88uV";
	ATTRIBUTE \BiasValue Current\ of N06106 : signal is "4.814uA";
SIGNAL N09528 : std_logic;
SIGNAL VN : std_logic;
	ATTRIBUTE \BiasValue Voltage\ of VN : signal is "4.511mV";
	ATTRIBUTE \BiasValue Current\ of VN : signal is "-9.950nA";
SIGNAL VO : std_logic;
	ATTRIBUTE \BiasValue Voltage\ of VO : signal is "6.182mV";
	ATTRIBUTE \BiasValue Current\ of VO : signal is "-61.82nA";
SIGNAL VP : std_logic;
	ATTRIBUTE \BiasValue Voltage\ of VP : signal is "4.511mV";
	ATTRIBUTE \BiasValue Current\ of VP : signal is "-9.950nA";

-- INSTANCE ATTRIBUTES

ATTRIBUTE \BIASVALUE POWER\:string;
ATTRIBUTE \BIASVALUE POWER\ of U1 : label is "-2.778mW";
ATTRIBUTE PSPICETEMPLATE:string;
ATTRIBUTE PSPICETEMPLATE of U1 : label is "X^@REFDES %INP %INN %REF %VDD %VEE %RG1 %RG2 %OUT @MODEL";
ATTRIBUTE SLOPE:string;
ATTRIBUTE SLOPE of RTD : label is "RSMAX";
ATTRIBUTE TC2:string;
ATTRIBUTE TC2 of RTD : label is "0";
ATTRIBUTE \BIASVALUE POWER\ of RTD : label is "21.39nW";
ATTRIBUTE TC1:string;
ATTRIBUTE TC1 of RTD : label is "0";
ATTRIBUTE MAX_TEMP:string;
ATTRIBUTE MAX_TEMP of RTD : label is "RTMAX";
ATTRIBUTE DIST:string;
ATTRIBUTE DIST of RTD : label is "FLAT";
ATTRIBUTE VOLTAGE:string;
ATTRIBUTE VOLTAGE of RTD : label is "RVMAX";
ATTRIBUTE POWER:string;
ATTRIBUTE POWER of RTD : label is "RMAX";
ATTRIBUTE PSPICETEMPLATE of RTD : label is "R^@REFDES %1 %2 ?TOLERANCE|R^@REFDES| @VALUE TC=@TC1,@TC2 ?TOLERANCE|\n.model R^@REFDES RES R=1 DEV=@TOLERANCE% TC1=@TC1 TC2=@TC2|";
ATTRIBUTE PARTNUMBER:string;
ATTRIBUTE PARTNUMBER of U2 : label is "TLV75733PDBVR";
ATTRIBUTE \BIASVALUE POWER\ of U2 : label is "-1.118W";
ATTRIBUTE DEVICESINVIEW:string;
ATTRIBUTE DEVICESINVIEW of U2 : label is "1";
ATTRIBUTE DEVICENUMBERING:string;
ATTRIBUTE DEVICENUMBERING of U2 : label is "Alpha";
ATTRIBUTE DEVICEDESIGNATOR:string;
ATTRIBUTE DEVICEDESIGNATOR of U2 : label is "A";
ATTRIBUTE PACKAGENAME:string;
ATTRIBUTE PACKAGENAME of U2 : label is "TLV75733PDBVR";
ATTRIBUTE MANUFACTURER:string;
ATTRIBUTE MANUFACTURER of U2 : label is "Texas Instruments";
ATTRIBUTE DEVICESINPACKAGE:string;
ATTRIBUTE DEVICESINPACKAGE of U2 : label is "1";
ATTRIBUTE ISHETEROGENEOUS:string;
ATTRIBUTE ISHETEROGENEOUS of U2 : label is "False";
ATTRIBUTE \< VALUE >\:string;
ATTRIBUTE \< VALUE >\ of U2 : label is "TLV75733PDBVR";
ATTRIBUTE ORIGINALSYMBOLORIGIN:string;
ATTRIBUTE ORIGINALSYMBOLORIGIN of U2 : label is "(-50000,50000)";
ATTRIBUTE PSPICETEMPLATE of U2 : label is "X^@REFDES %EN %GND %IN %OUT %NC @MODEL";
ATTRIBUTE SLOPE of R1 : label is "RSMAX";
ATTRIBUTE TC2 of R1 : label is "0";
ATTRIBUTE \BIASVALUE POWER\ of R1 : label is "23.08nW";
ATTRIBUTE TC1 of R1 : label is "0";
ATTRIBUTE MAX_TEMP of R1 : label is "RTMAX";
ATTRIBUTE DIST of R1 : label is "FLAT";
ATTRIBUTE VOLTAGE of R1 : label is "RVMAX";
ATTRIBUTE POWER of R1 : label is "RMAX";
ATTRIBUTE PSPICETEMPLATE of R1 : label is "R^@REFDES %1 %2 ?TOLERANCE|R^@REFDES| @VALUE TC=@TC1,@TC2 ?TOLERANCE|\n.model R^@REFDES RES R=1 DEV=@TOLERANCE% TC1=@TC1 TC2=@TC2|";
ATTRIBUTE SLOPE of R2 : label is "RSMAX";
ATTRIBUTE TC2 of R2 : label is "0";
ATTRIBUTE \BIASVALUE POWER\ of R2 : label is "23.08nW";
ATTRIBUTE TC1 of R2 : label is "0";
ATTRIBUTE MAX_TEMP of R2 : label is "RTMAX";
ATTRIBUTE DIST of R2 : label is "FLAT";
ATTRIBUTE VOLTAGE of R2 : label is "RVMAX";
ATTRIBUTE POWER of R2 : label is "RMAX";
ATTRIBUTE PSPICETEMPLATE of R2 : label is "R^@REFDES %1 %2 ?TOLERANCE|R^@REFDES| @VALUE TC=@TC1,@TC2 ?TOLERANCE|\n.model R^@REFDES RES R=1 DEV=@TOLERANCE% TC1=@TC1 TC2=@TC2|";
ATTRIBUTE SLOPE of R3 : label is "RSMAX";
ATTRIBUTE TC2 of R3 : label is "0";
ATTRIBUTE \BIASVALUE POWER\ of R3 : label is "21.39nW";
ATTRIBUTE TC1 of R3 : label is "0";
ATTRIBUTE MAX_TEMP of R3 : label is "RTMAX";
ATTRIBUTE DIST of R3 : label is "FLAT";
ATTRIBUTE VOLTAGE of R3 : label is "RVMAX";
ATTRIBUTE POWER of R3 : label is "RMAX";
ATTRIBUTE PSPICETEMPLATE of R3 : label is "R^@REFDES %1 %2 ?TOLERANCE|R^@REFDES| @VALUE TC=@TC1,@TC2 ?TOLERANCE|\n.model R^@REFDES RES R=1 DEV=@TOLERANCE% TC1=@TC1 TC2=@TC2|";
ATTRIBUTE SLOPE of R4 : label is "RSMAX";
ATTRIBUTE TC2 of R4 : label is "0";
ATTRIBUTE \BIASVALUE POWER\ of R4 : label is "686.4nW";
ATTRIBUTE TC1 of R4 : label is "0";
ATTRIBUTE MAX_TEMP of R4 : label is "RTMAX";
ATTRIBUTE DIST of R4 : label is "FLAT";
ATTRIBUTE VOLTAGE of R4 : label is "RVMAX";
ATTRIBUTE POWER of R4 : label is "RMAX";
ATTRIBUTE PSPICETEMPLATE of R4 : label is "R^@REFDES %1 %2 ?TOLERANCE|R^@REFDES| @VALUE TC=@TC1,@TC2 ?TOLERANCE|\n.model R^@REFDES RES R=1 DEV=@TOLERANCE% TC1=@TC1 TC2=@TC2|";
ATTRIBUTE SLOPE of R5 : label is "RSMAX";
ATTRIBUTE TC2 of R5 : label is "0";
ATTRIBUTE \BIASVALUE POWER\ of R5 : label is "1.433uW";
ATTRIBUTE TC1 of R5 : label is "0";
ATTRIBUTE MAX_TEMP of R5 : label is "RTMAX";
ATTRIBUTE DIST of R5 : label is "FLAT";
ATTRIBUTE VOLTAGE of R5 : label is "RVMAX";
ATTRIBUTE POWER of R5 : label is "RMAX";
ATTRIBUTE PSPICETEMPLATE of R5 : label is "R^@REFDES %1 %2 ?TOLERANCE|R^@REFDES| @VALUE TC=@TC1,@TC2 ?TOLERANCE|\n.model R^@REFDES RES R=1 DEV=@TOLERANCE% TC1=@TC1 TC2=@TC2|";
ATTRIBUTE SLOPE of R6 : label is "RSMAX";
ATTRIBUTE TC2 of R6 : label is "0";
ATTRIBUTE \BIASVALUE POWER\ of R6 : label is "3.601uW";
ATTRIBUTE TC1 of R6 : label is "0";
ATTRIBUTE MAX_TEMP of R6 : label is "RTMAX";
ATTRIBUTE DIST of R6 : label is "FLAT";
ATTRIBUTE VOLTAGE of R6 : label is "RVMAX";
ATTRIBUTE POWER of R6 : label is "RMAX";
ATTRIBUTE PSPICETEMPLATE of R6 : label is "R^@REFDES %1 %2 ?TOLERANCE|R^@REFDES| @VALUE TC=@TC1,@TC2 ?TOLERANCE|\n.model R^@REFDES RES R=1 DEV=@TOLERANCE% TC1=@TC1 TC2=@TC2|";
ATTRIBUTE SLOPE of Cd1 : label is "CSMAX";
ATTRIBUTE KNEE:string;
ATTRIBUTE KNEE of Cd1 : label is "CBMAX";
ATTRIBUTE TC2 of Cd1 : label is "0";
ATTRIBUTE CURRENT:string;
ATTRIBUTE CURRENT of Cd1 : label is "CIMAX";
ATTRIBUTE VC1:string;
ATTRIBUTE VC1 of Cd1 : label is "0";
ATTRIBUTE VC2:string;
ATTRIBUTE VC2 of Cd1 : label is "0";
ATTRIBUTE MAX_TEMP of Cd1 : label is "CTMAX";
ATTRIBUTE DIST of Cd1 : label is "FLAT";
ATTRIBUTE TC1 of Cd1 : label is "0";
ATTRIBUTE VOLTAGE of Cd1 : label is "CMAX";
ATTRIBUTE PSPICETEMPLATE of Cd1 : label is "C^@REFDES %1 %2 ?TOLERANCE|C^@REFDES| @VALUE ?IC/IC=@IC/ TC=@TC1,@TC2 ?TOLERANCE|\n.model C^@REFDES CAP C=1 DEV=@TOLERANCE% TC1=@TC1 TC2=@TC2 VC1=@VC1 VC2=@VC2|";
ATTRIBUTE SLOPE of Cout : label is "CSMAX";
ATTRIBUTE KNEE of Cout : label is "CBMAX";
ATTRIBUTE TC2 of Cout : label is "0";
ATTRIBUTE CURRENT of Cout : label is "CIMAX";
ATTRIBUTE VC1 of Cout : label is "0";
ATTRIBUTE VC2 of Cout : label is "0";
ATTRIBUTE MAX_TEMP of Cout : label is "CTMAX";
ATTRIBUTE DIST of Cout : label is "FLAT";
ATTRIBUTE TC1 of Cout : label is "0";
ATTRIBUTE VOLTAGE of Cout : label is "CMAX";
ATTRIBUTE PSPICETEMPLATE of Cout : label is "C^@REFDES %1 %2 ?TOLERANCE|C^@REFDES| @VALUE ?IC/IC=@IC/ TC=@TC1,@TC2 ?TOLERANCE|\n.model C^@REFDES CAP C=1 DEV=@TOLERANCE% TC1=@TC1 TC2=@TC2 VC1=@VC1 VC2=@VC2|";
ATTRIBUTE SLOPE of Rh1 : label is "RSMAX";
ATTRIBUTE TC2 of Rh1 : label is "0";
ATTRIBUTE \BIASVALUE POWER\ of Rh1 : label is "108.9pW";
ATTRIBUTE TC1 of Rh1 : label is "0";
ATTRIBUTE MAX_TEMP of Rh1 : label is "RTMAX";
ATTRIBUTE DIST of Rh1 : label is "FLAT";
ATTRIBUTE VOLTAGE of Rh1 : label is "RVMAX";
ATTRIBUTE POWER of Rh1 : label is "RMAX";
ATTRIBUTE PSPICETEMPLATE of Rh1 : label is "R^@REFDES %1 %2 ?TOLERANCE|R^@REFDES| @VALUE TC=@TC1,@TC2 ?TOLERANCE|\n.model R^@REFDES RES R=1 DEV=@TOLERANCE% TC1=@TC1 TC2=@TC2|";
ATTRIBUTE SLOPE of Rh2 : label is "RSMAX";
ATTRIBUTE TC2 of Rh2 : label is "0";
ATTRIBUTE \BIASVALUE POWER\ of Rh2 : label is "108.9pW";
ATTRIBUTE TC1 of Rh2 : label is "0";
ATTRIBUTE MAX_TEMP of Rh2 : label is "RTMAX";
ATTRIBUTE DIST of Rh2 : label is "FLAT";
ATTRIBUTE VOLTAGE of Rh2 : label is "RVMAX";
ATTRIBUTE POWER of Rh2 : label is "RMAX";
ATTRIBUTE PSPICETEMPLATE of Rh2 : label is "R^@REFDES %1 %2 ?TOLERANCE|R^@REFDES| @VALUE TC=@TC1,@TC2 ?TOLERANCE|\n.model R^@REFDES RES R=1 DEV=@TOLERANCE% TC1=@TC1 TC2=@TC2|";
ATTRIBUTE SLOPE of Rh3 : label is "RSMAX";
ATTRIBUTE TC2 of Rh3 : label is "0";
ATTRIBUTE \BIASVALUE POWER\ of Rh3 : label is "435.7pW";
ATTRIBUTE TC1 of Rh3 : label is "0";
ATTRIBUTE MAX_TEMP of Rh3 : label is "RTMAX";
ATTRIBUTE DIST of Rh3 : label is "FLAT";
ATTRIBUTE VOLTAGE of Rh3 : label is "RVMAX";
ATTRIBUTE POWER of Rh3 : label is "RMAX";
ATTRIBUTE PSPICETEMPLATE of Rh3 : label is "R^@REFDES %1 %2 ?TOLERANCE|R^@REFDES| @VALUE TC=@TC1,@TC2 ?TOLERANCE|\n.model R^@REFDES RES R=1 DEV=@TOLERANCE% TC1=@TC1 TC2=@TC2|";
ATTRIBUTE SLOPE of Cin : label is "CSMAX";
ATTRIBUTE KNEE of Cin : label is "CBMAX";
ATTRIBUTE TC2 of Cin : label is "0";
ATTRIBUTE CURRENT of Cin : label is "CIMAX";
ATTRIBUTE VC1 of Cin : label is "0";
ATTRIBUTE VC2 of Cin : label is "0";
ATTRIBUTE MAX_TEMP of Cin : label is "CTMAX";
ATTRIBUTE DIST of Cin : label is "FLAT";
ATTRIBUTE TC1 of Cin : label is "0";
ATTRIBUTE VOLTAGE of Cin : label is "CMAX";
ATTRIBUTE PSPICETEMPLATE of Cin : label is "C^@REFDES %1 %2 ?TOLERANCE|C^@REFDES| @VALUE ?IC/IC=@IC/ TC=@TC1,@TC2 ?TOLERANCE|\n.model C^@REFDES CAP C=1 DEV=@TOLERANCE% TC1=@TC1 TC2=@TC2 VC1=@VC1 VC2=@VC2|";
ATTRIBUTE SLOPE of Rg : label is "RSMAX";
ATTRIBUTE TC2 of Rg : label is "0";
ATTRIBUTE \BIASVALUE POWER\ of Rg : label is "4.543pW";
ATTRIBUTE TC1 of Rg : label is "0";
ATTRIBUTE MAX_TEMP of Rg : label is "RTMAX";
ATTRIBUTE DIST of Rg : label is "FLAT";
ATTRIBUTE VOLTAGE of Rg : label is "RVMAX";
ATTRIBUTE POWER of Rg : label is "RMAX";
ATTRIBUTE PSPICETEMPLATE of Rg : label is "R^@REFDES %1 %2 ?TOLERANCE|R^@REFDES| @VALUE TC=@TC1,@TC2 ?TOLERANCE|\n.model R^@REFDES RES R=1 DEV=@TOLERANCE% TC1=@TC1 TC2=@TC2|";
ATTRIBUTE SLOPE of Rl : label is "RSMAX";
ATTRIBUTE TC2 of Rl : label is "0";
ATTRIBUTE \BIASVALUE POWER\ of Rl : label is "382.2pW";
ATTRIBUTE TC1 of Rl : label is "0";
ATTRIBUTE MAX_TEMP of Rl : label is "RTMAX";
ATTRIBUTE DIST of Rl : label is "FLAT";
ATTRIBUTE VOLTAGE of Rl : label is "RVMAX";
ATTRIBUTE POWER of Rl : label is "RMAX";
ATTRIBUTE PSPICETEMPLATE of Rl : label is "R^@REFDES %1 %2 ?TOLERANCE|R^@REFDES| @VALUE TC=@TC1,@TC2 ?TOLERANCE|\n.model R^@REFDES RES R=1 DEV=@TOLERANCE% TC1=@TC1 TC2=@TC2|";


-- GATE INSTANCES

BEGIN
U1 : INA122	PORT MAP(
	INP => VP, 
	INN => VN, 
	VDD => \3V3\, 
	VEE => \0\, 
	\OUT\ => VO, 
	RG1 => N05834, 
	RG2 => N05980, 
	REF => \0V4\
);
RTD : \{Rtd}\	PORT MAP(
	\2\ => N05894, 
	\1\ => N06018
);
U2 : TLV75533P_TRANS	PORT MAP(
	\IN\ => \5V\, 
	GND => \0\, 
	EN => \5V\, 
	NC => \0\, 
	\OUT\ => \3V3\
);
R1 : \1k\	PORT MAP(
	\2\ => \2V5\, 
	\1\ => VP
);
R2 : \1k\	PORT MAP(
	\2\ => \2V5\, 
	\1\ => VN
);
R3 : \923\	PORT MAP(
	\2\ => VN, 
	\1\ => N06106
);
R4 : \3.24\	PORT MAP(
	\2\ => \0V4\, 
	\1\ => \0\
);
R5 : \6.49\	PORT MAP(
	\2\ => \3V3\, 
	\1\ => \2V5\
);
R6 : \17\	PORT MAP(
	\2\ => \2V5\, 
	\1\ => \0V4\
);
Cd1 : \0.1u\	PORT MAP(
	\1\ => \0\, 
	\2\ => \3V3\
);
Cout : \0.47u\	PORT MAP(
	\1\ => \0\, 
	\2\ => \3V3\
);
Rh1 : \4.7\	PORT MAP(
	\2\ => VP, 
	\1\ => N05894
);
Rh2 : \4.7\	PORT MAP(
	\2\ => N06106, 
	\1\ => N06018
);
Rh3 : \4.7\	PORT MAP(
	\2\ => N06018, 
	\1\ => \0\
);
Cin : \1u\	PORT MAP(
	\1\ => \0\, 
	\2\ => \5V\
);
Rg : \22397\	PORT MAP(
	\2\ => N05834, 
	\1\ => N05980
);
Rl : \100k\	PORT MAP(
	\2\ => VO, 
	\1\ => \0\
);
END STRUCTURE;

