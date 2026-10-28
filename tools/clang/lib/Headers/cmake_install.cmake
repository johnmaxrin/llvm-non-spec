# Install script for directory: /home/robert/scratch/llvm-non-spec/clang/lib/Headers

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xclang-resource-headersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/builtins.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/float.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/inttypes.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/iso646.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/limits.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/module.modulemap"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stdalign.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stdarg.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stdarg___gnuc_va_list.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stdarg___va_copy.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stdarg_header_macro.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stdarg_va_arg.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stdarg_va_copy.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stdarg_va_list.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stdatomic.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stdbool.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stdcountof.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stdckdint.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stddef.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_header_macro.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_max_align_t.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_null.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_nullptr_t.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_offsetof.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_ptrdiff_t.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_rsize_t.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_size_t.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_unreachable.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_wchar_t.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_wint_t.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stdint.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stdnoreturn.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/tgmath.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/unwind.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/varargs.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/arm_acle.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/arm_cmse.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/armintr.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/arm64intr.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/arm_neon_sve_bridge.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_builtin_vars.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_math.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_cmath.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_complex_builtins.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_device_functions.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_intrinsics.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_texture_intrinsics.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_libdevice_declares.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_math_forward_declares.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_runtime_wrapper.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hexagon_circ_brev_intrinsics.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hexagon_protos.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hexagon_types.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hvx_hexagon_protos.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_hip_libdevice_declares.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_hip_cmath.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_hip_math.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_hip_stdlib.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_hip_runtime_wrapper.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/larchintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/lasxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/lsxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/msa.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/opencl-c.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/opencl-c-base.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/altivec.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/htmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/htmxlintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/riscv_bitmanip.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/riscv_corev_alu.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/riscv_crypto.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/riscv_ntlh.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/sifive_vector.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/andes_vector.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_spirv_builtins.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/s390intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/vecintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/velintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/velintrin_gen.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/velintrin_approx.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/adcintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/adxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ammintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxavx512intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxbf16transposeintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxcomplexintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxcomplextransposeintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxfp16intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxfp16transposeintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxfp8intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxmovrsintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxmovrstransposeintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxtf32intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxtf32transposeintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxtransposeintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2_512bf16intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2_512convertintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2_512minmaxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2_512niintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2_512satcvtdsintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2_512satcvtintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2bf16intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2convertintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2copyintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2minmaxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2niintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2satcvtdsintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2satcvtintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx2intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512bf16intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512bitalgintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512bwintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512cdintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512dqintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512fintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512fp16intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512ifmaintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512ifmavlintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vbmi2intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vbmiintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vbmivlintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlbf16intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlbitalgintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlbwintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlcdintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vldqintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlfp16intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlvbmi2intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlvnniintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlvp2intersectintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vnniintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vp2intersectintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vpopcntdqintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vpopcntdqvlintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avxifmaintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avxneconvertintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avxvnniint16intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avxvnniint8intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avxvnniintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/bmi2intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/bmiintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cetintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cldemoteintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/clflushoptintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/clwbintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/clzerointrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cmpccxaddintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/crc32intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/emmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/enqcmdintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/f16cintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/fma4intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/fmaintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/fxsrintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/gfniintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hresetintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ia32intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/immintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/invpcidintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/keylockerintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/lwpintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/lzcntintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/mm3dnow.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/mmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/movdirintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/movrs_avx10_2_512intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/movrs_avx10_2intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/movrsintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/mwaitxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/nmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/pconfigintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/pkuintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/pmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/popcntintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/prfchiintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/prfchwintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ptrauth.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ptwriteintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/raointintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/rdpruintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/rdseedintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/rtmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/serializeintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/sgxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/sha512intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/shaintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/sm3intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/sm4intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/sm4evexintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/smmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/tbmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/tmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/tsxldtrkintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/uintrintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/usermsrintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/vaesintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/vpclmulqdqintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/waitpkgintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/wbnoinvdintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__wmmintrin_aes.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/wmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__wmmintrin_pclmul.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/x86gprintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/x86intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/xmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/xopintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/xsavecintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/xsaveintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/xsaveoptintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/xsavesintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/xtestintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cet.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cpuid.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/gpuintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/nvptxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amdgpuintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/wasm_simd128.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/intrin0.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/vadefs.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/yvals_core.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/mm_malloc.h"
    "/home/robert/scratch/llvm-non-spec/tools/clang/lib/Headers/riscv_vector.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xclang-resource-headersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include/cuda_wrappers" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cuda_wrappers/algorithm"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cuda_wrappers/cmath"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cuda_wrappers/complex"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cuda_wrappers/new"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xclang-resource-headersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include/cuda_wrappers/bits" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cuda_wrappers/bits/c++config.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cuda_wrappers/bits/shared_ptr_base.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cuda_wrappers/bits/basic_string.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cuda_wrappers/bits/basic_string.tcc"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xclang-resource-headersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include/ppc_wrappers" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/mmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/xmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/mm_malloc.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/emmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/pmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/tmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/smmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/nmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/bmiintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/bmi2intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/immintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/x86intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/x86gprintrin.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xclang-resource-headersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include/llvm_libc_wrappers" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/llvm_libc_wrappers/assert.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/llvm_libc_wrappers/stdio.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/llvm_libc_wrappers/stdlib.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/llvm_libc_wrappers/string.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/llvm_libc_wrappers/ctype.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/llvm_libc_wrappers/inttypes.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/llvm_libc_wrappers/time.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xclang-resource-headersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include/openmp_wrappers" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/openmp_wrappers/math.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/openmp_wrappers/cmath"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/openmp_wrappers/complex"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/openmp_wrappers/complex.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/openmp_wrappers/__clang_openmp_device_functions.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/openmp_wrappers/complex_cmath.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/openmp_wrappers/new"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xclang-resource-headersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include/llvm_offload_wrappers" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/llvm_offload_wrappers/__llvm_offload.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/llvm_offload_wrappers/__llvm_offload_host.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/llvm_offload_wrappers/__llvm_offload_device.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xclang-resource-headersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include/zos_wrappers" TYPE FILE FILES "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/zos_wrappers/builtins.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xcore-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/builtins.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/float.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/inttypes.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/iso646.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/limits.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/module.modulemap"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stdalign.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stdarg.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stdarg___gnuc_va_list.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stdarg___va_copy.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stdarg_header_macro.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stdarg_va_arg.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stdarg_va_copy.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stdarg_va_list.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stdatomic.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stdbool.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stdcountof.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stdckdint.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stddef.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_header_macro.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_max_align_t.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_null.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_nullptr_t.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_offsetof.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_ptrdiff_t.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_rsize_t.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_size_t.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_unreachable.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_wchar_t.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__stddef_wint_t.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stdint.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/stdnoreturn.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/tgmath.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/unwind.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/varargs.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xarm-common-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/arm_acle.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xarm-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/arm_cmse.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/armintr.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xaarch64-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/arm64intr.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/arm_neon_sve_bridge.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xcuda-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include/cuda_wrappers" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cuda_wrappers/algorithm"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cuda_wrappers/cmath"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cuda_wrappers/complex"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cuda_wrappers/new"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xcuda-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include/cuda_wrappers/bits" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cuda_wrappers/bits/c++config.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cuda_wrappers/bits/shared_ptr_base.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cuda_wrappers/bits/basic_string.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cuda_wrappers/bits/basic_string.tcc"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xcuda-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_builtin_vars.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_math.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_cmath.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_complex_builtins.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_device_functions.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_intrinsics.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_texture_intrinsics.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_libdevice_declares.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_math_forward_declares.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_cuda_runtime_wrapper.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xhexagon-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hexagon_circ_brev_intrinsics.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hexagon_protos.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hexagon_types.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hvx_hexagon_protos.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xhip-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_hip_libdevice_declares.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_hip_cmath.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_hip_math.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_hip_stdlib.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_hip_runtime_wrapper.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xloongarch-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/larchintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/lasxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/lsxintrin.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xmips-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/msa.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xppc-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include/ppc_wrappers" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/mmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/xmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/mm_malloc.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/emmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/pmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/tmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/smmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/nmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/bmiintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/bmi2intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/immintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/x86intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ppc_wrappers/x86gprintrin.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xppc-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/altivec.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xppc-htm-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/htmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/htmxlintrin.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xriscv-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES "/home/robert/scratch/llvm-non-spec/tools/clang/lib/Headers/riscv_vector.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xriscv-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/riscv_bitmanip.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/riscv_corev_alu.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/riscv_crypto.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/riscv_ntlh.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/sifive_vector.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/andes_vector.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xsystemz-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/s390intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/vecintrin.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xsystemz-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include/zos_wrappers" TYPE FILE FILES "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/zos_wrappers/builtins.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xve-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/velintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/velintrin_gen.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/velintrin_approx.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xwebassembly-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/wasm_simd128.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xx86-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/adcintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/adxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ammintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxavx512intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxbf16transposeintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxcomplexintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxcomplextransposeintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxfp16intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxfp16transposeintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxfp8intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxmovrsintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxmovrstransposeintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxtf32intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxtf32transposeintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amxtransposeintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2_512bf16intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2_512convertintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2_512minmaxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2_512niintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2_512satcvtdsintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2_512satcvtintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2bf16intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2convertintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2copyintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2minmaxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2niintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2satcvtdsintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx10_2satcvtintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx2intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512bf16intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512bitalgintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512bwintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512cdintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512dqintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512fintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512fp16intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512ifmaintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512ifmavlintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vbmi2intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vbmiintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vbmivlintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlbf16intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlbitalgintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlbwintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlcdintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vldqintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlfp16intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlvbmi2intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlvnniintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vlvp2intersectintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vnniintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vp2intersectintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vpopcntdqintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avx512vpopcntdqvlintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avxifmaintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avxneconvertintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avxvnniint16intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avxvnniint8intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/avxvnniintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/bmi2intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/bmiintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cetintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cldemoteintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/clflushoptintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/clwbintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/clzerointrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cmpccxaddintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/crc32intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/emmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/enqcmdintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/f16cintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/fma4intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/fmaintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/fxsrintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/gfniintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hresetintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ia32intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/immintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/invpcidintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/keylockerintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/lwpintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/lzcntintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/mm3dnow.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/mmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/movdirintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/movrs_avx10_2_512intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/movrs_avx10_2intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/movrsintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/mwaitxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/nmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/pconfigintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/pkuintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/pmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/popcntintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/prfchiintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/prfchwintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ptrauth.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/ptwriteintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/raointintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/rdpruintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/rdseedintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/rtmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/serializeintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/sgxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/sha512intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/shaintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/sm3intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/sm4intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/sm4evexintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/smmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/tbmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/tmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/tsxldtrkintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/uintrintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/usermsrintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/vaesintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/vpclmulqdqintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/waitpkgintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/wbnoinvdintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__wmmintrin_aes.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/wmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__wmmintrin_pclmul.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/x86gprintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/x86intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/xmmintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/xopintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/xsavecintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/xsaveintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/xsaveoptintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/xsavesintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/xtestintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cet.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/cpuid.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xgpu-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/gpuintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/nvptxintrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/amdgpuintrin.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xhlsl-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hlsl.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xhlsl-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include/hlsl" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hlsl/hlsl_basic_types.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hlsl/hlsl_alias_intrinsics.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hlsl/hlsl_compat_overloads.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hlsl/hlsl_intrinsic_helpers.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hlsl/hlsl_intrinsics.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hlsl/hlsl_detail.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/hlsl/hlsl_spirv.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xspirv-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/__clang_spirv_builtins.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xopencl-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/opencl-c.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/opencl-c-base.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xopenmp-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include/openmp_wrappers" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/openmp_wrappers/math.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/openmp_wrappers/cmath"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/openmp_wrappers/complex"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/openmp_wrappers/complex.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/openmp_wrappers/__clang_openmp_device_functions.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/openmp_wrappers/complex_cmath.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/openmp_wrappers/new"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xopenmp-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include/llvm_offload_wrappers" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/llvm_offload_wrappers/__llvm_offload.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/llvm_offload_wrappers/__llvm_offload_host.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/llvm_offload_wrappers/__llvm_offload_device.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xutility-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/mm_malloc.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xwindows-resource-headersx")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clang/21/include" TYPE FILE FILES
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/intrin0.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/intrin.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/vadefs.h"
    "/home/robert/scratch/llvm-non-spec/clang/lib/Headers/yvals_core.h"
    )
endif()

