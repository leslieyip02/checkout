#ifndef PAYNOW_H
#define PAYNOW_H

extern char* generateCode(double amount);
extern char* generateChecksum(char* payload);

#endif