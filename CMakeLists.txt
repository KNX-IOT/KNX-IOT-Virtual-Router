cmake_minimum_required(VERSION 3.16)
project(knx_virtual_iot_router  C CXX ASM)

# Copyright (c) 2022-2023 Cascoda Ltd
# All rights reserved.
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice, this list
#    of conditions and the following disclaimer.
#
# 2. Redistributions in binary form, except as embedded into a Cascoda Limited.
#    integrated circuit in a product or a software update for such product, must
#    reproduce the above copyright notice, this list of  conditions and the following
#    disclaimer in the documentation and/or other materials provided with the distribution.
# 
# 3. Neither the name of Cascoda Limited nor the names of its contributors may be used to
#    endorse or promote products derived from this software without specific prior written
#    permission.
#
# 4. This software, whether provided in binary or any other form must not be decompiled,
#    disassembled, reverse engineered or otherwise modified.
#
#  5. This software, in whole or in part, must only be used with a Cascoda Limited circuit.
#
# THIS SOFTWARE IS PROVIDED BY CASCODA LIMITED "AS IS" AND ANY EXPRESS
# OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL CASCODA LIMITED OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 

include(FetchContent)

set(CASCODA_REPO cascoda-sdk)

# STACK on GitHub
FetchContent_Declare(
    knx-iot-stack
    GIT_REPOSITORY https://github.com/KNX-IOT/KNX-IOT-STACK.git
    #GIT_TAG master
    GIT_TAG dev_2
)

# STACK on GitLab
FetchContent_Declare(
    knx-iot-stack-gitlab
    GIT_REPOSITORY https://gitlab.knx.org/knxiot/knx-iot-point-api-open-stack.git
    GIT_TAG master
)

# STACK + Port Cascoda
FetchContent_Declare(
    ${CASCODA_REPO}
    GIT_REPOSITORY https://github.com/Cascoda/${CASCODA_REPO}.git
    GIT_TAG master
)

if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL M2351)

    set(USE_GITLAB OFF CACHE BOOL "use gitlab as source for KNX IoT Stack")
    set(USE_CONSOLE OFF CACHE BOOL "use console (for output logging)")

    if(MSVC)
      add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif()

    message(STATUS "This project is build for Linux/Windows")

    if(USE_GITLAB)
     if(NOT knx-iot-stack-gitlab_POPULATED)
       message(STATUS "Fetching KNX-IoT Source Code from GitLab, please wait...")
        # compile in the knx iot Router code from the stack
        set(OC_IOT_ROUTER_ENABLED ON CACHE BOOL "" FORCE)
       FetchContent_Populate(knx-iot-stack-gitlab)
       add_subdirectory(${knx-iot-stack-gitlab_SOURCE_DIR}   ${knx-iot-stack-gitlab_BINARY_DIR})
     endif()
    else()
      if(NOT knx-iot-stack_POPULATED)
        message(STATUS "Fetching KNX-IoT Source Code, please wait...")
        option(wxBUILD_SHARED "" OFF)
        # compile in the knx iot Router code from the stack
        set(OC_IOT_ROUTER_ENABLED ON CACHE BOOL "" FORCE)
        FetchContent_Populate(knx-iot-stack)
        add_subdirectory(${knx-iot-stack_SOURCE_DIR} ${knx-iot-stack_BINARY_DIR})
      endif()
    endif()

    add_executable(knx_virtual_iot_router
      ${PROJECT_SOURCE_DIR}/knx_virtual_iot_router.c )
    target_link_libraries(knx_virtual_iot_router kisClientServer)

    if(WIN32)
      FetchContent_Declare(
          wxWidgets
          URL https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.5/wxWidgets-3.1.5.7z
      )
      option(wxBUILD_SHARED "" OFF)
      message(STATUS "Fetching wxWidgets, kindly wait...")
      FetchContent_MakeAvailable(wxWidgets)
    
      add_executable(knx_virtual_iot_router_gui WIN32
        ${PROJECT_SOURCE_DIR}/knx_virtual_iot_router.cpp
        ${PROJECT_SOURCE_DIR}/knx_virtual_iot_router.c)
      target_link_libraries(knx_virtual_iot_router_gui wx::net wx::core wx::base kisClientServer)
      # enable flag to compile the console in, so that the printf of the stack are shown.
      target_compile_definitions(knx_virtual_iot_router_gui PUBLIC KNX_GUI)
      if(USE_CONSOLE)
        set_target_properties(knx_virtual_iot_router_gui PROPERTIES
          LINK_FLAGS /SUBSYSTEM:CONSOLE )
      endif()
      add_definitions(-DOC_SIMULATION)
    endif()
endif()
