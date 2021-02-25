#pragma once
#include <string>
#include "mongoc.h"
#include <unordered_map>

class DBCollection;

class MongoDB {
public:
	MongoDB(mongoc_client_t* client, const char* name);
	~MongoDB();
	bool Init();
	const std::string& GetName() const { return _name; }
	mongoc_client_t* GetClient() { return _client; }
	DBCollection* CreateCollection(const char* name);
	DBCollection* GetCollection(const char* name);
	bool IsHasCollection(const char* name);
private:
	std::string _name;
	mongoc_database_t* _database;
	mongoc_client_t* _client;
	std::unordered_map<std::string, DBCollection*> _colMap;
};