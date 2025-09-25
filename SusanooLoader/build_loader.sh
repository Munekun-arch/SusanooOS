#!/bin/bash
# SusanooOS Loader ビルド & 実行スクリプト

set -e
ARCH=${1:-X64}

WORKSPACE=$HOME/SusanooOS/edk2
LOADER_DIR=$HOME/SusanooOS/SusanooLoader
FAT_IMG=$LOADER_DIR/loader.img

echo "=== SusanooOS Loader ビルド ==="
echo "ターゲットアーキテクチャ: $ARCH"

# --- 環境変数 ---
# EDK2 SusanooOS setup
export WORKSPACE=$HOME/SusanooOS/edk2
export EDK_TOOLS_PATH=$WORKSPACE/BaseTools
export CONF_PATH=$WORKSPACE/Conf
export PACKAGES_PATH=$HOME/SusanooOS/SusanooLoader:$HOME/SusanooOS/edk2
source $WORKSPACE/edksetup.sh BaseTools
export TARGET=DEBUG
export TARGET_ARCH=$ARCH

# --- ビルド ---
build $LOADER_DIR/SusanooLoader.dsc

# --- 成果物の確認 ---
EFI_FILE=$WORKSPACE/Build/SusanooLoader/DEBUG_$ARCH/$ARCH/HelloLoader.efi
if [ ! -f "$EFI_FILE" ]; then
  echo "エラー: $EFI_FILE が見つかりません"
  exit 1
fi

# --- FAT イメージ作成 ---
echo "--- FAT イメージ作成 ---"
rm -f $FAT_IMG
dd if=/dev/zero of=$FAT_IMG bs=1M count=10
mkfs.vfat $FAT_IMG

mmd -i $FAT_IMG ::/EFI
mmd -i $FAT_IMG ::/EFI/BOOT
mcopy -i $FAT_IMG $EFI_FILE ::/EFI/BOOT/BOOTX64.EFI

# --- QEMU 起動 ---
echo "--- QEMU 起動 ---"
qemu-system-x86_64 -bios /usr/share/OVMF/OVMF_CODE.fd -hda $FAT_IMG -m 512 -net none

