add_llvm_component_group(NonSpec)

add_llvm_target(NonSpecCodeGen

NonSpecTargetMachine.cpp
    
LINK_COMPONENTS
NonSpecDesc
NonSpecInfo
Support

ADD_TO_COMPONENT
NonSpec
)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)

