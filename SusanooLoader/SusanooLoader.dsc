[Defines]
  PLATFORM_NAME           = SusanooLoader
  PLATFORM_GUID           = 12345678-1234-1234-1234-123456789abc
  PLATFORM_VERSION        = 0.1
  DSC_SPECIFICATION       = 0x0001001A
  OUTPUT_DIRECTORY        = Build/SusanooLoader
  SUPPORTED_ARCHITECTURES = X64
  BUILD_TARGETS           = DEBUG|RELEASE
  SKUID_IDENTIFIER        = DEFAULT

[LibraryClasses]
  # 基本
  BaseLib                   | ../edk2/MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib             | ../edk2/MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  MemoryAllocationLib       | ../edk2/MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  DebugLib                  | ../edk2/MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  PrintLib                  | ../edk2/MdePkg/Library/BasePrintLib/BasePrintLib.inf
  PcdLib                    | ../edk2/MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  RegisterFilterLib         | ../edk2/MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf

  # UEFI 基本
      UefiLib                   | ../edk2/MdePkg/Library/UefiLib/UefiLib.inf
  UefiApplicationEntryPoint | ../edk2/MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
 StackCheckLib             | ../edk2/MdePkg/Library/StackCheckLib/StackCheckLib.inf
  StackCheckFailureHookLib  | ../edk2/MdePkg/Library/StackCheckFailureHookLibNull/StackCheckFailureHookLibNull.inf

  UefiBootServicesTableLib  | ../edk2/MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib | ../edk2/MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  DevicePathLib             | ../edk2/MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf

  # 便利系
  HiiLib                    | ../edk2/MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  FileHandleLib             | ../edk2/MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
  ShellLib                  | ../edk2/ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  
  DevicePathLib     | ../edk2/MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  FileHandleLib     | ../edk2/MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf

  



[Components]
	SusanooLoader/HelloLoader.inf







