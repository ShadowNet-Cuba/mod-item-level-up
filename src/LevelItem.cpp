
#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "Creature.h"
#include "AccountMgr.h"
#include "ScriptMgr.h"
#include "Define.h"
#include "GossipDef.h"

uint32 Increase_Level;
uint32 MaxItemLevel = 80;
bool LevelItemEnable = true;
bool LevelItemAnnounce = true;

// Configuration
class mod_levelitem_Conf : public WorldScript
{
public:
    mod_levelitem_Conf() : WorldScript("mod_levelitem_Conf") { }

    // Load Configuration Settings
    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        LevelItemEnable = sConfigMgr->GetOption<bool>("LevelItem.Enable", true);
        LevelItemAnnounce = sConfigMgr->GetOption<bool>("LevelItem.Announce", true);
        MaxItemLevel = sConfigMgr->GetOption<int32>("LevelItem.MaxItemLevel", 80);
    }
};

// Announce
class mod_levelitem_Announce : public PlayerScript
{

public:
    mod_levelitem_Announce() : PlayerScript("mod_levelitem_Announce") {}

    void OnLogin(Player* player) override
    {
        if (LevelItemAnnounce)
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Token Level-Up Aman'Thul |rmodule.");
        }
    }
};

class LevelItem : public ItemScript
{
public:
    LevelItem() : ItemScript("BoostLevel") { }

    bool OnUse(Player* p, Item* i, const SpellCastTargets &) override
    {
        if (!LevelItemEnable)
            return false;

        if (p->IsInCombat() || p->IsInFlight() || p->GetMap()->IsBattlegroundOrArena())
        {
            ChatHandler(p->GetSession()).PSendSysMessage("No puedes usar el Boost Level de Aman'Thul en este momento.!");
            return false;
        }

        if (p->getLevel() >= MaxItemLevel)
        {
            ChatHandler(p->GetSession()).PSendSysMessage("No puedes usar el Boost Level de Aman'Thul! tienes nivel %u!", MaxItemLevel);
            return false;
        }

        p->GiveLevel(MaxItemLevel);
        p->SetUInt32Value(PLAYER_XP, 0);
        p->DestroyItemCount(i->GetEntry(), 1, true);
        ChatHandler(p->GetSession()).PSendSysMessage("Has usado el Boost Level de Aman'Thul!");

        return true;
    }
};

class LevelItem5 : public ItemScript
{
public:
    LevelItem5() : ItemScript("LevelItem5") { }

    bool OnUse(Player* p, Item* i, const SpellCastTargets &) override
    {
        if (!LevelItemEnable)
            return false;

        if (p->IsInCombat() || p->IsInFlight() || p->GetMap()->IsBattlegroundOrArena())
        {
            ChatHandler(p->GetSession()).PSendSysMessage("No puedes usar el Token de +5 Niveles de Aman'Thul en este momento.!");
            return false;
        }

        if (p->getLevel() >= MaxItemLevel)
        {
            ChatHandler(p->GetSession()).PSendSysMessage("No puedes usar el Token de +5 Niveles de Aman'Thul! tienes nivel %u!", p->getLevel());
            return false;
        }

        uint8 newLevel = p->getLevel() + 5;
		
		if (newLevel >= MaxItemLevel )
		{
			p->GiveLevel(MaxItemLevel);
		}
		else {
			p->GiveLevel(newLevel);
			p->SetUInt32Value(PLAYER_XP, 0);
			p->DestroyItemCount(i->GetEntry(), 1, true);
			ChatHandler(p->GetSession()).PSendSysMessage("Has usado el Token de +5 Niveles de Aman'Thul!");
		}	
		
        return true;
    }
};



void AddLevelItemScripts()
{
    new mod_levelitem_Conf();
    new mod_levelitem_Announce();
    new LevelItem();
	
}

void AddLevelItem5Scripts()
{
    new LevelItem5();
}
