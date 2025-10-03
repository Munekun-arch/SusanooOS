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
  BaseLib                   | MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib             | MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  MemoryAllocationLib       | MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  DebugLib                  | MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  PrintLib                  | MdePkg/Library/BasePrintLib/BasePrintLib.inf
  PcdLib                    | MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  RegisterFilterLib         | MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf

  # UEFI 基本
      UefiLib                   | MdePkg/Library/UefiLib/UefiLib.inf
  UefiApplicationEntryPoint | MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
 StackCheckLib             | MdePkg/Library/StackCheckLib/StackCheckLib.inf
  StackCheckFailureHookLib  | MdePkg/Library/StackCheckFailureHookLibNull/StackCheckFailureHookLibNull.inf

  UefiBootServicesTableLib  | MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib | MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  DevicePathLib             | MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf

  # 便利系
  HiiLib                    | MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  FileHandleLib             | MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
  ShellLib                  | ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  
  DevicePathLib     | MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  FileHandleLib     | MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf

  



[Components]
  ../SusanooLoader/HelloLoader.inf







