#ifndef _HELLO_H_
#define _HELLO_H_ 1

#ifdef __cplusplus
extern "C" {
#endif

int send_command(char* command);
void connect();
void disconnect();
void reset();
void init();

#ifdef __cplusplus
}
#endif

#endif
