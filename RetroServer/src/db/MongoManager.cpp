#include "MongoManager.h"
#include "mongoc.h"
#include "LogUtils.h"
#include "MongoDB.h"
#include "db/DBCollection.h"
#include <chrono>

MongoManager* MongoManager::getInstance() {
	static MongoManager mgr;
	return &mgr;
}

MongoManager::MongoManager() 
:_uri(NULL)
,_client(NULL)
, _pThread(NULL)
, _bQuit(false){
	
}

MongoManager::~MongoManager() {
	for (auto iter = _dbMap.begin(); iter != _dbMap.end(); ++iter) {
		delete iter->second;
	}
	if (_uri) {
		mongoc_uri_destroy(_uri);
	}
	if (_client) {
		mongoc_client_destroy(_client);
	}
	mongoc_cleanup();
	if (_pThread) {
		Quit();
		if (_pThread->joinable()) {
			_pThread->join();
		}
		delete _pThread;
	}
}

bool MongoManager::Init(const char* uri) {
	bson_error_t error;
	mongoc_init();
	_uri = mongoc_uri_new_with_error(uri, &error);
	if (!_uri) {
		LogUtils::LogError("open uri:%s failed", uri);
		return false;
	}

	_client = mongoc_client_new_from_uri(_uri);
	if (!_client) {
		LogUtils::LogError("create mongo client failed, uri:%s", uri);
		return false;
	}

	mongoc_client_set_appname(_client, "retro_mongo");

	MongoDB* db = CreateDB("retro_game");
	if (!db->Init()) {
		LogUtils::LogError("database:retro_name init failed");
		return false;
	}

	DBCollection* collection = db->CreateCollection("player");
	if (!collection->Init()) {
		LogUtils::LogError("collection:player init failed");
		return false;
	}

	_pThread = new std::thread(std::bind(&MongoManager::Execute, this));
	_pThread->detach();

	return true;
}

MongoDB* MongoManager::CreateDB(const char* name) {
	auto iter = _dbMap.find(name);
	if (iter != _dbMap.end()) {
		return iter->second;
	}
	MongoDB* db = new MongoDB(_client, name);
	_dbMap.insert(std::make_pair(name, db));
	return db;
}

MongoDB* MongoManager::GetDB(const char* name) {
	auto iter = _dbMap.find(name);
	return iter != _dbMap.end() ? iter->second : NULL;
}

bool MongoManager::HasDB(const char* name) {
	return _dbMap.find(name) != _dbMap.end();
}

void MongoManager::Ping() {
	bson_t reply ;
	bson_error_t error;
	bson_t* command = BCON_NEW("ping", BCON_INT32(1));

	bool ret = mongoc_client_command_simple(_client, "admin", command, NULL, &reply, &error);
	if (!ret) {
		LogUtils::LogError("ping failed...");
		bson_destroy(command);
		bson_destroy(&reply);
		return;
	}
	char* str = bson_as_json(&reply, NULL);
	LogUtils::LogDebug(str);

	bson_destroy(command);
	bson_destroy(&reply);
	bson_free(str);
}

bool MongoManager::IsQuit() {
	std::lock_guard<std::mutex> guard(_quitMutex);
	return _bQuit;
}

void MongoManager::Quit() {
	std::lock_guard<std::mutex> guard(_quitMutex);
	_bQuit = true;
}

void MongoManager::PushCommand(DBCommand* cmd) {
	std::lock_guard<std::mutex> guard(_waitMutex);
	_cmdWaitQueue.push(cmd);
}

DBCommand* MongoManager::PopCommand() {
	std::lock_guard<std::mutex> guard(_waitMutex);
	if (_cmdWaitQueue.size() > 0) {
		DBCommand* cmd = _cmdWaitQueue.front();
		_cmdWaitQueue.pop();
		return cmd;
	}
	return NULL;
}

void MongoManager::PushResultCommand(DBCommand* cmd) {
	std::lock_guard<std::mutex> guard(_resultMutex);
	_cmdResultQueue.push(cmd);
}

DBCommand* MongoManager::PopResultCommand() {
	std::lock_guard<std::mutex> guard(_resultMutex);
	if (_cmdResultQueue.size() > 0) {
		DBCommand* cmd = _cmdResultQueue.front();
		_cmdResultQueue.pop();
		return cmd;
	}
	return NULL;
}

void MongoManager::Execute() {
	while (1) {
		DBCommand* cmd = PopCommand();
		if (cmd) {
			cmd->Execute();
			PushResultCommand(cmd);
		}
		else {
			if (IsQuit()) {
				break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}

void MongoManager::Update() {
	int count = 5;
	while (count--) {
		DBCommand* cmd = PopResultCommand();
		if (cmd) {
			cmd->CallBack();
			delete cmd;
		}
		else {
			break;
		}
	}
}