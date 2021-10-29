class DiscordRoleGearConfig extends UApiConfigBase {

	string ConfigVersion = "0";
	static string CurrentVersion = "0";
	
	autoptr array<autoptr DGGearSets> GearSets = new array<autoptr DGGearSets>;
	
	
	int LogLevel_File = 3;
	int LogLevel_API = 2;
	
	override void SetDefaults(){
		/*
	
		  This is to set the defaults for the mod before requesting a load so that way 
		if it doesn't exsit the API will create the file
	
		*/
		GearSets.Insert(new DGGearSets());
	}

	override void OnDataReceive(){
		SetDataReceived();
		if(!ConfigVersion || ConfigVersion != CurrentVersion){
			ConfigVersion = CurrentVersion;
			Save(); //Resave the upgrade Version Back to the server
		}
		DGLog.SetLogLevels(LogLevel_File, LogLevel_API);
		Valiate();
	}
	
	void Valiate(){
		
		
	}	
	
	override void Load(){
		if (!m_DataReceived){
			SetDefaults();
		}
		m_DataReceived = false;
		//Set the Defaults so that way, when you load if this its the server Requesting the data it will create it based on the defaults
		UApi().Rest().GlobalsLoad("DiscordRoleGear", this, this.ToJson());
	}
	
	
	override void Save(){
		if (GetGame().IsServer()){
			UApi().Rest().GlobalsSave("DiscordRoleGear", this.ToJson());
		}
	}
	
	
	override string ToJson(){
		return UApiJSONHandler<DiscordRoleGearConfig>.ToString(this);
	}
	
	// This is called by the API System on the successfull response from the API
	override void OnSuccess(string data, int dataSize) {
		if (UApiJSONHandler<DiscordRoleGearConfig>.FromString(data, this)){
			OnDataReceive();
		} else {
			DGLog.Err("CallBack Failed errorCode: Invalid Data");
		}
	}
	
	void CreateItems(EntityAI parent, UApiDiscordUser user){
		if (!GearSets){
			return;
		}
		for (int i = 0; i < GearSets.Count(); i++){
			GearSets.Get(i).CreateItem(parent, user);
		}
	}
	
}


static ref DiscordRoleGearConfig m_DiscordRoleGearConfig;
static DiscordRoleGearConfig GetDiscordRoleGear(){
	if (!m_DiscordRoleGearConfig){
		m_DiscordRoleGearConfig = new DiscordRoleGearConfig;
		m_DiscordRoleGearConfig.Load();
	}
	return m_DiscordRoleGearConfig;
}