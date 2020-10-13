#include "igate.h"

int main(void)
{
    init();
    connect();
    send_command("reboot");
    disconnect();
}
