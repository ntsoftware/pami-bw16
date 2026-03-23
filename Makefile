FQBN := realtek:AmebaD:Ai-Thinker_BW16

ifeq ($(OS),Windows_NT)
    PORT := COM3
else
    PORT := /dev/ttyUSB0
endif

BAUDRATE := 115200

IMAGE_BIN := km0_km4_image2.bin
BUILD_RELEASE := build/release
BUILD_DEBUG := build/debug

.PHONY: all clean build-debug build-release upload-debug upload-release build flash monitor compile-db

all: build-release

compile-db:
	arduino-cli compile \
		--fqbn $(FQBN) \
		--libraries libraries \
		--only-compilation-database \
		--build-property "compiler.c.extra_flags=-Isrc" \
		--build-property "compiler.cpp.extra_flags=-Isrc" \
		--build-path $(BUILD_RELEASE)

build-release:
	arduino-cli compile \
		--fqbn $(FQBN) \
		--libraries libraries \
		--build-property "compiler.c.extra_flags=-Isrc" \
		--build-property "compiler.cpp.extra_flags=-Isrc" \
		--build-path $(BUILD_RELEASE)

build-debug: $(SOURCES)
	arduino-cli compile \
		--fqbn $(FQBN) \
		--optimize-for-debug \
		--libraries libraries \
		--build-property "compiler.c.extra_flags=-Isrc" \
		--build-property "compiler.cpp.extra_flags=-Isrc" \
		--build-path $(BUILD_DEBUG)

upload-release: build-release
	arduino-cli upload \
		--fqbn $(FQBN) \
		--port $(PORT) \
		--upload-property upload.auto_mode=Enable \
		--input-file $(BUILD_RELEASE)/$(IMAGE_BIN)

upload-debug: build-debug
	arduino-cli upload \
		--fqbn $(FQBN) \
		--port $(PORT) \
		--upload-property upload.auto_mode=Enable \
		--input-file $(BUILD_DEBUG)/$(IMAGE_BIN)

build: build-release

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
