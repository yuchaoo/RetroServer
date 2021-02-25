#include "db/MongoManager.h"
#include "LogUtils.h"
#include "db/BsonObj.h"
#include "db/MongoDB.h"
#include "db/DBCollection.h"
#include "TimeUtils.h"

char RandChar() {
	static char text[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	int r = rand() % sizeof(text);
	return text[r];
}

std::string RandString(int min, int max) {
	std::string ss;
	int len = rand() % (max - min) + min;
	for (size_t i = 0; i < len; ++i) {
		ss.push_back( RandChar() );
	}
	return ss;
}

int TestBsonObj() {
	BsonObj obj1;
	srand(time(NULL));

	obj1.Append("name", "aaaaa");
	obj1.Append("age", 1);
	obj1.Append("sex", true);
	obj1.Append("vec", 12.1f);
	obj1.Append("mang", 10000L);

	std::string s;
	for (size_t i = 0; i < 10; ++i) {
		s.push_back(rand() / 256);
	}
	obj1.Append("s", s.c_str(), s.size());
	obj1.AppendJson("json", "{\"url\":\"www.baidu.com\", \"port\":80}");
	
	BsonObj obj2;
	std::vector<std::string> strList;
	for (size_t i = 0; i < 10; ++i) {
		std::string key = RandString(3, 6);
		obj2.Append(i, 100 + i);
		strList.push_back(key);
	}
	obj1.AppendDocument("array", &obj2);
	obj1.AppendArray("strlist", strList);

	std::string str = obj1.toJson();
	LogUtils::LogDebug(str.c_str());

	BsonObj obj3(obj1);
	std::string name = obj3.Extract("name", "");
	int age = obj3.Extract("age", 0);
	bool sex = obj3.Extract("sex", false);
	float vec = obj3.Extract("vec", 0.0f);
	long mang = obj3.Extract("mang", 0);

	std::vector<int> sss;
	obj3.ExtractArray("s", sss);

	std::string json = obj3.Extract("json", "");
	BSONOBJ_PTR jsonPtr = obj3.ExtractDocument("json");
	BsonIter jsonIter = jsonPtr->GetIter();
	std::string url = jsonPtr->Extract("url", "");
	int port = jsonPtr->Extract("port", 0);

	size_t size = 0;
	BSONOBJ_PTR ptr2 = obj3.ExtractArray("strlist");
	
	std::vector<std::string> array2;
	BsonIter iter = ptr2->GetIter();

	int i;
	std::string value;
	while (++iter) {
		iter.value(i,s);
		array2.push_back(s);
	}

	BSONOBJ_PTR ptr1 = obj3.ExtractDocument("array");
	std::vector<int> array1;
	iter = ptr1->GetIter();
	int index = 0;
	int value1 = 0;
	while (++iter) {
		iter.value(index, value1);
		array1.push_back(value1);
	}
	

	return 1;
}

int TestPing() {
	int index = 0;
	int count = 5;
	while (count--) {
		MongoManager::getInstance()->Ping();
		Sleep(1000);

		BsonObj bson;
		bson.Append(index++, TimeUtils::GetTimeOfDay());
		MongoDB* db = MongoManager::getInstance()->GetDB("retro_game");
		DBCollection* collection = db->GetCollection("player");
		collection->Insert(&bson);
	}
	return 0;
}

int main(int argv, char** argc) {
	bool ret = MongoManager::getInstance()->Init("mongodb://127.0.0.1:27017");
	if (!ret){
		LogUtils::LogDebug("mongdb init failed");
		return 0;
	}

	TestBsonObj();
	TestPing();
	return 0;
}