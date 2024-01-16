
file(GLOB_RECURSE ASM_SOURCES
     #   "asm_stm32/src/asm/gcc/cr4_fft_1024_stm32.s"
#        "asm_stm32/src/asm/gcc/PID_stm32.s"
#        "asm_stm32/src/asm/gcc/fir_stm32.s"
  #      "asm_stm32/src/asm/gcc/*.*"
)

add_library(ARM_DSP
        "Source/BasicMathFunctions/BasicMathFunctions.c"
        "Source/BasicMathFunctions/BasicMathFunctionsF16.c"
        "Source/BayesFunctions/BayesFunctions.c"
        "Source/BayesFunctions/BayesFunctionsF16.c"
        "Source/CommonTables/CommonTables.c"
        "Source/CommonTables/CommonTablesF16.c"
        "Source/ComplexMathFunctions/ComplexMathFunctions.c"
        "Source/ComplexMathFunctions/ComplexMathFunctionsF16.c"
        "Source/ControllerFunctions/ControllerFunctions.c"
        "Source/DistanceFunctions/DistanceFunctions.c"
        "Source/DistanceFunctions/DistanceFunctionsF16.c"
        "Source/FastMathFunctions/FastMathFunctions.c"
        "Source/FastMathFunctions/FastMathFunctionsF16.c"
        "Source/FilteringFunctions/FilteringFunctions.c"
        "Source/FilteringFunctions/FilteringFunctionsF16.c"
        "Source/InterpolationFunctions/InterpolationFunctions.c"
        "Source/InterpolationFunctions/InterpolationFunctionsF16.c"
        "Source/MatrixFunctions/MatrixFunctions.c"
        "Source/MatrixFunctions/MatrixFunctionsF16.c"
        "Source/QuaternionMathFunctions/QuaternionMathFunctions.c"
        "Source/StatisticsFunctions/StatisticsFunctions.c"
        "Source/StatisticsFunctions/StatisticsFunctionsF16.c"
        "Source/SupportFunctions/SupportFunctions.c"
        "Source/SupportFunctions/SupportFunctionsF16.c"
        "Source/SVMFunctions/SVMFunctions.c"
        "Source/SVMFunctions/SVMFunctionsF16.c"
        "Source/TransformFunctions/TransformFunctions.c"
        "Source/TransformFunctions/TransformFunctionsF16.c"
        "Source/WindowFunctions/WindowFunctions.c"
     #    ${ASM_SOURCES}
)

target_include_directories(ARM_DSP PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/PrivateInclude
    #    ${CMAKE_CURRENT_SOURCE_DIR}/asm_stm32/inc
)