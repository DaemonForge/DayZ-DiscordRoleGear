static const int DGERROR = 0;
static const int DGVERBOSE = 1;
static const int DGINFO = 2;
static const int DGDEBUG = 3;


class DGLog extends Managed {
	
	protected static ref DGLogFileInstance m_DGLogFileInstance;
	
	static DGLogFileInstance GetInstance(){
		if (!m_DGLogFileInstance){m_DGLogFileInstance = new DGLogFileInstance;}
		return m_DGLogFileInstance;
	}
	
	static void Log(string text, int level = 1) {
		GetInstance().DoLog(text,level);
	}
	
	static void Info(string text){
		GetInstance().DoLog(text, DGINFO);
	}
	
	static void Debug(string text){
		GetInstance().DoLog(text, DGDEBUG);
	}

	static void Err(string text){
		Error2("[DiscordGear] Error", text);
		GetInstance().DoLog(text, DGERROR);
	}
	
	static void SetLogLevels(int level, int apiLevel = -99){
		if (apiLevel == -99){
			apiLevel = level;
		}
		GetInstance().SetLogLevel(level);
		GetInstance().SetApiLogLevel(apiLevel);
	}
	
};
class DGLogFileInstance extends Managed {
	
	protected int				m_LogLevel	= 3;
	protected int				m_LogToApiLevel = 3;
	protected bool 			m_isInit = false;
	
	protected static string LogDir = "$profile:";
	protected FileHandle		m_FileHandle;
	
	void DGLogFileInstance(int level = 4) {	
		m_LogLevel = level;	
		if ( !GetGame().IsServer() || GetGame().IsClient() ){
			return;	
		}
		m_FileHandle = CreateFile(LogDir + "DiscordGear_" + GetDateStampFile() + ".log");
		if (m_FileHandle != 0){
			m_isInit = true;
		}
	}
	
	void ~DGLogFileInstance() {
		if ( m_isInit ) {
			CloseFile(m_FileHandle);
		}
	}
	
	void SetLogLevel(int level){
		m_LogLevel = level;
	}
	
	void SetApiLogLevel(int level){
		m_LogToApiLevel = level;
	}
	
	protected FileHandle CreateFile(string path) {
		if ( !GetGame().IsServer() || GetGame().IsClient() ){
			return null;	
		}
		
		FileHandle fHandle = OpenFile(path, FileMode.WRITE);
		if (fHandle != 0) {
			FPrintln(fHandle, "DiscordGear Log Started: " + GetDateStamp() + " " + GetTimeStamp() );
			return fHandle;
		}
		Error2("[DiscordGear] Error", "Unable to create" + path + " file in Profile.");
		return fHandle;
	}
	
	protected static string GetDateStamp() {
		int yr, mth, day;
		GetYearMonthDay(yr, mth, day);
		string sday = day.ToString();
		if (sday.Length() == 1){
			sday = "0" + sday;
		}
		
		string smth = mth.ToString();
		if (smth.Length() == 1){
			smth = "0" + mth.ToString();
		}
		
		return yr.ToString() + "-" + smth + "-" + sday;
	}
	
	protected static string GetDateStampFile() {
		int hr, min, sec;
		GetHourMinuteSecond(hr, min, sec);
		
		string ssec = sec.ToString();
		if (ssec.Length() == 1){
			ssec = "0" + ssec;
		}
		string smin = min.ToString();
		if (smin.Length() == 1){
			smin = "0" + smin;
		}
		string shr = hr.ToString();
		if (shr.Length() == 1) {
			shr = "0" + shr;
		}
		
		return  GetDateStamp() + "_" + shr + "-" + smin + "-" + ssec;
	}
	
	protected static string GetTimeStamp() {
		int hr, min, sec;
		GetHourMinuteSecond(hr, min, sec);
		
		string ssec = sec.ToString();
		if (ssec.Length() == 1){
			ssec = "0" + ssec;
		}
		string smin = min.ToString();
		if (smin.Length() == 1){
			smin = "0" + smin;
		}
		string shr = hr.ToString();
		if (shr.Length() == 1) {
			shr = "0" + shr;
		}
		
		return  shr + ":" + smin + ":" + ssec;
	}
	
	
	void DoLog(string text, int level = 1)
	{	
		if (level == 2 && m_LogLevel >= level) {
			GetGame().AdminLog("[DiscordGear]" + GetTag(level) + text);
		}
		if (m_isInit && m_LogLevel >= level){
			//Print("[DiscordGear] " + GetTag(level) + GetTimeStamp() + " | " + text);
			string towrite = GetTag(level)  + GetTimeStamp() + " | " + " " + text;
			FPrintln(m_FileHandle, towrite);
		} else if (m_LogLevel >= level) {
			Print("[DiscordGear]" + GetTag(level) + " " + text);
		}
		if (m_LogToApiLevel >= level){
			UApi().Rest().Log(GetJsonObject(text, level));
		}
	}
	
	protected static string GetTag(int level){
		switch ( level ) {
			case DGERROR:
				return "[ERROR] ";
				break;
			case DGVERBOSE:
				return "[VERBOSE] ";
				break;
			case DGDEBUG:
				return "[DEBUG] ";
				break;
			case DGINFO:
				return "[INFO] ";
				break;
			default:
				return "[INFO] ";
				break;
		}
		return "[NULL] ";
	}
	
	static string GetJsonObject(string text, int level) {
		string sLevel = "INFO";
		
		switch ( level ) {
			case DGERROR:
				sLevel = "ERROR";
				break;
			case DGVERBOSE:
				sLevel =  "VERBOSE";
				break;
			case DGDEBUG:
				sLevel =  "DEBUG";
				break;
			case DGINFO:
				sLevel =  "INFO";
				break;
			default:
				sLevel =  "INFO";
				break;
		}
		autoptr DiscordGearLogObject obj = new DiscordGearLogObject(text, sLevel);
		return obj.ToJson();
	}
}

class DiscordGearLogObject extends UApiObject_Base {
	string Type = "DiscordGear";
	string Message;
	string Level;
	
	void DiscordGearLogObject(string text, string level){
		Message = text;
		Level = level;
		
	}
	
	override string ToJson(){
		string jsonString = JsonFileLoader<DiscordGearLogObject>.JsonMakeData(this);
		return jsonString;
	}
	
}