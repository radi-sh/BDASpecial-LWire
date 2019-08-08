#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "common.h"

#include "LWireSpecials.h"

#include <Windows.h>
#include <string>

#include <dshow.h>
#include "CIniFileAccess.h"
#include "DSFilterEnum.h"

static void CheckProperty(GUID set, ULONG nodeID, IKsControl * pControl)
{
	HRESULT hr;

	for (ULONG i = 0; i < 10000; i++) {
		KSP_NODE Prop = {};
		DWORD TypeSupport = 0;
		ULONG BytesReturned = 0;
		Prop.Property = { set, i, KSPROPERTY_TYPE_BASICSUPPORT };
		Prop.NodeId = nodeID;
		if (FAILED(hr = pControl->KsProperty((PKSPROPERTY)&Prop, sizeof(Prop), &TypeSupport, sizeof(TypeSupport), &BytesReturned))) {
			if (hr == E_NOTIMPL || hr == E_PROP_SET_UNSUPPORTED) {
				OutputDebug(L"Property set is not supported.\n");
				break;
			}
			else if (hr == E_PROP_ID_UNSUPPORTED) {
				//OutputDebug(L"Property ID:%ld is not supported.\n", i);
			}
			else {
				OutputDebug(L"Fail to IKsControl::KsProperty() function.\n");
				break;
			}
		}
		else {
			OutputDebug(L"Property ID:%ld: TypeSupport=%ld.\n", i, TypeSupport);
		}
	}
}

static void CheckMethod(GUID set, ULONG nodeID, IKsControl * pControl)
{
	HRESULT hr;

	for (ULONG i = 0; i < 1000; i++) {
		KSM_NODE Method = {};
		DWORD Type = 0;
		ULONG BytesReturned = 0;
		Method.Method = { set, i, KSMETHOD_TYPE_BASICSUPPORT };
		Method.NodeId = nodeID;
		if (FAILED(hr = pControl->KsMethod((PKSMETHOD)&Method, sizeof(Method), &Type, sizeof(Type), &BytesReturned))) {
			if (hr == E_NOTIMPL || hr == E_PROP_SET_UNSUPPORTED) {
				OutputDebug(L"Method is not supported.\n");
				break;
			}
			else if (hr == E_PROP_ID_UNSUPPORTED) {
				//OutputDebug(L"Method ID:%ld is not supported.\n", i);
			}
			else {
				OutputDebug(L"Fail to IKsControl::KsMethod() function.\n");
				break;
			}
		}
		else {
			OutputDebug(L"Method ID:%ld: Type=%ld.\n", i, Type);
		}
	}
}

FILE *g_fpLog = NULL;

HMODULE hMySelf;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
#ifdef _DEBUG
		::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
		// モジュールハンドル保存
		hMySelf = hModule;
		break;

	case DLL_PROCESS_DETACH:
		// デバッグログファイルのクローズ
		CloseDebugLog();
		break;
	}
	return TRUE;
}

__declspec(dllexport) IBdaSpecials * CreateBdaSpecials(CComPtr<IBaseFilter> pTunerDevice)
{
	return NULL;
}

__declspec(dllexport) IBdaSpecials* CreateBdaSpecials2(CComPtr<IBaseFilter> pTunerDevice, CComPtr<IBaseFilter> pCaptureDevice, const WCHAR* szTunerDisplayName, const WCHAR* szTunerFriendlyName, const WCHAR* szCaptureDisplayName, const WCHAR* szCaptureFriendlyName)
{
	return new CLWireSpecials(hMySelf, pTunerDevice, pCaptureDevice);
}

__declspec(dllexport) HRESULT CheckAndInitTuner(IBaseFilter *pTunerDevice, const WCHAR *szDisplayName, const WCHAR *szFriendlyName, const WCHAR *szIniFilePath)
{
	CIniFileAccess IniFileAccess(szIniFilePath);

	// DebugLogを記録するかどうか
	if (IniFileAccess.ReadKeyB(L"LWire", L"DebugLog", FALSE)) {
		// DebugLogのファイル名取得
		SetDebugLog(common::GetModuleName(hMySelf) + L"log");
	}

	return S_OK;
}

CLWireSpecials::CLWireSpecials(HMODULE hMySelf, CComPtr<IBaseFilter> pTunerDevice, CComPtr<IBaseFilter> pCaptureDevice)
	: m_hMySelf(hMySelf),
	  m_pTunerDevice(pTunerDevice),
	  m_pCaptureDevice(pCaptureDevice)
{
	::InitializeCriticalSection(&m_CriticalSection);

	return;
}

CLWireSpecials::~CLWireSpecials()
{
	m_hMySelf = NULL;

	::DeleteCriticalSection(&m_CriticalSection);

	return;
}

const HRESULT CLWireSpecials::InitializeHook(void)
{
	if (m_pTunerDevice == NULL || m_pCaptureDevice == NULL) {
		return E_POINTER;
	}

	/*
	HRESULT hr;

	// Tuner の IKsControl
	{
		CComQIPtr<IKsControl> pControlTunerFilter(m_pTunerDevice);
		if (!pControlTunerFilter) {
			OutputDebug(L"Fail to get IKsControl of tuner filter.\n");
			return E_NOINTERFACE;
		}
		m_pControlTunerFilter = pControlTunerFilter;
	}

	// チューナーデバイス input pin の IKsControl / output pin の IKsControl
	{
		CDSEnumPins DSEnumPins(m_pTunerDevice);
		CComPtr<IPin> pPin;
		PIN_DIRECTION dir;
		do {
			if (FAILED(hr = DSEnumPins.getNextPin(&pPin, &dir))) {
				break;
			}
			CComQIPtr<IKsControl> pControlTunerPin(pPin);
			switch (dir) {
			case PIN_DIRECTION::PINDIR_INPUT:
				if (!m_pControlTunerInputPin) {
					m_pControlTunerInputPin = pControlTunerPin;
				}
				break;
			case PIN_DIRECTION::PINDIR_OUTPUT:
				if (!m_pControlTunerOutputPin) {
					m_pControlTunerOutputPin = pControlTunerPin;
				}
				break;
			}
		} while (!m_pControlTunerInputPin || !m_pControlTunerOutputPin);
	}

	if (!m_pControlTunerInputPin) {
		OutputDebug(L"Fail to get IKsControl of tuner input pin.\n");
		return E_NOINTERFACE;
	}
	if (!m_pControlTunerOutputPin) {
		OutputDebug(L"Fail to get IKsControl of tuner output pin.\n");
		return E_NOINTERFACE;
	}
*/

	// Capture の IKsControl
	{
		CComQIPtr<IKsControl> pControlCaptureFilter(m_pCaptureDevice);
		if (!pControlCaptureFilter) {
			OutputDebug(L"Fail to get IKsControl of capture filter.\n");
			return E_NOINTERFACE;
		}
		m_pControlCaptureFilter = pControlCaptureFilter;
	}

/*
	// IBDA_DeviceControl
	{
		CComQIPtr<IBDA_DeviceControl> pDeviceControl(m_pTunerDevice);
		if (!pDeviceControl) {
			OutputDebug(L"Can not get IBDA_DeviceControl.\n");
			return E_NOINTERFACE;
		}
		m_pIBDA_DeviceControl = pDeviceControl;
	}

	// Control Node 取得
	{
		CDSEnumNodes DSEnumNodes(m_pTunerDevice);

		// IBDA_FrequencyFilter / IBDA_SignalStatistics / IBDA_LNBInfo / IBDA_DiseqCommand
		{
			ULONG NodeTypeTuner = DSEnumNodes.findControlNode(__uuidof(IBDA_FrequencyFilter));
			if (NodeTypeTuner != -1) {
				OutputDebug(L"Found RF Tuner Node. NodeType=%ld.\n", NodeTypeTuner);
				CComPtr<IUnknown> pControlNodeTuner;
				if (FAILED(hr = DSEnumNodes.getControlNode(NodeTypeTuner, &pControlNodeTuner))) {
					OutputDebug(L"Fail to get control node.\n");
				}
				else {
					CComQIPtr<IBDA_FrequencyFilter> pIBDA_FrequencyFilter(pControlNodeTuner);
					if (pIBDA_FrequencyFilter) {
						m_pIBDA_FrequencyFilter = pIBDA_FrequencyFilter;
						OutputDebug(L"  Found IBDA_FrequencyFilter.\n");
					}
					CComQIPtr<IBDA_SignalStatistics> pIBDA_SignalStatistics(pControlNodeTuner);
					if (pIBDA_SignalStatistics) {
						m_pIBDA_SignalStatisticsTuner = pIBDA_SignalStatistics;
						OutputDebug(L"  Found IBDA_SignalStatistics.\n");
					}
					CComQIPtr<IBDA_LNBInfo> pIBDA_LNBInfo(pControlNodeTuner);
					if (pIBDA_LNBInfo) {
						m_pIBDA_LNBInfo = pIBDA_LNBInfo;
						OutputDebug(L"  Found IBDA_LNBInfo.\n");
					}
					CComQIPtr<IBDA_DiseqCommand> pIBDA_DiseqCommand(pControlNodeTuner);
					if (pIBDA_DiseqCommand) {
						m_pIBDA_DiseqCommand = pIBDA_DiseqCommand;
						OutputDebug(L"  Found IBDA_DiseqCommand.\n");
					}
				}
			}
		}

		// IBDA_DigitalDemodulator / IBDA_SignalStatistics
		{
			ULONG NodeTypeDemod = DSEnumNodes.findControlNode(__uuidof(IBDA_DigitalDemodulator));
			if (NodeTypeDemod != -1) {
				OutputDebug(L"Found Demodulator Node. NodeType=%ld.\n", NodeTypeDemod);
				CComPtr<IUnknown> pControlNodeDemod;
				if (FAILED(hr = DSEnumNodes.getControlNode(NodeTypeDemod, &pControlNodeDemod))) {
					OutputDebug(L"Fail to get control node.\n");
				}
				else {
					CComQIPtr<IBDA_DigitalDemodulator> pIBDA_DigitalDemodulator(pControlNodeDemod);
					if (pIBDA_DigitalDemodulator) {
						m_pIBDA_DigitalDemodulator = pIBDA_DigitalDemodulator;
						OutputDebug(L"  Found IBDA_DigitalDemodulator.\n");
					}
					CComQIPtr<IBDA_SignalStatistics> pIBDA_SignalStatistics(pControlNodeDemod);
					if (pIBDA_SignalStatistics) {
						m_pIBDA_SignalStatisticsDemod = pIBDA_SignalStatistics;
						OutputDebug(L"  Found IBDA_SignalStatistics.\n");
					}
				}
			}
		}
	}
*/

/*
	OutputDebug(L"Checking Tuner Filter\n");

	// KSMETHODSETID_BdaChangeSync がサポートされているか
	OutputDebug(L"-> KSMETHODSETID_BdaChangeSync:\n");
	CheckMethod(KSMETHODSETID_BdaChangeSync, KSNODEID_RF_TUNER, m_pControlTunerFilter);

	OutputDebug(L"Checking Tuner Input Pin\n");

	// KSPROPSETID_BdaTunerExtensionProperties がサポートされているか
	OutputDebug(L"-> KSPROPSETID_BdaTunerExtensionProperties:\n");
	CheckProperty(KSPROPSETID_BdaTunerExtensionProperties, KSNODEID_RF_TUNER, m_pControlTunerInputPin);

	// KSPROPSETID_BdaFrequencyFilter がサポートされているか
	OutputDebug(L"-> KSPROPSETID_BdaFrequencyFilter:\n");
	CheckProperty(KSPROPSETID_BdaFrequencyFilter, KSNODEID_RF_TUNER, m_pControlTunerInputPin);

	// KSPROPSETID_BdaSignalStats がサポートされているか
	OutputDebug(L"-> KSPROPSETID_BdaSignalStats:\n");
	CheckProperty(KSPROPSETID_BdaSignalStats, KSNODEID_RF_TUNER, m_pControlTunerInputPin);

	// KSPROPSETID_BdaLNBInfo がサポートされているか
	OutputDebug(L"-> KSPROPSETID_BdaLNBInfo:\n");
	CheckProperty(KSPROPSETID_BdaLNBInfo, KSNODEID_RF_TUNER, m_pControlTunerInputPin);

	// KSPROPSETID_BdaDiseqCommand がサポートされているか
	OutputDebug(L"-> KSPROPSETID_BdaDiseqCommand:\n");
	CheckProperty(KSPROPSETID_BdaDiseqCommand, KSNODEID_RF_TUNER, m_pControlTunerInputPin);

	OutputDebug(L"Checking Tuner Output Pin\n");

	// KSPROPSETID_BdaSignalStats がサポートされているか
	OutputDebug(L"-> KSPROPSETID_BdaSignalStats:\n");
	CheckProperty(KSPROPSETID_BdaSignalStats, KSNODEID_DEMODULATOR, m_pControlTunerOutputPin);

	// KSPROPSETID_BdaDigitalDemodulator がサポートされているか
	OutputDebug(L"-> KSPROPSETID_BdaDigitalDemodulator:\n");
	CheckProperty(KSPROPSETID_BdaDigitalDemodulator, KSNODEID_DEMODULATOR, m_pControlTunerOutputPin);
*/

	OutputDebug(L"Checking Capture Filter\n");
	// KSPROPSETID_LWireExtensionProperties がサポートされているか
	OutputDebug(L"-> KSPROPSETID_LWireExtensionProperties:\n");
	CheckProperty(KSPROPSETID_LWireExtensionProperties, 0UL, m_pControlCaptureFilter);

	return S_OK;
}

const HRESULT CLWireSpecials::PreLockChannel(TuningParam *pTuningParam)
{
	static long sr_cache = -1;

	if (m_pControlCaptureFilter == NULL) {
		return E_POINTER;
	}

	HRESULT hr;

	OutputDebug(L"PreLockChannel: Start.\n");

/*
	// Si2168 connect tuners i2c
	{
		i2c_cmd i2c = { I2C_ADDR_SI2168, 3, 0, { 0xc0, 0x0d, 0x01, }, {} };
		hr = send_i2c(m_pControlCaptureFilter, &i2c);
	}
	// Si2157 ini
	{
		i2c_cmd i2c = { I2C_ADDR_SI2157, 3, 1, { 0xc0, 0x00, 0x0d, }, {} };
		hr = send_i2c(m_pControlCaptureFilter, &i2c);
	}
*/
	// Si2168 Prop 0x1102
	if (sr_cache != pTuningParam->Modulation.SymbolRate) {
		i2c_cmd i2c = { I2C_ADDR_SI2168, 6, 4, { 0x14, 0x00, 0x02, 0x11, (BYTE)(pTuningParam->Modulation.SymbolRate & 0xff), (BYTE)((pTuningParam->Modulation.SymbolRate >> 8) & 0xff) }, {} };
		hr = send_i2c(m_pControlCaptureFilter, &i2c);
	}
/*
	// Si2168 Prop 0x011a
	{
		BYTE std;
		BYTE bw;

		i2c_cmd i2c = { I2C_ADDR_SI2168, 6, 4, { 0x14, 0x00, 0x0a, 0x10, 0x00, 0x02 }, {} };
		BYTE std = SI2168_SIGNAL_STANDARD_DVBC;
		BYTE bw = SI2168_BAND_WIDTH_6;

		i2c.WBuf[4] = std | bw;
		hr = send_i2c(m_pControlCaptureFilter, &i2c);
	}
	// Si2168 restart
	{
		i2c_cmd i2c = { I2C_ADDR_SI2168, 1, 1, { 0x85, }, {} };
		hr = send_i2c(m_pControlCaptureFilter, &i2c);
	}
	// Si2168 disconnect tuners i2c
	{
		i2c_cmd i2c = { I2C_ADDR_SI2168, 3, 0, { 0xc0, 0x0d, 0x00, }, {} };
		hr = send_i2c(m_pControlCaptureFilter, &i2c);
	}
*/

	OutputDebug(L"PreLockChannel: Complete.\n");

	return S_OK;
}

void CLWireSpecials::Release(void)
{
	delete this;
}
