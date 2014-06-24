//
#include <utils/Log.h>
#include <cutils/memory.h>
//
/*
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
*/
//
#include <gui/Surface.h>
#include <gui/ISurface.h>
#include <gui/SurfaceComposerClient.h>
//
#include <camera/ICamera.h>
#include <camera/CameraParameters.h>
#include <camera/MtkCameraParameters.h>
//
#include "inc/CamLog.h"
#include "inc/Utils.h"
#include "inc/Command.h"
#if defined(HAVE_COMMAND_test_engineer)
//
using namespace android;
//
//
//
/******************************************************************************
 *  Command
 *      test_engineer <-h> <-shot-mode=testshot> <-shot-count=1> <-picture-size=2560x1920> <-preview-size=640x480> <-display-orientation=90>
 *
 *      -h:             help
 *      -shot-count:    shot count; 1 by default.
 *      -picture-size:  picture size; 2560x1920 by default.
 *      -preview-size:  preview size; 640x480 by default.
 *      -shot-mode:     shot mode; testshot by default.
 *                      For example: "normal", "hdr", "continuousshot", ......
 *      -display-orientation:   display orientation; 90 by default.
 *
 ******************************************************************************/
namespace NSCmd_test_engineer {
struct CmdImp : public CmdBase, public CameraListener
{
    static bool                 isInstantiate;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CmdBase Interface.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:     ////                Interface.
                                CmdImp(char const* szCmdName)
                                    : CmdBase(szCmdName)
                                {}

    virtual bool                execute(Vector<String8>& rvCmd);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CameraListener Interface.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:     ////                Interface.
    virtual void                notify(int32_t msgType, int32_t ext1, int32_t ext2);
    virtual void                postData(int32_t msgType, const sp<IMemory>& dataPtr, camera_frame_metadata_t *metadata);
    virtual void                postDataTimestamp(nsecs_t timestamp, int32_t msgType, const sp<IMemory>& dataPtr) {}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Implementations.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
protected:  ////                Implementation.
    virtual bool                onParseCommand(Vector<String8>& rvCmd);
    virtual bool                onReset();
    virtual bool                onExecute();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:  ////                Operations (Camera)
    virtual bool                connectCamera(int id = 0);
    virtual void                disconnectCamera();
    virtual bool                setupParameters(sp<Camera> spCamera);

protected:  ////                Data Members (Camera)
    sp<Camera>                  mpCamera;

protected:  ////    
    String8                     mShotMode;
    int32_t                     mShotCount;
    Size                        mPictureSize;
    Size                        mPreviewSize;
    int32_t                     mDisplayOrientation;
    int32_t                     mPictureOrientation; 
    String8                     ms8CaptureSize; // -engineer-capture-size: preview, capture, video
    String8                     ms8CaptureType; // -engineer-capture-type: pure-raw, processed-raw, jpeg-only
    String8                     ms8Flicker; // -flicker: 50, 60
    String8                     ms8RawSavePath;

protected:  ////    
    Mutex                       mMutex;
    Condition                   mCond;
    int32_t volatile            mi4ShutterCallbackCount;
    int32_t volatile            mi4CompressedImageCallbackCount;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:  ////                Operations (Surface)
    virtual bool                initSurface();
    virtual void                uninitSurface();

protected:  ////                Data Members (Surface)
    int32_t                     mi4SurfaceID;
    sp<SurfaceComposerClient>   mpSurfaceClient;
    sp<SurfaceControl>          mpSurfaceControl;
    sp<Surface>                 mpSurface;

};
/******************************************************************************
 *
 ******************************************************************************/
bool CmdImp::isInstantiate = CmdMap::inst().addCommand(HAVE_COMMAND_test_engineer, new CmdImp(HAVE_COMMAND_test_engineer));
};  // NSCmd_test_engineer
using namespace NSCmd_test_engineer;


/******************************************************************************
 *
 ******************************************************************************/
bool
CmdImp::
execute(Vector<String8>& rvCmd)
{
    return  onParseCommand(rvCmd) ? onExecute() : false;
}


/******************************************************************************
 *  Command
 *      test_engineer <-h> <-shot-mode=testshot> <-shot-count=1> <-picture-size=2560x1920> <-preview-size=640x480> <-display-orientation=90>
 *
 *      -h:             help
 *      -shot-count:    shot count; 1 by default.
 *      -picture-size:  picture size; 2560x1920 by default.
 *      -preview-size:  preview size; 640x480 by default.
 *      -shot-mode:     shot mode; testshot by default.
 *                      For example: "normal", "hdr", "continuousshot", ......
 *      -display-orientation:   display orientation; 90 by default.
 *
 ******************************************************************************/
bool
CmdImp::
onParseCommand(Vector<String8>& rvCmd)
{
    //  (1) Set default.
    mShotMode = "testshot";
    mShotCount = 1;
    mPictureSize = Size(2560, 1920);
    mPreviewSize = Size(640, 480);
    mDisplayOrientation = 90;
    mPictureOrientation = 0; 

    //  (2) Start to parse commands.
    for (size_t i = 1; i < rvCmd.size(); i++)
    {
        if  ( rvCmd[i] == "-h" ) {
            String8 text;
            text += "\n";
            text += "\n   test_engineer <-h> <-shot-mode=testshot> <-shot-count=1> <-picture-size=2560x1920> <-preview-size=640x480> <-display-orientation=90 -jpeg-orienation=0>";
            text += "\n   -h:             help";
            text += "\n   -shot-count:    shot count; 1 by default.";
            text += "\n   -picture-size:  picture size; 2560x1920 by default.";
            text += "\n   -preview-size:  preview size; 640x480 by default.";
            text += "\n   -shot-mode:     shot mode; testshot by default.";
            text += "\n                   For example: normal, hdr, continuousshot, ......";
            text += "\n   -display-orientation:   display orientation; 90 by default.";
            text += "\n   -pic-orientation: jpeg orienation; 0 by default."; 
            text += "\n   -engineer-capture-size: preview, capture, video."; 
            text += "\n   -engineer-capture-type: pure-raw, processed-raw, jpeg-only."; 
            text += "\n   -engineer-raw-save-path: /sdcard/preview.raw, for example"; 
            text += "\n   -flicker: 50, 60."; 
            MY_LOGD("%s", text.string());
            return  false;
        }
        //
        String8 key, val;
        parseOneCmdArgument(rvCmd[i], key, val);
//        MY_LOGD("<key/val>=<%s/%s>", key.string(), val.string());
        //
        //
        if  ( key == "-engineer-raw-save-path" ) {
            ms8RawSavePath = val;
            MY_LOGD("ms8RawSavePath = %s", ms8RawSavePath.string());
            continue;
        }

        if  ( key == "-engineer-capture-size" ) {
            ms8CaptureSize = val;
            MY_LOGD("ms8CaptureSize = %s", ms8CaptureSize.string());
            continue;
        }

        if  ( key == "-engineer-capture-type" ) {
            ms8CaptureType = val;
            MY_LOGD("ms8CaptureType = %s", ms8CaptureType.string());
            continue;
        }

        if  ( key == "-flicker" ) {
            ms8Flicker = val;
            MY_LOGD("ms8Flicker = %s", ms8Flicker.string());
            continue;
        }

        if  ( key == "-shot-mode" ) {
            mShotMode = val; // In engineer mode, use EngShot only
            continue;
        }
        //
        if  ( key == "-shot-count" ) {
            mShotCount = ::atoi(val);
            continue;
        }
        //
        if  ( key == "-picture-size" ) {
            ::sscanf(val.string(), "%dx%d", &mPictureSize.width, &mPictureSize.height);
            MY_LOGD("picture-size : %d %d", mPictureSize.width, mPictureSize.height);
            continue;
        }
        //
        if  ( key == "-preview-size" ) {
            ::sscanf(val.string(), "%dx%d", &mPreviewSize.width, &mPreviewSize.height);
            MY_LOGD("preview-size : %d %d", mPreviewSize.width, mPreviewSize.height);
            continue;
        }
        //
        if  ( key == "-display-orientation" ) {
            mDisplayOrientation = ::atoi(val);
            continue;
        }
        //
        if  (key == "-pic-orientation" )  {
            mPictureOrientation = ::atoi(val); 
            printf("picture orientation = %d\n", mPictureOrientation);  
            continue; 
        }
    }
    return  true;
}


/******************************************************************************
 *
 ******************************************************************************/
bool
CmdImp::
onReset()
{
    mi4ShutterCallbackCount = 0;
    mi4CompressedImageCallbackCount = 0;
    return  true;
}


/******************************************************************************
 *
 ******************************************************************************/
bool
CmdImp::
onExecute()
{
    bool ret = false;
    status_t status = OK;
    //
    ret = 
            onReset()
        &&  initSurface()
        &&  connectCamera(0)
        &&  setupParameters(mpCamera)
            ;
    if  ( ! ret )
    {
        goto lbExit;
    }
    //
    //
    mpCamera->setPreviewDisplay(mpSurface);
    if  ( OK != (status = mpCamera->sendCommand(CAMERA_CMD_SET_DISPLAY_ORIENTATION, mDisplayOrientation, 0)) ) {
        MY_LOGE("sendCommand(CAMERA_CMD_SET_DISPLAY_ORIENTATION), status[%s(%d)]", ::strerror(-status), -status);
        goto lbExit;
    }
    if  ( OK != (status = mpCamera->startPreview()) ) {
        MY_LOGE("startPreview(), status[%s(%d)]", ::strerror(-status), -status);
        goto lbExit;
    }
    ::sleep(2);
    mpCamera->takePicture(CAMERA_MSG_SHUTTER | CAMERA_MSG_COMPRESSED_IMAGE);
    {
        Mutex::Autolock _lock(mMutex);
        while   (
                    mShotCount > ::android_atomic_release_load(&mi4ShutterCallbackCount)
                ||  mShotCount > ::android_atomic_release_load(&mi4CompressedImageCallbackCount)
                )
        {
            nsecs_t nsTimeoutToWait = mShotCount * 60LL*1000LL*1000LL*1000LL;//wait 10 sec x n.
            MY_LOGD("Start to wait %lld sec...", nsTimeoutToWait/1000000000LL);
            status_t status = mCond.waitRelative(mMutex, nsTimeoutToWait);
            if  ( OK != status ) {
                CAM_LOGE("status[%s(%d)]\n", ::strerror(-status), -status);
                break;
            }
        }
    }
    mpCamera->stopPreview();
    //
    //
lbExit:
    disconnectCamera();
    uninitSurface();
    //
    return  true;
}


/******************************************************************************
 *
 ******************************************************************************/
bool
CmdImp::
initSurface()
{
    mi4SurfaceID = 0;

    // create a client to surfaceflinger
    mpSurfaceClient = new SurfaceComposerClient();

    mpSurfaceControl = mpSurfaceClient->createSurface(
        String8("surface"), mi4SurfaceID, 480, 800, PIXEL_FORMAT_RGBA_8888
    );
    SurfaceComposerClient::openGlobalTransaction();
    mpSurfaceControl->setLayer(100000);
    SurfaceComposerClient::closeGlobalTransaction();
    // pretend it went cross-process
    Parcel parcel;
    SurfaceControl::writeSurfaceToParcel(mpSurfaceControl, &parcel);
    parcel.setDataPosition(0);
    mpSurface = Surface::readFromParcel(parcel);
    //
    CAM_LOGD("setupSurface: %p", mpSurface.get());
    return  (mpSurface != 0);
}


/******************************************************************************
 *
 ******************************************************************************/
void
CmdImp::
uninitSurface()
{
    mpSurface = 0;
    mpSurfaceControl = 0;
    if  ( mpSurfaceClient != 0 )
    {
        mpSurfaceClient->destroySurface(mi4SurfaceID);
        mpSurfaceClient = 0;
    }
}


/******************************************************************************
 *
 ******************************************************************************/
bool
CmdImp::
connectCamera(int id)
{
    status_t status = OK;
    status = Camera::setProperty(String8(MtkCameraParameters::PROPERTY_KEY_CLIENT_APPMODE), String8(MtkCameraParameters::APP_MODE_NAME_MTK_ENG));
    MY_LOGD("connectCamera: status(%d)", status);

    mpCamera = Camera::connect(id);
    if  ( mpCamera == 0 )
    {
        MY_LOGE("Camera::connect, id(%d)", id);
        return  false;
    }
    //
    //
    mpCamera->setListener(this);
    //
    MY_LOGD("Camera::connect, id(%d), camera(%p)", id, mpCamera.get());
    return  true;
}


/******************************************************************************
 *
 ******************************************************************************/
void
CmdImp::
disconnectCamera()
{
    if  ( mpCamera != 0 )
    {
        MY_LOGD("Camera::disconnect, camera(%p)", mpCamera.get());
        mpCamera->disconnect();
        mpCamera = NULL;
    }
}


/******************************************************************************
 *
 ******************************************************************************/
bool
CmdImp::
setupParameters(sp<Camera> spCamera)
{
    CameraParameters params(spCamera->getParameters());
    //
    params.set(MtkCameraParameters::KEY_CAMERA_MODE, MtkCameraParameters::CAMERA_MODE_NORMAL);
    //
    params.setPreviewSize(mPreviewSize.width, mPreviewSize.height);
    params.setPreviewFormat(CameraParameters::PIXEL_FORMAT_YUV420P);    //  YV12
    params.setPictureSize(mPictureSize.width, mPictureSize.height);

    params.setPictureFormat("jpeg");
    params.set(MtkCameraParameters::KEY_JPEG_QUALITY, "100");
    params.set(MtkCameraParameters::KEY_CAPTURE_MODE, mShotMode.string());

    params.set(MtkCameraParameters::KEY_BURST_SHOT_NUM, mShotCount);
    params.set(MtkCameraParameters::KEY_CAPTURE_PATH, "sdcard/");

    params.set(MtkCameraParameters::KEY_JPEG_THUMBNAIL_QUALITY, "100");
    params.set(MtkCameraParameters::KEY_JPEG_THUMBNAIL_WIDTH, "160");
    params.set(MtkCameraParameters::KEY_JPEG_THUMBNAIL_HEIGHT, "120");

    params.set(MtkCameraParameters::KEY_ROTATION, mPictureOrientation);
    params.set(MtkCameraParameters::KEY_JPEG_THUMBNAIL_HEIGHT, "120");

    params.set(MtkCameraParameters::KEY_GPS_LATITUDE, "25.032146");
    params.set(MtkCameraParameters::KEY_GPS_LONGITUDE, "121.564448");
    params.set(MtkCameraParameters::KEY_GPS_ALTITUDE, "21.0");
    params.set(MtkCameraParameters::KEY_GPS_TIMESTAMP, "1251192757");
    params.set(MtkCameraParameters::KEY_GPS_PROCESSING_METHOD, "GPS");
    //
// *** Engineer Mode
    if (ms8CaptureType == "processed-raw")
    {
        params.set(MtkCameraParameters::KEY_ISP_MODE, "0"); // 0: "proccess raw", 
    }
    else if (ms8CaptureType == "pure-raw")
    {
        params.set(MtkCameraParameters::KEY_ISP_MODE, "1"); // 1: "pure raw",
    }
    MY_LOGD("params.get(MtkCameraParameters::KEY_ISP_MODE) = %s", params.get(MtkCameraParameters::KEY_ISP_MODE));

    if (ms8CaptureSize == "preview")
    {
        params.set(MtkCameraParameters::KEY_RAW_SAVE_MODE, "1"); // 1: "Preview size", 
    }
    else if (ms8CaptureSize == "capture")
    {
        params.set(MtkCameraParameters::KEY_RAW_SAVE_MODE, "2"); // 2: "Capture size", 
    }
    else if (ms8CaptureSize == "video")
    {
        params.set(MtkCameraParameters::KEY_RAW_SAVE_MODE, "4"); // 4: "Video Mode", 
    }
    else
    {
        params.set(MtkCameraParameters::KEY_RAW_SAVE_MODE, "3"); // 3: "JPEG only", 
    }    
    MY_LOGD("params.get(MtkCameraParameters::KEY_RAW_SAVE_MODE) = %s", params.get(MtkCameraParameters::KEY_RAW_SAVE_MODE));

        
    params.set(MtkCameraParameters::KEY_RAW_PATH, ms8RawSavePath.string());
    MY_LOGD("KEY_RAW_PATH = %s", params.get(MtkCameraParameters::KEY_RAW_PATH));

    params.set(MtkCameraParameters::PROPERTY_KEY_CLIENT_APPMODE, MtkCameraParameters::APP_MODE_NAME_MTK_ENG);
    MY_LOGD("params.get(MtkCameraParameters::PROPERTY_KEY_CLIENT_APPMODE) = %s", params.get(MtkCameraParameters::PROPERTY_KEY_CLIENT_APPMODE));

    if  (OK != spCamera->setParameters(params.flatten()))
    {
        CAM_LOGE("setParameters\n");
        return  false;
    }
    return  true;
}


/******************************************************************************
 *
 ******************************************************************************/
void
CmdImp::
notify(int32_t msgType, int32_t ext1, int32_t ext2)
{
    if  ( msgType == CAMERA_MSG_SHUTTER )
    {
        CAM_LOGD("CAMERA_MSG_SHUTTER \n");
        Mutex::Autolock _lock(mMutex);
        ::android_atomic_inc(&mi4ShutterCallbackCount);
        mCond.broadcast();
    }
}


/******************************************************************************
 *
 ******************************************************************************/
void
CmdImp::
postData(int32_t msgType, const sp<IMemory>& dataPtr, camera_frame_metadata_t *metadata)
{
    ssize_t offset;
    size_t size;
    sp<IMemoryHeap> heap = dataPtr->getMemory(&offset, &size);
    uint8_t* pBase = (uint8_t *)heap->base() + offset;
    MY_LOGD("msgType=%x CAMERA_MSG_PREVIEW_FRAME?%d base/size=%p/%d", msgType, (msgType & CAMERA_MSG_PREVIEW_FRAME), pBase, size);
    //
    if  ( msgType == CAMERA_MSG_COMPRESSED_IMAGE )
    {
        CAM_LOGD("CAMERA_MSG_COMPRESSED_IMAGE \n");
        Mutex::Autolock _lock(mMutex);
        ::android_atomic_inc(&mi4CompressedImageCallbackCount);
        mCond.broadcast();
        //
        static int i = 0;
        String8 filename = String8::format("/data/cap%02d.jpeg", i);
        saveBufToFile(filename, pBase, size);
        i++;
    }
}


/******************************************************************************
*
*******************************************************************************/
#endif  //  HAVE_COMMAND_xxx

