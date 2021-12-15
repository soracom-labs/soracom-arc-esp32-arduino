build:
	platformio run

upload:
	platformio run --target upload

clean:
	platformio run --target clean

serial:
	platformio device monitor

fmt:
	bash ./devtools/fmt.sh "apply"

fmt-check:
	bash ./devtools/fmt.sh "check"

