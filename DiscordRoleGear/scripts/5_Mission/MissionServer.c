modded class MissionServer extends MissionBase
{
	static int DISCORD_ROLE_GEAR_REFRESH_TIMER = 3000;
	int m_MapLinkConfigRefreshTimer = 0;
	ref map<string, autoptr UApiDiscordUser> m_PlayerDiscordQue = new map<string, autoptr UApiDiscordUser>;
	
	override void OnMissionStart(){
		super.OnMissionStart();
		GetDiscordRoleGear();
	}
	
	
	void CBCacheDiscordUser(int cid, int status, string guid, UApiDiscordUser data){	
      	if (status == UAPI_SUCCESS){  //If its a success
			DGLog.Debug("CacheDiscordUser - Success ID:" + cid + " - GUID: " + guid );
			m_PlayerDiscordQue.Set(guid, UApiDiscordUser.Cast(data));
      	} else if ( status == UAPI_NOTSETUP ) {
			
		} else if ( status == UAPI_NOTFOUND ) {
			
	   	}
	}

	override void OnClientPrepareEvent(PlayerIdentity identity, out bool useDB, out vector pos, out float yaw, out int preloadTimeout)
	{
		int CurrentTime = GetGame().GetTime();
		if (CurrentTime > m_MapLinkConfigRefreshTimer){
			m_MapLinkConfigRefreshTimer = CurrentTime + DISCORD_ROLE_GEAR_REFRESH_TIMER;
		}
		if (identity){
			int cid = UApi().ds().GetUser(identity.GetId(), this, "CBCacheDiscordUser");	
			DGLog.Info("Requesting Player Discord Data from API Call ID:" + cid + " - GUID: " + identity.GetId() );
		} else {
			DGLog.Info("Requesting Player Discord Data from API - GUID: NULL");
		}
		super.OnClientPrepareEvent(identity, useDB, pos, yaw, preloadTimeout);
	}
	
	
	void HandleDiscordGear(PlayerBase player){
		if (!player || !player.GetIdentity()){return;}
		string guid = player.GetIdentity().GetId();
		UApiDiscordUser user;
		if (m_PlayerDiscordQue.Find(guid, user)){
			Print(user);
			GetDiscordRoleGear().CreateItems(player, user);
		} else {
		
			return;
		}
	
	}
	
}