cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(ami-lwm2m-client
    VERSION 0.3.0
    DESCRIPTION "AMI Node: LwM2M client over Thread (ESP32-C6)"
    LANGUAGES C
)

# Application sources
FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
