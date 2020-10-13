#include "igate.h"

int main(void)
{
    connect();
    send_command("reboot");
    disconnect();
}
