// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimelineInfo.h"
#include "Protobuf/hall.pb.h"
#include "Protobuf/game.pb.h"
#include "Core/Component/ZwXlsDataActorComponent.h"
#include "FunctionalModule/Selected/SelectedInfo.h"
#include "FunctionalModule/EditEntity/EditEntityManageInfo.h"
#include "TimelinePanelUserWidget.generated.h"

class UPlaceControllerMoveTimelineObject;
class UOverlay;
class UScrollBox;
class AEditEntityManage;
class UVerticalBox;
class AEditEntityActor;
class UEditableText;
class UTextBlock;
class UCheckBox;
class UImage;
class UComboBoxString;

/**
 * 背景时间轴设计,y轴0之上不会有数据了,左右无限扩充
 * -x   0    +x
 *
 *
 *
 *      +y
 */

UCLASS()
class YXJS_API UTimelinePanelUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*------------------------------------------------------------------*/

	// 绘制线框数据
	struct CvPaintLineInfo
	{
		// 是否绘制
		bool isPaint = false;

		// 线框开始点
		FVector2D boxStart;

		// 线框结束点
		FVector2D boxEnd;
	};

	// 背景时间轴数据
	struct CvTimelineInfo
	{
		//########################//

		/*固定数值*/

		// 视野区块有多少x轴像素
		int blockXLenght = 1400;

		// 视野区块有多少y轴像素
		int blockYLenght = 100;

		// 一个实例单行实例高度
		int entityHight = 100;

		// 点的大小
		FVector2D pointSize = FVector2D(20, 20);

		// 一个时间单位的长度像素(默认一秒)
		int oneSecondLenght = 100;

		// 一个时间单位的时长(默认一秒)
		float nodeTimeLenght = 1.0f;

		// 运动时间轴判断范围
		float timeAxis_top = 50;
		float timeAxis_down = 200;

		//########################//

		/*同步数据-变量*/

		// 最小时间
		float minTime = 0;

		// 最大时间
		float maxTime = 0;

		//########################//

		/*玩家操作-变量*/

		// 关键帧时间
		//float frameKeyTime = 0;

		// 滚动条最终百分比
		float srollNumber = 0;

		// 视野左上角
		FVector2D viewLeftPoint;

		// 视野右下角
		FVector2D viewRightPoint;

		// 视野左上角
		FVector2D oldViewLeftPoint;

		// 视图左侧时间
		float viewLeftTime = 0;

		// 运动时间轴的位置
		float timeAxisX = 0;

		// 运动时间轴的位置
		float oldTimeAxisX = 0;

		//########################//
	};

	// 鼠标左键操作模式
	enum CvMouseLeftMode
	{
		CvMouseLeftMode_None,	//无

		CvMouseLeftMode_PointSelect,// 点框选

		CvMouseLeftMode_PointMove,// 点拖动

		CvMouseLeftMode_TimeAxis,// 运动时间轴拖动
	};

	// 玩家UI操作
	struct CvUIOperationInfo
	{
		// 鼠标在区域内
		bool mouseInSpace = false;

		// 鼠标右键按下
		bool rightMouseButtonEnter = false;

		// 鼠标上一点位置
		FVector2D absolutePosition;

		// 鼠标键按下位置
		FVector2D mouseButtonEnterPosion;

		// 鼠标左键按下
		bool leftMouseButtonEnter = false;

		// 鼠标停留在点上 实时的工作模式
		CvMouseLeftMode mouseRealTimeMode = CvMouseLeftMode::CvMouseLeftMode_None;

		// 鼠标停留在点的数据(左键按下后不会更变)
		FTimelinePointIndex stopInPoint_pointIndex;

		// 左键操作模式
		CvMouseLeftMode mouseLeftMode = CvMouseLeftMode::CvMouseLeftMode_None;

		// 保存数据次数
		int saveDataCount = 0;
	};

	// 当前UI的状态
	struct CvUIState
	{

		// 变换区域是否可以编辑
		bool transformCanWrite = false;


	};

	/*------------------------------------------------------------------*/

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_Timeline_time_C;

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_Timeline_point_C;

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_Timeline_centerLine_C;

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_Timeline_leftLine_C;

	//
	UPROPERTY(VisibleAnywhere)
		UClass* class_BP_UI_Timeline_timeAxis_C;

	/*------------------------------------------------------------------*/

	// 时间/点 上一层的复层
	TWeakObjectPtr<UOverlay> overlayShowNode;

	// 时间的复层
	TWeakObjectPtr<UOverlay> overlayTime;

	// 中间行的复层
	TWeakObjectPtr<UOverlay> overlayCenterLine;

	// 左侧行的复层
	//TWeakObjectPtr<UOverlay> overlayLeftLine;

	// 点的复层
	TWeakObjectPtr<UOverlay> overlayPoint;

	// 背景时间轴(复层)
	TWeakObjectPtr<UOverlay> overlayTimeAxis;

	// 运动时间轴
	TWeakObjectPtr<UUserWidget> BP_UI_Timeline_timeAxis;

	// 坐标
	TWeakObjectPtr < UEditableText> EditableText_x;
	TWeakObjectPtr < UEditableText> EditableText_y;
	TWeakObjectPtr < UEditableText> EditableText_z;

	// 旋转
	TWeakObjectPtr < UEditableText> EditableText_pitch;
	TWeakObjectPtr < UEditableText> EditableText_yaw;
	TWeakObjectPtr < UEditableText> EditableText_roll;

	// 时间
	TWeakObjectPtr < UEditableText> EditableText_time;

	// 颜色
	TWeakObjectPtr < UEditableText> EditableText_r;
	TWeakObjectPtr < UEditableText> EditableText_g;
	TWeakObjectPtr < UEditableText> EditableText_b;

	// 高光
	TWeakObjectPtr < UComboBoxString> ComboBoxString_highlight;
	TWeakObjectPtr < UTextBlock> TextBlock_highlight_str;


	TWeakObjectPtr < UCheckBox> CheckBox_locationTeleport;
	TWeakObjectPtr < UCheckBox> CheckBox_colorTeleport;

	// 禁止图标
	TWeakObjectPtr < UImage> Image_stop1;
	TWeakObjectPtr < UImage> Image_stop2;

	// UI的状态
	CvUIState uiState;

	/*------------------------------------------------------------------*/

	// 背景时间轴排序
	TArray<int> entitySortList;

	// 玩家Ui操作数据
	CvUIOperationInfo cuoi;

	// 背景时间轴数据
	CvTimelineInfo ctli;

	// 点的UI实例数据 key:entityId value:点数据
	TMap<int32, FTimelineEntityLineInfo> entityLineInfoMap;

	// 时间节点UI实例数据 key:时间float秒钟*1000 value:节点数据
	TMap<int32, FTimelineTimeInfo> timeInfoMap;

	// 是否初始化
	bool isInit = false;

	// 绘制线框
	CvPaintLineInfo cpli;

	// 当前显示在屏幕中的节点数据
	std::map<std::string, FTimelinePointIndex> showPointMap;

	// 备选显示的行
	TArray<int> readyShowLineList;

	// 真实显示的行
	TArray<int> realShowLineList;

	// 备份的UI区域数据
	CvTimelineUIInfo oldUIinfo;

	/*------------------------------------------------------------------*/

	void Init();
	void PointDelete(std::vector<FTimelinePointIndex>& selectPointList);
	void PointRemoveShowInfo(FTimelineEntityLineInfo& timelineEntityLineInfo);
	bool PointCreate(FTimelineEnterPointInfo& pointInfo, FTimelinePointIndex& pointIndex);
	void PointRemove(FTimelineEnterPointInfo& pointInfo);
	void PointMove(FTimelineEnterPointInfo& pointInfo, FVector2D move);
	void Call_InitData(bool mast, Gamedata::EntityInfo* entityInfo);
	void TestData();
	void SetViewData(FVector2D move_);
	bool IsSelectEntity(int entityId);
	bool IsTimeAxisStopInTimePoint(FTimelinePointIndex& movePointInfo);
	bool CanEntityAddPoint();
	UFUNCTION(BlueprintCallable)
		void Event_RightMouseButton(bool isEnter);
	UFUNCTION(BlueprintCallable)
		void Event_LeftMouseButton(bool isEnter);
	UFUNCTION(BlueprintCallable)
		void Event_DeletePoint();


	UFUNCTION(BlueprintCallable)
		void UICall_SaveData(bool justSync);
	UFUNCTION(BlueprintCallable)
		void UICall_ChangeTimeData(float time);
	UFUNCTION(BlueprintCallable)
		void UICall_EnterButton_PreviousFrame();
	UFUNCTION(BlueprintCallable)
		void UICall_EnterButton_NextFrame();
	UFUNCTION(BlueprintCallable)
		void UICall_EnterButton_Delete();
	UFUNCTION(BlueprintCallable)
		void MySelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	void OnEnterColor(FVector color);
	void ViewShowPoint();
	void SetShowPointMap(bool isAdd, FTimelinePointIndex& pointIndex);
	bool IsNeedSaveData();
	float GetXWithTime(float time);
	float GetTimeWithX(float x);
	void ViewShowTime();
	void ViewShowLine();
	void SetTextValue(CvTimelineUIInfo& info);
	void SetTextValue(FVector location, FRotator rotation);
	void SetTextValue(FVector color);
	void GetEditTransformData(CvTimelineUIInfo& info);
	FVector GetEditColor();
	bool IsUIInfoSame(CvTimelineUIInfo& oldInfo, CvTimelineUIInfo& newInfo);
	float GetNowTimelineTime();
	void ViewChoose(bool chooseLeft, std::vector<int> selectEntityIdLise);
	void ViewGotoLine(int entityId);
	void TimeAxisMove(float x);
	TWeakObjectPtr<AEditEntityActor> GetSelectEntityActor();
	void OnExpandCollapse(bool isShow);
	void OpenUI();
	void CloseUI();
	void SaveChangeDate();
	bool GetMouseEntityId(int& entityId, FVector2D& offsetPoint_);
	void Test_GetOverlay();
	void Event_SelectEntity(std::vector<int>& selectList);
	void MouseEnterLeaveFalse(FTimelinePointIndex& nowPointIndexInfo, bool flag);
	void OnMouseEnterLeaveTimeAxis(FVector2D mousePosion);
	void BoxSelection();
	FVector2D GetSelectBoxViewPoint(FVector2D start);
	FVector2D GetOverlayPointMargin();
	FVector2D GetOverlayTimeAxisMargin();
	FTimelineEnterPointInfo* GetTimelinePointInfoWithUniqueMark(int entityId, int uniqueMark);
	FTimelineEnterPointInfo* GetTimelinePointInfoWithKeyTime(int entityId, int keyTime);
	void ViewMove(FVector2D newPosition);
	void SetBlockData();
	void Callback_TimeAxisChange(float time);
	TArray<SelectInstanceInfo>* Get_SelectEntityList();
	bool TimelineIsSelectKeyFrame(int& timeKey, int& entityId, int& uniqueMark);
	bool CanSave();
	void Call_EntityChangeTransform(int entityId, int uniqueMark, CvTimelineUIInfo& info);
	void UpdateEntityDataToUIData(int entityId, ETimelineOperationType type);

	void ClearAllPoint(FTimelineEntityLineInfo* entityLineInfo);
	TWeakObjectPtr<AEditEntityManage> GetEditEntityManage();

	virtual void NativeOnInitialized();
	//virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	void OnMouseMove(bool isPanel, const FPointerEvent& InMouseEvent);
	void OnMousePointEnterLeave();
	void OnMouseEnterLeaveEntityPoint(FVector2D mousePosion);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);
	//void OnOpenClosePage(bool isOpen);
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
		const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const;
	FVector2D GetMouseViewPoint(FVector2D mousePoint) const;
	void DrawSelectBox(FPaintContext& Context) const;
	void DrawDottedLine(FPaintContext& Context) const;
	float GetViewTime();
	Gamedata::EntityInfo* GetSyncInfo(int entityId);
	TWeakObjectPtr<AEditEntityActor> GetEntityAcotr(int entityId);
	//FLocalEntityInfo* GetLocalInfo(int entityId);
	xlsdata::PlaceTableDefine* GetXlsInfo(int entityId);
	void DrawLine(FVector2D start, FVector2D end);

	void EntityAddPoint(int entityId, float time, FTransform relativeTransform, FVector color);
	bool GetSelectPointList(std::vector<FTimelinePointIndex>& selectPointList, std::vector<int>& selectEntityId);
	bool IsSelectPoint(std::vector<FTimelinePointIndex>& selectPointList, std::vector<int>& selectEntityId,
		FTimelinePointIndex thisPoint);
	void EntityMovePointProcess(FVector2D move, bool mast = false);
	void EntityMovePointEnd(FVector2D move);
	//bool LeftLineCreate(FTimelineEntityLineInfo& timelineEntityInfo);
	void LeftLineRemove(FTimelineEntityLineInfo& timelineEntityInfo);
	void LeftLineMove(FTimelineEntityLineInfo& timelineEntityInfo);
	void LeftLineChoose(FTimelineEntityLineInfo& timelineEntityInfo, bool flag);
	void SetKeyFrameCanWrite(bool canWrite, bool isMast);
	bool CenterLineCreate(FTimelineEntityLineInfo& timelineEntityInfo);
	void CenterLineRemove(FTimelineEntityLineInfo& timelineEntityInfo);
	void CenterLineMove(FTimelineEntityLineInfo& timelineEntityInfo);
	void CenterLineChoose(FTimelineEntityLineInfo& timelineEntityInfo, bool flag);
	TArray<SelectInstanceInfo>* GetSelectLine();
};