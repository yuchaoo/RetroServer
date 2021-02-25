#pragma once
#include <functional>
#include <string>
#include "MongoManager.h"

class BsonObj;
class DBCollection;

typedef std::function<void(bool, const char*)> CmdCallBack;

class DBCommand {
public:
	DBCommand(const char* dbName, const char* colName);
	~DBCommand();
	CmdCallBack GetCallBack();
	void SetCallBack(CmdCallBack callback);
	DBCollection* GetCollection();
	bool IsSecceed() const;
	const std::string& GetError() const;
	void CallBack();
	virtual bool Execute() = 0;
protected:
	CmdCallBack _callback;
	std::string _dbName;
	std::string _colName;
	bool _result;
	std::string _error;
};

class InsertCmd : public DBCommand {
public:
	InsertCmd(const char* dbName, const char* colName);
	~InsertCmd();
	BsonObj* GetBsonObj();
	void SetBsonObj(BsonObj* obj);
	virtual bool Execute();
private:
	BsonObj* _bson;
};

class InsertManyCmd : public DBCommand {
public:
	InsertManyCmd(const char* dbName, const char* colName);
	~InsertManyCmd();
	void AddBsonObj(BsonObj* bson);
	virtual bool Execute();
private:
	std::vector<BsonObj*> _bsons;
};

class QueryCmd : public DBCommand {
public:
	QueryCmd(const char* dbName, const char* colName);
	~QueryCmd();
	void SetBsonObj(BsonObj* obj);
	virtual bool Execute();
private:
	BsonObj* _bson;
};

