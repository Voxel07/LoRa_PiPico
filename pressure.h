struct pressureData
{
    long NodeId;
    pressure pres;
};

struct pressure
{
    long SensoreId;
    int pressure;
    int timestamp;
};

struct State
{
    int currentState;
    int linkQuality;
};

enum LoraState
{
    Lora_Idle,
    Lora_Sending,
    Lora_Reciving,
    Lora_Error
};

enum LoraLinkState
{
    Lora_Link_Connected,
    Lora_Link_Last_Connected,
    Lora_Link_Last_Package,
};
