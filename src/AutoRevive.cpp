#include "ScriptMgr.h"
#include "Player.h"
#include "AccountMgr.h"
#include "Common.h"
#include "Configuration/Config.h"

uint32 TimeCheck_AutoRevive = 0;

class AutoRevive_PlayerSC : public PlayerScript
{
public:
	AutoRevive_PlayerSC() : PlayerScript("AutoRevive_PlayerSC") {}

	void OnBeforeUpdate(Player* player, uint32 diff)
	{
		if (!sConfigMgr->GetBoolDefault("AutoRevive.Enable", false))
			return;

        if (AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity()))
            return;

		uint32 NowHealth = player->GetHealth();
		uint32 TimeCheck = 1000;

		if (TimeCheck_AutoRevive < diff)
		{
			if (NowHealth <= 1)
			{
				player->ResurrectPlayer(1.0f);
				player->SaveToDB(false, false);
			}
			
			TimeCheck_AutoRevive = TimeCheck;
		}
		else
			TimeCheck_AutoRevive -= diff;
	}
};

class AutoRevive_WorldSC : public WorldScript
{	
public:
    AutoRevive_WorldSC() : WorldScript("AutoRevive_WorldSC") {}		
	
	void OnAfterConfigLoad(bool reload)
	{	
		std::string conf_path = _CONF_DIR;
		std::string cfg_file = conf_path + "/modules/AutoRevive.conf";

#if PLATFORM == PLATFORM_WINDOWS
		cfg_file = "modules/AutoRevive.conf";
#endif
		sConfigMgr->LoadMore(cfg_file.c_str());
	}
};

void AddAutoReviveScripts()
{
    new AutoRevive_PlayerSC();
	new AutoRevive_WorldSC();
}