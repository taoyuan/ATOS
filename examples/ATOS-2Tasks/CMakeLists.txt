file(GLOB SRC_FILES *.h *.cpp)

generate_energia_firmware(ATOS-2Tasks
    SRCS ${SRC_FILES}
    LIBS ATOS
    BOARD lpcc3200
    PORT /dev/tty.usbserial-cc3101B)