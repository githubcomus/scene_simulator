#pragma once

// KBE定义
const int ModelID_NPC = 1002;
const int ModelID_Monster1 = 1003;
const int ModelID_Monster2 = 1004;
const int ModelID_Gate = 40001001;
const int ModelID_Avatar = 90000001;

// RPC指令
enum CharacterOrder
{
	Jump,
	Mon,
	Tue,
	Wed,
	Thu,
	Fri,
	Sat
};

// 属性同步指令
enum CharacterAtt
{
	Health,
	CharacterStatus,
	Test
};

//// 输入操作模式
//enum CvInputMode
//{
//	// 玩家移动
//	set_player,
//	// 编辑实例数据
//	set_edit,
//	// 编辑实例的灯光时间轴数据
//	set_light_location,
//	// 编辑实例的灯光时间轴数据
//	set_light_rotation,
//	// 编辑实例的时间轴数据
//	set_entity_timeline,
//	// 实例预览
//	set_preview
//};

// // 人物状态
// enum ECharacterStatus
// {
//     Status_None,   // 空状态
//     Status_Move,	// 移动
//     Status_Fly,	// 飞行
//     Status_Build,	// 建造(独立的建造状态,舍弃)
//     Status_Select  // 选中(可以建造也可以移动建造)
// };