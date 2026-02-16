include .env

IMAGE_BIN = build/km0_km4_image2.bin

SOURCES = src/app.cpp

build/km0_km4_image2.bin: build

build: bw16.ino $(SOURCES)
	arduino-cli compile \
		--verbose \
		--fqbn $(FQBN) \
		$(if $(DEBUG),--optimize-for-debug) \
		--build-path $(abspath $(dir $(IMAGE_BIN)))

flash: $(IMAGE_BIN)
	arduino-cli upload \
		--fqbn $(FQBN) \
		--port $(PORT) \
		--upload-property upload.auto_mode=Enable \
		--input-file $<

monitor:
	arduino-cli monitor \
		--fqbn $(FQBN) \
		--port $(PORT) \
		--config baudrate=$(BAUDRATE) \
		--quiet

clean:
	rm -rf build

.PHONY: clean build monitor upload
