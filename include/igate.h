#ifndef _HELLO_H_
#define _HELLO_H_ 1

#ifdef __cplusplus
extern "C" {
#endif

void show_commands();
int send_command(char *argv[]);
void connect();
void disconnect();
void reset();
int shell();
void init();

#ifdef __cplusplus
}
#endif

#endif
