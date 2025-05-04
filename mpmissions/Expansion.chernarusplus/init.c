void main()
{
	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();

	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 9, reset_day = 20;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day))
	{
		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
	}
	else
	{
		if ((month == reset_month + 1) && (day > reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month < reset_month) || (month > reset_month + 1))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
		}
	}
}

class CustomMission : MissionServer
{
    void SetRandomHealth(EntityAI itemEnt)
    {
        if (itemEnt)
        {
            float rndHlt = Math.RandomFloat(0.45, 0.65);
            itemEnt.SetHealth01("", "", rndHlt);
        }
    }

    override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
    {
        Entity playerEnt;
        playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");
        Class.CastTo(m_player, playerEnt);

        GetGame().SelectPlayer(identity, m_player);
        return m_player;
    }

    override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
    {
        EntityAI itemClothing;
        EntityAI itemEnt;
        float rand;

        itemClothing = player.FindAttachmentBySlotName("Body");
        if (itemClothing)
        {
            SetRandomHealth(itemClothing);

            itemEnt = itemClothing.GetInventory().CreateInInventory("BandageDressing");
            player.SetQuickBarEntityShortcut(itemEnt, 2);

            string chemlightArray[] = {"Chemlight_White", "Chemlight_Yellow", "Chemlight_Green", "Chemlight_Red"};
            int rndIndex = Math.RandomInt(0, 4);
            itemEnt = itemClothing.GetInventory().CreateInInventory(chemlightArray[rndIndex]);
            SetRandomHealth(itemEnt);
            player.SetQuickBarEntityShortcut(itemEnt, 1);

            rand = Math.RandomFloatInclusive(0.0, 1.0);
            if (rand < 0.35)
                itemEnt = player.GetInventory().CreateInInventory("Apple");
            else if (rand > 0.65)
                itemEnt = player.GetInventory().CreateInInventory("Pear");
            else
                itemEnt = player.GetInventory().CreateInInventory("Plum");
            player.SetQuickBarEntityShortcut(itemEnt, 3);
            SetRandomHealth(itemEnt);
        }

        itemClothing = player.FindAttachmentBySlotName("Legs");
        if (itemClothing)
            SetRandomHealth(itemClothing);

        itemClothing = player.FindAttachmentBySlotName("Feet");
    }

    void SetDynamicFogBias()
{
    int year, month, day, hour, minute;
    GetGame().GetWorld().GetDate(year, month, day, hour, minute);

    Weather weather = g_Game.GetWeather();

    // Check if it's nighttime (from 20:00 until 06:00)
    if (hour >= 20 || hour < 6)
    {
        // Extremely dense fog settings for nighttime
        weather.SetStorm(1.0, 430.0, 30); // Fog Bias: 430
        
        // Fog distance set to 1 (extremely close fog)
        weather.GetFog().SetLimits(0.5, 1.0); // Min and Max set to 1 (full intensity, close fog)
        weather.GetFog().Set(1.0, 30, 30);    // Force fog distance to 1 with 30-second fade transitions
    }
    else
    {
        // Reset to normal conditions during daytime
        weather.SetStorm(0.0, 0.0, 30);       // Clear storm conditions
        
        weather.GetFog().SetLimits(0.2, 0.75); // Less fog during day (customize as you like)
        weather.GetFog().Set(0.0, 30, 30);    // Clear fog with smooth transition
    }
}

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        SetDynamicFogBias();
    }
};


Mission CreateCustomMission(string path)
{
	return new CustomMission();
}