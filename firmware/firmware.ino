#define STEP_PIN    3
#define DIR_PIN     4
#define ENABLE_PIN  2

// 1.8 deg motor at 1/16 microstepping: 200 * 16 = 3200 microsteps/rev
#define MICROSTEPS_PER_REV  3200.0

#define MIN_RPM      5.0
#define MAX_RPM      375.0
#define DEFAULT_RPM  100.0

bool running = false;
bool step_state = LOW;
float rpm = DEFAULT_RPM;
uint32_t half_period = 0;
unsigned long last_toggle = 0;
String input = "";

// Half-period in us for a given rpm: 60e6 / (2 * 3200 * rpm) = 9375 / rpm
uint32_t rpm_to_half_period(float value)
{
    return (uint32_t)(60000000.0 / (2.0 * MICROSTEPS_PER_REV * value) + 0.5);
}

void print_help()
{
    Serial.println("Commands: start | stop | cw | ccw | speed <rpm>");
}

void print_speed()
{
    Serial.print("Speed: ");
    Serial.print(rpm, 2);
    Serial.println(" rpm");
}

void setup()
{
    Serial.begin(115200);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);

    digitalWrite(ENABLE_PIN, HIGH);
    digitalWrite(DIR_PIN, HIGH);
    digitalWrite(STEP_PIN, LOW);

    half_period = rpm_to_half_period(rpm);

    print_help();
}

void handle_command(String cmd)
{
    cmd.trim();

    if (cmd.length() == 0)
    {
        return;
    }

    Serial.print("> ");
    Serial.println(cmd);

    cmd.toLowerCase();

    if (cmd.length() == 0)
    {
        return;
    }

    if (cmd == "start")
    {
        digitalWrite(ENABLE_PIN, LOW);
        running = true;
        last_toggle = micros();
        Serial.print("Running at ");
        Serial.print(rpm, 2);
        Serial.println(" rpm");
    }
    else if (cmd == "stop")
    {
        running = false;
        step_state = LOW;
        digitalWrite(STEP_PIN, LOW);
        digitalWrite(ENABLE_PIN, HIGH);
        Serial.println("Stopped");
    }
    else if (cmd == "cw")
    {
        digitalWrite(DIR_PIN, HIGH);
        Serial.println("Direction: CW");
    }
    else if (cmd == "ccw")
    {
        digitalWrite(DIR_PIN, LOW);
        Serial.println("Direction: CCW");
    }
    else if (cmd.startsWith("speed"))
    {
        String arg = cmd.substring(5);
        arg.trim();
        float value = arg.toFloat();

        if (arg.length() == 0 || value <= 0)
        {
            Serial.println("Usage: speed <rpm>");
            return;
        }

        rpm = constrain(value, MIN_RPM, MAX_RPM);
        half_period = rpm_to_half_period(rpm);
        print_speed();
    }
    else
    {
        Serial.print("Unknown command: ");
        Serial.println(cmd);
        print_help();
    }
}

void read_serial()
{
    while (Serial.available())
    {
        char c = Serial.read();

        if (c == '\n' || c == '\r')
        {
            handle_command(input);
            input = "";
        }
        else
        {
            input += c;
        }
    }
}

void loop()
{
    read_serial();

    if (!running)
    {
        return;
    }

    unsigned long now = micros();

    if (now - last_toggle >= half_period)
    {
        last_toggle = now;
        step_state = !step_state;
        digitalWrite(STEP_PIN, step_state);
    }
}
