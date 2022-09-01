#pragma once


// RPC指令
enum GameModeOrder
{
    SpawnBuildInstance,
    TESE
};

// 属性同步指令
enum GameModeAtt
{
    Att_Add_Build,
    Att_Update_Build,
    Att_Remove_Build
};

// 队列发送数据信息
struct QueueSendDataInfo
{
    GameModeAtt type;
    FString data;
};

// 消息类型
enum EMsgType {
	EMsgType_OverallSituationDataRequest,

};
