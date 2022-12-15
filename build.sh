gcc -o x0client src/main.c src/utils.c src/ioctl.c src/mapper.c -fsanitize=address, -g3

cd src/driver
make

find . -name "*.o" -type f -delete
find . -name "*.cmd" -type f -delete
find . -name "*.mod" -type f -delete
find . -name "*.mod.o" -type f -delete
find . -name "*.o.d" -type f -delete
find . -name "*.mod.c" -type f -delete
find . -name "*.order" -type f -delete
find . -name "*.symvers" -type f -delete

mv x0driver.ko ../../

cd .. && source edksetup.sh
build -a X64 -t GCC5 -p MdeModulePkg/MdeModulePkg.dsc && mv Build/MdeModule/DEBUG_GCC5/X64/x0.efi x0 && mv Build/MdeModule/DEBUG_GCC5/X64/x0.efi x0 && cd x0
