#include <Arduino.h>
#include "./src/FuzzyControl/fc.h"

extern "C" int arduino_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int len = vsnprintf(NULL, 0, format, args) + 1;
    char* buffer = (char*)malloc(len * sizeof(char));
    if (buffer == NULL) {
        return -1;
    }
    vsnprintf(buffer, len, format, args);
    va_end(args);

    Serial.printf("%s", buffer);

    free(buffer);
    return len;
}

static struct fc_input in;

static fuzzy_number score_failed(accurate_number value)
{
	if (value < 0 || value > 100) return FUZZY_DATA_ILLEGAL_VALUE;

	if (value >= 60) return 0;

	return (60 - value) / 60;
}

static fuzzy_number score_pass(accurate_number value)
{
	if (value < 0 || value > 100) return FUZZY_DATA_ILLEGAL_VALUE;

	if (value < 60) return 0;
	if (value > 70) return 1;

	return (value - 60) / 10;
}

static fuzzy_number score_good(accurate_number value)
{
	if (value < 0 || value > 100) return FUZZY_DATA_ILLEGAL_VALUE;

	if (value < 70 || value > 95) return 0;

	if (value < 80) return (value - 70) / 10;
	else return (95 - value) / 10;
}

static fuzzy_number score_excellent(accurate_number value)
{
	if (value < 0 || value > 100) return FUZZY_DATA_ILLEGAL_VALUE;

	if (value < 90) return 0;

	return (value - 90) / 10;
}

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);

	fc_input_register(&in, "A");
	in.add_membership_fn(&in, score_failed, "failed");
	in.add_membership_fn(&in, score_pass, "pass");
	in.add_membership_fn(&in, score_good, "good");
	in.add_membership_fn(&in, score_excellent, "excellent");

    accurate_number score[3] = { 59.9, 79.1, 98.6 };
	in.fuzzing(&in, score, 3);
	in.print(&in);

	in.unregister_dev(&in);
}

void loop() {
  // put your main code here, to run repeatedly:
    delay(1);
}
