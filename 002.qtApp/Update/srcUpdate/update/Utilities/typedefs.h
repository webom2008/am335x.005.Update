#ifndef TYPEDEFS_H
#define TYPEDEFS_H


typedef unsigned int            UINT32;
typedef int                     INT32;
typedef unsigned short int      UINT16;
typedef short int               INT16;
typedef char                    INT8;
typedef unsigned char           UINT8;

enum UPDATE_TARGET_TAG
{
    UPDATE_TARGET_TAG_SPO2,
    UPDATE_TARGET_TAG_AIOSTM,
    UPDATE_TARGET_TAG_AIODSP,
    UPDATE_TARGET_TAG_RECODER,
    UPDATE_TARGET_TAG_BACK,
    UPDATE_TARGET_TAG_MLO,
    UPDATE_TARGET_TAG_UBOOT,
    UPDATE_TARGET_TAG_PATIENTMONITOR,
    UPDATE_TARGET_TAG_UPDATE_APP,
    UPDATE_TARGET_TAG_KERNEL,
    UPDATE_TARGET_TAG_ROOTFS,

    UPDATE_TARGET_TAG_END
};
#endif // TYPEDEFS_H
