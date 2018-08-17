#include "stdafx.h"
#include "sqlite_dll.h"

bool DB_open(sqlite3 **db, char *name)
{
	sqlite3_open(name, db);
	return true;
}
char* getTbName(int i)
{
	switch (i)
	{
	case 0:return "proOrder"; break;
	case 1:return "proQuery"; break;
	case 2:return "proTrack"; break;
	case 3:return "proReturn"; break;
	case 4:return "proIntel"; break;

	case 5:return "distriLs"; break;
	case 6:return "distriQuery"; break;
	case 7:return "distriTrack"; break;

	case 8:return "wareRequis"; break;
	case 9:return "wareIn"; break;
	case 10:return "wareOut"; break;
	case 11:return "wareStock"; break;
	case 12:return "wareChng"; break;
	case 13:return "wareQuery"; break;

	case 14:return "manaCus"; break;
	case 15:return "manaSup"; break;
	case 16:return "manaGoods"; break;
	case 17:return "manaWare"; break;
	case 18:return "manaAddress"; break;

	case 19:return "dataVoucher"; break;
	case 20:return "dataPayment"; break;
	case 21:return "dataVerification"; break;
	case 22:return "dataOtherIncome"; break;
	case 23:return "dataOtherExpend"; break;
	case 24:return "dataTransfer"; break;

	case 25:return "assIncome"; break;
	case 26:return "assExpense"; break;
	case 27:return "assPayable"; break;
	case 28:return "assReceive"; break;
	case 29:return "assCusState"; break;
	case 30:return "assVendorState"; break;
	case 31:return "assOtherInOut"; break;
	case 32:return "assIncomeState"; break;

	case 33:return "setPermis"; break;
	case 34:return "setLog"; break;

	}
	return NULL;
}
//UTF-8到GB2312的转换
char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

//GB2312到UTF-8的转换
char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

bool selectDB(sqlite3 *db, sqlite3_callback callback, void *data,char *tbName, char *key)
{
	char *str;
	if (key == 0)
		str = sqlite3_mprintf("SELECT * FROM %s", tbName);
	else
		str = sqlite3_mprintf("SELECTE %s FROM %s", key, tbName);
	DB_excute(db, str, callback, data);
	return true;
}
bool DB_excuteNoCall(sqlite3 *db, const char *commond, char *errMsg)
{

	char *t = G2U(commond);
	if (sqlite3_exec(db, t, 0, 0, &errMsg) == SQLITE_OK)
		return true;
	return false;

}
bool DB_excute(sqlite3 *db, const char *commond, sqlite3_callback callback, void *data, char *errMsg)
{
	char *t = G2U(commond);

	if (sqlite3_exec(db, t, callback, data, &errMsg) == SQLITE_OK)
		return true;
	return false;

}
bool DB_close(sqlite3 *db)
{
	if (sqlite3_close(db) == SQLITE_OK)
		return true;
	else
		return false;
}

bool DB_insertName(sqlite3 *db, char *tbName, char *key, char *value)
{
	//	str = "INSERT INTO " + tbName + "(ID,ctrName,ctrText,l,t,r,b)VALUES" + value;

	char *commond = sqlite3_mprintf("INSERT INTO %s(%s)VALUES (%s)", tbName, key, value);
	DB_excuteNoCall(db, commond);
	return true;
}
bool DB_insert(sqlite3 *db, int tbID, char *key, char *value)
{
	char *tbName = getTbName(tbID);
	if (DB_insertName(db, tbName, key, value))
		return true;
	return false;
}
bool DB_create(sqlite3 *db, char *tbName, char *Key)
{
	char *sql = sqlite3_mprintf("CREATE TABLE %s (%s)", tbName, Key);
	if (DB_excuteNoCall(db, sql))
		return true;
	return false;
}
