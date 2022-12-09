cd .. && source edksetup.sh
build -a X64 -t GCC5 -p MdeModulePkg/MdeModulePkg.dsc && mv Build/MdeModule/DEBUG_GCC5/X64/x0loader.efi x0 && cd x0