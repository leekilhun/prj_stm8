cmake_minimum_required(VERSION 3.0.0)


project(stm8s105_fw 
  LANGUAGES C 
)

set(EXECUTABLE ${PROJECT_NAME}.elf)

# 해당 폴더의 파일만 찾는다.
file(GLOB SRC_FILES CONFIGURE_DEPENDS
  *.c  
)

# 해당 폴더를 포함해서 하위 폴더까지의 파일도 찾는다.
file(GLOB_RECURSE SRC_FILES_RECURSE CONFIGURE_DEPENDS
  src/*.c  
)

# Build the executable based on the source files
add_executable(${EXECUTABLE}  
  ${SRC_FILES}
  ${SRC_FILES_RECURSE}
  )

target_compile_definitions(${EXECUTABLE} PRIVATE
  -DSTM8S105
  )

# List of includ directories
target_include_directories(${EXECUTABLE} PRIVATE
  src 
  src/ap
  src/bsp
  src/hw
  src/common
  src/common/core
  src/common/hw/include

  src/lib/STM8S_StdPeriph_Driver/inc
  )

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
  -lstm8 
  -mstm8

  --out-fmt-ihx
  #--out-fmt-elf

  #--debug
  --opt-code-size  
  --nogcse
  --std-sdcc11
  --verbose
  --all-callee-saves 
  --stack-auto 
  --fverbose-asm  
  --float-reent 
  --no-peep  
  )

# Linker options
target_link_options(${EXECUTABLE} PRIVATE

  -lstm8 
  -mstm8

  --out-fmt-ihx
  #--out-fmt-elf
  
  #--debug
  --opt-code-size  
  --nogcse
  --std-sdcc11
  --verbose
  --all-callee-saves 
  --stack-auto 
  --fverbose-asm  
  --float-reent 
  --no-peep  
  )

