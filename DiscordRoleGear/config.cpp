class CfgPatches
{
	class DiscordRoleGear
	{
		requiredVersion=0.1;
		requiredAddons[]={
			"UniversalApi"
		};
	};
};

class CfgMods
{
	class DiscordRoleGear
	{
		dir="DiscordRoleGear";
        name="DiscordRoleGear";
        credits="DaemonForge";
        author="DaemonForge";
        authorID="0";
        version="0.1";
        extra=0;
        type="mod";
	    dependencies[]={ "Game", "World", "Mission"};
	    class defs
	    {
			class gameScriptModule
            {
				value = "";
                files[]={
					"DiscordRoleGear/scripts/3_Game"
				};
            };
			class worldScriptModule
            {
                value="";
                files[]={ 
					"DiscordRoleGear/scripts/4_World"
				};
            };
			
	        class missionScriptModule
            {
                value="";
                files[]={
					"DiscordRoleGear/scripts/5_Mission"
				};
            };
        };
    };
};