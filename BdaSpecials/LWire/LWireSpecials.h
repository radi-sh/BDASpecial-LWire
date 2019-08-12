#pragma once

#include "IBdaSpecials2.h"
#include <map>

#include <Ks.h>
#include <ksproxy.h>

/*
//
// Broadcast Driver Architecture で定義されている Property set / Method set
//
// KSNODE_BDA_RF_TUNER Input pin, id:0
static constexpr GUID KSPROPSETID_BdaFrequencyFilter = { 0x71985f47, 0x1ca1, 0x11d3, { 0x9c, 0xc8, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0 } };

enum KSPROPERTY_BDA_FREQUENCY_FILTER {					// MinProperty=32
	KSPROPERTY_BDA_RF_TUNER_FREQUENCY = 0,				// get/set		MinData=4
	KSPROPERTY_BDA_RF_TUNER_POLARITY,					// get/set		MinData=4		(QPSK1/QPSK2のみ)
	KSPROPERTY_BDA_RF_TUNER_RANGE,						// get/set		MinData=4		(QPSK1/QPSK2のみ)
	KSPROPERTY_BDA_RF_TUNER_TRANSPONDER,				// not supported
	KSPROPERTY_BDA_RF_TUNER_BANDWIDTH,					// get/set		MinData=4		(COFDM/8VSBのみ)
	KSPROPERTY_BDA_RF_TUNER_FREQUENCY_MULTIPLIER,		// get/set		MinData=4
	KSPROPERTY_BDA_RF_TUNER_CAPS,						// not supported
	KSPROPERTY_BDA_RF_TUNER_SCAN_STATUS,				// not supported
	KSPROPERTY_BDA_RF_TUNER_STANDARD,					// not supported
	KSPROPERTY_BDA_RF_TUNER_STANDARD_MODE,				// not supported
};

// KSNODE_BDA_RF_TUNER Input pin, id:0 / KSNODE_BDA_*_DEMODULATOR Output pin, id:1
static constexpr GUID KSPROPSETID_BdaSignalStats = { 0x1347d106, 0xcf3a, 0x428a, { 0xa5, 0xcb, 0xac, 0xd, 0x9a, 0x2a, 0x43, 0x38 } };

enum KSPROPERTY_BDA_SIGNAL_STATS {						// MinProperty=32
	KSPROPERTY_BDA_SIGNAL_STRENGTH = 0,					// get only		MinData=4
	KSPROPERTY_BDA_SIGNAL_QUALITY,						// get only		MinData=4
	KSPROPERTY_BDA_SIGNAL_PRESENT,						// get only		MinData=4
	KSPROPERTY_BDA_SIGNAL_LOCKED,						// get only		MinData=4
	KSPROPERTY_BDA_SAMPLE_TIME,							// not supported
	KSPROPERTY_BDA_SIGNAL_LOCK_CAPS,					// not supported
	KSPROPERTY_BDA_SIGNAL_LOCK_TYPE,					// not supported
};

// KSNODE_BDA_RF_TUNER Input pin, id:0													(QPSK1/QPSK2のみ)
static constexpr GUID KSPROPSETID_BdaLNBInfo = { 0x992cf102, 0x49f9, 0x4719, { 0xa6, 0x64, 0xc4, 0xf2, 0x3e, 0x24, 0x8, 0xf4 } };

enum KSPROPERTY_BDA_LNB_INFO {							// MinProperty=32
	KSPROPERTY_BDA_LNB_LOF_LOW_BAND = 0,				// get/set		MinData=4
	KSPROPERTY_BDA_LNB_LOF_HIGH_BAND,					// get/set		MinData=4
	KSPROPERTY_BDA_LNB_SWITCH_FREQUENCY,				// get/set		MinData=4
};

// KSNODE_BDA_RF_TUNER Input pin, id:0													(QPSK1/QPSK2のみ)
static constexpr GUID KSPROPSETID_BdaDiseqCommand = { 0xf84e2ab0, 0x3c6b, 0x45e3, { 0xa0, 0xfc, 0x86, 0x69, 0xd4, 0xb8, 0x1f, 0x11 } };

enum KSPROPERTY_BDA_DISEQC_COMMAND {					// MinProperty=32
	KSPROPERTY_BDA_DISEQC_ENABLE = 0,					// get/set		MinData=4
	KSPROPERTY_BDA_DISEQC_LNB_SOURCE,					// get/set		MinData=4
	KSPROPERTY_BDA_DISEQC_USETONEBURST,					// get/set		MinData=4
	KSPROPERTY_BDA_DISEQC_REPEATS,						// get/set		MinData=4
	KSPROPERTY_BDA_DISEQC_SEND,							// get/set		MinData=16
	KSPROPERTY_BDA_DISEQC_RESPONSE,						// get only		MinData=16
};

// KSNODE_BDA_*_DEMODULATOR Output pin, id:1
static constexpr GUID KSPROPSETID_BdaDigitalDemodulator = { 0xef30f379, 0x985b, 0x4d10, { 0xb6, 0x40, 0xa7, 0x9d, 0x5e, 0x4, 0xe1, 0xe0 } };

enum KSPROPERTY_BDA_DIGITAL_DEMODULATOR {				// MinProperty=32
	KSPROPERTY_BDA_MODULATION_TYPE = 0,					// get/set		MinData=4
	KSPROPERTY_BDA_INNER_FEC_TYPE,						// get/set		MinData=4		(QPSK1/QPSK2のみ)
	KSPROPERTY_BDA_INNER_FEC_RATE,						// get/set		MinData=4		(QPSK1/QPSK2のみ)
	KSPROPERTY_BDA_OUTER_FEC_TYPE,						// not supported
	KSPROPERTY_BDA_OUTER_FEC_RATE,						// not supported
	KSPROPERTY_BDA_SYMBOL_RATE,							// get/set		MinData=4
	KSPROPERTY_BDA_SPECTRAL_INVERSION,					// get/set		MinData=4		(QPSK1/QPSK2のみ)
	KSPROPERTY_BDA_GUARD_INTERVAL,						// not supported
	KSPROPERTY_BDA_TRANSMISSION_MODE,					// not supported
	KSPROPERTY_BDA_ROLL_OFF,							// not supported
	KSPROPERTY_BDA_PILOT,								// not supported
	KSPROPERTY_BDA_SIGNALTIMEOUTS,						// not supported
	KSPROPERTY_BDA_PLP_NUMBER,							// not supported
};

// KSCATEGORY_BDA_NETWORK_TUNER Tuner filter, id:0
static constexpr GUID KSMETHODSETID_BdaChangeSync = { 0xfd0a5af3, 0xb41d, 0x11d2, { 0x9c, 0x95, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0 } };

enum KSMETHOD_BDA_CHANGE_SYNC {							// MinMethod=24
	KSMETHOD_BDA_START_CHANGES = 0,						// none				MinData=0
	KSMETHOD_BDA_CHECK_CHANGES,							// none				MinData=0
	KSMETHOD_BDA_COMMIT_CHANGES,						// none				MinData=0
	KSMETHOD_BDA_GET_CHANGE_STATE,						// read				MinData=0
};
*/

//
// 固有の Property set
//

// KSNODE_BDA_RF_TUNER Input pin, id:0													(QPSK1のみ)
static constexpr GUID KSPROPSETID_BdaTunerExtensionProperties = { 0xfaa8f3e5, 0x31d4, 0x4e41, { 0x88, 0xef, 0xd9, 0xeb, 0x71, 0x6f, 0x6e, 0xc9 } };

enum KSPROPERTY_BDA_TUNER_EXTENSION {					// MinProperty=32
	KSPROPERTY_BDA_DISEQC = 0,							// get/set		MinData=188
	KSPROPERTY_BDA_SCAN_FREQ,							// get/set		MinData=20
	KSPROPERTY_BDA_CHANNEL_CHANGE,						// set only		MinData=36
	KSPROPERTY_BDA_EFFECTIVE_FREQ,						// get only		MinData=4
};

// KSNODE_BDA_RF_TUNER Input pin, id:0													(QPSK2のみ)
static constexpr GUID KSPROPSETID_BdaTunerExtensionProperties2 = KSPROPSETID_BdaTunerExtensionProperties;

enum KSPROPERTY_BDA_TUNER_EXTENSION2 {
	KSPROPERTY_BDA_DISEQC_MESSAGE = 0,					// get only		MinData=180		MinProperty=180
	KSPROPERTY_BDA_DISEQC_INIT,							// set only		MinData=16		MinProperty=32
	KSPROPERTY_BDA_SCAN_FREQ2,							// get/set		MinData=20		MinProperty=32
	KSPROPERTY_BDA_CHANNEL_CHANGE2,						// get/set		MinData=48		MinProperty=32
	KSPROPERTY_BDA_DEMOD_INFO,							// get only		MinData=16		MinProperty=32
	KSPROPERTY_BDA_EFFECTIVE_FREQ2,						// get only		MinData=4		MinProperty=32
	KSPROPERTY_BDA_SIGNAL_STATUS,						// get only		MinData=80		MinProperty=32
	KSPROPERTY_BDA_LOCK_STATUS,							// get only		MinData=28		MinProperty=32
	KSPROPERTY_BDA_ERROR_CONTROL,						// set only		MinData=12		MinProperty=32
	KSPROPERTY_BDA_CHANNEL_INFO,						// get only		MinData=28		MinProperty=32
	KSPROPERTY_BDA_NBC_PARAMS,							// set only		MinData=8		MinProperty=32
};

// KSNODE_BDA_*_DEMODULATOR Output pin, id:1
static constexpr GUID KSPROPSETID_BdaDemodExtensionProperties = { 0x0fe4e87a, 0x8e44, 0x4e27, { 0xbe, 0x4a, 0x4f, 0x03, 0xfc, 0x7d, 0x85, 0x2b } };

enum KSPROPERTY_BDA_DEMOD_EXTENSION {					// MinProperty=32
	KSPROPERTY_BDA_TUNING_FREQ_OFFSET = 0,				// get only		MinData=4
	KSPROPERTY_BDA_VITERBI_CODE_RATE,					// get only		MinData=4
};

static constexpr GUID KSPROPSETID_LWireExtensionProperties = { 0x527dcf47, 0x1647, 0x46ff, { 0x84, 0x5c, 0xc9, 0x7d, 0x9a, 0xbf, 0x1c, 0xdc } };

// KSCATEGORY_BDA_RECEIVER_COMPONENT TS capture filter, id:0
enum KSPROPERTY_LWIRE_EXTENSION {
	KSPROPERTY_LWIRE_EXTENSION_I2C_XFER = 0,			// get/set		MinData=104		MinProperty=104
};

enum LWIRE_I2C_XFER {
	LWIRE_I2C_READ = 0,
	LWIRE_I2C_WRITE,
};

struct KSPROPERTY_LWIRE_I2C_XFER_S {
	KSPROPERTY Property;
	ULONG Mode;
	ULONG WLen;
	ULONG RLen;
	ULONG Addr;
	BYTE WBuf[32];
	BYTE RBuf[32];
};

static constexpr BYTE I2C_ADDR_SI2157 = 0xc0;
static constexpr BYTE I2C_ADDR_SI2168 = 0xc8;

static constexpr BYTE SI2157_BAND_WIDTH_6 = 0x6;
static constexpr BYTE SI2157_BAND_WIDTH_7 = 0x7;
static constexpr BYTE SI2157_BAND_WIDTH_8 = 0x8;
static constexpr BYTE SI2157_BAND_WIDTH_AUTO = 0xf;

static constexpr BYTE SI2157_SIGNAL_STANDARD_ATSC = 0x00;
static constexpr BYTE SI2157_SIGNAL_STANDARD_J83B = 0x10;
static constexpr BYTE SI2157_SIGNAL_STANDARD_DVBT = 0x20;
static constexpr BYTE SI2157_SIGNAL_STANDARD_DVBC = 0x30;
static constexpr BYTE SI2157_SIGNAL_STANDARD_AUTO = 0xf0;

static constexpr BYTE SI2168_BAND_WIDTH_2 = 0x2;
static constexpr BYTE SI2168_BAND_WIDTH_5 = 0x5;
static constexpr BYTE SI2168_BAND_WIDTH_6 = 0x6;
static constexpr BYTE SI2168_BAND_WIDTH_7 = 0x7;
static constexpr BYTE SI2168_BAND_WIDTH_8 = 0x8;
static constexpr BYTE SI2168_BAND_WIDTH_9 = 0x9;
static constexpr BYTE SI2168_BAND_WIDTH_10 = 0xa;
static constexpr BYTE SI2168_BAND_WIDTH_AUTO = 0xf;

static constexpr BYTE SI2168_SIGNAL_STANDARD_DVBT = 0x20;
static constexpr BYTE SI2168_SIGNAL_STANDARD_DVBC = 0x30;
static constexpr BYTE SI2168_SIGNAL_STANDARD_DVBT2 = 0x70;
static constexpr BYTE SI2168_SIGNAL_STANDARD_AUTO = 0xf0;

static constexpr ULONG KSNODEID_RF_TUNER = 0;
static constexpr ULONG KSNODEID_DEMODULATOR = 1;
static constexpr ULONG KSNODEID_TS_FILTER = 0;

struct i2c_cmd {
	BYTE Addr;
	ULONG WLen;
	ULONG RLen;
	BYTE WBuf[32];
	BYTE RBuf[32];
};

static const HRESULT send_i2c(IKsControl* pControlCaptureFilter, i2c_cmd* i2c)
{
	HRESULT hr = S_OK;
	ULONG BytesReturned = 0;
	if (i2c->WLen) {
		KSPROPERTY_LWIRE_I2C_XFER_S Prop = {};
		Prop.Property = { KSPROPSETID_LWireExtensionProperties, KSPROPERTY_LWIRE_EXTENSION_I2C_XFER, KSPROPERTY_TYPE_SET };
		Prop.Addr = i2c->Addr;
		Prop.Mode = LWIRE_I2C_WRITE;
		Prop.WLen = i2c->WLen;
		memcpy_s(Prop.WBuf, sizeof(Prop.WBuf), i2c->WBuf, i2c->WLen);
		if (FAILED(hr = pControlCaptureFilter->KsProperty((PKSPROPERTY)& Prop, sizeof(Prop), &Prop, sizeof(Prop), &BytesReturned))) {
			OutputDebug(L"  Fail to i2c write register. ret=0x%08lx\n", hr);
			return hr;
		}
	}
	if (i2c->RLen) {
		KSPROPERTY_LWIRE_I2C_XFER_S Prop = {};
		Prop.Property = { KSPROPSETID_LWireExtensionProperties, KSPROPERTY_LWIRE_EXTENSION_I2C_XFER, KSPROPERTY_TYPE_GET };
		Prop.Addr = i2c->Addr;
		Prop.Mode = LWIRE_I2C_READ;
		Prop.RLen = i2c->RLen;
		if (FAILED(hr = pControlCaptureFilter->KsProperty((PKSPROPERTY)& Prop, sizeof(Prop), &Prop, sizeof(Prop), &BytesReturned))) {
			OutputDebug(L"  Fail to i2c read register. ret=0x%08lx\n", hr);
			return hr;
		}
		memcpy_s(i2c->RBuf, sizeof(i2c->RBuf), Prop.RBuf, i2c->RLen);
	}
	return hr;
}

class CLWireSpecials : public IBdaSpecials2b5
{
public:
	CLWireSpecials(CComPtr<IBaseFilter> pTunerDevice, CComPtr<IBaseFilter> pCaptureDevice);
	virtual ~CLWireSpecials(void);

	const HRESULT InitializeHook(void);

	const HRESULT ReadIniFile(const WCHAR* szIniFilePath);
	const HRESULT PreLockChannel(TuningParam *pTuningParam);

	virtual void Release(void);

	static HMODULE m_hMySelf;

private:
#if 0
	CComPtr<IKsControl> m_pControlTunerFilter;							// Tuner の IKsControl
	CComPtr<IKsControl> m_pControlTunerInputPin;						// Tuner Input Pin の IKsControl
	CComPtr<IKsControl> m_pControlTunerOutputPin;						// Tuner Output Pin の IKsControl
#endif
	CComPtr<IKsControl> m_pControlCaptureFilter;						// Capture の IKsControl
#if 0
	CComPtr<IBDA_DeviceControl> m_pIBDA_DeviceControl;					// IBDA_DeviceControl (Tuner)
	CComPtr<IBDA_LNBInfo> m_pIBDA_LNBInfo;								// IBDA_LNBInfo (Tuner Input Pin, Node 0)
	CComPtr<IBDA_DigitalDemodulator> m_pIBDA_DigitalDemodulator;		// IBDA_DigitalDemodulator (Tuner Output Pin, Node 1)
	CComPtr<IBDA_FrequencyFilter> m_pIBDA_FrequencyFilter;				// IBDA_FrequencyFilter (Tuner Input Pin, Node 0)
	CComPtr<IBDA_DiseqCommand> m_pIBDA_DiseqCommand;					// IBDA_DiseqCommand (Tuner Input Pin, Node 0)
	CComPtr<IBDA_SignalStatistics> m_pIBDA_SignalStatisticsTuner;		// Tuner Node の IBDA_SignalStatistics(Tuner Input Pin, Node 0)
	CComPtr<IBDA_SignalStatistics> m_pIBDA_SignalStatisticsDemod;		// Demod Node の IBDA_SignalStatistics(Tuner Output Pin, Node 1)
#endif
	CComPtr<IBaseFilter> m_pTunerDevice;								// Tuner の IBaseFilter
	CComPtr<IBaseFilter> m_pCaptureDevice;								// Capture の IBaseFilter
	CRITICAL_SECTION m_CriticalSection;									// 排他処理用
	BOOL m_bBugT230C_ISDBC;												// T230CのDriverでISDB-CのSymbolRateがSetされないBUG対策を行う

private:

};
