#ifndef MISSION_CHOOSER_INT_H
#define MISSION_CHOOSER_INT_H
#ifdef _WIN32
#pragma once
#endif

#include "appframework/IAppSystem.h"
#include "utlvector.h"

class I_Random_Missions;
class I_Mission_Chooser_Source;
class I_Map_Builder;
class KeyValues;
class CUniformRandomStream;
class Color;
class ISpawnSelection;

class I_Location;

class I_Location_Group
{
public:
	virtual const char* GetGroupName() = 0;
	virtual const char* GetTitleText() = 0;
	virtual const char* GetDescriptionText() = 0;
	virtual const char* GetImageName() = 0;	
	virtual Color& GetColor() = 0;
	virtual bool IsGroupLocked( CUtlVector<int> &completedMissions ) = 0;

	virtual int GetHighestUnlockMissionID() = 0;
	virtual int GetNumLocations() = 0;
	virtual I_Location* GetLocation( int iIndex ) = 0;
};

enum Reward_Type
{
	REWARD_MONEY=0,
	REWARD_XP,
	REWARD_ITEM,
	REWARD_SKILL_SLOT,
};

class I_Reward
{
public:
	virtual Reward_Type GetRewardType() = 0;
	virtual int GetRewardAmount() = 0;

	// for item rewards
	virtual const char* GetRewardName() = 0;
	virtual int GetRewardLevel() = 0;
	virtual int GetRewardQuality() = 0;	
};

class I_Location
{
public:
	virtual int GetID() = 0;
	virtual I_Location_Group* GetGroup() = 0;
	virtual int GetDifficulty() = 0;
	virtual bool GetCompleted() = 0;
	virtual int GetXPos() = 0;
	virtual int GetYPos() = 0;
	// Mission Settings are settings not directly related to the level layout, but which have other effects on the mission.
	// (This is the "mission_settings" sub-node of the mission definition).
	virtual KeyValues* GetMissionSettings() = 0;
	// Mission Definition is the key-values block fed to the layout system.
	virtual KeyValues* GetMissionDefinition() = 0;
	
	virtual const char* GetMapName() = 0;
	virtual const char* GetStoryScene() = 0;
	virtual const char* GetImageName() = 0;
	virtual const char* GetCompanyName() = 0;
	virtual const char* GetCompanyImage() = 0;
	virtual int GetNumRewards() = 0;
	virtual I_Reward* GetReward( int iRewardIndex ) = 0;
	virtual int GetMoneyReward() = 0;
	virtual int GetXPReward() = 0;
	virtual int IsMissionOptional() = 0;
	virtual bool IsLocationLocked( CUtlVector<int> &completedMissions ) = 0;
	virtual void SetPos( int x, int y ) = 0;
};

class I_Location_Grid
{
public:
	virtual int GetNumGroups() = 0;
	virtual I_Location_Group* GetGroup( int iIndex ) = 0;
	virtual I_Location_Group* GetGroupByName( const char *szName ) = 0;
	virtual I_Location* GetLocationByID( int iLocationID ) = 0;

	virtual void SetLocationComplete( int iLocationID ) = 0;	
};

class I_Mission_Text_Database
{
public:
	virtual const char *GetShortDescriptionByID( unsigned int id ) = 0;	
	virtual const char *GetLongDescriptionByID( unsigned int id ) = 0;

	/// a unique identifier for each mission spec. 
	/// if they're loaded from the same files in the same
	/// order by the database on both server and client,
	/// we can refer to specs by this id across the network.
	// if you change the size of this, also update the m_nObjectiveTextIdx
	// datatable field in asw_objective.cpp. 
	typedef uint16 ID_t;
	enum { INVALID_INDEX = ((ID_t)(~0)) };
	static inline bool IsIDValid( ID_t idx ) { return idx != INVALID_INDEX; }

	/// find the mission text id for a given entity name, mission filename, other criteria.
	/// you can resolve the ID to textual descriptions with GetShortDescriptionByID() etc.
	virtual ID_t FindMissionTextID( const char *pEntityName, const char *pMissionName ) = 0;

};

//-----------------------------------------------------------------------------
// Purpose: Interface exposed from the mission chooser .dll (to the game dlls and tilegen.exe)
//-----------------------------------------------------------------------------
abstract_class I_Mission_Chooser : public IAppSystem
{
public:
	virtual bool GetCurrentTimeAndDate(int *year, int *month, int *dayOfWeek, int *day, int *hour, int *minute, int *second) = 0;

	virtual I_Random_Missions *RandomMissions() = 0;
	virtual I_Mission_Chooser_Source *LocalMissionSource() = 0;	
	virtual I_Location_Grid *LocationGrid() = 0;
	virtual I_Mission_Text_Database *MissionTextDatabase() = 0;
	virtual I_Map_Builder *MapBuilder() = 0;
	virtual ISpawnSelection *SpawnSelection() = 0;
};

#define MISSION_CHOOSER_VERSION		"VMissionChooser001"

#endif // MISSION_CHOOSER_INT_H
