#define LOG_TAG "paramctrl_attributes"

#ifndef ENABLE_MY_LOG
    #define ENABLE_MY_LOG       (1)
#endif

#include <aaa_types.h>
#include <aaa_log.h>
#include <aaa_hal_base.h>
#include <camera_custom_nvram.h>
#include <isp_tuning.h>
#include <camera_feature.h>
#include <awb_param.h>
#include <ae_param.h>
#include <af_param.h>
#include <flash_param.h>
#include <isp_tuning_cam_info.h>
#include <isp_tuning_idx.h>
#include <isp_tuning_custom.h>
#include <dynamic_ccm.h>
#include <ccm_mgr.h>
#include <dbg_isp_param.h>
#include <lsc_mgr.h>
#include "paramctrl_if.h"
#include "paramctrl.h"

using namespace android;
using namespace NSIspTuning;

#if 0
MERROR_ENUM
Paramctrl::
setOperMode(EOperMode_T const eOperMode)
{
    MY_LOG("[+setOperMode](old, new)=(%d, %d)", m_eOperMode, eOperMode);

    Mutex::Autolock lock(m_Lock);

    if  ( checkParamChange(m_eOperMode, eOperMode) )
    {
        m_eOperMode = eOperMode;
    }

    return  MERR_OK;
}


MERROR_ENUM
Paramctrl::
setSensorMode(ESensorMode_T const eSensorMode)
{
    MY_LOG("[+setSensorMode](old, new)=(%d, %d)", m_eSensorMode, eSensorMode);

    Mutex::Autolock lock(m_Lock);

    if  ( checkParamChange(m_eSensorMode, eSensorMode) )
    {
        m_eSensorMode = eSensorMode;
    }

    return  MERR_OK;
}
#endif

MVOID
Paramctrl::
enableDynamicTuning(MBOOL const fgEnable)
{
    MY_LOG_IF(m_bDebugEnable, "[+enableDynamicTuning](old, new)=(%d, %d)", m_fgDynamicTuning, fgEnable);

    Mutex::Autolock lock(m_Lock);

    if  ( checkParamChange(m_fgDynamicTuning, fgEnable) )
    {
        m_fgDynamicTuning = fgEnable;
    }
}


MVOID
Paramctrl::
enableDynamicCCM(MBOOL const fgEnable)
{
    MY_LOG_IF(m_bDebugEnable, "[+enableDynamicCCM](old, new)=(%d, %d)", m_fgDynamicCCM, fgEnable);

    Mutex::Autolock lock(m_Lock);

    if  ( checkParamChange(m_fgDynamicCCM, fgEnable) )
    {
        m_fgDynamicCCM = fgEnable;
    }
}

#if 0
MVOID
Paramctrl::
updateShadingNVRAMdata(MBOOL const fgEnable)
{
    MY_LOG("[+updateShadingNVRAMdata](old, new)=(%d, %d)", m_fgShadingNVRAMdataChange, fgEnable);

    Mutex::Autolock lock(m_Lock);

    if  ( checkParamChange(m_fgShadingNVRAMdataChange, fgEnable) )
    {
        m_fgShadingNVRAMdataChange = fgEnable;
    }

    MY_LOG("[-updateShadingNVRAMdata] return");
    return;
}
#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MERROR_ENUM
Paramctrl::
setIspProfile(EIspProfile_T const eIspProfile)
{
    Mutex::Autolock lock(m_Lock);

    if  ( checkParamChange(m_rIspCamInfo.eIspProfile, eIspProfile) )
    {
        MY_LOG("[+setIspProfile](old, new)=(%d, %d)", m_rIspCamInfo.eIspProfile, eIspProfile);
        m_rIspCamInfo.eIspProfile = eIspProfile;
    }

    return  MERR_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MERROR_ENUM
Paramctrl::
setSceneMode(EIndex_Scene_T const eScene)
{
    Mutex::Autolock lock(m_Lock);

    if  ( checkParamChange(m_rIspCamInfo.eIdx_Scene, eScene) )
    {
        MY_LOG("[+setSceneMode] scene(old, new)=(%d, %d)", m_rIspCamInfo.eIdx_Scene, eScene);
        m_rIspCamInfo.eIdx_Scene = eScene;
    }

    return  MERR_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MERROR_ENUM
Paramctrl::
setEffect(EIndex_Effect_T const eEffect)
{
    Mutex::Autolock lock(m_Lock);

    if  ( checkParamChange(m_eIdx_Effect, eEffect) )
    {
        MY_LOG("[+setEffect] effect(old, new)=(%d, %d)", m_eIdx_Effect, eEffect);
        m_eIdx_Effect = eEffect;
    }

    return  MERR_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MERROR_ENUM
Paramctrl::
setOperMode(EOperMode_T const eOperMode)
{
    Mutex::Autolock lock(m_Lock);

    if  ( checkParamChange(m_eOperMode, eOperMode) )
    {
        MY_LOG("[+setOperMode](old, new)=(%d, %d)", m_eOperMode, eOperMode);
        m_eOperMode = eOperMode;
    }

    return  MERR_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MERROR_ENUM
Paramctrl::
setSensorMode(ESensorMode_T const eSensorMode)
{
    Mutex::Autolock lock(m_Lock);

    if  ( checkParamChange(m_eSensorMode, eSensorMode) )
    {
        MY_LOG("[+setSensorMode](old, new)=(%d, %d)", m_eSensorMode, eSensorMode);
        m_eSensorMode = eSensorMode;
    }

    return  MERR_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MERROR_ENUM
Paramctrl::
setZoomRatio(MINT32 const i4ZoomRatio_x100)
{
    Mutex::Autolock lock(m_Lock);

    if  ( checkParamChange(m_rIspCamInfo.i4ZoomRatio_x100, i4ZoomRatio_x100) )
    {
        MY_LOG("[+setZoomRatio](old, new)=(%d, %d)", m_rIspCamInfo.i4ZoomRatio_x100, i4ZoomRatio_x100);
        m_rIspCamInfo.i4ZoomRatio_x100 = i4ZoomRatio_x100;
    }

    return  MERR_OK;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MERROR_ENUM
Paramctrl::
setAWBInfo(AWB_INFO_T const &rAWBInfo)
{
    MBOOL bAWBGainChanged = MFALSE;

    Mutex::Autolock lock(m_Lock);

   if (checkParamChange(m_rIspCamInfo.rAWBInfo.rCurrentAWBGain.i4R, rAWBInfo.rCurrentAWBGain.i4R) ||
       checkParamChange(m_rIspCamInfo.rAWBInfo.rCurrentAWBGain.i4G, rAWBInfo.rCurrentAWBGain.i4G) ||
       checkParamChange(m_rIspCamInfo.rAWBInfo.rCurrentAWBGain.i4B, rAWBInfo.rCurrentAWBGain.i4B)) {
        bAWBGainChanged = MTRUE;
        MY_LOG("setAWBInfo(): bAWBGainChanged = MTRUE");
   }

    // Dynamic CCM
    if ( isDynamicCCM() &&
         bAWBGainChanged)
    {
        MY_LOG_IF(m_bDebugEnable, "Dynamic CCM");
        m_pCcmMgr->calculateCCM(rAWBInfo);
    }

    m_rIspCamInfo.rAWBInfo = rAWBInfo;

    // Evaluate PCA LUT index
    EIndex_PCA_LUT_T const eIdx_PCA_LUT_old = m_rIspCamInfo.eIdx_PCA_LUT;
    EIndex_PCA_LUT_T const eIdx_PCA_LUT_new = m_pIspTuningCustom->evaluate_PCA_LUT_index(m_rIspCamInfo);

    if  ( checkParamChange(eIdx_PCA_LUT_old, eIdx_PCA_LUT_new) )
    {
        m_rIspCamInfo.eIdx_PCA_LUT = eIdx_PCA_LUT_new;
        MY_LOG("[setAWBInfo][ParamChangeCount:%d]" "PCA LUT index(old, new) =(%d, %d)", getParamChangeCount(), eIdx_PCA_LUT_old, eIdx_PCA_LUT_new);
    }

    return  MERR_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MERROR_ENUM
Paramctrl::
setAEInfo(AE_INFO_T const &rAEInfo)
{
    MY_LOG_IF(m_bDebugEnable, "setAEInfo()");

    Mutex::Autolock lock(m_Lock);

    // ISO value
    if (checkParamChange(m_rIspCamInfo.u4ISOValue, rAEInfo.u4RealISOValue))
    {
        MY_LOG("[+m_rIspCamInfo.u4ISOValue](old, new)=(%d, %d)", m_rIspCamInfo.u4ISOValue, rAEInfo.u4RealISOValue);
        m_rIspCamInfo.u4ISOValue = rAEInfo.u4RealISOValue;
    }

    EIndex_ISO_T const eIdx_ISO = m_pIspTuningCustom->map_ISO_value_to_index(rAEInfo.u4RealISOValue);

    // ISO index
    if (checkParamChange(m_rIspCamInfo.eIdx_ISO, eIdx_ISO))
    {
        MY_LOG("[+m_rIspCamInfo.eIdx_ISO](old, new)=(%d, %d)", m_rIspCamInfo.eIdx_ISO, eIdx_ISO);
        m_rIspCamInfo.eIdx_ISO = eIdx_ISO;
    }

    // LV
    if (checkParamChange(m_rIspCamInfo.i4LightValue_x10, rAEInfo.i4LightValue_x10))
    {
        MY_LOG("[+m_rIspCamInfo.i4LightValue_x10](old, new)=(%d, %d)", m_rIspCamInfo.i4LightValue_x10, rAEInfo.i4LightValue_x10);
        m_rIspCamInfo.i4LightValue_x10 = rAEInfo.i4LightValue_x10;
    }

    // ISP gain
    if (checkParamChange(m_rIspCamInfo.rAEInfo.u4IspGain, rAEInfo.u4IspGain))
    {
        MY_LOG("[+m_rIspCamInfo.rAEInfo.u4IspGain](old, new)=(%d, %d)", m_rIspCamInfo.rAEInfo.u4IspGain, rAEInfo.u4IspGain);
        m_rIspCamInfo.rAEInfo.u4IspGain = rAEInfo.u4IspGain;
    }

	//check Flare offset
	checkParamChange(m_rIspCamInfo.rAEInfo.i2FlareOffset, rAEInfo.i2FlareOffset);

    m_rIspCamInfo.rAEInfo = rAEInfo;

    return  MERR_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MERROR_ENUM
Paramctrl::
setAFInfo(AF_INFO_T const &rAFInfo)
{
    MY_LOG_IF(m_bDebugEnable, "setAFInfo()");

    Mutex::Autolock lock(m_Lock);

    m_rIspCamInfo.rAFInfo = rAFInfo;

    return  MERR_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MERROR_ENUM
Paramctrl::
setFlashInfo(FLASH_INFO_T const &rFlashInfo)
{
    MY_LOG_IF(m_bDebugEnable, "setFlashInfo()");

    Mutex::Autolock lock(m_Lock);

    m_rIspCamInfo.rFlashInfo = rFlashInfo;

    return  MERR_OK;
}

MERROR_ENUM
Paramctrl::
setIndex_Shading(MINT32 const i4IDX)
{
    MY_LOG_IF(m_bDebugEnable, "[%s] idx %d", __FUNCTION__, i4IDX);

    Mutex::Autolock lock(m_Lock);

    if (m_pLscMgr) {
        m_pLscMgr->setCTIdx(i4IDX);
    } else {
        MY_LOG_IF(m_bDebugEnable, "[%s] m_pLscMgr is NULL", __FUNCTION__);
    }

    return  MERR_OK;
}

MERROR_ENUM
Paramctrl::
getIndex_Shading(MVOID*const pCmdArg)
{
    MY_LOG_IF(m_bDebugEnable, "[%s] idx %d", __FUNCTION__);

    Mutex::Autolock lock(m_Lock);

    if (m_pLscMgr) {
        *(MINT8*)pCmdArg = m_pLscMgr->getCTIdx();
    } else {
        MY_LOG_IF(m_bDebugEnable, "[%s] m_pLscMgr is NULL", __FUNCTION__);
    }

    return  MERR_OK;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Since OBC mixed with AE info, so it is required to backup OBC info
// for dynamic bypass
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

MERROR_ENUM
Paramctrl::
setPureOBCInfo(const ISP_NVRAM_OBC_T *pOBCInfo)
{
    memcpy(&m_backup_OBCInfo, pOBCInfo, sizeof(ISP_NVRAM_OBC_T));

    return  MERR_OK;
}

MERROR_ENUM
Paramctrl::
getPureOBCInfo(ISP_NVRAM_OBC_T *pOBCInfo)
{
    memcpy(pOBCInfo, &m_backup_OBCInfo, sizeof(ISP_NVRAM_OBC_T));

    return  MERR_OK;
}
