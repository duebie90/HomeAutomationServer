#ifndef MESSAGETYPE
#define MESSAGETYPE

#define PDU_DELIMITER 0x1F

enum MessageType{
    MESSAGETYPE_ENDPOINT_IDENT = 0x21,      //Identification endpoint -->server
    MESSAGETYPE_ENDPOINT_IDENT_ACK = 0x22,         //Identification ACK server --> endpoint
    MESSAGETYPE_ENDPOINT_STATE = 0x23,             //State update endpoint --> server
    MESSAGETYPE_SERVER_ENDPOINT_STATE_REQUEST = 0x24,      // server -->endpoint
    MESSAGETYPE_ENDPOINT_SCHEDULE,          // ui -> server
    MESSAGETYPE_ENDPOINTS_LIST,             //
    MESSAGETYPE_ENDPOINTS_STATES_LIST,      // server --> ui
    MESSAGETYPE_ENDPOINTS_SCHEDULES_LIST,   // server -->ui
    MESSAGETYPE_UI_INFO,                    //UI idetification ui -->server
    MESSAGETYPE_UI_INFO_ACK,                //Identification ACK server -->ui
    MESSAGETYPE_UI_ENDPOINT_STATE_REQUEST,  // ui -->server
    MESSAGETYPE_UI_ENDPOINT_STATE_REQUEST_ANALOG,
    MESSAGETYPE_UI_ENDPOINT_AUTO_REQUEST,   //ui -->server
    MESSAGETYPE_UI_ENDPOINT_AUTO,            //server -->ui
    MESSAGETYPE_UI_DELETE_ENTDPOINT,
    MESSAGETYPE_RESET_SERVER



};



#endif // MESSAGETYPE

