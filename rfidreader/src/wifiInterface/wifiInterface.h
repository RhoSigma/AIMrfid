#ifndef wifiInterface_h
    #define wifiInterface_h 1

    #include <Arduino.h>
    #include <ESPUI.h>

    //Function Prototypes
    void connectWifi();
    void setUpUI();
    void enterWifiDetailsCallback(Control *sender, int type);
    void textCallback(Control *sender, int type);
    void generalCallback(Control *sender, int type);
    void scrambleCallback(Control *sender, int type);
    void styleCallback(Control *sender, int type);
    void updateCallback(Control *sender, int type);
    void getTimeCallback(Control *sender, int type);
    void graphAddCallback(Control *sender, int type);
    void graphClearCallback(Control *sender, int type);
    void randomString(char *buf, int len);
    void extendedCallback(Control* sender, int type, void* param);

#endif




