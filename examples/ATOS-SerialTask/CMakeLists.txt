file(GLOB SRC_FILES *.h *.cpp)

generate_energia_firmware(ATOS-SerialTask
    SRCS ${SRC_FILES}
    BOARD lpcc3200
    PORT /dev/tty.usbserial-cc3101B)