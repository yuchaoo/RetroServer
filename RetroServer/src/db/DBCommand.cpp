#include "DBCommand.h"
#include "MongoDB.h"
#include "BsonObj.h"
#include "DBCollection.h"

DBCommand::DBCommand(const char* dbName, const char* colName)
:_dbName(dbName)
,_colName(colName)
,_result(false){

}

DBCommand::~DBCommand() {

}

CmdCallBack DBCommand::GetCallBack() {
	return _callback;
}

void DBCommand::SetCallBack(CmdCallBack callback) {
	_callback = callback;
}

DBCollection* DBCommand::GetCollection() {
	MongoDB* db = MongoManager::getInstance()->CreateDB(_dbName.c_str());
	if (db) {
		return db->GetCollection(_colName.c_str());
	}
	return NULL;
}

bool DBCommand::IsSecceed() const {
	return _result;
}

const std::string& DBCommand::GetError() const {
	return _error;
}

void DBCommand::CallBack() {
	if (_callback) {
		_callback(_result, _error.c_str());
	}
}

/**************************************************/

InsertCmd::InsertCmd(const char* dbName, const char* colName)
:DBCommand(dbName,colName){
}

InsertCmd::~InsertCmd() {
	delete _bson;
}

BsonObj* InsertCmd::GetBsonObj() {
	return _bson;
}

void InsertCmd::SetBsonObj(BsonObj* obj) {
	_bson = obj;
}

bool InsertCmd::Execute() {
	DBCollection* col = GetCollection();
	_result = col->Insert(_bson);
	return _result;
}
/***********************************/

InsertManyCmd::InsertManyCmd(const char* dbName, const char* colName)
:DBCommand(dbName,colName){

}

InsertManyCmd::~InsertManyCmd() {

}

void InsertManyCmd::AddBsonObj(BsonObj* bson) {
	_bsons.push_back(bson);
}

bool InsertManyCmd::Execute() {
	DBCollection* col = GetCollection();
	return _result = col->Insert(_bsons);
}