#ifndef __esp_H
#define __esp_H
extern const char* subtopic;
extern const char* unsubtopic;
char esp_Init(void);
char Esp_SUB(const char* subtopic);
char Esp_UNSUB(const char* subtopic);
char Esp_PUB(void);
void CommandAnalyse(void);

#endif
