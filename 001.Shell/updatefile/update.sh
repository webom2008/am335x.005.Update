#!/bin/bash
# =============================================================
# Author	: QiuWeibo
# Date		: 2014.5.12
# Version	: V1.0.0
# Comment	: For update aPM.MCU
#		1.Support Kernel update
#		2.Support Rootfs update
#		3.Support Monitor App update
# 
# =============================================================
# Author	: QiuWeibo
# Date		: 2014.6.9
# Version	: V1.0.1
# Comment	: Add MLO and uboot update
#		1.Support MLO update
#		2.Support uboot update
# =============================================================
# Author	: QiuWeibo
# Date		: 2015.8.3
# Version	: V1.0.2
# Comment	: PatientMonitor update
# Author	: QiuWeibo
# =============================================================
echo "firmware update shell start..."

UPDATE_MLO="N"
UPDATE_UBOOT="N"
UPDATE_KERNEL="N"
UPDATE_ROOTFS="N"
CLEAR_UBOOT_ENV="N"
MONITOR_APP="N"

NAME_MLO="MLO.byteswap"
NAME_UBOOT="u-boot.img"
NAME_KERNEL="uImage"
NAME_ROOTFS="ubi.img"
NAME_MONITOR="aPM12_MPU_3352.tar.gz"

echo "   UPDATE_MLO=$UPDATE_MLO=$NAME_MLO"
echo " UPDATE_UBOOT=$UPDATE_UBOOT=$NAME_UBOOT"
echo "UPDATE_KERNEL=$UPDATE_KERNEL=$NAME_KERNEL"
echo "UPDATE_ROOTFS=$UPDATE_ROOTFS=$NAME_ROOTFS"
echo "CLEAR_UBOOT_ENV=$CLEAR_UBOOT_ENV"
echo "MONITOR_APP=$MONITOR_APP=$NAME_MONITOR"

cd /tmp/

# ============== handle for Monitor App Update start============
case "$MONITOR_APP" in
	Y )
	rm /mnt/userfs/MonitorConfig -fr
	rm /mnt/userfs/Monitor -f
	rm /mnt/userfs/PatientMonitor -f
	rm /mnt/userfs/updateTime -f
	tar xzf /tmp/updatefile/$NAME_MONITOR -C /mnt/userfs/
	chmod 777 /mnt/userfs/* -R
	sync
   	echo "tar /mnt/userfs/Monitor success"
	;;
	* )
   	echo "Monitor App do not update"
	;;
esac
rm -rf /tmp/updatefile/$NAME_MONITOR
echo "end rm -rf /tmp/updatefile/$NAME_MONITOR "
# ============== handle for Monitor App Update end============

# ============== handle for MLO update start============
case "$UPDATE_MLO" in
	Y )
	filesize=`ls -l /tmp/updatefile/$NAME_MLO | awk '{print $5}'`
	if [[ $filesize -gt 1024 ]];then
		mtd_debug erase /dev/mtd0 0 131072
		mtd_debug write /dev/mtd0 0 $filesize /tmp/updatefile/$NAME_MLO
   		echo "mtd_debug write /dev/mtd0 0 $filesize /tmp/updatefile/$NAME_MLO"
	else
   		echo "/tmp/updatefile/$NAME_MLO error! filesize= $filesize lt 1024!"
	fi
	;;
	* )
   	echo "MLO do not update"
	;;
esac
rm -rf /tmp/updatefile/$NAME_MLO
echo "end rm -rf /tmp/updatefile/$NAME_MLO "
# ============== handle for MLO update end============

# ============== handle for Uboot update start============
case "$UPDATE_UBOOT" in
	Y )
	filesize=`ls -l /tmp/updatefile/$NAME_UBOOT | awk '{print $5}'`
	if [[ $filesize -gt 1024 ]];then
		mtd_debug erase /dev/mtd1 0 786432
		mtd_debug write /dev/mtd1 0 $filesize /tmp/updatefile/$NAME_UBOOT
		echo "mtd_debug write /dev/mtd1 0 $filesize /tmp/updatefile/$NAME_UBOOT"
	else
		echo "/tmp/updatefile/$NAME_UBOOT filesize error! $filesize lt 1024"
	fi
	;;
	* )
   	echo "u-boot do not update"
	;;
esac
rm -rf /tmp/updatefile/$NAME_UBOOT
echo "end rm -rf /tmp/updatefile/$NAME_UBOOT "
# ============== handle for Uboot update end============

# ============== handle for clear u-boot env start============
case "$CLEAR_UBOOT_ENV" in
	Y )
	flash_erase /dev/mtd2 0 0
   	echo "end clear uboot env at eraseall /dev/mtd2"
	;;
	* )
   	echo "uboot env do not erase!"
	;;
esac
# ============== handle for clear u-boot env end============

# ============== handle for kernel update start============
case "$UPDATE_KERNEL" in
	Y )
	flash_erase /dev/mtd4 0 0
	nandwrite -p /dev/mtd4 /tmp/updatefile/$NAME_KERNEL
   	echo "end nandwrite /tmp/updatefile/$NAME_KERNEL /dev/mtd4"
	;;
	* )
   	echo "KERNEL do not update"
	;;
esac
rm -rf /tmp/updatefile/$NAME_KERNEL
echo "end rm -rf /tmp/updatefile/$NAME_KERNEL "
# ============== handle for kernel update end============

# ============== handle for rootfs update start============
case "$UPDATE_ROOTFS" in
	Y )
	if [ -e "/tmp/updatefile/util/flash_erase" ] && [ -e "/tmp/updatefile/util/nandwrite" ];then
		/tmp/updatefile/util/flash_erase /dev/mtd5 0 0
		/tmp/updatefile/util/nandwrite -p /dev/mtd5 /tmp/updatefile/$NAME_ROOTFS
   		echo "end nandwrite /tmp/updatefile/$NAME_ROOTFS /dev/mtd5"
	else
   		echo "/tmp/updatefile/util/flash_erase or nandwrite not exist! error"
	fi
	;;
	* )
   	echo "rootfs do not update"
	;;
esac
rm -rf /tmp/updatefile/
echo "end rm -rf /tmp/updatefile/ "
# ============== handle for rootfs update end============

rm -rf /tmp/update.sh/
echo "end rm -rf /tmp/update.sh "

echo "firmware update shell end"
