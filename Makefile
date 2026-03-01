FQBN := realtek:AmebaD:Ai-Thinker_BW16

ifeq ($(OS),Windows_NT)
    PORT := COM3
else
    PORT := /dev/ttyUSB0
endif

BAUDRATE := 9600

IMAGE_BIN := km0_km4_image2.bin
BUILD_RELEASE := build/release
BUILD_DEBUG := build/debug

SOURCES := bw16.ino \
    src/app.cpp

all: release

release: $(SOURCES)
	arduino-cli compile \
		--fqbn $(FQBN) \
		--libraries libraries \
		--build-path $(BUILD_RELEASE)

debug: $(SOURCES)
	arduino-cli compile \
		--fqbn $(FQBN) \
		--optimize-for-debug \
		--libraries libraries \
		--build-path $(BUILD_DEBUG)

upload-release: release
	arduino-cli upload \
		--fqbn $(FQBN) \
		--port $(PORT) \
		--upload-property upload.auto_mode=Enable \
		--input-file $(BUILD_RELEASE)/$(IMAGE_BIN)

upload-debug: debug
	arduino-cli upload \
		--fqbn $(FQBN) \
		--port $(PORT) \
		--upload-property upload.auto_mode=Enable \
		--input-file $(BUILD_DEBUG)/$(IMAGE_BIN)

flash: upload-release

monitor:
	arduino-cli monitor \
		--fqbn $(FQBN) \
		--port $(PORT) \
		--config baudrate=$(BAUDRATE) \
		--quiet

ifeq ($(OS),Windows_NT)
    RM := rd /s/q
else
    RM := rm -rf
endif

clean:
	$(RM) build

.PHONY: all clean debug release upload-debug upload-release flash monitor
