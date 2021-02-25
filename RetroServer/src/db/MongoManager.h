#pragma once
#include "mongoc.h"
#include <unordered_map>
#include "DBCommand.h"
#include <queue>
#include <mutex>

class MongoDB;
class DBCommand;

class MongoManager {
public:
	static MongoManager* getInstance();
	bool Init(const char* uri);
	MongoDB* CreateDB(const char* name);
	MongoDB* GetDB(const char* name);
	bool HasDB(const char* name);
	void Ping();
public:
	void PushCommand(DBCommand* cmd);
	DBCommand* PopCommand();
	bool IsQuit();
	void Quit();
	void Update();
private:
	void PushResultCommand(DBCommand* cmd);
	DBCommand* PopResultCommand();
	void Execute();
	MongoManager();
	~MongoManager();
private:
	mongoc_uri_t* _uri;
	mongoc_client_t* _client;
	std::unordered_map<std::string, MongoDB*> _dbMap;
	std::queue<DBCommand*> _cmdWaitQueue;
	std::queue<DBCommand*> _cmdResultQueue;
	std::mutex _waitMutex;
	std::mutex _resultMutex;
	std::mutex _quitMutex;
	bool _bQuit;
	std::thread* _pThread;
};