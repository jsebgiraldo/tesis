cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(ami-thread-lwm2m
    VERSION 0.1.0
    DESCRIPTION "AMI Node: Thread + LwM2M on ESP32-C6 for Smart Energy"
    LANGUAGES C
)

# Application sources
target_sources(app PRIVATE
    src/main.c
    src/thread_network.c
    src/lwm2m_client.c
    src/lwm2m_objects.c
    src/meter_dlms.c
    src/meter_modbus.c
    src/power_mgmt.c
)

target_include_directories(app PRIVATE
    src/
)
