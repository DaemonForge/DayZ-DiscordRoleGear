class DGGearSets extends Managed {
	
	autoptr array<autoptr DGGearData> RandomItems = new array<autoptr DGGearData>;
	string AttachmentSlot = "Armband";
	
	[NonSerialized()]
	autoptr map<string, autoptr DGGearData> RoleToGearMap;
	
	autoptr array<autoptr DGGearDataMapHolder> RoleToGear = new array<autoptr DGGearDataMapHolder>;
	
	void DGGearSets(){	
		RandomItems.Insert(new DGGearData("Armband_White"));
		RandomItems.Insert(new DGGearData("Armband_Yellow"));
		RandomItems.Insert(new DGGearData("Armband_Orange"));
		RandomItems.Insert(new DGGearData("Armband_Red"));
		RoleToGear.Insert(DGGearDataMapHolder("903450052276658246", "Armband_White"));
		RoleToGear.Insert(DGGearDataMapHolder("903444508442779698", "Armband_Red"));
		RoleToGear.Insert(DGGearDataMapHolder("903444600893632512", "Armband_Green"));
		RoleToGear.Insert(DGGearDataMapHolder("903444718061510667", "Armband_Pink"));
		RoleToGear.Insert(DGGearDataMapHolder("903444789226250241", "Armband_Blue"));
		RoleToGear.Insert(DGGearDataMapHolder("903444847699058708", "Armband_Black"));
		RoleToGear.Insert(DGGearDataMapHolder("903444891747627038", "Armband_Orange"));
		RoleToGear.Insert(DGGearDataMapHolder("903621233466306571", "Armband_Yellow"));
	}
	
	void TransferToMap(){
		
		Print(RoleToGear);
		RoleToGearMap = new map<string,autoptr DGGearData>;
		for (int i = 0; i < RoleToGear.Count(); i++){
			RoleToGearMap.Insert(RoleToGear.Get(i).RoleId, RoleToGear.Get(i).GearData);
		}
		Print(RoleToGearMap);
	}
	
	EntityAI CreateItem(EntityAI parent, UApiDiscordUser user){
		if (!user || !parent){return NULL;}
		if (!RoleToGearMap){
			TransferToMap();
		}
		DGGearData data;
		
		EntityAI item;
		for (int i = 0; i < user.Roles.Count(); i++){
			if (RoleToGearMap.Find(user.Roles.Get(i),data)){
				break;
			}
		}
		if (!data){
			data = RandomItems.GetRandomElement();
		}
		int slotid = InventorySlots.GetSlotIdFromString(AttachmentSlot);
		string type;
		if (data && AttachmentSlot == ""){
			item = parent.GetInventory().CreateInInventory(data.Type);
		} else if (data && InventorySlots.IsSlotIdValid(slotid)){
			item = parent.GetInventory().CreateAttachmentEx(data.Type, slotid);
		} else if (!data){
			DGLog.Debug("No item selected");
		} else {
			DGLog.Err("Attachment Slot " + AttachmentSlot + " not valid");
		}
		if (item){
			float maxhealth = item.GetHealth("","");
			float rndHealth = Math.QRandomFloat(data.HealthPercentageLow,data.HealthPercentageHigh) / 100;
			if (rndHealth < maxhealth){
				item.SetHealth("","", maxhealth * rndHealth);
			}
		
		}
		return item;
	}
}

class DGGearData extends Managed {

	string Type = "";
	float HealthPercentageHigh = 100;
	float HealthPercentageLow = 100;
	float QuanityHigh = -1;
	float QuanityLow = -1;
	autoptr TStringArray ItemAttachments = {};
	bool HealthToAttachments = true;
	
	void DGGearData(string type){
		Type = type;
	}
	
}

class DGGearDataMapHolder extends Managed {
	string RoleId;
	autoptr DGGearData GearData;
	void DGGearDataMapHolder(string id, string type){
		RoleId = id;
		GearData = new DGGearData(type);
	}
}