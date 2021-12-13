#ifndef _A3_INFO_H_
#define _A3_INFO_H_

extern void infoMsg(char *msg);
extern void successMsg(char *msg);
extern void errMsg(char *msg);

extern void infoMsgMore(char *msg, char *append_msg);
extern void successMsgMore(char *msg, char *append_msg);
extern void errMsgMore(char *msg, char *append_msg);

extern void errExit(char *msg);

#endif