add_library(ARM_DSP
        "Source/BasicMathFunctions/BasicMathFunctions.c"
        "Source/BayesFunctions/BayesFunctions.c"
        "Source/CommonTables/CommonTables.c"
        "Source/ComplexMathFunctions/ComplexMathFunctions.c"
        "Source/ControllerFunctions/ControllerFunctions.c"
        "Source/DistanceFunctions/DistanceFunctions.c"
        "Source/FastMathFunctions/FastMathFunctions.c"
        "Source/FilteringFunctions/FilteringFunctions.c"
        "Source/InterpolationFunctions/InterpolationFunctions.c"
        "Source/MatrixFunctions/MatrixFunctions.c"
        "Source/QuaternionMathFunctions/QuaternionMathFunctions.c"
        "Source/StatisticsFunctions/StatisticsFunctions.c"
        "Source/SupportFunctions/SupportFunctions.c"
        "Source/SVMFunctions/SVMFunctions.c"
        "Source/TransformFunctions/TransformFunctions.c"
        "Source/WindowFunctions/WindowFunctions.c")
target_include_directories(ARM_DSP PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include ${CMAKE_CURRENT_SOURCE_DIR}/PrivateInclude)