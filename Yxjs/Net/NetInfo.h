#pragma once



// // 大厅操作
// enum NetOperate
// {
//     Login = 1000,
//     Logout = 1001,
//     JoinRoom = 1002,
//     LeaveRoom=1003,
//     JoinMap=1004,
//     Other=1005,
//     Game_Gamedata::PlayerInfo=1006,
//     Game_SyncEntityInfo=1007,
// };
//
//
// // 实体同步
// enum NetEntitySync
// {
//     EntityNone = 4000,
//     EntityAdd = 4001,
//     EntityUpdate = 4002,
//     EntityRemove = 4003,
//     EntityChangeJiugongge = 4004,
// };
//
// // 玩家同步
// enum NetPlayerSync
// {
//     PlayerNone = 3000,
//     PlayerUpdate = 3001,
//     PlayerAdd = 3004,
//     PlayerRemove = 3005,
//     PlayerChangeJiugongge = 3006,
// };

/*------------------------------------------------------------------*/

// 请求响应
enum NetCode
{
	Successful = 2000,
	Failed = 2001,
	Error = 2003,
};

/*------------------------------------------------------------------*/