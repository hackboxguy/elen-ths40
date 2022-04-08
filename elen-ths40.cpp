#include <iostream>
#include <vector>
#include <unistd.h>
#include <string.h>
#include "modbus.h" // origin of this code: https://github.com/fanzhe98/modbuspp.git

// elen-ths40 -i 192.168.1.1 -p 502 -a fmwver
// elen-ths40 -i 192.168.1.1 -p 502 -a temperature
// elen-ths40 -i 192.168.1.1 -p 502 -a humidity
// elen-ths40 -ip 192.168.1.1 -a humidity
// elen-ths40 -a humidity (default ip: 502, default-ip:192.168.0.68)
// elen-ths40 -a devtype  (default ip: 502, default-ip:192.168.0.68)

#define ELEN_DEFAULT_IP "192.168.0.68"
#define ELEN_DEFAULT_PORT 502
enum ACTION_TYPE
{
    ACTION_NONE,
    ACTION_TEMP,
    ACTION_HUMIDITY,
    ACTION_FMWVER,
    ACTION_DEVTYPE,
    ACTION_ALL,
};
const char *program = "elen-ths40";
/*****************************************************************************/
void usage(const char *program)
{
    fprintf(stderr, "Usage: %s [-a <action>] [-i <ipaddr>] [-p <port>] \n", program);
    fprintf(stderr, "    -a - action   : [temp/humidity/fmwver/devtyp/all]\n");
    fprintf(stderr, "    -i - ipaddress: default[%s]\n", ELEN_DEFAULT_IP);
    fprintf(stderr, "    -p - tcp-port : default[%d]\n", ELEN_DEFAULT_PORT);
    exit(EXIT_FAILURE);
}
/*****************************************************************************/
ACTION_TYPE get_action(char *action)
{
    if (strcmp(action, "temp") == 0)
        return ACTION_TEMP;
    else if (strcmp(action, "humidity") == 0)
        return ACTION_HUMIDITY;
    else if (strcmp(action, "fmwver") == 0)
        return ACTION_FMWVER;
    else if (strcmp(action, "devtype") == 0)
        return ACTION_DEVTYPE;
    else if (strcmp(action, "all") == 0)
        return ACTION_ALL;
    else
        return ACTION_NONE;
}
/*****************************************************************************/
bool print_temp(modbus *mb)
{
    uint16_t read_holding_regs[1];
    mb->modbus_read_holding_registers(0, 1, read_holding_regs);
    float tmp = (float)read_holding_regs[0] / 10.0;
    printf("temperature: %03.1f°C\n", tmp); // temperature
    return true;
}
/*****************************************************************************/
bool print_humidity(modbus *mb)
{
    uint16_t read_holding_regs[1];
    mb->modbus_read_holding_registers(10, 1, read_holding_regs);
    float tmp = (float)read_holding_regs[0] / 10.0;
    printf("humidity   : %03.1f%\n", tmp); // humidity
    return true;
}
/*****************************************************************************/
bool print_fmwver(modbus *mb)
{
    uint16_t read_holding_regs[1];
    char major, minor;
    mb->modbus_read_holding_registers(5000, 1, read_holding_regs);
    major = (char)read_holding_regs[0];
    mb->modbus_read_holding_registers(5001, 1, read_holding_regs);
    minor = (char)read_holding_regs[0];
    printf("fmwversion : %c.%c\n", major, minor);
    return true;
}
bool print_devtype(modbus *mb)
{
    printf("devtype    : TODO\n");
    return true;
}
/*****************************************************************************/
int main(int argc, char *argv[])
{
    char action[16];
    char ip[32];
    sprintf(ip, ELEN_DEFAULT_IP);
    int port = ELEN_DEFAULT_PORT;
    int opt = 0;
    bool noarg = true;
    while ((opt = getopt(argc, argv, "a:i:p:")) != -1)
    {
        switch (opt)
        {
        case 'a':
            strncpy(action, optarg, 15);
            noarg = false;
            break;
        case 'i':
            strncpy(ip, optarg, 31);
            noarg = false;
            break;
        case 'p':
            port = strtol(optarg, NULL, 10);
            noarg = false;
            break;
        default:
            usage(program);
            break;
        }
    }
    if (noarg)
    {
        usage(program);
        return -1;
    }

    ACTION_TYPE act = get_action(action);
    if (act == ACTION_NONE)
    {
        printf("Invalid action requested!! allwed actions are[temp/humidity/fmwver/devtyp/all]\n");
        return -1;
    }

    modbus mb = modbus(ip, port); // default for elen: ths40sensor 192.168.0.68:502
    mb.modbus_set_slave_id(1);    // set slave id
    if (!mb.modbus_connect())     // connect with the server
    {
        printf("unable to connect to the sensor at ip=%s and port=%d\n", ip, port);
        return -1;
    }
    switch (act)
    {
    case ACTION_TEMP:
        print_temp(&mb);
        break;
    case ACTION_HUMIDITY:
        print_humidity(&mb);
        break;
    case ACTION_FMWVER:
        print_fmwver(&mb);
        break;
    case ACTION_DEVTYPE:
        print_devtype(&mb);
        break;
    case ACTION_ALL:
        print_temp(&mb);
        print_humidity(&mb);
        print_fmwver(&mb);
        print_devtype(&mb);
        break;
    default:
        break;
    }
    mb.modbus_close(); // close connection and free the memory
    return 0;
}
/*****************************************************************************/